// GraphPosition.cpp

#include <iostream>
#include <vector>
#include <set>

// Forward declaration of Graph. Likely will have functions like GetNeighbors, HasVertex (bool if vertex exists in graph), 
// GetModificationCount (provides count of # times graph has been modified so GraphPosition can use to detect changes during 
// traversal), and more. Also assumes Graph class initializes vertices with an ID for easy tracking/position usage.
class Graph;

/*
 * GraphPosition class
 * Represents a position within a Graph, allowing for traversal.
 */
class GraphPosition {
private:
    const Graph& graph;               // Reference to the Graph.
    int currentVertexID;           // Current vertex ID.
    std::set<int> visitedVertices; // Tracks visited verticec (by ID).
    std::vector<int> traversalPath; // Ordered traversal path.

    // Helper function to mark a vertex as visited and record it in the path.
    void MarkVisited(int vertexID) {
        visitedVertices.insert(vertexID);
        traversalPath.push_back(vertexID);
    }

public:
    // Constructor requires a reference to an existing Graph and a valid starting vertex ID.
    GraphPosition(const Graph& g, int vertexStartID)
        : graph(g), currentVertex(vertexStartID), initialModificationCount(g.GetModificationCount())
    {
        if (!graph.HasVertex(vertexStartID)) {
            throw std::invalid_argument("Starting vertex does not exist in the graph.");
        }
        MarkVisited(vertexStartID);
    }

    // Returns the current vertex ID.
    int GetCurrentVertex() const {
        return currentVertexID;
    }

    // Directly set the current vertex.
    // (This does not change the underlying Graph; it only moves the GraphPosition.)
    void SetCurrentVertex(int vertexID) {
        if (!graph.HasVertex(vertexID)) {
            throw std::invalid_argument("Vertex does not exist in the graph.");
        }
        currentVertexID = vertexID;
        MarkVisited(vertexID);
    }

    // Attempts to advance to an unvisited neighbor of the current vertex.
    // Returns true if successful; false if no unvisited neighbor exists.
    bool AdvanceToNextNeighbor() {
        // Check if the graph has been modified since this traversal began.
        if (graph.GetModificationCount() != initialModificationCount) {
            throw std::runtime_error("Graph was modified during traversal.");
        }

        // Retrieve neighbors from the graph.
        std::vector<int> neighbors = graph.GetNeighbors(currentVertexID);
        for (int neighborID : neighbors) {
            // Move to the first unvisited neighbor.
            if (visitedVertices.find(neighborID) == visitedVertices.end()) {
                currentVertexID = neighborID;
                MarkVisited(neighborID);
                return true;
            }
        }
        // No unvisited neighbor found.
        return false;
    }

    // Returns the ordered traversal path.
    const std::vector<int>& GetTraversalPath() const {
        return traversalPath;
    }

}