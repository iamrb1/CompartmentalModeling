#include "GraphPosition.hpp"

// Implementation of the helper function.
void GraphPosition::MarkVisited(int vertexID) {
    visitedVertices.insert(vertexID);
    traversalPath.push_back(vertexID);
}

// Constructor implementation.
GraphPosition::GraphPosition(const Graph& g, int vertexStartID)
    : graph(g), currentVertexID(vertexStartID), initialModificationCount(g.GetModificationCount())
{
    if (!graph.HasVertex(vertexStartID)) {
        throw std::invalid_argument("Starting vertex does not exist in the graph.");
    }
    MarkVisited(vertexStartID);
}

// Returns the current vertex ID.
int GraphPosition::GetCurrentVertex() const {
    return currentVertexID;
}

// Sets the current vertex ID and marks it as visited.
void GraphPosition::SetCurrentVertex(int vertexID) {
    if (!graph.HasVertex(vertexID)) {
        throw std::invalid_argument("Vertex does not exist in the graph.");
    }
    currentVertexID = vertexID;
    MarkVisited(vertexID);
}

// Attempts to advance to an unvisited neighbor.
bool GraphPosition::AdvanceToNextNeighbor() {
    // Check if the graph has been modified since traversal began.
    if (graph.GetModificationCount() != initialModificationCount) {
        throw std::runtime_error("Graph was modified during traversal.");
    }

    // Retrieve vertex neighbors.
    std::vector<int> neighbors = graph.GetNeighbors(currentVertexID);
    for (int neighborID : neighbors) {
        // Move to the first unvisited neighbor.
        if (visitedVertices.find(neighborID) == visitedVertices.end()) {
            currentVertexID = neighborID;
            MarkVisited(neighborID);
            return true;
        }
    }
    // No unvisited neighbor.
    return false;
}

// Returns the ordered traversal path.
const std::vector<int>& GraphPosition::GetTraversalPath() const {
    return traversalPath;
}