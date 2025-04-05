#pragma once
#include <algorithm>
#include <cassert>
#include <cmath>
#include <deque>
#include <emscripten.h>
#include <iostream>
#include <queue>
#include <set>
#include <stdexcept>
#include <unordered_set>
#include <vector>

#include "../Graph/Graph.hpp"
#include "../Graph/Vertex.hpp"

// Citation: AI was used to create doxygen comments
namespace cse {

  /**
   * A class that manages traversal through a graph data structure
   * @tparam VERTEX_DATA_T The data type stored in the graph's vertices
   */
  template <typename VERTEX_DATA_T> class GraphPosition; // Forward declaration

  namespace TraversalModes {
    /**
     * Helper function to get sorted neighbors of a vertex
     * @param vertex The vertex whose neighbors to sort
     * @return Vector of pairs containing edge ID and edge pointer, sorted by destination vertex ID
     */
    template <typename VERTEX_DATA_T> auto GetSortedNeighbors(const Vertex<VERTEX_DATA_T> &vertex) {
      using EdgePair = std::pair<std::string, std::weak_ptr<Edge<VERTEX_DATA_T>>>;
      std::vector<EdgePair> neighbors(vertex.GetEdges().begin(), vertex.GetEdges().end());

      std::sort(neighbors.begin(), neighbors.end(), [](const auto &edge1, const auto &edge2) {
        return edge1.second.lock()->GetTo().GetId() < edge2.second.lock()->GetTo().GetId();
      });
      return neighbors;
    }

    /**
     * Creates a Depth-First Search traversal function
     * @tparam VERTEX_DATA_T The data type stored in the graph's vertices
     * @return Function object implementing DFS traversal
     */
    template <typename VERTEX_DATA_T>
    auto DFS() -> std::function<bool(GraphPosition<VERTEX_DATA_T> &)>; // Forward declare the function

    /**
     * Creates a Breadth-First Search traversal function
     * @tparam VERTEX_DATA_T The data type stored in the graph's vertices
     * @return Function object implementing BFS traversal
     */
    template <typename VERTEX_DATA_T>
    auto BFS() -> std::function<bool(GraphPosition<VERTEX_DATA_T> &)>; // Forward declare the function

    /**
     * Creates an A* pathfinding traversal function
     * @tparam VERTEX_DATA_T The data type stored in the graph's vertices
     * @param destination The target vertex to find a path to
     * @return Function object implementing A* pathfinding
     */
    template <typename VERTEX_DATA_T>
    auto AStar(Vertex<VERTEX_DATA_T> &destination)
        -> std::function<bool(GraphPosition<VERTEX_DATA_T> &)>; // Forward declare the function
  } // namespace TraversalModes

