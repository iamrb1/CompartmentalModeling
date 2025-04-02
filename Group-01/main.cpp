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

  // Helper function to draw edges as thin rectangles (lines)
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
    EM_ASM({
      var canvas = document.getElementById("canvas");
      // This is not the same as the width/height on the screen
      // This is the virtual size of the canvas, basically how many pixels
      // it has to be drawn on
      canvas.width = 1000;
      canvas.height = 1000;

      var ctx = canvas.getContext("2d");
      ctx.fillStyle = "black";
      ctx.fillRect(0, 0, canvas.width, canvas.height);
    });

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
            var x = event.clientX - rect.left;
            var y = event.clientY - rect.top;

            // Scale coordinates to match canvas size
            x = (x / rect.width) * canvas.width;
            y = (y / rect.height) * canvas.height;

            Module._handleCanvasClick(x, y);
          });
    });
  }

  void InitializeControlZone() {
    EM_ASM({
      var mainElement = document.getElementById("main");
      var controlZone = document.createElement("div");

      /** CONTROL BUTTONS **/
      // Clear
      var clearButton = document.createElement('button');
      clearButton.textContent = "Clear Graph";
      clearButton.addEventListener('click', function() { Module._clearCanvas(); });
      controlZone.appendChild(clearButton);

      // Add Vertex
      var addVertexButton = document.createElement('button');
      addVertexButton.textContent = "Add Vertex";
      addVertexButton.addEventListener('click', function() { Module._addVertex(); });
      controlZone.appendChild(addVertexButton);

      /** SELECTED VERTEX DASHBOARD **/
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
    RedrawCanvas();
  }

  void AddVertex() {
    int id = g.GetVertices().size();
    int x = r.GetInt(0, 1000);
    int y = r.GetInt(0, 1000);

    g.AddVertex(std::to_string(++id), "blue", x, y);

    RedrawCanvas();
  }

  void HandleCanvasClick(double x, double y) {
    auto vertices = g.GetVertices();
    for (auto v : vertices) {
      double dx = v->GetX() - x;
      double dy = v->GetY() - y;
      double distance = std::sqrt(dx * dx + dy * dy);

      if (distance <= VERTEX_RADIUS) {
        HandleSelectedVertex(*v);
        return;
      }
    }

    // If no vertex found, clear selection
    EM_ASM({
      document.getElementById("selectedVertexTitle").innerHTML = "No Selected Vertex";
      document.getElementById("selectedVertexId").innerHTML = "";
      document.getElementById("selectedVertexX").innerHTML = "";
      document.getElementById("selectedVertexY").innerHTML = "";
    });
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