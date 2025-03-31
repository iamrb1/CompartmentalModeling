#include "Graph/Graph.hpp"
#include "GraphPosition/GraphPosition.hpp"
#include <cmath>
#include <emscripten.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// Helper function to draw a thin rectangle (line) between two points
void drawLine(int x1, int y1, int x2, int y2, const char *color, int thickness = 2) {
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

// Helper function to draw edges as thin rectangles (lines)
void drawEdges(const cse::Graph<std::string> &graph) {
  auto edges = graph.GetEdges();
  for (auto edge : edges) {
    auto &from = edge->GetFrom();
    auto &to = edge->GetTo();
    int x1 = from.GetX();
    int y1 = from.GetY();
    int x2 = to.GetX();
    int y2 = to.GetY();
    drawLine(x1, y1, x2, y2, "gray");
  }
}

extern "C" {
void resizeCanvas() {
  EM_ASM({
    var canvas = document.getElementById('canvas');
    if (!canvas) {
      canvas = document.createElement('canvas');
      canvas.id = 'canvas';
      document.body.appendChild(canvas);
    }

    canvas.width = Math.min(window.innerWidth, 800);   // Set minimum width to 800px
    canvas.height = Math.min(window.innerHeight, 500); // Set minimum height to 500px
  });
}

void drawCircle(int x, int y, int radius, const char *color) {
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

void clearCanvas() {
  EM_ASM({
    var canvas = document.getElementById('canvas');
    if (canvas) {
      var ctx = canvas.getContext('2d');
      ctx.clearRect(0, 0, canvas.width, canvas.height); // Clear the canvas
    }
  });
}

void drawGraph() {
  clearCanvas();

  // Create a simple graph with 3 vertices and 1 edge
  cse::Graph<std::string> graph;

  auto v1 = graph.AddVertex("A", "red", 100, 100);
  auto v2 = graph.AddVertex("B", "green", 200, 200);
  auto v3 = graph.AddVertex("C", "blue", 150, 100);
  graph.AddEdge("A", "B", 2);
  graph.AddEdge("B", "C", 2);

  // Draw edges as thin rectangles (lines)
  drawEdges(graph);

  // Draw vertices as circles
  auto vertices = graph.GetVertices();
  for (auto v : vertices) {
    drawCircle(v->GetX(), v->GetY(), 10, v->GetData().c_str());
    std::cout << "Drew" << *v << std::endl;
  }
}
}

int main() {
  EM_ASM({
    var clearButton = document.createElement('button');
    clearButton.textContent = "Clear Canvas";

    var addButton = document.createElement('button');
    addButton.textContent = "Add Circles";

    var graphButton = document.createElement('button');
    graphButton.textContent = "Draw Graph";

    document.body.appendChild(clearButton);
    document.body.appendChild(addButton);
    document.body.appendChild(graphButton);

    clearButton.addEventListener('click', function() { Module._clearCanvas(); });
    addButton.addEventListener('click', function() { Module._drawOnce(); });
    graphButton.addEventListener('click', function() { Module._drawGraph(); });

    window.addEventListener('resize', function() { Module._resizeCanvas(); }); // Resize canvas on window resize
    Module._resizeCanvas();                                                    // Initial canvas setup
    setTimeout(function() { Module._drawGraph(); }, 500);
  });
  return 0;
}