  template <typename VERTEX_DATA_T> class GraphPosition {
  private:
    /** Reference to the graph being traversed */
    const Graph<VERTEX_DATA_T> &graph;
    /** Pointer to the current vertex in the traversal */
    Vertex<VERTEX_DATA_T> const *currentVertex;
    /** Set of visited vertex IDs during traversal */
    std::set<std::string> visitedVertices;
    /** Vector storing the path taken during traversal */
    std::vector<Vertex<VERTEX_DATA_T> const *> traversalPath;
    /** Function object defining the traversal strategy, defaults to DFS */
    std::function<bool(GraphPosition<VERTEX_DATA_T> &)> traversalFunction = TraversalModes::DFS<VERTEX_DATA_T>();
    /** Queue used for BFS traversal */
    std::deque<Vertex<VERTEX_DATA_T> const *> traversalQueue;
    /** Stack used for DFS traversal */
    std::vector<Vertex<VERTEX_DATA_T> const *> traversalStack;

  public:
    void MarkVisited(Vertex<VERTEX_DATA_T> const &vertex);
    GraphPosition(const Graph<VERTEX_DATA_T> &g, Vertex<VERTEX_DATA_T> const &startVertex);
    Vertex<VERTEX_DATA_T> const &GetCurrentVertex() const;
    void SetCurrentVertex(Vertex<VERTEX_DATA_T> const &vertex);
    bool AdvanceToNextNeighbor();
    const std::vector<Vertex<VERTEX_DATA_T> const *> &GetTraversalPath() const;
    void ResetTraversalPath();
    void AddToTraversalPath(Vertex<VERTEX_DATA_T> const &v);
    void ReverseTraversalPath();
    void ResetTraversal(Vertex<VERTEX_DATA_T> const &newStartVertex);
    GraphPosition &operator++();
    void TraverseGraph();
    explicit operator bool() const;

    /**
     * Checks if a vertex has been visited
     * @param vertex The vertex to check
     * @return true if the vertex has been visited, false otherwise
     */
    bool IsVisited(Vertex<VERTEX_DATA_T> const &vertex) {
      return visitedVertices.find(vertex.GetId()) != visitedVertices.end();
    };

    /**
     * Gets the traversal stack used for DFS
     * @return Reference to the traversal stack
     */
    std::vector<Vertex<VERTEX_DATA_T> const *> &GetTraversalStack() { return traversalStack; }

    /**
     * Gets the traversal queue used for BFS
     * @return Reference to the traversal queue
     */
    std::deque<Vertex<VERTEX_DATA_T> const *> &GetTraversalQueue() { return traversalQueue; }

    /**
     * Sets the traversal strategy
     * @param newTraversalFunction Function implementing the desired traversal strategy
     */
    void SetTraversalMode(std::function<bool(GraphPosition<VERTEX_DATA_T> &)> newTraversalFunction) {
      traversalFunction = newTraversalFunction;
    }

    /**
     * Gets a vertex by its ID
     * @param v_id The ID of the vertex to retrieve
     * @return Reference to the requested vertex
     */
    cse::Vertex<VERTEX_DATA_T> &GetVertex(std::string const v_id) { return graph.GetVertex(v_id); }
  };

  // Function Implementations

  /**
   * Constructs a GraphPosition with a reference to a graph and a starting vertex
   * @param g The graph being traversed
   * @param startVertex Reference to the starting vertex
   */
  template <typename VERTEX_DATA_T>
  GraphPosition<VERTEX_DATA_T>::GraphPosition(const Graph<VERTEX_DATA_T> &g, Vertex<VERTEX_DATA_T> const &startVertex)
      : graph(g), currentVertex(&startVertex) {}

  /**
   * Marks a vertex as visited and adds it to the traversal path
   * @param vertex The vertex to mark as visited
   */
  template <typename VERTEX_DATA_T>
  void GraphPosition<VERTEX_DATA_T>::MarkVisited(Vertex<VERTEX_DATA_T> const &vertex) {
    visitedVertices.insert(vertex.GetId());
    traversalPath.push_back(&vertex);
  }

  /**
   * Retrieves the current vertex in the traversal
   * @return Reference to the current vertex
   * @throws assert failure if current vertex is null
   */
  template <typename VERTEX_DATA_T>
  Vertex<VERTEX_DATA_T> const &GraphPosition<VERTEX_DATA_T>::GetCurrentVertex() const {
    assert(currentVertex != nullptr && "GetCurrentVertex() should never return nullptr!");
    return *currentVertex;
  }

  /**
   * Updates the current vertex in the traversal
   * @param vertex The vertex to set as the current
   */
  template <typename VERTEX_DATA_T>
  void GraphPosition<VERTEX_DATA_T>::SetCurrentVertex(Vertex<VERTEX_DATA_T> const &vertex) {
    currentVertex = &vertex;
  }

  /**
   * Advances to the next neighbor in the traversal using the current strategy
   * @return true if a move was made, false otherwise
   */
  template <typename VERTEX_DATA_T> bool GraphPosition<VERTEX_DATA_T>::AdvanceToNextNeighbor() {
    return traversalFunction(*this);
  }

  /**
   * Gets the path of vertices visited during traversal
   * @return A vector of pointers to the visited vertices
   */
  template <typename VERTEX_DATA_T>
  const std::vector<Vertex<VERTEX_DATA_T> const *> &GraphPosition<VERTEX_DATA_T>::GetTraversalPath() const {
    return traversalPath;
  }

