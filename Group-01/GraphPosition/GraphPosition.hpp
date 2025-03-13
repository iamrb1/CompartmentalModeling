#pragma once

#include "../Graph/Graph.hpp"
#include <cassert>
#include <deque>
#include <iostream>
#include <set>
#include <stdexcept>
#include <vector>

namespace cse {

  enum class TraversalMode { DFS, BFS, RANDOM, WEIGHTED };

  template <typename VERTEX_DATA_T>
  class GraphPosition {
  private:
    const Graph<VERTEX_DATA_T> &graph;
    const Vertex<VERTEX_DATA_T> *currentVertex;
    // Set of strings of visited vertices by ID
    std::set<std::string> visitedVertices;
    std::vector<const Vertex<VERTEX_DATA_T> *> traversalPath;
    [[maybe_unused]] TraversalMode traversalMode = TraversalMode::DFS; // Default to DFS
    std::deque<const Vertex<VERTEX_DATA_T> *> traversalQueue           // Needed for BFS

    void MarkVisited(const Vertex<VERTEX_DATA_T> &vertex);

  public:
    GraphPosition(const Graph<VERTEX_DATA_T> &g, const Vertex<VERTEX_DATA_T> *startVertex, TraversalMode mode = TraversalMode::DFS);

    const Vertex<VERTEX_DATA_T> &GetCurrentVertex() const;
    void SetCurrentVertex(const Vertex<VERTEX_DATA_T> &vertex);
    bool AdvanceToNextNeighbor();
    const std::vector<const Vertex<VERTEX_DATA_T> *> &GetTraversalPath() const;
    void ResetTraversal(const Vertex<VERTEX_DATA_T> &newStartVertex);
    // NEED TO WRITE FUNCTION EXPRESSIONS NOT JUST DECLARE THEM
    GraphPosition &operator++();    // Advances to next vertex
    explicit operator bool() const; // Checks if more traversal is possible
  };


  // Function Implementations

  template <typename VERTEX_DATA_T>
  void GraphPosition<VERTEX_DATA_T>::MarkVisited(const Vertex<VERTEX_DATA_T> &vertex) {
    visitedVertices.insert(vertex.GetId()); // Store the actual vertex object
    traversalPath.push_back(&vertex);
  }

  template <typename VERTEX_DATA_T>
  GraphPosition<VERTEX_DATA_T>::GraphPosition(const Graph<VERTEX_DATA_T> &g, const Vertex<VERTEX_DATA_T> *startVertex, TraversalMode mode)
      : graph(g), currentVertex(startVertex), traversalMode(mode) {
    if (!startVertex) {
      throw std::invalid_argument("GraphPosition must be initialized with a non-null vertex!");
    }
    MarkVisited(*startVertex);
  }

  template <typename VERTEX_DATA_T>
  const Vertex<VERTEX_DATA_T> &GraphPosition<VERTEX_DATA_T>::GetCurrentVertex() const {
    assert(currentVertex != nullptr && "GetCurrentVertex() should never return nullptr!");
    return *currentVertex;
  }

  template <typename VERTEX_DATA_T>
  void GraphPosition<VERTEX_DATA_T>::SetCurrentVertex(const Vertex<VERTEX_DATA_T> &vertex) {
    currentVertex = &vertex;
    MarkVisited(*currentVertex);
  }

  // NEED TO MODIFY CODE TO SUPPORT DIFFERENT TRAVERSAL MODES
  template <typename VERTEX_DATA_T>
  bool GraphPosition<VERTEX_DATA_T>::AdvanceToNextNeighbor() {
    assert(currentVertex != nullptr && "Current vertex should never be nullptr!");

    for (auto &[neighborID, weakEdge] : currentVertex->GetEdges()) {
      if (auto edge = weakEdge.lock()) {
        const Vertex<VERTEX_DATA_T> &neighbor = edge->GetTo();

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

  template <typename VERTEX_DATA_T>
  const std::vector<const Vertex<VERTEX_DATA_T> *> &GraphPosition<VERTEX_DATA_T>::GetTraversalPath() const {
    return traversalPath;
  }

  template <typename VERTEX_DATA_T>
  void GraphPosition<VERTEX_DATA_T>::ResetTraversal(const Vertex<VERTEX_DATA_T> &newStartVertex) {
    visitedVertices.clear();
    traversalPath.clear();
    SetCurrentVertex(newStartVertex);
  }

  template <typename VERTEX_DATA_T>
  GraphPosition<VERTEX_DATA_T> &GraphPosition<VERTEX_DATA_T>::operator++() {
    return *this;
  }

  template <typename VERTEX_DATA_T>
  GraphPosition<VERTEX_DATA_T>::operator bool() const {
    return false;
  }

} // namespace cse