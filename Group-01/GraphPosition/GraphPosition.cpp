#include "GraphPosition.hpp"

namespace cse {

template <class T>
void GraphPosition<T>::MarkVisited(std::string vertexID) {
    visitedVertices.insert(vertexID);
    traversalPath.push_back(vertexID);
}

template <class T>
GraphPosition<T>::GraphPosition(const Graph<T>& g, std::string vertexStartID)
    : graph(g), currentVertexID(vertexStartID)
{
    if (!graph.GetVertex(vertexStartID).GetID().empty()) { // Ensure vertex exists
        MarkVisited(vertexStartID);
    } else {
        throw std::invalid_argument("Starting vertex does not exist in the graph.");
    }
}

template <class T>
std::string GraphPosition<T>::GetCurrentVertex() const {
    return currentVertexID;
}

template <class T>
void GraphPosition<T>::SetCurrentVertex(std::string vertexID) {
    if (!graph.GetVertex(vertexID).GetID().empty()) {
        currentVertexID = vertexID;
        MarkVisited(vertexID);
    } else {
        throw std::invalid_argument("Vertex does not exist in the graph.");
    }
}

template <class T>
bool GraphPosition<T>::AdvanceToNextNeighbor() {
    std::vector<std::string> neighbors = graph.GetVertex(currentVertexID).GetNeighbors();
    for (const std::string& neighborID : neighbors) {
        if (visitedVertices.find(neighborID) == visitedVertices.end()) {
            currentVertexID = neighborID;
            MarkVisited(neighborID);
            return true;
        }
    }
    return false;
}

template <class T>
const std::vector<std::string>& GraphPosition<T>::GetTraversalPath() const {
    return traversalPath;
}

} // namespace cse