  /**
   * Resets traversal state and sets a new starting vertex
   * @param newStartVertex The new starting vertex for traversal
   */
  template <typename VERTEX_DATA_T> void GraphPosition<VERTEX_DATA_T>::ResetTraversalPath() {
    traversalPath.clear();
  }

  /**
   * Adds a vertex pointer to the traversal path
   * @param v Reference to the vertex to add
   */
  template <typename VERTEX_DATA_T>
  void GraphPosition<VERTEX_DATA_T>::AddToTraversalPath(Vertex<VERTEX_DATA_T> const &v) {
    traversalPath.push_back(&v);
  }

  /**
   * Reverses the order of the current traversal path
   */
  template <typename VERTEX_DATA_T> void GraphPosition<VERTEX_DATA_T>::ReverseTraversalPath() {
    std::reverse(traversalPath.begin(), traversalPath.end());
  }

  /**
   * Resets the traversal state with a new starting vertex
   * @param newStartVertex The vertex to restart traversal from
   */
  template <typename VERTEX_DATA_T>
  void GraphPosition<VERTEX_DATA_T>::ResetTraversal(Vertex<VERTEX_DATA_T> const &newStartVertex) {
    visitedVertices.clear();
    traversalPath.clear();
    traversalStack.clear();
    traversalQueue = {};
    SetCurrentVertex(newStartVertex);
  }

  /**
   * Advances to the next vertex in the traversal
   * @return Reference to the updated GraphPosition object
   */
  template <typename VERTEX_DATA_T> GraphPosition<VERTEX_DATA_T> &GraphPosition<VERTEX_DATA_T>::operator++() {
    if (!AdvanceToNextNeighbor()) {
      currentVertex = nullptr;
    }
    return *this;
  }

  /**
   * Traverses the graph using the provided traversal strategy until no next node available
   */
  template <typename VERTEX_DATA_T> inline void GraphPosition<VERTEX_DATA_T>::TraverseGraph() {
    while ((bool)++(*this)) {
    }
  }

  /**
   * Checks if traversal is still ongoing
   * @return true if current vertex is valid, false otherwise
   */
  template <typename VERTEX_DATA_T> GraphPosition<VERTEX_DATA_T>::operator bool() const {
    return currentVertex != nullptr;
  }

  namespace TraversalModes {

    /**
     * Provides a depth-first search traversal strategy
     * @return A function that performs DFS traversal on a GraphPosition
     */
    template <typename VERTEX_DATA_T> auto DFS() -> std::function<bool(GraphPosition<VERTEX_DATA_T> &)> {
      return [](GraphPosition<VERTEX_DATA_T> &graphPosition) {
        // Recursive implementation of DFS using a stack
        auto dfs_implementation = [&](GraphPosition<VERTEX_DATA_T> &gp, auto &dfs) -> bool {
          auto &stack = gp.GetTraversalStack();

          // Initialize stack with current vertex if empty
          if (stack.empty()) {
            stack.push_back(&gp.GetCurrentVertex());
          }

          // Get the vertex at top of stack
          auto &current = *stack.back();
          if (gp.IsVisited(current)) {
            return false; // Skip if already visited
          }

          // Get all neighbors and sort them by ID for consistent traversal
          auto neighbors = GetSortedNeighbors(current);
          // Find first unvisited neighbor
          auto nonVisited = std::find_if(neighbors.begin(), neighbors.end(), [&](auto &p) {
            if (auto edge = p.second.lock()) {
              return !gp.IsVisited(edge->GetTo());
            }
            return false;
          });

          // If no unvisited neighbors, mark current as visited and backtrack
          if (nonVisited == neighbors.end()) {
            stack.pop_back();
            gp.SetCurrentVertex(current);
            gp.MarkVisited(current);
            return true;
          }

          // Push unvisited neighbor to stack and continue DFS
          auto &c = (nonVisited->second.lock())->GetTo();
          stack.push_back(&c);
          return dfs(gp, dfs); // recursive call
        };

        return dfs_implementation(graphPosition, dfs_implementation);
      };
    }

