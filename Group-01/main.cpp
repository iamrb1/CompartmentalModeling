#include <emscripten.h>

#include <cmath>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "GraphFactory.hpp"
#include "Graph/Graph.hpp"
#include "Graph/GraphJson.hpp"
#include "GraphPosition/GraphPosition.hpp"
#include "Random/Random.hpp"

const int VERTEX_RADIUS = 15;
const int CANVAS_WIDTH = 2000;
const int CANVAS_HEIGHT = 2000;
const double ARROW_SIZE = 25;  // Size of arrow head
const double ARROW_ANGLE = 0.5;  // Angle in radians (roughly 30 degrees)
const double ARROW_DELTA = 0.92;

class Shape {
private:
  static void drawArrowHead(int x1, int y1, int x2, int y2, const char* color) {
    EM_ASM_(
        {
          var ctx = document.getElementById('canvas').getContext('2d');
          ctx.fillStyle = UTF8ToString($4);
          
          var dX = $2 - $0;
          var dY = $3 - $1;
          var arrowSize = Math.sqrt(dX * dX + dY * dY);
          var arrowDelta = 1 - ($8/2/arrowSize);
          var startX = $0 + dX * arrowDelta;
          var startY = $1 + dY * arrowDelta;

          // Calculate arrow angles
          var angle = Math.atan2(dY, dX);
          
          // Calculate arrow points
          ctx.beginPath();
          ctx.moveTo(startX, startY);  // Tip of the arrow
          ctx.lineTo(
              startX - $5 * Math.cos(angle - $6),
              startY - $5 * Math.sin(angle - $6)
          );
          ctx.lineTo(
              startX - $5 * Math.cos(angle + $6),
              startY - $5 * Math.sin(angle + $6)
          );
          ctx.closePath();
          ctx.fill();
        },
        x1, y1, x2, y2, color, ARROW_SIZE, ARROW_ANGLE, ARROW_DELTA, VERTEX_RADIUS);
  }

public:
  static void drawLine(int x1, int y1, int x2, int y2, const char* color, int thickness = 2) {
    // Draw the main line
    EM_ASM_(
        {
          var ctx = document.getElementById('canvas').getContext('2d');
          ctx.strokeStyle = UTF8ToString($4);
          ctx.lineWidth = $5;
          ctx.beginPath();
          ctx.moveTo($0, $1);
          ctx.lineTo($2, $3);
          ctx.stroke();
        },
        x1, y1, x2, y2, color, thickness);
    
    // Add arrow at the end
    drawArrowHead(x1, y1, x2, y2, color);
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
  std::optional<std::reference_wrapper<const cse::Vertex<std::string>>> startVertex; // Traversal start vertex
  std::optional<std::reference_wrapper<const cse::Vertex<std::string>>> aStarDestVertex; // Astar traversal destination vertex
  bool currentlyTraversing = false;

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
      std::string color = "gray"; // default
      if (traversal) {
        if (&traversal->GetCurrentVertex() == v)
          color = "red";
        else if (traversal->IsVisited(*v))
          color = "green";
      }
      Shape::drawCircle(v->GetX(), v->GetY(), VERTEX_RADIUS, color.c_str());
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

  void CreateSelectedVertexSection() {
    EM_ASM_(
        {
        /** Selected vertex group */
        var selectedVertexDiv = document.createElement('div');
        selectedVertexDiv.setAttribute("id", "selectedVertexContainer");
        var selectedVertexTitle = document.createElement('h2');
        selectedVertexTitle.setAttribute("id", "selectedVertexTitle");
        var selectedVertexId = document.createElement('h3');
        selectedVertexId.setAttribute("id", "selectedVertexId");
        var selectedVertexX = document.createElement('h3');
        selectedVertexX.setAttribute("id", "selectedVertexX");
        var selectedVertexY = document.createElement('h3');
        selectedVertexY.setAttribute("id", "selectedVertexY");

        selectedVertexDiv.appendChild(selectedVertexTitle);
        selectedVertexDiv.appendChild(selectedVertexId);
        selectedVertexDiv.appendChild(selectedVertexX);
        selectedVertexDiv.appendChild(selectedVertexY);

        selectedVertexTitle.innerHTML = "No Selected Vertex";

        // Start traversal vertex display info
        var startVertexDiv = document.createElement('div');
        startVertexDiv.id = "startVertexInfo";
        var startVertexTitle = document.createElement('h2');
        startVertexTitle.setAttribute("id", "startVertexTitle");
        startVertexTitle.innerHTML = "Start Vertex";
        var startVertexId = document.createElement('h3');
        startVertexId.setAttribute("id", "startVertexId");
        startVertexDiv.appendChild(startVertexTitle);
        startVertexDiv.appendChild(startVertexId);

        // A* destination traversal vertex display info
        var aStarDestVertexDiv = document.createElement('div');
        aStarDestVertexDiv.id = "aStarDestVertexInfo";
        var aStarDestVertexTitle = document.createElement('h2');
        aStarDestVertexTitle.setAttribute("id", "aStarDestVertexTitle");
        aStarDestVertexTitle.innerHTML = "A* Destination Vertex";
        var aStarDestVertexId = document.createElement('h3');
        aStarDestVertexId.setAttribute("id", "aStarDestVertexId");
        aStarDestVertexDiv.appendChild(aStarDestVertexTitle);
        aStarDestVertexDiv.appendChild(aStarDestVertexId);

        
        var selectedOptionsDiv = document.createElement('div');
        selectedOptionsDiv.classList.add("control-section");
        
        var selectedOptionsDivContainer = document.createElement('div');
        selectedOptionsDivContainer.classList.add("control-section-container");
        /** HEADER */
        var selectedVertexGroupHeader = document.createElement('h2');
        selectedVertexGroupHeader.setAttribute("id", "selectedVertexGroupHeader");
        selectedVertexGroupHeader.innerHTML = "Currently Selected Information";

        selectedOptionsDiv.appendChild(selectedVertexDiv);
        selectedOptionsDiv.appendChild(startVertexDiv);
        selectedOptionsDiv.appendChild(aStarDestVertexDiv);
        selectedOptionsDivContainer.appendChild(selectedVertexGroupHeader);
        selectedOptionsDivContainer.appendChild(selectedOptionsDiv);

        window.selectedVertices = selectedOptionsDivContainer;
      /** END -- Selected vertex group */
        });
  }

  void CreateTraversalSection() {
    EM_ASM({
      var container = document.createElement('div');
      container.classList.add("control-section-container");
      
      var header = document.createElement('h2');
      header.id = "traversalSectionContainerHeader";
      header.innerHTML = "Traversal Options";
      
      var optionsDiv = document.createElement("div");
      optionsDiv.classList.add("control-section");
      
      // Create mode selection group
      var modeGroup = document.createElement("div");
      modeGroup.classList.add("button-group");
      
      var traversalSelect = document.createElement('select');
      traversalSelect.setAttribute("id", "traversalMode");
      
      // Add DFS option
      var dfsOption = document.createElement('option');
      dfsOption.value = "DFS";
      dfsOption.textContent = "DFS";
      traversalSelect.appendChild(dfsOption);
      
      // Add BFS option
      var bfsOption = document.createElement('option');
      bfsOption.value = "BFS";
      bfsOption.textContent = "BFS";
      traversalSelect.appendChild(bfsOption);
      
      // Add A* option
      var astarOption = document.createElement('option');
      astarOption.value = "A*";
      astarOption.textContent = "A*";
      traversalSelect.appendChild(astarOption);
      
      // Container for checkboxes
      var checkboxContainer = document.createElement('div');
      checkboxContainer.id = 'selectedCheckboxContainer';

      // Select start vertex checkbox
      var selectStartCheckbox = document.createElement('input');
      selectStartCheckbox.type = 'checkbox';
      selectStartCheckbox.id = 'selectStartToggle';
      var selectStartLabel = document.createElement('label');
      selectStartLabel.htmlFor = 'selectStartToggle';
      selectStartLabel.textContent = ' Select Start Vertex';
      
      checkboxContainer.appendChild(selectStartCheckbox);
      checkboxContainer.appendChild(selectStartLabel);

      // Select A* destination vertex checkbox
      var selectADestCheckbox = document.createElement('input');
      selectADestCheckbox.type = 'checkbox';
      selectADestCheckbox.id = 'selectADestToggle';
      var selectADestLabel = document.createElement('label');
      selectADestLabel.htmlFor = 'selectADestToggle';
      selectADestLabel.textContent = ' Select A* Destination Vertex';
      
      checkboxContainer.appendChild(selectADestCheckbox);
      checkboxContainer.appendChild(selectADestLabel);

      modeGroup.appendChild(traversalSelect);
      modeGroup.appendChild(checkboxContainer);
      
      // Create control buttons group
      var controlGroup = document.createElement("div");
      controlGroup.classList.add("button-group");
      
      var stepButton = document.createElement('button');
      stepButton.id = "stepTraversalButton";
      stepButton.textContent = "Next Step";
      stepButton.addEventListener('click', function() { Module._stepTraversal(); });
      
      var fullButton = document.createElement('button');
      fullButton.id = "startTraversalButton";
      fullButton.textContent = "Traverse All";
      fullButton.addEventListener('click', function() { Module._fullTraversal(); });
      
      var clearButton = document.createElement('button');
      clearButton.textContent = "Clear Traversal";
      clearButton.addEventListener('click', function() { Module._clearTraversal(); });
      
      controlGroup.appendChild(stepButton);
      controlGroup.appendChild(fullButton);
      controlGroup.appendChild(clearButton);
      
      optionsDiv.appendChild(modeGroup);
      optionsDiv.appendChild(controlGroup);
      
      container.appendChild(header);
      container.appendChild(optionsDiv);
      
      window.traversalSection = container;
    });
  }

  void CreateGraphManipulationSection() {
    EM_ASM({
      var container = document.createElement('div');
      container.classList.add("control-section-container");
      
      var header = document.createElement('h2');
      header.innerHTML = "Graph Manipulation";
      
      var manipulationDiv = document.createElement("div");
      manipulationDiv.classList.add("control-section");

      // Vertex Add Group
      var addGroup = document.createElement("div");
      addGroup.classList.add("button-group");
      
      var idInput = document.createElement("input");
      idInput.id = "vertexIdInput";
      idInput.placeholder = "ID";
      idInput.type = "text";
      
      var xInput = document.createElement("input");
      xInput.id = "vertexXInput";
      xInput.placeholder = "X";
      xInput.type = "number";
      
      var yInput = document.createElement("input");
      yInput.id = "vertexYInput";
      yInput.placeholder = "Y";
      yInput.type = "number";
      
      var addButton = document.createElement("button");
      addButton.textContent = "Add Vertex";
      addButton.addEventListener("click", function() {
        var id = document.getElementById("vertexIdInput").value;
        var x = parseInt(document.getElementById("vertexXInput").value);
        var y = parseInt(document.getElementById("vertexYInput").value);
        if (id && !isNaN(x) && !isNaN(y)) {
          Module._addVertexWithParams(stringToNewUTF8(id), x, y);
        } else {
          alert("Please fill out all fields.");
        }
      });
      
      addGroup.appendChild(idInput);
      addGroup.appendChild(xInput);
      addGroup.appendChild(yInput);
      addGroup.appendChild(addButton);

      // Vertex Delete Group
      var deleteGroup = document.createElement("div");
      deleteGroup.classList.add("button-group");
      
      var deleteInput = document.createElement("input");
      deleteInput.id = "deleteVertexIdInput";
      deleteInput.placeholder = "ID of vertex to delete";
      deleteInput.type = "text";
      
      var deleteButton = document.createElement("button");
      deleteButton.textContent = "Delete Vertex";
      deleteButton.addEventListener("click", function() {
        var id = document.getElementById("deleteVertexIdInput").value;
        if (id) {
          Module._deleteVertex(stringToNewUTF8(id));
        } else {
          alert("Please enter a valid vertex ID.");
        }
      });
      
      deleteGroup.appendChild(deleteInput);
      deleteGroup.appendChild(deleteButton);

      // Edge Group
      var edgeGroup = document.createElement("div");
      edgeGroup.classList.add("button-group");
      
      var vertex1Input = document.createElement("input");
      vertex1Input.id = "vertexId1Input";
      vertex1Input.placeholder = "Vertex ID 1";
      vertex1Input.type = "text";
      
      var vertex2Input = document.createElement("input");
      vertex2Input.id = "vertexId2Input";
      vertex2Input.placeholder = "Vertex ID 2";
      vertex2Input.type = "text";
      
      var weightInput = document.createElement("input");
      weightInput.id = "edgeWeightInput";
      weightInput.placeholder = "Edge Weight";
      weightInput.type = "number";
      
      var edgeButton = document.createElement("button");
      edgeButton.textContent = "Toggle Edge";
      edgeButton.addEventListener("click", function() {
        var id1 = document.getElementById("vertexId1Input").value;
        var id2 = document.getElementById("vertexId2Input").value;
        var weight = parseInt(document.getElementById("edgeWeightInput").value);
        if (id1 && id2 && !isNaN(weight)) {
          Module._toggleEdge(stringToNewUTF8(id1), stringToNewUTF8(id2), weight);
        } else {
          alert("Please fill out both vertex IDs and the weight.");
        }
      });
      
      edgeGroup.appendChild(vertex1Input);
      edgeGroup.appendChild(vertex2Input);
      edgeGroup.appendChild(weightInput);
      edgeGroup.appendChild(edgeButton);

      // Add all groups to the main div
      manipulationDiv.appendChild(addGroup);
      manipulationDiv.appendChild(deleteGroup);
      manipulationDiv.appendChild(edgeGroup);
      
      container.appendChild(header);
      container.appendChild(manipulationDiv);
      
      window.graphManipulationSection = container;
    });
  }

  void CreateGraphGenerationSection() {
    EM_ASM({
      var container = document.createElement('div');
      container.classList.add("control-section-container");
      
      var header = document.createElement('h2');
      header.innerHTML = "Graph Generation & Import/Export";
      
      var generationDiv = document.createElement("div");
      generationDiv.classList.add("control-section");
      
      // Generation controls
      var generationGroup = document.createElement("div");
      generationGroup.classList.add("button-group");
      
      // Sample Graph Selection
      var sampleGraphSelect = document.createElement('select');
      sampleGraphSelect.id = "sampleGraphSelect";
      
      var defaultOption = document.createElement('option');
      defaultOption.value = "DefaultGraph";
      defaultOption.textContent = "Default Tree";
      
      var cycleOption = document.createElement('option');
      cycleOption.value = "CycleGraph";
      cycleOption.textContent = "Cycle Graph";
      
      var completeOption = document.createElement('option');
      completeOption.value = "CompleteGraph";
      completeOption.textContent = "Complete Graph";
      
      var binaryOption = document.createElement('option');
      binaryOption.value = "BinaryTree";
      binaryOption.textContent = "Binary Tree";
      
      var butterflyOption = document.createElement('option');
      butterflyOption.value = "ButterflyGraph";
      butterflyOption.textContent = "Butterfly Graph";
      
      var gridOption = document.createElement('option');
      gridOption.value = "GridGraph";
      gridOption.textContent = "Grid Graph";
      
      sampleGraphSelect.appendChild(defaultOption);
      sampleGraphSelect.appendChild(cycleOption);
      sampleGraphSelect.appendChild(completeOption);
      sampleGraphSelect.appendChild(binaryOption);
      sampleGraphSelect.appendChild(butterflyOption);
      sampleGraphSelect.appendChild(gridOption);
      
      var useSampleButton = document.createElement('button');
      useSampleButton.textContent = "Use Sample Graph";
      useSampleButton.addEventListener('click', function() {
        var selected = document.getElementById("sampleGraphSelect").value;
        switch(selected) {
          case "DefaultGraph": Module._chooseSampleGraph(0); break;
          case "CycleGraph": Module._chooseSampleGraph(1); break;
          case "CompleteGraph": Module._chooseSampleGraph(2); break;
          case "BinaryTree": Module._chooseSampleGraph(3); break;
          case "ButterflyGraph": Module._chooseSampleGraph(4); break;
          case "GridGraph": Module._chooseSampleGraph(5); break;
        }
      });

      generationGroup.appendChild(sampleGraphSelect);
      generationGroup.appendChild(useSampleButton);
      
      var clearButton = document.createElement('button');
      clearButton.textContent = "Clear Graph";
      clearButton.addEventListener('click', function() { Module._clearCanvas(); });
      
      var vertexLabel = document.createElement('label');
      vertexLabel.setAttribute("for", "vertexCount");
      vertexLabel.innerText = "Vertices: ";
      var vertexInput = document.createElement('input');
      vertexInput.setAttribute("id", "vertexCount");
      vertexInput.setAttribute("type", "number");
      vertexInput.setAttribute("min", "1");
      vertexInput.setAttribute("value", "1");

      var edgeLabel = document.createElement('label');
      edgeLabel.setAttribute("for", "edgeCount");
      edgeLabel.innerText = "Edges: ";
      var edgeInput = document.createElement('input');
      edgeInput.setAttribute("id", "edgeCount");
      edgeInput.setAttribute("type", "number");
      edgeInput.setAttribute("min", "0");
      edgeInput.setAttribute("value", "0");

      var randomGraphButton = document.createElement('button');
      randomGraphButton.textContent = "Random Graph";
      randomGraphButton.addEventListener('click', function() {
        var vertices = parseInt(document.getElementById("vertexCount").value);
        var edges = parseInt(document.getElementById("edgeCount").value);
        Module._randomGraph(vertices, edges);
      });

      generationGroup.appendChild(clearButton);
      generationGroup.appendChild(vertexLabel);
      generationGroup.appendChild(vertexInput);
      generationGroup.appendChild(edgeLabel);
      generationGroup.appendChild(edgeInput);
      generationGroup.appendChild(randomGraphButton);

      // Import/Export controls
      var importExportGroup = document.createElement("div");
      importExportGroup.classList.add("button-group");

      var importButton = document.createElement('button');
      importButton.textContent = "Import Graph";
      importButton.addEventListener('click', function() {
        var fileInput = document.createElement('input');
        fileInput.type = 'file';
        fileInput.accept = '.json';
        fileInput.style.display = 'none';
        document.body.appendChild(fileInput);

        fileInput.addEventListener('change', function(e) {
          var file = e.target.files[0];
          if (!file) return;

          var reader = new FileReader();
          reader.onload = function(e) {
            var contents = e.target.result;
            Module._importGraph(stringToNewUTF8(contents));
          };
          reader.readAsText(file);
        });

        fileInput.click();
        document.body.removeChild(fileInput);
      });

      var exportButton = document.createElement('button');
      exportButton.textContent = "Export Graph";
      exportButton.addEventListener('click', function() {
        var jsonStr = UTF8ToString(Module._exportGraph());
        console.log(jsonStr);
        var dataStr = "data:text/json;charset=utf-8," + encodeURIComponent(jsonStr);
        var downloadAnchorNode = document.createElement('a');
        downloadAnchorNode.setAttribute("href", dataStr);
        downloadAnchorNode.setAttribute("download", "graph.json");
        document.body.appendChild(downloadAnchorNode);
        downloadAnchorNode.click();
        downloadAnchorNode.remove();
      });

      importExportGroup.appendChild(importButton);
      importExportGroup.appendChild(exportButton);

      generationDiv.appendChild(generationGroup);
      generationDiv.appendChild(importExportGroup);
      
      container.appendChild(header);
      container.appendChild(generationDiv);
      
      window.graphGenerationSection = container;
    });
  }

  void InitializeControlZone() {
    EM_ASM({
      var mainElement = document.getElementById("main");
      var controlZone = document.createElement("div");
      controlZone.classList.add("control-zone");
      
      // Add all sections
      controlZone.appendChild(window.selectedVertices);
      controlZone.appendChild(window.traversalSection);
      controlZone.appendChild(window.graphManipulationSection);
      controlZone.appendChild(window.graphGenerationSection);
      
      mainElement.appendChild(controlZone);
    });
  }

public:
  GraphVisualizer() {
    // Initialize UI components
    CreateSelectedVertexSection();
    CreateTraversalSection();
    CreateGraphManipulationSection();
    CreateGraphGenerationSection();
    InitializeControlZone();
    
    // Initialize canvas and sample graph
    InitiateCanvas();
    ChooseSampleGraph(GraphFactory::GridGraph);
  }

  void ChooseSampleGraph(auto func){
    g = func();
    traversal.reset();
    ClearVertexSelection();
    ClearTraversal();
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
    RedrawCanvas();
  }

  void ClearGraph() {
    g.ClearGraph();
    ClearTraversal();
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
    else if (mode == 2) {
      if (!aStarDestVertex.has_value()) {
        EM_ASM({ alert("Please select a destination vertex before starting A* traversal."); });
        return;
      }
      auto& dest = const_cast<cse::Vertex<std::string>&>(aStarDestVertex->get());
      pos.SetTraversalMode(cse::TraversalModes::AStar<std::string>(dest));
    }
    traversal.emplace(std::move(pos));
  }

  /**
   * Resets the traversal to have not traversed anything
   */
  void ClearTraversal() {
    startVertex.reset();
    aStarDestVertex.reset();
    EM_ASM({
      document.getElementById("startVertexId").innerHTML = "";
      document.getElementById("aStarDestVertexId").innerHTML = "";
      var stepBtn = document.getElementById("stepTraversalButton");
      var fullBtn = document.getElementById("startTraversalButton");
      stepBtn.style.backgroundColor = "#3f3f3f";
      fullBtn.style.backgroundColor = "#3f3f3f";
    });

    currentlyTraversing = false;

    if (traversal) {
      traversal.reset();
      RedrawCanvas();
    }
  }

  // Helper function that adds a vertex to the graph and paints it
  void AddVertex() {
    int id = g.GetVertices().size();
    int x = r.GetInt(0, CANVAS_WIDTH);
    int y = r.GetInt(0, CANVAS_HEIGHT);

    g.AddVertex(std::to_string(++id), "gray", x, y);
    ClearTraversal(); // Resets traversal when graph is modified
  }

  // Allows the user to delete a vertex of their choosing
  void DeleteVertex(const char *id) {
    std::string idCpp(id);
    if (!g.HasVertex(idCpp)) {
      EM_ASM({ alert("Error: Vertex with this ID does not exist!"); });
      return;
    }
    g.RemoveVertex(idCpp);
    RedrawCanvas();
  }

  // Allows the user to add and/or delete an edge between 2 verticies
  void ToggleEdge(const char *id1, const char *id2, int weight) {
    std::string idCpp1(id1);
    std::string idCpp2(id2);
    const std::string edgeId = idCpp1 + "-" + idCpp2;
    if (g.HasEdge(edgeId)) {
      g.RemoveEdge(edgeId);
    } else if (!g.HasVertex(idCpp1) || !g.HasVertex(idCpp2)) {
      EM_ASM({ alert("Error: Entered vertices must exist!"); });
    } else {
      g.AddEdge(idCpp1, idCpp2, weight);
    }
    RedrawCanvas();
  }

  // Allows the user to add a vertex where they chose
  void AddVertexWithParams(const char *id, int x, int y) {
    std::string idCpp(id);
    if (g.HasVertex(idCpp)) {
      EM_ASM({ alert("Error: Vertex with this ID already exists!"); });
      return;
    }
    g.AddVertex(idCpp, "blue", x, y);
    RedrawCanvas();
  }

  // Receives the virtual coordinate inside of the canvas.
  void HandleCanvasClick(double x, double y) {
    auto vertex = FindVertexAtPosition(x, y);
    if (vertex.has_value()) {
      // Check if the select start toggle is on 
      bool selectStart = EM_ASM_INT({
        return document.getElementById("selectStartToggle").checked;
      });

      // Check if the select Astar destination toggle is on 
      bool selectADest = EM_ASM_INT({
        return document.getElementById("selectADestToggle").checked;
      });

      // Show alert and don't allow if trying to select both starting vertex and Astar destination vertex.
      if (selectADest && selectStart) {
        EM_ASM({
          alert("Can't select both starting vertex and Astar destination at the same time. Please unselect one.");
        });
        return;
      }
  
      if (selectStart) {
        if (currentlyTraversing) {
          // Show alert if trying to change start during traversal
          EM_ASM({
            alert("Traversal already in progress. Clear traversal to change the starting vertex.");
          });
          return;
        }

        startVertex = vertex.value(); // Store selected vertex
        // Update UI to reflect selected start vertex
        EM_ASM_({
          var stepBtn = document.getElementById("stepTraversalButton");
          var fullBtn = document.getElementById("startTraversalButton");
          stepBtn.style.backgroundColor = "#4361ee";
          fullBtn.style.backgroundColor = "#4361ee";
          document.getElementById("startVertexId").innerHTML = "ID: " + UTF8ToString($0);
        }, vertex->get().GetId().c_str());
      } else if (selectADest){   
        // Get the traversal type to make sure it's Astar
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
        if (mode != 2) {
          // Show alert if trying to change Astar destination and not in an A* traversal mode
          EM_ASM({
            alert("You cannot select an Astar destination vertex when not in Astar traversal mode.");
          });
          return;
        }
        if (currentlyTraversing) {
          // Show alert if trying to change Astar destination during traversal
          EM_ASM({
            alert("Traversal already in progress. Clear traversal to change the Astar destination vertex.");
          });
          return;
        }

        aStarDestVertex = vertex.value(); // Store Astar destination vertex

        // Update UI to reflect selected Astar destination vertex
        EM_ASM_({
          var stepBtn = document.getElementById("stepTraversalButton");
          var fullBtn = document.getElementById("startTraversalButton");
          stepBtn.style.backgroundColor = "#4361ee";
          fullBtn.style.backgroundColor = "#4361ee";
          document.getElementById("aStarDestVertexId").innerHTML = "ID: " + UTF8ToString($0);
        }, vertex->get().GetId().c_str());
      } else {
        HandleSelectedVertex(vertex->get()); // Just show info
      }    
    } else {
      ClearVertexSelection();
    }
  }

  void StartTraversal() {
    if (g.GetVertices().empty()) return;
    auto& start = startVertex.has_value() ? const_cast<cse::Vertex<std::string>&>(startVertex->get())
                                        : g.GetVertex("ID1");
    UpdateTraversalMode(start);
    currentlyTraversing = true;
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
        setTimeout(function() { alert("Traversal Finished!"); }, 150); // wait 150ms to let canvas update
      });
    }
  }

  /**
   * Traverse through a graph step by step when pressing the next step button
   */
  void StepTraversal() {
    if (!startVertex.has_value()) {
      EM_ASM({
        alert("Please select a starting vertex with the check box below before stepping through the traversal.");
      });
      return;
    }

    // make sure Astar conditions satisfied first
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
    if (mode == 2 && !aStarDestVertex.has_value()) {
      // Show alert if trying to start an Astar traversal without a destination
      EM_ASM({
        alert("You cannot start an Astar traversal without selecting a destination.");
      });
      return;
    }

    if (g.HasCycle()) {
      EM_ASM({
        alert("Can't traverse a Graph with a cycle");
      });
      return;
    }

    
    if (!traversal.has_value()) {
      StartTraversal();
      std::cout << "Started traversal" << std::endl;
    };

    std::cout << traversal.value().GetCurrentVertex() << std::endl;
    if (traversal && traversal->AdvanceToNextNeighbor()) {
      RedrawCanvas();
    } else {
      RedrawCanvas();
      EM_ASM({
        setTimeout(function() { alert("Traversal Finished!"); }, 150); // wait 150ms to let canvas update
      });
    }
  }

  /**
   * Traverse all the way through a graph in one button press
   */
  void FullTraversal() {
    if (!startVertex.has_value()) {
      EM_ASM({
        alert("Please select a starting vertex before starting the traversal.");
      });
      return;
    }

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
void clearCanvas() {
  init.ClearGraph();
}

void addVertex() {
  init.AddVertex();
}

void startTraversal() {
  init.StartTraversal();
}

void stepTraversal() {
  init.StepTraversal();
}

void toggleEdge(const char *id1, const char *id2, int weight) {
  init.ToggleEdge(id1, id2, weight);
}

void addVertexWithParams(const char *id, int x, int y) {
  init.AddVertexWithParams(id, x, y);
}

void deleteVertex(const char *id) {
  init.DeleteVertex(id);
}

void fullTraversal() {
  init.FullTraversal();
}

void randomGraph(int vertices, int edges) {
  init.RandomGraph(vertices, edges);
}

void clearTraversal() {
  init.ClearTraversal();
}

void handleCanvasClick(double x, double y) {
  init.HandleCanvasClick(x, y);
}

char *exportGraph() {
  return init.ExportGraph();
}

bool importGraph(const char *jsonStr) {
  return init.ImportGraph(jsonStr);
}

void chooseSampleGraph(int type) {
  switch(type) {
    case 0: init.ChooseSampleGraph(GraphFactory::DefaultGraph); break;
    case 1: init.ChooseSampleGraph(GraphFactory::CycleGraph); break;
    case 2: init.ChooseSampleGraph(GraphFactory::CompleteGraph); break;
    case 3: init.ChooseSampleGraph(GraphFactory::BinaryTree); break;
    case 4: init.ChooseSampleGraph(GraphFactory::ButterflyGraph); break;
    case 5: init.ChooseSampleGraph(GraphFactory::GridGraph); break;
  }
}
}

int main() {
  return 0;
}
