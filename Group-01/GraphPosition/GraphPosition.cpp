#include "GraphPosition.hpp"

namespace cse {

  void GraphPosition::MarkVisited(Vertex const &vertex) {
    visitedVertices.insert(vertex.GetId()); // Store the actual vertex object
    traversalPath.push_back(&vertex);
  }

  GraphPosition::GraphPosition(const Graph &g, Vertex const *startVertex, TraversalMode mode)
      : graph(g), currentVertex(startVertex), traversalMode(mode) {
    if (!startVertex) {
      throw std::invalid_argument("GraphPosition must be initialized with a non-null vertex!");
    }
    MarkVisited(*startVertex);
  }

  Vertex const &GraphPosition::GetCurrentVertex() const {
    assert(currentVertex != nullptr && "GetCurrentVertex() should never return nullptr!");
    return *currentVertex;
  }

  void GraphPosition::SetCurrentVertex(Vertex const &vertex) {
    currentVertex = &vertex;
    MarkVisited(*currentVertex);
  }

  bool GraphPosition::AdvanceToNextNeighbor() {
    // NEED TO MODIFY CODE TO SUPPORT DIFFERENT TRAVERSAL MODES
    // Get the current vertex's edges
    assert(currentVertex != nullptr && "Current vertex should never be nullptr!");
    for (auto &[neighborID, weakEdge] : currentVertex->GetEdges()) {
      if (auto edge = weakEdge.lock()) { // Lock weak_ptr to access Edge
        cse::Vertex const &neighbor = edge->GetTo();

        // If neighbor has not been visited, move to it
        if (visitedVertices.find(neighbor.GetId()) == visitedVertices.end()) {
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