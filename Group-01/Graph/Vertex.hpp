#pragma once
#include <cassert>
#include <iostream>
#include <istream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "../Util/Util.hpp"
#include "GraphExceptions.hpp"

namespace cse {

template <typename VERTEX_DATA_T>
class Edge;  ///< Forward declaration of Edge

template <typename VERTEX_DATA_T>
class Vertex : public StreamSerializable {
 private:
  std::string id;      ///< Unique identifier for the vertex
  VERTEX_DATA_T data;  ///< Custom vertex data
  double x{0};         ///< X coordinate position of the vertex
  double y{0};         ///< Y coordinate position of the vertex
  std::map<std::string, std::weak_ptr<Edge<VERTEX_DATA_T>>>
      edges{};  ///< Map of edges connected to this vertex, keyed by destination
                ///< vertex ID
  bool show_id = true;  ///< Display the vertex ID

 protected:
  std::string GetTypeName() const override { return "VERTEX"; }
  std::vector<std::pair<std::string, SerializableProperty>> GetPropertyMap()
      override;
  void SetId(std::string newId) override { id = newId; };

 private:
 public:
  Vertex() = delete;
  Vertex(std::string id, VERTEX_DATA_T data) : id(id), data(data) {};
  Vertex(std::string id, VERTEX_DATA_T data, double x, double y)
      : id(id), data(data), x(x), y(y) {};
  Vertex(std::string id, double x, double y) : id(id), data(), x(x), y(y) {};
  Vertex(std::istream &f, size_t prefix_size) { FromFile(f, prefix_size); }

  void CleanupExpiredEdges();
  void AddEdge(std::weak_ptr<Edge<VERTEX_DATA_T>> const &e);
  void RemoveEdge(std::string const id);
  void SetShowId(bool show) { show_id = show; }
  bool IsConnected(const Vertex<VERTEX_DATA_T> &destination) const;
  bool GetShowId() const { return show_id; }

  std::string GetId() const override { return id; }
  const std::map<std::string, std::weak_ptr<Edge<VERTEX_DATA_T>>> &GetEdges()
      const {
    return edges;
  }
  constexpr double GetX() const noexcept { return x; }
  constexpr double GetY() const noexcept { return y; }
  VERTEX_DATA_T GetData() const { return data; }
  void SetData(VERTEX_DATA_T newData) { data = newData; }

  std::shared_ptr<Edge<VERTEX_DATA_T>> const GetEdge(
      Vertex<VERTEX_DATA_T> const &to) const;
  std::shared_ptr<Edge<VERTEX_DATA_T>> const GetEdge(
      std::string const to_id) const;

  template <typename T>
  friend std::ostream &operator<<(std::ostream &os, const Vertex<T> &v);
  template <typename T>
  friend bool operator==(const Vertex<T> &lhs, const Vertex<T> &rhs);
  template <typename T, bool IS_BIDIRECTIONAL>
  friend class Graph;
};

// Function Implementations

/**
 * Adds an edge to this vertex
 * @param e The edge to add
 */
template <typename VERTEX_DATA_T>
void Vertex<VERTEX_DATA_T>::AddEdge(
    std::weak_ptr<Edge<VERTEX_DATA_T>> const &e) {
  if (auto edge = e.lock()) {
    auto &toVertex = edge->GetTo();
    edges[toVertex.GetId()] = e;
  }
}

template <typename VERTEX_DATA_T>
void Vertex<VERTEX_DATA_T>::RemoveEdge(std::string const id) {
  if (edges.find(id) != edges.end()) {
    edges.erase(id);
    return;
  }
  throw cse::edge_not_found_error("Edge not found when removing it");
}

/**
 * Checks if this vertex is connected to another vertex
 * @param destination The vertex to check connection with
 * @return true if vertices are connected, false otherwise
 */
template <typename VERTEX_DATA_T>
bool Vertex<VERTEX_DATA_T>::IsConnected(
    const Vertex<VERTEX_DATA_T> &destination) const {
  auto it = edges.find(destination.GetId());
  if (it == edges.end()) {
    return false;
  }

  if (it->second.expired()) {
    return false;
  }
  return true;
}

/**
 * Removes any expired edge references from the edges collection
 */
template <typename VERTEX_DATA_T>
void Vertex<VERTEX_DATA_T>::CleanupExpiredEdges() {
  for (auto it = edges.begin(); it != edges.end();) {
    if (it->second.expired()) {
      it = edges.erase(it);
    } else {
      ++it;
    }
  }
}

/**
 * Gets the edge connecting this vertex to another vertex
 * @param to The destination vertex
 * @return Shared pointer to the edge
 * @throws runtime_error if edge doesn't exist
 */
template <typename VERTEX_DATA_T>
std::shared_ptr<Edge<VERTEX_DATA_T>> const Vertex<VERTEX_DATA_T>::GetEdge(
    Vertex<VERTEX_DATA_T> const &to) const {
  return GetEdge(to.GetId());
}

template <typename VERTEX_DATA_T>
std::shared_ptr<Edge<VERTEX_DATA_T>> const Vertex<VERTEX_DATA_T>::GetEdge(
    std::string const to_id) const {
  auto it = edges.find(to_id);
  if (it == edges.end() || it->second.expired()) {
    throw edge_not_found_error("Edge from " + id + " to " + to_id);
  }
  return it->second.lock();
}

/**
 * Gets the serializable properties for this vertex
 * @return Vector of property pairs for serialization
 */
template <typename VERTEX_DATA_T>
std::vector<std::pair<std::string, SerializableProperty>>
Vertex<VERTEX_DATA_T>::GetPropertyMap() {
  std::vector<std::pair<std::string, SerializableProperty>> properties;

  properties.emplace_back(
      "X", SerializableProperty(
               [this](const std::string &value) { this->x = std::stod(value); },
               [this](std::ostream &s) { s << x; }));

  properties.emplace_back(
      "Y", SerializableProperty(
               [this](const std::string &value) { this->y = std::stod(value); },
               [this](std::ostream &s) { s << y; }));

  return properties;
}

template <typename VERTEX_DATA_T>
std::ostream &operator<<(std::ostream &os, const Vertex<VERTEX_DATA_T> &v) {
  os << "Vertex(" << v.id << ")";
  return os;
}

template <typename VERTEX_DATA_T>
bool operator==(const Vertex<VERTEX_DATA_T> &lhs,
                const Vertex<VERTEX_DATA_T> &rhs) {
  return lhs.id == rhs.id && lhs.x == rhs.x && lhs.y == rhs.y;
}

}  // namespace cse
