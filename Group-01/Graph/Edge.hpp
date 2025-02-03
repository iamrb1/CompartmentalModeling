#pragma once
#include <string>

#include "Vertex.hpp"

namespace cse498 {
  struct Edge {
    std::string id;
    Vertex *from;
    Vertex *to;
    bool bidirectional;
  };
} // namespace cse498
