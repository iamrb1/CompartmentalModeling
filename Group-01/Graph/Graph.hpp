#pragma once
#include <fstream>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "Edge.hpp"
#include "Vertex.hpp"

namespace cse {

  template <class T> class Graph {
  private:
    std::unordered_map<std::string, cse::Vertex> vertices;
    std::unordered_map<std::string, cse::Edge> edges;

  public:
    cse::Vertex AddVertex(std::string id, T data, std::optional<double> X = std::nullopt,
                          std::optional<double> Y = std::nullopt);
    cse::Vertex RemoveVertex(std::string id);
    cse::Vertex &GetVertex(std::string id);

    cse::Edge AddEdge(std::string v1_id, std::string v2_id, bool bidirectional = false);
    cse::Edge AddEdge(cse::Vertex &v1, cse::Vertex &v2, bool bidirectional = false);
    cse::Edge RemoveEdge(cse::Edge &edge);
    cse::Edge RemoveEdge(std::string const &edge_id);
    cse::Edge GetEdge(std::string const &edge_id);
    std::optional<cse::Edge> GetEdge(cse::Vertex &from, cse::Vertex &to);
    std::optional<cse::Edge> GetEdge(std::string from_id, std::string to_id);

    void ToFile(std::fstream &s);
    static Graph FromFile(std::fstream &s);
  };
} // namespace cse