    /**
     * Provides a breadth-first search traversal strategy
     * @return A function that performs BFS traversal on a GraphPosition
     */
    template <typename VERTEX_DATA_T> auto BFS() -> std::function<bool(GraphPosition<VERTEX_DATA_T> &)> {
      return [](GraphPosition<VERTEX_DATA_T> &graphPosition) {
        auto &queue = graphPosition.GetTraversalQueue();

        // Initialize queue with starting vertex if empty
        if (queue.empty()) {
          queue.push_back(&graphPosition.GetCurrentVertex());
          graphPosition.MarkVisited(graphPosition.GetCurrentVertex());
        }

        if (queue.empty()) {
          return false; // No more vertices to visit
        }

        // Process the front of the queue
        Vertex<VERTEX_DATA_T> const *current = queue.front();
        queue.pop_front();
        graphPosition.SetCurrentVertex(*current);

        // Get and sort neighbors for consistent traversal order
        std::vector<std::pair<std::string, std::weak_ptr<Edge<VERTEX_DATA_T>>>> neighbors(current->GetEdges().begin(),
                                                                                          current->GetEdges().end());
        std::sort(neighbors.begin(), neighbors.end(), [](const auto &edge1, const auto &edge2) {
          return edge1.second.lock()->GetTo().GetId() < edge2.second.lock()->GetTo().GetId();
        });

        // Add all unvisited neighbors to queue
        for (const auto &edge : neighbors) {
          if (auto edgePtr = edge.second.lock()) {
            auto &neighbor = edgePtr->GetTo();
            if (!graphPosition.IsVisited(neighbor)) {
              queue.push_back(&neighbor);
              graphPosition.MarkVisited(neighbor);
            }
          }
        }

        return !queue.empty(); // Continue if there are more vertices to process
      };
    }

    /**
     * Helper struct for A* algorithm storing vertex and distance
     */
    template <typename VERTEX_DATA_T> struct VectorDistancePair {
      Vertex<VERTEX_DATA_T> const *v;
      double distance;
      VectorDistancePair(Vertex<VERTEX_DATA_T> const *vertex, double d) : v(vertex), distance(d) {}
    };

    /**
     * Less-than comparison for VectorDistancePair
     * @tparam VERTEX_DATA_T Type of the vertex data
     * @param lhs Left-hand side operand
     * @param rhs Right-hand side operand
     * @return true if lhs.distance < rhs.distance
     */
    template <typename VERTEX_DATA_T>
    bool operator<(const VectorDistancePair<VERTEX_DATA_T> &lhs, const VectorDistancePair<VERTEX_DATA_T> &rhs) {
      return lhs.distance < rhs.distance;
    }

    /**
     * Greater-than comparison for VectorDistancePair
     * @tparam VERTEX_DATA_T Type of the vertex data
     * @param lhs Left-hand side operand
     * @param rhs Right-hand side operand
     * @return true if lhs.distance > rhs.distance
     */
    template <typename VERTEX_DATA_T>
    bool operator>(const VectorDistancePair<VERTEX_DATA_T> &lhs, const VectorDistancePair<VERTEX_DATA_T> &rhs) {
      return operator<(rhs, lhs);
    }

    /**
     * Less-than or equal-to comparison for VectorDistancePair
     * @tparam VERTEX_DATA_T Type of the vertex data
     * @param lhs Left-hand side operand
     * @param rhs Right-hand side operand
     * @return true if lhs.distance <= rhs.distance
     */
    template <typename VERTEX_DATA_T>
    bool operator<=(const VectorDistancePair<VERTEX_DATA_T> &lhs, const VectorDistancePair<VERTEX_DATA_T> &rhs) {
      return !operator>(lhs, rhs);
    }

    /**
     * Greater-than or equal-to comparison for VectorDistancePair
     * @tparam VERTEX_DATA_T Type of the vertex data
     * @param lhs Left-hand side operand
     * @param rhs Right-hand side operand
     * @return true if lhs.distance >= rhs.distance
     */
    template <typename VERTEX_DATA_T>
    bool operator>=(const VectorDistancePair<VERTEX_DATA_T> &lhs, const VectorDistancePair<VERTEX_DATA_T> &rhs) {
      return !operator<(lhs, rhs);
    }

