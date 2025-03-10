#include "GraphPosition.hpp"

namespace cse {

  void GraphPosition::MarkVisited(Vertex const &vertex) {
    visitedVertices.insert(&vertex); // Store the actual vertex object
    traversalPath.push_back(&vertex);
  }

  GraphPosition::GraphPosition(const Graph &g, Vertex const *startVertex, TraversalMode mode)
      : graph(g), currentVertex(startVertex), traversalMode(mode) {
    MarkVisited(*startVertex);
  }

  Vertex const &GraphPosition::GetCurrentVertex() const {
    return *currentVertex;
  }

  void GraphPosition::SetCurrentVertex(Vertex const &vertex) {
    currentVertex = &vertex;
    MarkVisited(*currentVertex);
  }

  bool GraphPosition::AdvanceToNextNeighbor() {
    // NEED TO MODIFY CODE TO SUPPORT DIFFERENT TRAVERSAL MODES
    // Get the current vertex's edges
    for (auto &[neighborID, weakEdge] : currentVertex->GetEdges()) {
      if (auto edge = weakEdge.lock()) { // Lock weak_ptr to access Edge
        Vertex const &neighbor = (edge->GetFrom() == *currentVertex) ? edge->GetTo() : edge->GetFrom();

        // If neighbor has not been visited, move to it
        if (visitedVertices.find(&neighbor) == visitedVertices.end()) {
          SetCurrentVertex(neighbor);
          return true;
        }
      }
    }

    // No unvisited neighbors found
    return false;
  }

  const std::vector<Vertex const *> &GraphPosition::GetTraversalPath() const {
    return traversalPath;
  }

  void GraphPosition::ResetTraversal(Vertex const &newStartVertex) {
    // Clear traversal history
    visitedVertices.clear();
    traversalPath.clear();

    // Set new starting position
    SetCurrentVertex(newStartVertex);
  }

  // NEED TO WRITE FULL FUNCTION
  GraphPosition &GraphPosition::operator++() {
    return *this;
  }

  // NEED TO WRITE FULL FUNCTION
  GraphPosition::operator bool() const {
    return false;
  }

} // namespace cse