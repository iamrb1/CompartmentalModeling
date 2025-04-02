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
    // Draw edges as thin rectangles (lines)
    DrawEdges(g);

    // Draw vertices as circles
    auto vertices = g.GetVertices();
    for (auto v : vertices) {
      Shape::drawCircle(v->GetX(), v->GetY(), VERTEX_RADIUS, v->GetData().c_str());
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

      // Add button group and vertex info to control zone
      controlZone.appendChild(buttonGroup);
      controlZone.appendChild(selectedVertexDiv);
      mainElement.appendChild(controlZone);
    });
  }

public:
  GraphVisualizer() {
    // Initial values as example
    g.AddVertex("ID1", "blue", 150, 200);
    g.AddVertex("ID2", "red", 120, 250);
    g.AddVertex("ID3", "green", 100, 210);

    g.AddEdge("ID1", "ID2", 2);
    g.AddEdge("ID1", "ID3", 2);

    InitiateCanvas();
    InitializeControlZone();
    RedrawCanvas();
  }

  void ClearGraph() {
    g.ClearGraph();
    ClearVertexSelection();
    RedrawCanvas();
  }

  void AddVertex() {
    int id = g.GetVertices().size();
    int x = r.GetInt(0, CANVAS_WIDTH);
    int y = r.GetInt(0, CANVAS_HEIGHT);

    g.AddVertex(std::to_string(++id), "blue", x, y);

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
}

int main() {
  return 0;
}