#include "GraphPosition.hpp"

namespace cse {

  void GraphPosition::MarkVisited(std::shared_ptr<Vertex> vertex) {
    assert(vertex != nullptr && "MarkVisited must be given a valid vertex!");
    visitedVertices.insert(vertex); // Store the actual vertex object
    traversalPath.push_back(vertex);
  }

  GraphPosition::GraphPosition(const Graph &g, std::shared_ptr<Vertex> startVertex)
      : graph(g), currentVertex(startVertex) {
    if (!startVertex) {
      throw std::invalid_argument("GraphPosition must be initialized with a non-null vertex!");
    }
    MarkVisited(startVertex);
  }

  std::shared_ptr<Vertex> GraphPosition::GetCurrentVertex() const {
    assert(currentVertex != nullptr && "GetCurrentVertex() should never return nullptr!");
    return currentVertex;
  }

  void GraphPosition::SetCurrentVertex(std::shared_ptr<Vertex> vertex) {
    assert(vertex != nullptr && "SetCurrentVertex must receive a non-null vertex!");
    currentVertex = vertex;
    MarkVisited(currentVertex);
  }

  bool GraphPosition::AdvanceToNextNeighbor() {
    assert(currentVertex != nullptr && "Current vertex should never be nullptr!");
    // Get the current vertex's edges
    for (auto &[neighborID, weakEdge] : currentVertex->GetEdges()) {
      if (auto edge = weakEdge.lock()) { // Lock weak_ptr to access Edge
        std::shared_ptr<Vertex> neighbor = (edge->GetFrom() == currentVertex) ? edge->GetTo() : edge->GetFrom();
        assert(neighbor != nullptr && "Neighbor vertex should not be nullptr!");

        // If neighbor has not been visited, move to it
        if (visitedVertices.find(neighbor) == visitedVertices.end()) {
          currentVertex = neighbor;
          MarkVisited(currentVertex);
          return true;
        }
      }
    }

    // No unvisited neighbors found
    return false;
  }

  const std::vector<std::shared_ptr<Vertex>> &GraphPosition::GetTraversalPath() const {
    return traversalPath;
  }

  void GraphPosition::ResetTraversal(std::shared_ptr<Vertex> newStartVertex) {
    assert(newStartVertex != nullptr && "ResetTraversal must receive a non-null vertex!");

    // Clear traversal history
    visitedVertices.clear();
    traversalPath.clear();

    // Set new starting position
    currentVertex = newStartVertex;
    MarkVisited(currentVertex);
}

} // namespace cse