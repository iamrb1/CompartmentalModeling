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
    std::shared_ptr<Vertex> currentVertex;
    std::set<std::shared_ptr<Vertex>> visitedVertices;
    std::vector<std::shared_ptr<Vertex>> traversalPath;
    [[maybe_unused]] TraversalMode traversalMode = TraversalMode::DFS; // Default to DFS
    std::deque<std::shared_ptr<Vertex>> traversalQueue;                // Needed for BFS

    void MarkVisited(std::shared_ptr<Vertex> vertex);

  public:
    GraphPosition(const Graph &g, std::shared_ptr<Vertex> startVertex, TraversalMode mode = TraversalMode::DFS);

    std::shared_ptr<Vertex> GetCurrentVertex() const;
    void SetCurrentVertex(std::shared_ptr<Vertex> vertex);
    bool AdvanceToNextNeighbor();
    const std::vector<std::shared_ptr<Vertex>> &GetTraversalPath() const;
    void ResetTraversal(std::shared_ptr<Vertex> newStartVertex);
    // NEED TO WRITE FUNCTION EXPRESSIONS NOT JUST DECLARE THEM
    GraphPosition &operator++();    // Advances to next vertex
    explicit operator bool() const; // Checks if more traversal is possible
  };

} // namespace cse