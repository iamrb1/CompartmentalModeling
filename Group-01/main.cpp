#include <emscripten.h>

#include <cmath>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "Graph/Graph.hpp"
#include "Graph/GraphJson.hpp"
#include "GraphPosition/GraphPosition.hpp"
#include "Random/Random.hpp"

const int VERTEX_RADIUS = 10;
const int CANVAS_WIDTH = 1000;
const int CANVAS_HEIGHT = 1000;

class Shape {
 public:
  static void drawLine(int x1, int y1, int x2, int y2, const char *color,
                       int thickness = 2) {
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
  cse::GraphJson<std::string> graphJson{g};

  static bool IsPointInRange(double x1, double y1, double x2, double y2,
                             double range) {
    double dx = x1 - x2;
    double dy = y1 - y2;
    return std::sqrt(dx * dx + dy * dy) <= range;
  }

  void ClearVertexSelection() {
    EM_ASM({
      document.getElementById("selectedVertexTitle").innerHTML =
          "No Selected Vertex";
      document.getElementById("selectedVertexId").innerHTML = "";
      document.getElementById("selectedVertexX").innerHTML = "";
      document.getElementById("selectedVertexY").innerHTML = "";
    });
  }

  std::optional<std::reference_wrapper<const cse::Vertex<std::string>>>
  FindVertexAtPosition(double x, double y) {
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
      std::string color = "gray"; // default
      if (traversal) {
        if (&traversal->GetCurrentVertex() == v) color = "red";
        else if (traversal->IsVisited(*v)) color = "green";
      }
      Shape::drawCircle(v->GetX(), v->GetY(), VERTEX_RADIUS, color.c_str());
    }
  }

