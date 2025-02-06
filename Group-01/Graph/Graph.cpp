#include "Graph.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>

/**
 * TODO @lspecht: Add integration with GraphPosition
 * TODO @lspecht: Add GetAllEdges from vertex
 * TODO @lspecht: Add tests cases
 */
template <class T>
cse::Vertex cse::Graph<T>::AddVertex(std::string id, T data, std::optional<double> X, std::optional<double> Y) {
  if (vertices.find(id) != vertices.end()) {
    throw std::runtime_error("Vertex already exists: " + id);
  }
  cse::Vertex v{id, X.value_or(0.0), Y.value_or(0.0)};
  vertices[id] = v;
  return v;
}

template <class T> cse::Vertex cse::Graph<T>::RemoveVertex(std::string id) {
  auto it = vertices.find(id);
  if (it == vertices.end()) {
    throw std::out_of_range("Vertex does not exist: " + id);
  }
  cse::Vertex removedVertex = it->second;
  vertices.erase(it);
  return removedVertex;
}

template <class T> cse::Vertex &cse::Graph<T>::GetVertex(std::string id) {
  if (vertices.find(id) == vertices.end()) {
    throw std::out_of_range("Vertex does not exist: " + id);
  }
  return vertices.at(id);
}

template <class T> cse::Edge cse::Graph<T>::AddEdge(std::string v1_id, std::string v2_id, bool bidirectional) {
  if (vertices.find(v1_id) == vertices.end() || vertices.find(v2_id) == vertices.end()) {
    throw std::out_of_range("One or both vertices do not exist.");
  }

  std::string edge_id = v1_id + "-" + v2_id;
  std::shared_ptr<cse::Edge> e;
  if (bidirectional) {
    e = std::make_shared<cse::BidirectionalEdge>({edge_id, &vertices[v1_id], &vertices[v2_id], bidirectional})
  } else {
    e = std::make_shared<cse::Edge>({edge_id, &vertices[v1_id], &vertices[v2_id], bidirectional})
  }

  GetVertex(v1_id).AddEdge(e);
  if (bidirectional) {
    GetVertex(v2_id).AddEdge(e);
  }

  edges[edge_id] = e;
  return e;
}

template <class T> cse::Edge cse::Graph<T>::AddEdge(cse::Vertex &v1, cse::Vertex &v2, bool bidirectional) {
  return AddEdge(v1.id, v2.id, bidirectional);
}

template <class T> cse::Edge cse::Graph<T>::RemoveEdge(cse::Edge &edge) {
  return RemoveEdge(edge.id);
}

template <class T> cse::Edge cse::Graph<T>::RemoveEdge(std::string const &edge_id) {
  auto it = edges.find(edge_id);
  if (it == edges.end()) {
    throw std::out_of_range("Edge does not exist: " + edge_id);
  }
  cse::Edge removedEdge = it->second;
  edges.erase(removedEdge.id);
  return removedEdge;
}

template <class T> cse::Edge cse::Graph<T>::GetEdge(std::string const &edge_id) {
  if (edges.find(edge_id) == edges.end()) {
    throw std::out_of_range("Edge does not exist.");
  }
  return edges.at(edge_id);
}

template <class T> std::optional<cse::Edge> cse::Graph<T>::GetEdge(cse::Vertex &from, cse::Vertex &to) {
  return from.
}

template <class T> std::optional<cse::Edge> cse::Graph<T>::GetEdge(std::string from_id, std::string to_id) {
  return GetEdge(GetVertex(from_id), GetVertex(to_id));
}

template <class T> void cse::Graph<T>::ToFile(std::fstream &s) {
  // TODO @lspecht: Implement this
}

template <class T> cse::Graph<T> cse::Graph<T>::FromFile(std::fstream &s) {
  cse::Graph<T> graph;
  // TODO @lspecht: Implement this
  return graph;
}
