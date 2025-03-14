#pragma once

#include "../Graph/Graph.hpp"
#include "../Graph/Vertex.hpp"

#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <set>
#include <stdexcept>
#include <unordered_set>
#include <vector>

namespace cse {

  template <typename VERTEX_DATA_T>
  class GraphPosition; // Forward declaration

  namespace TraversalModes {
    template <typename VERTEX_DATA_T>
    auto DFS() -> std::function<bool(GraphPosition<VERTEX_DATA_T> &)>; // Forward declare the function

    template <typename VERTEX_DATA_T>
    auto BFS() -> std::function<bool(GraphPosition<VERTEX_DATA_T> &)>; // Forward declare the function
  } // namespace TraversalModes

  template <typename VERTEX_DATA_T>
  class GraphPosition {
  private:
    [[maybe_unused]] const Graph<VERTEX_DATA_T> &graph;
    Vertex<VERTEX_DATA_T> const *currentVertex;
    // Set of visited vertices by ID
    std::set<std::string> visitedVertices;
    std::vector<Vertex<VERTEX_DATA_T> const *> traversalPath;
    std::function<bool(GraphPosition<VERTEX_DATA_T> &)> traversalFunction = TraversalModes::DFS<VERTEX_DATA_T>();
    std::deque<Vertex<VERTEX_DATA_T> const *> traversalQueue;
    std::vector<Vertex<VERTEX_DATA_T> const *> traversalStack;

  public:
    std::vector<Vertex<VERTEX_DATA_T> const *> &GetTraversalStack() { return traversalStack; }
    std::deque<Vertex<VERTEX_DATA_T> const *> &GetTraversalQueue() { return traversalQueue; }
    void MarkVisited(Vertex<VERTEX_DATA_T> const &vertex);
    bool IsVisited(Vertex<VERTEX_DATA_T> const &vertex) { return visitedVertices.find(vertex.GetId()) != visitedVertices.end(); };

    GraphPosition(const Graph<VERTEX_DATA_T> &g, Vertex<VERTEX_DATA_T> const *startVertex);

    Vertex<VERTEX_DATA_T> const &GetCurrentVertex() const;
    void SetCurrentVertex(Vertex<VERTEX_DATA_T> const &vertex);
    bool AdvanceToNextNeighbor();
    const std::vector<Vertex<VERTEX_DATA_T> const *> &GetTraversalPath() const;
    void ResetTraversal(Vertex<VERTEX_DATA_T> const &newStartVertex);
    GraphPosition &operator++();    // Advances to next vertex
    explicit operator bool() const; // Checks if more traversal is possible
    void SetTraversalMode(std::function<bool(GraphPosition<VERTEX_DATA_T> &)> newTraversalFunction) {
      traversalFunction = newTraversalFunction;
    }
  };


  // Function Implementations

  template <typename VERTEX_DATA_T>
  GraphPosition<VERTEX_DATA_T>::GraphPosition(const Graph<VERTEX_DATA_T> &g, Vertex<VERTEX_DATA_T> const *startVertex)
      : graph(g), currentVertex(startVertex) {
    if (!startVertex) {
      throw std::invalid_argument("GraphPosition must be initialized with a non-null vertex!");
    }
  }

  template <typename VERTEX_DATA_T>
  void GraphPosition<VERTEX_DATA_T>::MarkVisited(Vertex<VERTEX_DATA_T> const &vertex) {
    visitedVertices.insert(vertex.GetId());
    traversalPath.push_back(&vertex);
  }

  template <typename VERTEX_DATA_T>
  Vertex<VERTEX_DATA_T> const &GraphPosition<VERTEX_DATA_T>::GetCurrentVertex() const {
    assert(currentVertex != nullptr && "GetCurrentVertex() should never return nullptr!");
    return *currentVertex;
  }

  template <typename VERTEX_DATA_T>
  void GraphPosition<VERTEX_DATA_T>::SetCurrentVertex(Vertex<VERTEX_DATA_T> const &vertex) {
    currentVertex = &vertex;
  }

  template <typename VERTEX_DATA_T>
  bool GraphPosition<VERTEX_DATA_T>::AdvanceToNextNeighbor() {
    return traversalFunction(*this);
  }

