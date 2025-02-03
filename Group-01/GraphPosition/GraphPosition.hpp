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

class GraphPosition {
public:
    // Constructor: takes a reference to an existing Graph and a valid starting vertex ID.
    GraphPosition(const Graph& g, int vertexStartID);

    // Returns the current vertex ID.
    int GetCurrentVertex() const;

    // Sets the current vertex ID.
    // (This does not change the actual Graph, it just moves the GraphPosition.)
    void SetCurrentVertex(int vertexID);

    // Attempts to advance to an unvisited neighbor of the current vertex.
    // Returns true if successful; false if no unvisited neighbor exists.
    bool AdvanceToNextNeighbor();

    // Returns the ordered traversal path.
    const std::vector<int>& GetTraversalPath() const;

private:
    const Graph& graph;               // Reference to the Graph.
    int currentVertexID;              // Current vertex ID.
    std::set<int> visitedVertices;    // Tracks visited vertices (by ID).
    std::vector<int> traversalPath;   // Ordered traversal path.
    unsigned int initialModificationCount; // Graph modification count at construction.

    // Helper function to mark a vertex as visited and record it in the path.
    void MarkVisited(int vertexID);
};

#endif // GRAPHPOSITION_HPP