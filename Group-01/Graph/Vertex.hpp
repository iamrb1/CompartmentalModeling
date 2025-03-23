#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../Util/Util.hpp"

namespace cse {
  class Edge;
  class Vertex : public FileSerializable {
  private:
    /** Unique identifier for the vertex */
    std::string id;
    /** X coordinate position of the vertex */
    double x{0};
    /** Y coordinate position of the vertex */
    double y{0};
    /** Map of edges connected to this vertex, keyed by destination vertex ID */
    std::map<std::string, std::weak_ptr<Edge>> edges{};

  protected:
    std::string GetTypeName() const override { return "VERTEX"; }
    std::vector<std::pair<std::string, SerializableProperty>> GetPropertyMap() override;
    void SetId(std::string newId) override { id = newId; };

  private:
    void AddEdge(std::weak_ptr<Edge> const &e, std::shared_ptr<cse::Vertex> const &destination);
    void CleanupExpiredEdges();

  public:
    Vertex() = delete;
    Vertex(std::string id) : id(id) {};
    Vertex(std::string id, double x, double y) : id(id), x(x), y(y) {};
    Vertex(std::istream &f, size_t prefix_size);

    void AddEdge(std::weak_ptr<Edge> const &e);
    bool IsConnected(std::shared_ptr<cse::Vertex> const &destination);

    std::string GetId() const override { return id; }
    const std::map<std::string, std::weak_ptr<Edge>> &GetEdges() const { return edges; };
    double GetX() const { return x; };
    double GetY() const { return y; };

    std::shared_ptr<cse::Edge> const GetEdge(std::shared_ptr<cse::Vertex> const &to);

    friend std::ostream &operator<<(std::ostream &, const Vertex &);
    friend bool operator==(const Vertex &lhs, const Vertex &rhs);
    friend class Graph; // Allow Graph to call RemoveEdge
  };

  std::ostream &operator<<(std::ostream &os, const cse::Vertex &v);
  bool operator==(const Vertex &lhs, const Vertex &rhs);
} // namespace cse
