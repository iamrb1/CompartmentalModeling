#pragma once
#include <string>

#include "Vertex.h"

namespace cse498
{
  struct Edge
  {
    std::string id;
    Vertex *from;
    Vertex *to;
    bool bidirectional;
  };
}