  void HandleSelectedVertex(cse::Vertex<std::string> v) {
    EM_ASM(
        {
          document.getElementById("selectedVertexTitle").innerHTML =
              "Selected Vertex";
          document.getElementById("selectedVertexId").innerHTML =
              "ID: " + UTF8ToString($0);
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
      clearButton.addEventListener(
          'click', function() { Module._clearCanvas(); });
      buttonGroup.appendChild(clearButton);

      // Add Vertex button
      var addVertexButton = document.createElement('button');
      addVertexButton.textContent = "Add Vertex";
      addVertexButton.addEventListener(
          'click', function() { Module._addVertex(); });
      buttonGroup.appendChild(addVertexButton);

      // Random Graph button
      var separator = document.createElement("separator");
      separator.style.margin = "10px 0";
      buttonGroup.appendChild(separator);

      var randomGraphButton = document.createElement('button');
      randomGraphButton.textContent = "Random Graph";
      randomGraphButton.addEventListener('click', function() {
        var vertices = parseInt(document.getElementById("vertexCount").value);
        var edges = parseInt(document.getElementById("edgeCount").value);
        Module._randomGraph(vertices, edges);
      });
      buttonGroup.appendChild(randomGraphButton);
      var vertexLabel = document.createElement('label');
      vertexLabel.setAttribute("for", "vertexCount");
      vertexLabel.innerText = "Vertices: ";
      buttonGroup.appendChild(vertexLabel);

      // Vertex input field
      var vertexInput = document.createElement('input');
      vertexInput.setAttribute("id", "vertexCount");
      vertexInput.setAttribute("type", "number");
      vertexInput.setAttribute("placeholder", "Vertices");
      vertexInput.setAttribute("min", "1");
      vertexInput.setAttribute("value", "1");
      vertexInput.style.marginRight = "10px";
      buttonGroup.appendChild(vertexInput);

      // Edges input field
      var edgeLabel = document.createElement('label');
      edgeLabel.setAttribute("for", "edgeCount");
      edgeLabel.innerText = "Edges: ";
      buttonGroup.appendChild(edgeLabel);
      var edgeInput = document.createElement('input');
      edgeInput.setAttribute("id", "edgeCount");
      edgeInput.setAttribute("type", "number");
      edgeInput.setAttribute("placeholder", "Edges");
      edgeInput.setAttribute("min", "0");
      edgeInput.setAttribute("value", "0");
      edgeInput.style.marginRight = "10px";
      buttonGroup.appendChild(edgeInput);

      // Clear Traversal button
      var clearTraversalButton = document.createElement('button');
      clearTraversalButton.textContent = "Clear Traversal";
      clearTraversalButton.addEventListener('click', function () {
        Module._clearTraversal();
      });
      buttonGroup.appendChild(clearTraversalButton);

      /**
       * Used ChatGPT to assist adding support to 
       * Import and Export a Graph in JSON 
       */
      // Import Graph button
      var importButton = document.createElement('button');
      importButton.textContent = "Import Graph";
      importButton.addEventListener(
          'click', function() {
            // Create a file input element
            var fileInput = document.createElement('input');
            fileInput.type = 'file';
            fileInput.accept = '.json';
            fileInput.style.display = 'none';
            document.body.appendChild(fileInput);

            fileInput.addEventListener(
                'change', function(e) {
                  var file = e.target.files[0];
                  if (!file) return;

                  var reader = new FileReader();
                  reader.onload = function(e) {
                    var contents = e.target.result;

                    // Use cwrap to create a JavaScript function that calls the
                    // C++ function
                    var importGraph =
                        Module.cwrap('importGraph', 'boolean', ['string']);

                    // Call the import function
                    importGraph(contents);
                  };
                  reader.readAsText(file);
                });

            fileInput.click();
            document.body.removeChild(fileInput);
          });
      buttonGroup.appendChild(importButton);

      // Export Graph button
      var exportButton = document.createElement('button');
      exportButton.textContent = "Export Graph";
      exportButton.addEventListener(
          'click', function() {
            // Use cwrap to create a JavaScript function that calls the C++
            // function
            var exportGraph = Module.cwrap('exportGraph', 'string', []);

            // Call the export function and create a download link
            var jsonStr = exportGraph();
            var dataStr =
                "data:text/json;charset=utf-8," + encodeURIComponent(jsonStr);
            var downloadAnchorNode = document.createElement('a');
            downloadAnchorNode.setAttribute("href", dataStr);
            downloadAnchorNode.setAttribute("download", "graph.json");
            document.body.appendChild(downloadAnchorNode);
            downloadAnchorNode.click();
            downloadAnchorNode.remove();
          });
      buttonGroup.appendChild(exportButton);

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
      stepButton.addEventListener('click', function() { Module._stepTraversal(); });
      buttonGroup.appendChild(stepButton);

      // Traverse All button
      var fullButton = document.createElement('button');
      fullButton.textContent = "Traverse All";
      fullButton.addEventListener('click', function() { Module._fullTraversal(); });
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
    g.AddVertex("ID1", "gray", 500, 200);
    g.AddVertex("ID2", "gray", 430, 250);
    g.AddVertex("ID3", "gray", 570, 250);
    g.AddVertex("ID4", "gray", 380, 300);
    g.AddVertex("ID5", "gray", 480, 300);
    g.AddVertex("ID6", "gray", 520, 300);
    g.AddVertex("ID7", "gray", 620, 300);

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

  void RandomGraph(int vertices, int edges) {
    g.ClearGraph();
    for (int i = 0; i < vertices; i++) {
      int x = r.GetInt(0, CANVAS_WIDTH);
      int y = r.GetInt(0, CANVAS_HEIGHT);
      g.AddVertex(std::to_string(i), "blue", x, y);
    }

    for (int i = 0; i < edges; i++) {
      int index1 = r.GetInt(0, vertices - 1);
      int index2 = r.GetInt(0, vertices - 1);
      if (index1 != index2) {
        g.AddEdge(std::to_string(index1), std::to_string(index2), 2);
      }
    }

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

  /**
   * Updates the traversal with the correct mode
   */
  void UpdateTraversalMode(cse::Vertex<std::string> &start) {
    cse::GraphPosition<std::string> pos(g, start);

    int mode = EM_ASM_INT({
      var mode = document.getElementById("traversalMode").value;
      if (mode == "DFS")
        return 0;
      if (mode == "BFS")
        return 1;
      if (mode == "A*")
        return 2;
      return 0;
    });

    if (mode == 0)
      pos.SetTraversalMode(cse::TraversalModes::DFS<std::string>());
    else if (mode == 1)
      pos.SetTraversalMode(cse::TraversalModes::BFS<std::string>());
    else if (mode == 2)
      pos.SetTraversalMode(cse::TraversalModes::AStar<std::string>(g.GetVertex("ID6"))); // temp hardcoded target

    traversal.emplace(std::move(pos));
  }

  /**
   * Resets the traversal to have not traversed anything
   */
  void ClearTraversal() {
    if (traversal) {
      traversal.reset();
      RedrawCanvas();
    }
  }

  void AddVertex() {
    int id = g.GetVertices().size();
    int x = r.GetInt(0, CANVAS_WIDTH);
    int y = r.GetInt(0, CANVAS_HEIGHT);

    g.AddVertex(std::to_string(++id), "gray", x, y);
    ClearTraversal(); // Resets traversal when graph is modified
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
    auto &start = g.GetVertex("ID1");
    UpdateTraversalMode(start);
  }

  /**
   * Function to help delay the full traversal steps so the user can follow
   */
  // ChatGPT was used to help with the delay in traversal steps.
  static void StepTraversalAsync(void *arg) {
    auto *self = static_cast<GraphVisualizer *>(arg);
  
    if (self->traversal && self->traversal->AdvanceToNextNeighbor()) {
      self->RedrawCanvas();
      // Delay next step by 500ms
      emscripten_async_call(StepTraversalAsync, arg, 500);
    } else {
      self->RedrawCanvas();
      EM_ASM({
        setTimeout(function () {
          alert("Traversal Finished!");
        }, 150); // wait 150ms to let canvas update
      });
    }
  }
  
  /**
   * Traverse through a graph step by step when pressing the next step button
   */
  void StepTraversal() {
    if (!traversal.has_value()) StartTraversal();

    if (traversal && traversal->AdvanceToNextNeighbor()) {
      RedrawCanvas();
    } else {
      RedrawCanvas();
      EM_ASM({
        setTimeout(function () {
          alert("Traversal Finished!");
        }, 150); // wait 150ms to let canvas update
      });
    }
  }

  /**
   * Traverse all the way through a graph in one button press
   */
  void FullTraversal() {
    if (!traversal.has_value()) StartTraversal();

    // Kick off the async step loop
    emscripten_async_call(StepTraversalAsync, this, 0); // 0ms to start immediately
  }

  // Export the graph to JSON
  char *ExportGraph() {
    std::string jsonStr = graphJson.ExportToJson();
    char *result = (char *)malloc(jsonStr.length() + 1);
    strcpy(result, jsonStr.c_str());
    return result;
  }

  // Import the graph from JSON
  bool ImportGraph(const char *jsonStr) {
    bool success = graphJson.ImportFromJson(jsonStr);
    if (success) {
      RedrawCanvas();
    }
    return success;
  }
};

GraphVisualizer init{};

/**
 * Any functions that need to be called when something happens on the screen
 * like when a buttone is clicked must have an interface here
 */
extern "C" {
void clearCanvas() { init.ClearGraph(); }

void addVertex() { init.AddVertex(); }

void startTraversal() {
  init.StartTraversal();
}

void stepTraversal() {
  init.StepTraversal();
}

void fullTraversal() {
  init.FullTraversal();
}
  
void randomGraph(int vertices, int edges) {
  init.RandomGraph(vertices,edges);
}

void clearTraversal() {
  init.ClearTraversal();
}
  
void handleCanvasClick(double x, double y) { init.HandleCanvasClick(x, y); }

char *exportGraph() { return init.ExportGraph(); }

bool importGraph(const char *jsonStr) { return init.ImportGraph(jsonStr); }
}

int main() { return 0; }
