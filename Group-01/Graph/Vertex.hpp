#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace cse {
  class Edge;
  class Vertex {
  private:
    std::string id;
    double x, y = 0;
    std::map<std::string, std::weak_ptr<Edge>> edges{};
    void AddEdge(std::shared_ptr<Edge> const &e, cse::Vertex const &destination);

  public:
    // Vertex() = delete;
    Vertex(std::string id) : id(id) {};
    Vertex(std::string id, double x, double y) : id(id), x(x), y(y) {};

    void AddEdge(std::shared_ptr<Edge> const &e);
    bool IsConnected(Vertex const &destination);

    std::string GetId() const { return id; };
    double GetX() const { return x; };
    double GetY() const { return y; };

    cse::Edge const &GetEdge(Vertex const &to);

    friend std::ostream &operator<<(std::ostream &, const Vertex &);
    friend bool operator==(const Vertex &lhs, const Vertex &rhs);
  };

  std::ostream &operator<<(std::ostream &os, const cse::Vertex &v);
  bool operator==(const Vertex &lhs, const Vertex &rhs);
} // namespace cse
