#include "Graph/Graph.hpp"
#include "GraphPosition/GraphPosition.hpp"
#include "Random/Random.hpp"
#include <cmath>
#include <emscripten.h>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

const int VERTEX_RADIUS = 10;
const int CANVAS_WIDTH = 1000;
const int CANVAS_HEIGHT = 1000;

class Shape {
public:
  static void drawLine(int x1, int y1, int x2, int y2, const char *color, int thickness = 2) {
    EM_ASM_(
        {
          var canvas = document.getElementById('canvas');
          var ctx = canvas.getContext('2d');
          ctx.strokeStyle = UTF8ToString($4);
          ctx.lineWidth = $5;
          ctx.beginPath();
          ctx.moveTo($0, $1);
          ctx.lineTo($2, $3);
          ctx.stroke();
        },
        x1, y1, x2, y2, color, thickness);
  }

  static void drawCircle(int x, int y, int radius, const char *color) {
    EM_ASM_(
        {
          var canvas = document.getElementById('canvas');
          var ctx = canvas.getContext('2d');
          ctx.fillStyle = UTF8ToString($3);
          ctx.beginPath();
          ctx.arc($0, $1, $2, 0, Math.PI * 2);
          ctx.fill();
        },
        x, y, radius, color);
  }
};

class GraphVisualizer {
private:
  cse::Random r{0};
  std::optional<cse::Vertex<std::string>> selectedVertex;
  cse::Graph<std::string> g;
  std::optional<cse::GraphPosition<std::string>> traversal;

  static bool IsPointInRange(double x1, double y1, double x2, double y2, double range) {
    double dx = x1 - x2;
    double dy = y1 - y2;
    return std::sqrt(dx * dx + dy * dy) <= range;
  }

  void ClearVertexSelection() {
    EM_ASM({
      document.getElementById("selectedVertexTitle").innerHTML = "No Selected Vertex";
      document.getElementById("selectedVertexId").innerHTML = "";
      document.getElementById("selectedVertexX").innerHTML = "";
      document.getElementById("selectedVertexY").innerHTML = "";
    });
  }

  std::optional<std::reference_wrapper<const cse::Vertex<std::string>>> FindVertexAtPosition(double x, double y) {
    auto vertices = g.GetVertices();
    for (auto v : vertices) {
      if (IsPointInRange(v->GetX(), v->GetY(), x, y, VERTEX_RADIUS)) {
        return std::cref(*v);
      }
    }
    return std::nullopt;
  }

  void DrawEdges(const cse::Graph<std::string> &graph) {
    auto edges = graph.GetEdges();
    for (auto edge : edges) {
      auto &from = edge->GetFrom();
      auto &to = edge->GetTo();
      int x1 = from.GetX();
      int y1 = from.GetY();
      int x2 = to.GetX();
      int y2 = to.GetY();
      Shape::drawLine(x1, y1, x2, y2, "gray");
    }
  }

  void DrawGraph() {
    auto v = g.GetVertex("ID1");
    for (auto &[id, edge] : v.GetEdges()) {
      if (auto e = edge.lock()) {
        EM_ASM_({
          console.log("Edge from ID1 to", UTF8ToString($0), "via", UTF8ToString($1));
        }, e->GetTo().GetId().c_str(), id.c_str());
      }
    }
    // Draw edges as thin rectangles (lines)
    DrawEdges(g);

    // Draw vertices as circles
    auto vertices = g.GetVertices();
    for (auto v : vertices) {
      std::string color = "gray"; // default
      if (traversal) {
        if (traversal->IsVisited(*v)) color = "green";
        else if (&traversal->GetCurrentVertex() == v) color = "red";
      }
      Shape::drawCircle(v->GetX(), v->GetY(), VERTEX_RADIUS, color.c_str());
    }
  }

  void HandleSelectedVertex(cse::Vertex<std::string> v) {
    EM_ASM(
        {
          document.getElementById("selectedVertexTitle").innerHTML = "Selected Vertex";
          document.getElementById("selectedVertexId").innerHTML = "ID: " + UTF8ToString($0);
          document.getElementById("selectedVertexX").innerHTML = "X: " + $1;
          document.getElementById("selectedVertexY").innerHTML = "Y: " + $2;
        },
        v.GetId().c_str(), v.GetX(), v.GetY());
  }

