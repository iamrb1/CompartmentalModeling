#pragma once
#include "Vertex.hpp"
#include <iostream>
#include <string>

namespace cse {
  /**
   * A single direction edge
   */
  class Edge {
  private:
    std::string id;
    cse::Vertex &from;
    cse::Vertex &to;
    //
  public:
    Edge() = delete;
    virtual ~Edge() = default;
    Edge(std::string id, cse::Vertex &from, cse::Vertex &to) : id(id), from(from), to(to) {};
    bool IsBidirectional() { return false; };
    virtual bool IsConnected(cse::Vertex const &v1, cse::Vertex const &v2) { return v1 == from && v2 == to; };

    cse::Vertex const &GetFrom() { return from; };
    cse::Vertex const &GetTo() { return to; };
  };

  class BidirectionalEdge : cse::Edge {
  public:
    // TODO @lspecht: Handle GetFrom and GetTo on bidirectional case
    bool IsBidirectional() { return true; };
    bool IsConnected(cse::Vertex const &v1, cse::Vertex const &v2) override {
      return Edge::IsConnected(v1, v2) || Edge::IsConnected(v2, v1);
    };
  };
} // namespace cse
