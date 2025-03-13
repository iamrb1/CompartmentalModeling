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

namespace cse {

  template <typename VERTEX_DATA_T>
  class Edge; ///< Forward declaration of Edge

  template <typename VERTEX_DATA_T>
  class Vertex : public FileSerializable {
  private:
    std::string id; ///< Unique identifier for the vertex
    double x{0};    ///< X coordinate position of the vertex
    double y{0};    ///< Y coordinate position of the vertex
    std::map<std::string, std::weak_ptr<Edge<VERTEX_DATA_T>>>
        edges{}; ///< Map of edges connected to this vertex, keyed by destination vertex ID
    VERTEX_DATA_T data; ///< Custom vertex data

  protected:
    std::string GetTypeName() const override { return "VERTEX"; }
    std::vector<std::pair<std::string, SerializableProperty>> GetPropertyMap() override;
    void SetId(std::string newId) override { id = newId; };

  private:
    void AddEdge(std::weak_ptr<Edge<VERTEX_DATA_T>> const &e, const Vertex<VERTEX_DATA_T> &destination);
    void CleanupExpiredEdges();

  public:
    Vertex() = delete;
    Vertex(std::string id, VERTEX_DATA_T data) : id(id), data(data) {};
    Vertex(std::string id, VERTEX_DATA_T data, double x, double y) : id(id), data(data), x(x), y(y) {};
    Vertex(std::istream &f, size_t prefix_size) { FromFile(f, prefix_size); }

    void AddEdge(std::weak_ptr<Edge<VERTEX_DATA_T>> const &e);
    bool IsConnected(const std::shared_ptr<Vertex<VERTEX_DATA_T>> &destination) const;

    std::string GetId() const override { return id; }
    const std::map<std::string, std::weak_ptr<Edge<VERTEX_DATA_T>>> &GetEdges() const { return edges; }
    double GetX() const { return x; };
    double GetY() const { return y; };
    VERTEX_DATA_T GetData() const { return data; }
    void SetData(VERTEX_DATA_T newData) { data = newData; }

    std::shared_ptr<Edge<VERTEX_DATA_T>> GetEdge(const std::shared_ptr<Vertex<VERTEX_DATA_T>> &to) const;

    friend std::ostream &operator<<(std::ostream &, const Vertex &);
    friend bool operator==(const Vertex &lhs, const Vertex &rhs);
    friend class Graph; // Allow Graph to call RemoveEdge
  };

  // Function Implementations

  /**
   * Adds an edge to this vertex
   * @param e The edge to add
   */
  template <typename VERTEX_DATA_T>
  void Vertex<VERTEX_DATA_T>::AddEdge(std::weak_ptr<Edge<VERTEX_DATA_T>> const &e) {
    if (auto edge = e.lock()) {
      auto toVertex = edge->GetTo();
      edges[toVertex->GetId()] = e;
    }
  }

  /**
   * Checks if this vertex is connected to another vertex
   * @param destination The vertex to check connection with
   * @return true if vertices are connected, false otherwise
   */
  template <typename VERTEX_DATA_T>
  bool Vertex<VERTEX_DATA_T>::IsConnected(const std::shared_ptr<Vertex<VERTEX_DATA_T>> &destination) const {
    auto it = edges.find(destination->GetId());
    if (it == edges.end()) {
      return false;
    }

    if (it->second.expired()) {
      // Edge is not valid anymore
      edges.erase(it);
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
  std::shared_ptr<Edge<VERTEX_DATA_T>> Vertex<VERTEX_DATA_T>::GetEdge(const std::shared_ptr<Vertex<VERTEX_DATA_T>> &to) const {
    auto it = edges.find(to->GetId());
    if (it == edges.end() || it->second.expired()) {
      throw std::runtime_error("Edge from " + id + " to " + to->GetId() + " does not exist");
    }
    return it->second.lock();
  }

  template <typename VERTEX_DATA_T>
  std::ostream &operator<<(std::ostream &os, const Vertex<VERTEX_DATA_T> &v) {
    os << "Vertex(" << v.id << ")";
    return os;
  }

  template <typename VERTEX_DATA_T>
  bool operator==(const Vertex<VERTEX_DATA_T> &lhs, const Vertex<VERTEX_DATA_T> &rhs) {
    return lhs.id == rhs.id && lhs.x == rhs.x && lhs.y == rhs.y;
  }

} // namespace cse
