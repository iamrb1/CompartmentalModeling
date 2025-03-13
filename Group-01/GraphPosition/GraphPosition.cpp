#include "GraphPosition.hpp"

namespace cse {
  GraphPosition::GraphPosition(const Graph &g, Vertex const *startVertex) : graph(g), currentVertex(startVertex) {
    if (!startVertex) {
      throw std::invalid_argument("GraphPosition must be initialized with a non-null vertex!");
    }
    MarkVisited(*startVertex);
  }

  void GraphPosition::MarkVisited(Vertex const &vertex) {
    visitedVertices.insert(vertex.GetId()); // Store the actual vertex object
    traversalPath.push_back(&vertex);
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
    return traversalFunction(*this);
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

  GraphPosition &GraphPosition::operator++() {
    if (!AdvanceToNextNeighbor()) {
      // No more nodes to visit
      currentVertex = nullptr;
    }
    return *this;
  }

  GraphPosition::operator bool() const {
    return currentVertex != nullptr;
  }

  namespace TraversalModes {
    auto DFS() -> std::function<bool(GraphPosition &)> {
      return [](GraphPosition &graphPosition) {
        auto dfs_implementation = [&](GraphPosition &gp, auto &dfs) -> bool {
          auto &stack = gp.GetTraversalStack();

          if (stack.empty()) {
            stack.push_back(&gp.GetCurrentVertex());
          }
          cse::Vertex const &current = *stack.back();

          // Collect and sort neighbors alphabetically by Vertex ID
          std::vector<std::pair<std::string, std::weak_ptr<cse::Edge>>> neighbors(current.GetEdges().begin(),
                                                                                  current.GetEdges().end());
          std::sort(neighbors.begin(), neighbors.end(),
                    [](std::pair<std::string, std::weak_ptr<cse::Edge>> &edge1,
                       std::pair<std::string, std::weak_ptr<cse::Edge>> &edge2) {
                      return edge1.second.lock()->GetTo().GetId() < edge2.second.lock()->GetTo().GetId();
                    });

          // Find the first non visited neighbor and not in the stack
          auto nonVisited = std::find_if(neighbors.begin(), neighbors.end(),
                                         [&](std::pair<std::string, std::weak_ptr<cse::Edge>> &p) {
                                           auto weakEdge = p.second;
                                           if (auto edge = weakEdge.lock()) {
                                             cse::Vertex const &neighbor = edge->GetTo();
                                             return !gp.IsVisited(neighbor);
                                           }
                                           return false;
                                         });

          if (nonVisited == neighbors.end()) {
            stack.pop_back();
            gp.SetCurrentVertex(current);
            return true;
          }

          auto &c = (nonVisited->second.lock())->GetTo();
          stack.push_back(&c);
          gp.MarkVisited(current);
          return dfs(gp, dfs); // Recursive call
        };

        return dfs_implementation(graphPosition, dfs_implementation); // Start DFS from the current position
      };
    }
  } // namespace TraversalModes

} // namespace cse