  // Must be called when any updates happen to the state
  void RedrawCanvas() {
    EM_ASM(
        {
          var canvas = document.getElementById("canvas");
          canvas.width = $0;
          canvas.height = $1;
          var ctx = canvas.getContext("2d");
          ctx.fillStyle = "black";
          ctx.fillRect(0, 0, canvas.width, canvas.height);
        },
        CANVAS_WIDTH, CANVAS_HEIGHT);

    DrawGraph();
  }

  void InitiateCanvas() {
    EM_ASM({
      var mainElement = document.getElementById("main");
      var canvas = document.createElement('canvas');
      canvas.setAttribute("id", "canvas");
      mainElement.appendChild(canvas);

      // Canvas click handler
      canvas.addEventListener(
          'click', function(event) {
            var rect = canvas.getBoundingClientRect();
            var scaleX = canvas.width / rect.width;
            var scaleY = canvas.height / rect.height;

            var x = (event.clientX - rect.left) * scaleX;
            var y = (event.clientY - rect.top) * scaleY;

            Module._handleCanvasClick(x, y);
          });
    });
  }

  void InitializeControlZone() {
    EM_ASM({
      var mainElement = document.getElementById("main");
      var controlZone = document.createElement("div");
      controlZone.classList.add("control-zone");

      // Button group container
      var buttonGroup = document.createElement("div");
      buttonGroup.classList.add("button-group");

      // Clear button
      var clearButton = document.createElement('button');
      clearButton.textContent = "Clear Graph";
      clearButton.addEventListener('click', function() { Module._clearCanvas(); });
      buttonGroup.appendChild(clearButton);

      // Add Vertex button
      var addVertexButton = document.createElement('button');
      addVertexButton.textContent = "Add Vertex";
      addVertexButton.addEventListener('click', function() { Module._addVertex(); });
      buttonGroup.appendChild(addVertexButton);

      // Selected vertex info container
      var selectedVertexDiv = document.createElement('div');
      selectedVertexDiv.setAttribute("id", "selectedVertexContainer");
      var selectedVertexTitle = document.createElement('h2');
      selectedVertexTitle.setAttribute("id", "selectedVertexTitle");
      var selectedVertexId = document.createElement('h3');
      selectedVertexId.setAttribute("id", "selectedVertexId");
      var selectedVertexX = document.createElement('h4');
      selectedVertexX.setAttribute("id", "selectedVertexX");
      var selectedVertexY = document.createElement('h4');
      selectedVertexY.setAttribute("id", "selectedVertexY");

      selectedVertexDiv.appendChild(selectedVertexTitle);
      selectedVertexDiv.appendChild(selectedVertexId);
      selectedVertexDiv.appendChild(selectedVertexX);
      selectedVertexDiv.appendChild(selectedVertexY);

      selectedVertexTitle.innerHTML = "No Selected Vertex";

      // Dropdown to select traversal type
      var traversalSelect = document.createElement('select');
      traversalSelect.setAttribute("id", "traversalMode");
      var option1 = document.createElement('option');
      option1.value = "DFS";
      option1.textContent = "DFS";
      traversalSelect.appendChild(option1);
      var option2 = document.createElement('option');
      option2.value = "BFS";
      option2.textContent = "BFS";
      traversalSelect.appendChild(option2);
      var option3 = document.createElement('option');
      option3.value = "A*";
      option3.textContent = "A*";
      traversalSelect.appendChild(option3);
      buttonGroup.appendChild(traversalSelect);

      // Traverse Step button
      var stepButton = document.createElement('button');
      stepButton.textContent = "Next Step";
      stepButton.addEventListener('click', function () { Module._stepTraversal(); });
      buttonGroup.appendChild(stepButton);

      // Traverse All button
      var fullButton = document.createElement('button');
      fullButton.textContent = "Traverse All";
      fullButton.addEventListener('click', function () { Module._fullTraversal(); });
      buttonGroup.appendChild(fullButton);

      // Add button group and vertex info to control zone
      controlZone.appendChild(buttonGroup);
      controlZone.appendChild(selectedVertexDiv);
      mainElement.appendChild(controlZone);
    });
  }

public:
  GraphVisualizer() {
    // Initial values as example
    g.AddVertex("ID1", "blue", 500, 200);
    g.AddVertex("ID2", "red", 430, 250);
    g.AddVertex("ID3", "green", 570, 250);
    g.AddVertex("ID4", "blue", 380, 300);
    g.AddVertex("ID5", "red", 480, 300);
    g.AddVertex("ID6", "green", 520, 300);
    g.AddVertex("ID7", "blue", 620, 300);
    // g.AddVertex("ID8", "red", 120, 250);
    // g.AddVertex("ID9", "green", 100, 210);

    g.AddEdge("ID1", "ID2", 2);
    g.AddEdge("ID1", "ID3", 2);
    g.AddEdge("ID2", "ID4", 2);
    g.AddEdge("ID2", "ID5", 2);
    g.AddEdge("ID3", "ID6", 2);
    g.AddEdge("ID3", "ID7", 2);

    InitiateCanvas();
    InitializeControlZone();
    RedrawCanvas();
  }

