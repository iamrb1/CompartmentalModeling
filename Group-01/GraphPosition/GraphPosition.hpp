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

  class GraphPosition {
  private:
    [[maybe_unused]] const Graph &graph;
    Vertex const *currentVertex;
    // Set of strings of visited vertices by ID
    std::set<std::string> visitedVertices;
    std::vector<Vertex const *> traversalPath;
    [[maybe_unused]] TraversalMode traversalMode = TraversalMode::DFS; // Default to DFS
    std::deque<Vertex const *> traversalQueue;                         // Needed for BFS

    void MarkVisited(Vertex const &vertex);

  public:
    GraphPosition(const Graph &g, Vertex const *startVertex, TraversalMode mode = TraversalMode::DFS);

    Vertex const &GetCurrentVertex() const;
    void SetCurrentVertex(Vertex const &vertex);
    bool AdvanceToNextNeighbor();
    const std::vector<Vertex const *> &GetTraversalPath() const;
    void ResetTraversal(Vertex const &newStartVertex);
    // NEED TO WRITE FUNCTION EXPRESSIONS NOT JUST DECLARE THEM
    GraphPosition &operator++();    // Advances to next vertex
    explicit operator bool() const; // Checks if more traversal is possible
  };

} // namespace cse