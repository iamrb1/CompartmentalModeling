#pragma once

#include "../Graph/Graph.hpp"
#include <cassert>
#include <iostream>
#include <set>
#include <stdexcept>
#include <vector>

namespace cse {

  class GraphPosition {
  private:
    [[maybe_unused]] const Graph &graph;
    std::shared_ptr<Vertex> currentVertex;
    std::set<std::shared_ptr<Vertex>> visitedVertices;
    std::vector<std::shared_ptr<Vertex>> traversalPath;

    void MarkVisited(std::shared_ptr<Vertex> vertex);

  public:
    GraphPosition(const Graph &g, std::shared_ptr<Vertex> startVertex);

    std::shared_ptr<Vertex> GetCurrentVertex() const;
    void SetCurrentVertex(std::shared_ptr<Vertex> vertex);
    bool AdvanceToNextNeighbor();
    const std::vector<std::shared_ptr<Vertex>> &GetTraversalPath() const;
    void ResetTraversal(std::shared_ptr<Vertex> newStartVertex);
  };

} // namespace cse