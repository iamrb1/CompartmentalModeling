#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace cse {
  class Vertex {
  private:
    std::string id;
    double x, y = 0;
    std::map<std::string, std::weak_ptr<Edge>> edges;

  public:
    void AddEdge(std::shared_ptr<Vertex> destination, std::shared_ptr<Edge> e);
    bool IsConnected(std::shared_ptr<Vertex> destination);

    std::string GetId() const { return id; };
    cse::Edge &const GetEdge(Vertex &const to);

    friend std::ostream &operator<<(std::ostream &, const Vertex &);
  };

  std::ostream &operator<<(std::ostream &os, const cse::Vertex &v) {
    os << "Vertex(" << v.id << ")";
    return os;
  }

} // namespace cse
