#include "Vertex.hpp"
#include "Edge.hpp"
#include <cassert>
#include <iostream>

void cse498::Vertex::AddEdge(std::shared_ptr<cse498::Vertex> destination, std::shared_ptr<Edge> e) {
  assert(e->IsConnected(std::make_shared<Vertex>(this), destination));
  // TODO @lspecht: Should we include a check if the edge already exist?
  edges[destination->GetId()] = e;
}

// TODO @lspecht: Move expiration logic to a single function
bool cse498::Vertex::IsConnected(std::shared_ptr<Vertex> destination) {
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

cse498::Edge &const cse498::Vertex::GetEdge(Vertex &const to) {
  auto e = edges.at(to.GetId());
  if (e.expired()) {
    throw std::runtime_error("Vertex from" + id + " to " + to.GetId() + " does not exists");
  }
  return *(e.lock());
}