  void ClearGraph() {
    g.ClearGraph();
    traversal.reset();
    ClearVertexSelection();
    RedrawCanvas();
  }

  void AddVertex() {
    int id = g.GetVertices().size();
    int x = r.GetInt(0, CANVAS_WIDTH);
    int y = r.GetInt(0, CANVAS_HEIGHT);

    g.AddVertex(std::to_string(++id), "blue", x, y);
    traversal.reset();
    RedrawCanvas();
  }

  // Receives the virtual coordinate inside of the canvas.
  void HandleCanvasClick(double x, double y) {
    auto vertex = FindVertexAtPosition(x, y);
    if (vertex.has_value()) {
      HandleSelectedVertex(vertex.value());
    } else {
      ClearVertexSelection();
    }
  }

  void StartTraversal() {
    if (g.GetVertices().empty()) return;
    auto start = g.GetVertex("ID1"); // default to first vertex
    EM_ASM({
      console.log("Traversal started");
    });
    EM_ASM_({
      console.log("Start vertex ID is:", UTF8ToString($0));
    }, start.GetId().c_str());
  
    cse::GraphPosition<std::string> pos(g, start);
  
    int mode = EM_ASM_INT({
      var mode = document.getElementById("traversalMode").value;
      if (mode === "DFS") return 0;
      if (mode === "BFS") return 1;
      if (mode === "A*") return 2;
      return 0;
    });
  
    if (mode == 0) pos.SetTraversalMode(cse::TraversalModes::DFS<std::string>());
    else if (mode == 1) pos.SetTraversalMode(cse::TraversalModes::BFS<std::string>());
    else if (mode == 2) pos.SetTraversalMode(cse::TraversalModes::AStar<std::string>(g.GetVertex("ID1"))); // temp hardcoded target
  
    traversal.emplace(std::move(pos));

    RedrawCanvas();
  }
  
  void StepTraversal() {
    EM_ASM({ console.log("StepTraversal() triggered"); });

    if (!traversal.has_value()) {
      EM_ASM({ console.log("No traversal yet, starting..."); });
      StartTraversal();
    }

    if (traversal) {
      EM_ASM({ console.log("Attempting to advance..."); });

      if (traversal->AdvanceToNextNeighbor()) {
        EM_ASM({ console.log("Advanced to next"); });
        RedrawCanvas();
      } else {
        EM_ASM({ console.log("No more neighbors to visit"); });
      }
    }
  }
  
  void FullTraversal() {
    if (!traversal.has_value()) StartTraversal();
  
    if (traversal) {
      traversal->TraverseGraph();
      RedrawCanvas();
    }
  }
  
};

GraphVisualizer init{};

/**
 * Any functions that need to be called when something happens on the screen
 * like when a buttone is clicked must have an interface here
 */
extern "C" {
void clearCanvas() {
  init.ClearGraph();
}

void addVertex() {
  init.AddVertex();
}

void handleCanvasClick(double x, double y) {
  init.HandleCanvasClick(x, y);
}

void startTraversal() { 
  init.StartTraversal(); 
}

void stepTraversal() { 
  init.StepTraversal(); 
}

void fullTraversal() { 
  init.FullTraversal(); 
}
}

int main() {
  return 0;
}