    /**
     * Helper struct for storing traversal history in A* algorithm
     */
    struct TraversalHistory {
      double distance;
      std::string previousVectorId;
    };

    /**
     * Creates an A* pathfinding traversal function
     * @tparam VERTEX_DATA_T The data type stored in the graph's vertices
     * @param destination The target vertex to find a path to
     */
    template <typename VERTEX_DATA_T>
    auto AStar(Vertex<VERTEX_DATA_T> &destination) -> std::function<bool(GraphPosition<VERTEX_DATA_T> &)> {
      using VertexPair = VectorDistancePair<VERTEX_DATA_T>;
      using PriorityQueue = std::priority_queue<VertexPair, std::vector<VertexPair>, std::greater<VertexPair>>;
      using DistanceMap = std::map<std::string, TraversalHistory>;
      using EdgePair = std::pair<std::string, std::weak_ptr<Edge<VERTEX_DATA_T>>>;

      // Priority queue for vertices to visit, sorted by estimated total cost
      PriorityQueue q;
      // Map storing best known distances and previous vertices
      DistanceMap bestDistances;

      // Heuristic function: Euclidean distance to destination
      auto distanceCalculator = [&destination](Vertex<VERTEX_DATA_T> const &curr) {
        double xDist = std::abs(destination.GetX() - curr.GetX());
        double yDist = std::abs(destination.GetY() - curr.GetY());
        return std::sqrt(xDist * xDist + yDist * yDist);
      };

      return [q, distanceCalculator, bestDistances, &destination](GraphPosition<VERTEX_DATA_T> &graphPosition) mutable {
        if (q.empty()) {
          if (bestDistances.empty()) {
            // Initialize search with starting vertex
            auto const &curr = graphPosition.GetCurrentVertex();
            q.push(VertexPair(&curr, distanceCalculator(curr)));
            bestDistances[curr.GetId()] = TraversalHistory{0, ""};
          } else {
            // Search complete - reconstruct path from destination to start
            auto const &curr = graphPosition.GetCurrentVertex();
            auto bd = bestDistances.find(curr.GetId());
            if (bd != bestDistances.end()) {
              auto traversalHistoryNode = bd;
              graphPosition.AddToTraversalPath(curr);

              while (!traversalHistoryNode->second.previousVectorId.empty()) {
                auto v_id = traversalHistoryNode->second.previousVectorId;
                assert(bestDistances.find(v_id) != bestDistances.end());
                graphPosition.AddToTraversalPath(graphPosition.GetVertex(v_id));
                traversalHistoryNode = bestDistances.find(v_id);
              }
              graphPosition.ReverseTraversalPath();
            }
            return false;
          }
        }

        // Process vertex with lowest estimated total cost
        auto v_pair = q.top();
        auto const *v = v_pair.v;
        graphPosition.SetCurrentVertex(*v);
        double currDistance = bestDistances[v->GetId()].distance;
        q.pop();

        // Check if destination reached
        if (*v == destination) {
          q = PriorityQueue(); // Clear queue to trigger path reconstruction
          return true;
        }

        // Process all neighbors
        std::vector<EdgePair> neighbors(v->GetEdges().begin(), v->GetEdges().end());
        for (const auto &edge : neighbors) {
          if (auto edgePtr = edge.second.lock()) {
            auto const &neighbor = edgePtr->GetTo();
            // Calculate actual distance to neighbor through current path
            double actual_d = currDistance + edgePtr->GetWeight();
            // Estimate total cost (actual distance + heuristic)
            auto estimated_d = actual_d + distanceCalculator(neighbor);

            // Update if this is a better path to the neighbor
            auto bd = bestDistances.find(neighbor.GetId());
            if (bd == bestDistances.end() || actual_d < bd->second.distance) {
              bestDistances[neighbor.GetId()].distance = actual_d;
              bestDistances[neighbor.GetId()].previousVectorId = v->GetId();
              q.push(VertexPair(&neighbor, estimated_d));
            }
          }
        }
        return true;
      };
    }

  } // namespace TraversalModes

} // namespace cse