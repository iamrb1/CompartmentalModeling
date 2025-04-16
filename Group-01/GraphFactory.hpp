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
};