  template <typename VERTEX_DATA_T>
  const std::vector<Vertex<VERTEX_DATA_T> const *> &GraphPosition<VERTEX_DATA_T>::GetTraversalPath() const {
    return traversalPath;
  }

  template <typename VERTEX_DATA_T>
  void GraphPosition<VERTEX_DATA_T>::ResetTraversal(Vertex<VERTEX_DATA_T> const &newStartVertex) {
    visitedVertices.clear();
    traversalPath.clear();
    SetCurrentVertex(newStartVertex);
  }

  template <typename VERTEX_DATA_T>
  GraphPosition<VERTEX_DATA_T> &GraphPosition<VERTEX_DATA_T>::operator++() {
    if (!AdvanceToNextNeighbor()) {
      currentVertex = nullptr;
    }
    return *this;
  }

  template <typename VERTEX_DATA_T>
  GraphPosition<VERTEX_DATA_T>::operator bool() const {
    return currentVertex != nullptr;
  }

  namespace TraversalModes {
    template <typename VERTEX_DATA_T>
    auto DFS() -> std::function<bool(GraphPosition<VERTEX_DATA_T> &)> {
      return [](GraphPosition<VERTEX_DATA_T> &graphPosition) {
        auto dfs_implementation = [&](GraphPosition<VERTEX_DATA_T> &gp, auto &dfs) -> bool {
          auto &stack = gp.GetTraversalStack();

          if (stack.empty()) {
            stack.push_back(&gp.GetCurrentVertex());
          }
          auto &current = *stack.back();
          if (gp.IsVisited(current)) {
            return false;
          }

          std::vector<std::pair<std::string, std::weak_ptr<Edge<VERTEX_DATA_T>>>> neighbors(
              current.GetEdges().begin(), current.GetEdges().end());

          std::sort(neighbors.begin(), neighbors.end(),
                    [](auto &edge1, auto &edge2) {
                      return edge1.second.lock()->GetTo().GetId() < edge2.second.lock()->GetTo().GetId();
                    });

          auto nonVisited = std::find_if(neighbors.begin(), neighbors.end(),
                                        [&](auto &p) {
                                          auto weakEdge = p.second;
                                          if (auto edge = weakEdge.lock()) {
                                            return !gp.IsVisited(edge->GetTo());
                                          }
                                          return false;
                                        });

          if (nonVisited == neighbors.end()) {
            stack.pop_back();
            gp.SetCurrentVertex(current);
            gp.MarkVisited(current);
            return true;
          }

          auto &c = (nonVisited->second.lock())->GetTo();
          stack.push_back(&c);
          return dfs(gp, dfs);  // recursive call
        };

        return dfs_implementation(graphPosition, dfs_implementation);
      };
    }

    template <typename VERTEX_DATA_T>
    auto BFS() -> std::function<bool(GraphPosition<VERTEX_DATA_T> &)> {
      return [](GraphPosition<VERTEX_DATA_T> &graphPosition) {
        auto &queue = graphPosition.GetTraversalQueue();

        if (queue.empty()) {
          queue.push_back(&graphPosition.GetCurrentVertex());
          graphPosition.MarkVisited(graphPosition.GetCurrentVertex());
        }

        if (queue.empty()) {
          return false;
        }

        Vertex<VERTEX_DATA_T> const *current = queue.front();
        queue.pop_front();
        graphPosition.SetCurrentVertex(*current);

        std::vector<std::pair<std::string, std::weak_ptr<Edge<VERTEX_DATA_T>>>> neighbors(
            current->GetEdges().begin(), current->GetEdges().end());

        std::sort(neighbors.begin(), neighbors.end(),
                  [](const auto &edge1, const auto &edge2) {
                    return edge1.second.lock()->GetTo().GetId() < edge2.second.lock()->GetTo().GetId();
                  });

        for (const auto &edge : neighbors) {
          if (auto edgePtr = edge.second.lock()) {
            auto &neighbor = edgePtr->GetTo();
            if (!graphPosition.IsVisited(neighbor)) {
              queue.push_back(&neighbor);
              graphPosition.MarkVisited(neighbor);
            }
          }
        }

        return !queue.empty();
      };
    }
  } // namespace TraversalModes

} // namespace cse