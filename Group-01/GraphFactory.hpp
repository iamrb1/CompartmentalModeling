#include "Graph/Graph.hpp"

class GraphFactory {
  public:
  static cse::Graph<std::string> DefaultGraph() {
    cse::Graph<std::string> g;

    // Initial values as example
    g.AddVertex("ID1", "gray", 500, 200);
    g.AddVertex("ID2", "gray", 400, 300);
    g.AddVertex("ID3", "gray", 600, 300);
    g.AddVertex("ID4", "gray", 350, 400);
    g.AddVertex("ID5", "gray", 450, 400);
    g.AddVertex("ID6", "gray", 550, 400);
    g.AddVertex("ID7", "gray", 650, 400);

    g.AddEdge("ID1", "ID2", 2);
    g.AddEdge("ID1", "ID3", 2);
    g.AddEdge("ID2", "ID4", 2);
    g.AddEdge("ID2", "ID5", 2);
    g.AddEdge("ID3", "ID6", 2);
    g.AddEdge("ID3", "ID7", 2);

    return g;
  }

  static cse::Graph<std::string> CycleGraph() {
    cse::Graph<std::string> g;
    // Create a cycle with 6 vertices
    int centerX = 1000;
    int centerY = 1000;
    int radius = 300;
    
    for(int i = 0; i < 6; i++) {
      double angle = i * (2 * M_PI / 6);
      int x = centerX + radius * cos(angle);
      int y = centerY + radius * sin(angle);
      g.AddVertex("C" + std::to_string(i), "gray", x, y);
    }
    
    for(int i = 0; i < 6; i++) {
      g.AddEdge("C" + std::to_string(i), "C" + std::to_string((i + 1) % 6), 1);
    }
    return g;
  }

  static cse::Graph<std::string> CompleteGraph() {
    cse::Graph<std::string> g;
    // Create K5 (complete graph with 5 vertices)
    int centerX = 1000;
    int centerY = 1000;
    int radius = 250;
    
    for(int i = 0; i < 5; i++) {
      double angle = i * (2 * M_PI / 5);
      int x = centerX + radius * cos(angle);
      int y = centerY + radius * sin(angle);
      g.AddVertex("K" + std::to_string(i), "gray", x, y);
    }
    
    // Connect every vertex to every other vertex
    for(int i = 0; i < 5; i++) {
      for(int j = i + 1; j < 5; j++) {
        g.AddEdge("K" + std::to_string(i), "K" + std::to_string(j), 1);
      }
    }
    return g;
  }

  static cse::Graph<std::string> BinaryTree() {
    cse::Graph<std::string> g;
    // Create a perfect binary tree with 3 levels
    g.AddVertex("B0", "gray", 1000, 200);  // Root
    g.AddVertex("B1", "gray", 700, 400);   // Level 1
    g.AddVertex("B2", "gray", 1300, 400);
    g.AddVertex("B3", "gray", 550, 600);   // Level 2
    g.AddVertex("B4", "gray", 850, 600);
    g.AddVertex("B5", "gray", 1150, 600);
    g.AddVertex("B6", "gray", 1450, 600);

    g.AddEdge("B0", "B1", 1);
    g.AddEdge("B0", "B2", 1);
    g.AddEdge("B1", "B3", 1);
    g.AddEdge("B1", "B4", 1);
    g.AddEdge("B2", "B5", 1);
    g.AddEdge("B2", "B6", 1);
    return g;
  }

  static cse::Graph<std::string> ButterflyGraph() {
    cse::Graph<std::string> g;
    // Create butterfly shape (two triangles sharing a vertex)
    g.AddVertex("F0", "gray", 1000, 1000); // Center
    g.AddVertex("F1", "gray", 800, 800);   // Left triangle
    g.AddVertex("F2", "gray", 800, 1200);
    g.AddVertex("F3", "gray", 1200, 800);  // Right triangle
    g.AddVertex("F4", "gray", 1200, 1200);

    // Left triangle
    g.AddEdge("F0", "F1", 1);
    g.AddEdge("F1", "F2", 1);
    g.AddEdge("F2", "F0", 1);
    // Right triangle
    g.AddEdge("F0", "F3", 1);
    g.AddEdge("F3", "F4", 1);
    g.AddEdge("F4", "F0", 1);
    return g;
  }

  static cse::Graph<std::string> GridGraph() {
    cse::Graph<std::string> g;
    // Create a 3x3 grid
    int spacing = 200;
    int startX = 800;
    int startY = 800;
    
    for(int i = 0; i < 3; i++) {
      for(int j = 0; j < 3; j++) {
        std::string id = "G" + std::to_string(i * 3 + j);
        g.AddVertex(id, "gray", startX + j * spacing, startY + i * spacing);
      }
    }
    
    // Add horizontal edges
    for(int i = 0; i < 3; i++) {
      for(int j = 0; j < 2; j++) {
        g.AddEdge("G" + std::to_string(i * 3 + j),
                 "G" + std::to_string(i * 3 + j + 1), 1);
      }
    }
    
    // Add vertical edges
    for(int i = 0; i < 2; i++) {
      for(int j = 0; j < 3; j++) {
        g.AddEdge("G" + std::to_string(i * 3 + j),
                 "G" + std::to_string((i + 1) * 3 + j), 1);
      }
    }
    return g;
  }
};