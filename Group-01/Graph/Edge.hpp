#pragma once
#include <string>

#include "Vertex.hpp"

namespace cse {
  /**
   * A single direction edge
   */
  class Edge {
  private:
    std::string id;
    std::shared_ptr<cse::Vertex> from;
    std::shared_ptr<cse::Vertex> to;

  public:
    Edge(std::string id, std::shared_ptr<cse::Vertex> from, std::shared_ptr<cse::Vertex> to)
        : id(id), from(from), to(to) {};
    bool IsBidirectional() { return false; };
    virtual bool IsConnected(std::shared_ptr<cse::Vertex> v1, std::shared_ptr<cse::Vertex> v2) {
      return v1 == from && v2 == to;
    };
  };

  class BidirectionalEdge : cse::Edge {
  public:
    bool IsBidirectional() { return true; };
    bool IsConnected(std::shared_ptr<cse::Vertex> v1, std::shared_ptr<cse::Vertex> v2) override {
      return Edge::IsConnected(v1, v2) || Edge::IsConnected(v2, v1);
    };
  };
} // namespace cse
