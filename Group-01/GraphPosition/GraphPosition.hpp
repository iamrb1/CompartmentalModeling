#ifndef GRAPHPOSITION_HPP
#define GRAPHPOSITION_HPP

#include <iostream>
#include <vector>
#include <set>
#include <stdexcept>

// Forward declaration of Graph. 
// Likely will have functions like GetNeighbors, HasVertex (bool if vertex exists in graph), GetModificationCount (provides
// count of # times graph has been modified so GraphPosition can use to detect changes during 
// traversal), and more. Also assumes Graph class initializes vertices with an ID for easy tracking/position usage.
class Graph;

namespace cse498 {

template <class T>
class GraphPosition {
public:
    // Constructor: takes a reference to an existing Graph and a valid starting vertex ID.
    GraphPosition(const Graph<T>& g, std::string vertexStartID);

    // Returns the current vertex ID.
    std::string GetCurrentVertex() const;

    // Sets the current vertex ID.
    void SetCurrentVertex(std::string vertexID);

    // Attempts to advance to an unvisited neighbor.
    // Returns true if successful; false if no unvisited neighbor exists.
    bool AdvanceToNextNeighbor();

    // Returns the ordered traversal path.
    const std::vector<std::string>& GetTraversalPath() const;

private:
    const Graph<T>& graph;             // Reference to the Graph.
    std::string currentVertexID;       // Current vertex ID.
    std::set<std::string> visitedVertices;  // Tracks visited vertices.
    std::vector<std::string> traversalPath; // Ordered traversal path.

    // Helper function to mark a vertex as visited and record it in the path.
    void MarkVisited(std::string vertexID);
};

} // namespace cse498

#endif // GRAPHPOSITION_HPP