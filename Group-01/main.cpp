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
    EM_ASM_(
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
    EM_ASM_({
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

      // The fields required for adding and removing an edge
      var edgeButtonGroup = document.createElement("div");
      edgeButtonGroup.classList.add("button-group");
      edgeButtonGroup.style.border = "2px solid black";
      edgeButtonGroup.style.padding = "10px";
      edgeButtonGroup.style.marginBottom = "10px";

      var vertexId1Input = document.createElement("input");
      vertexId1Input.setAttribute("id", "vertexId1Input");
      vertexId1Input.setAttribute("placeholder", "Vertex ID 1");
      vertexId1Input.setAttribute("type", "text");
      edgeButtonGroup.appendChild(vertexId1Input);

      var vertexId2Input = document.createElement("input");
      vertexId2Input.setAttribute("id", "vertexId2Input");
      vertexId2Input.setAttribute("placeholder", "Vertex ID 2");
      vertexId2Input.setAttribute("type", "text");
      edgeButtonGroup.appendChild(vertexId2Input);

      var edgeWeightInput = document.createElement("input");
      edgeWeightInput.setAttribute("id", "edgeWeightInput");
      edgeWeightInput.setAttribute("placeholder", "Edge Weight");
      edgeWeightInput.setAttribute("type", "text");
      edgeButtonGroup.appendChild(edgeWeightInput);

      var addRemoveEdgeButton = document.createElement('button');
      addRemoveEdgeButton.textContent = "Toggle Edge";
      addRemoveEdgeButton.addEventListener(
          'click', function() {
            var id1 = document.getElementById("vertexId1Input").value;
            var id2 = document.getElementById("vertexId2Input").value;
            var weight = parseInt(document.getElementById("edgeWeightInput").value);
            if (id1 && id2 && !isNaN(weight)) {
              Module._toggleEdge(stringToNewUTF8(id1), stringToNewUTF8(id2), weight);
            } else {
              alert("Please fill out both vertex IDs and the weight.");
            }
          });
      edgeButtonGroup.appendChild(addRemoveEdgeButton);

      // Clear button group container
      var clearButtonGroup = document.createElement("div");
      clearButtonGroup.classList.add("button-group");

      // Clear button
      var clearButton = document.createElement('button');
      clearButton.textContent = "Clear Graph";
      clearButton.addEventListener('click', function() { Module._clearCanvas(); });
      clearButtonGroup.appendChild(clearButton);

      // Button group container
      var buttonGroup = document.createElement("div");
      buttonGroup.classList.add("button-group");
      buttonGroup.style.border = "2px solid black";
      buttonGroup.style.padding = "10px";
      buttonGroup.style.marginBottom = "10px";

      // The fields required for adding a vertex
      var idInput = document.createElement("input");
      idInput.setAttribute("id", "vertexIdInput");
      idInput.setAttribute("placeholder", "ID");
      idInput.setAttribute("type", "text");
      buttonGroup.appendChild(idInput);

      var xInput = document.createElement("input");
      xInput.setAttribute("id", "vertexXInput");
      xInput.setAttribute("placeholder", "X");
      xInput.setAttribute("type", "number");
      buttonGroup.appendChild(xInput);

      var yInput = document.createElement("input");
      yInput.setAttribute("id", "vertexYInput");
      yInput.setAttribute("placeholder", "Y");
      yInput.setAttribute("type", "number");
      buttonGroup.appendChild(yInput);

      // Add Vertex button
      var addVertexButton = document.createElement('button');
      addVertexButton.textContent = "Add Vertex";

      // This part is activly being assisted by chatgpt
      // It creates a vertex at the values given by the user
      // It currently doesn't store the id correctly for the vertex
      addVertexButton.addEventListener(
          'click', function() {
            var id = document.getElementById("vertexIdInput").value;
            var x = parseInt(document.getElementById("vertexXInput").value);
            var y = parseInt(document.getElementById("vertexYInput").value);
            if (id && !isNaN(x) && !isNaN(y)) {
              Module._addVertexWithParams(stringToNewUTF8(id), x, y);
            } else {
              alert("Please fill out all fields.");
            }
          });
      buttonGroup.appendChild(addVertexButton);

      // Button group container
      var deleteButtonGroup = document.createElement("div");
      deleteButtonGroup.classList.add("button-group");
      deleteButtonGroup.style.border = "2px solid black";
      deleteButtonGroup.style.padding = "10px";
      deleteButtonGroup.style.marginBottom = "10px";

      // The fields required for adding and removing an edge
      var deleteInput = document.createElement("input");
      deleteInput.setAttribute("id", "deleteVertexIdInput");
      deleteInput.setAttribute("placeholder", "ID of vertex to delete");
      deleteInput.setAttribute("type", "text");
      deleteButtonGroup.appendChild(deleteInput);

      // Delete Vertex button
      var deleteVertexButton = document.createElement('button');
      deleteVertexButton.textContent = "Delete Vertex";
      deleteVertexButton.addEventListener(
          'click', function() {
            var id = document.getElementById("deleteVertexIdInput").value;
            if (id) {
              console.log("Deleting vertex with ID: " + id);
              Module._deleteVertex(stringToNewUTF8(id));
            } else {
              alert("Please enter a valid vertex ID.");
            }
          });
      deleteButtonGroup.appendChild(deleteVertexButton);

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
      controlZone.appendChild(clearButtonGroup);
      controlZone.appendChild(edgeButtonGroup);
      controlZone.appendChild(deleteButtonGroup);
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

  // Helper function that adds a vertex to the graph and paints it
  void AddVertex() {
    int id = g.GetVertices().size();
    int x = r.GetInt(0, CANVAS_WIDTH);
    int y = r.GetInt(0, CANVAS_HEIGHT);

    g.AddVertex(std::to_string(++id), "blue", x, y);

    RedrawCanvas();
  }

  // Allows the user to delete a vertex of their choosing
  void DeleteVertex(const char *id) {
    std::string idCpp(id);
    if (!g.HasVertex(idCpp)) {
      std::cout << "No vertex found with ID: " << idCpp << std::endl;
      EM_ASM({ alert("Error: Vertex with this ID does not exist!"); });
      return;
    }

    g.RemoveVertex(idCpp);
    std::cout << "Vertex with ID " << idCpp << " has been deleted." << std::endl;
    RedrawCanvas();
  }

  // Allows the user to add and/or delete an edge between 2 verticies
  void ToggleEdge(const char *id1, const char *id2, int weight) {
    std::string idCpp1(id1);
    std::string idCpp2(id2);
    const std::string edgeId = idCpp1 + "-" + idCpp2;
    if (g.HasEdge(edgeId)) {
      g.RemoveEdge(edgeId);
    } else {
      g.AddEdge(idCpp1, idCpp2, weight);
    }
    RedrawCanvas();
  }

  // Allows the user to add a vertex where they chose
  void AddVertexWithParams(const char *id, int x, int y) {
    std::string idCpp(id);
    if (g.HasVertex(idCpp)) {
      std::cout << "Vertex with ID " << idCpp << " already exists!" << std::endl;
      EM_ASM({ alert("Error: Vertex with this ID already exists!"); });
      return;
    }
    g.AddVertex(idCpp, "blue", x, y);
    std::cout << "Total Vertices: " << g.GetVertices().size() << "id: " << idCpp << " " << std::endl;
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

void toggleEdge(const char *id1, const char *id2, int weight) {
  init.ToggleEdge(id1, id2, weight);
}

void addVertexWithParams(const char *id, int x, int y) {
  init.AddVertexWithParams(id, x, y);
}

void handleCanvasClick(double x, double y) {
  init.HandleCanvasClick(x, y);
}

void deleteVertex(const char *id) {
  init.DeleteVertex(id);
}
}
int main() {
  return 0;
}