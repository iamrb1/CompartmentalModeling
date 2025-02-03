#pragma once
#include <string>
#include <unordered_map>
#include <optional>
#include <vector>
#include <fstream>

#include "Vertex.hpp"
#include "Edge.hpp"

namespace cse498
{

  template <class T>
  class Graph
  {
  private:
    std::unordered_map<std::string, cse498::Vertex> vertices;
    std::unordered_map<std::string, cse498::Edge> edges;

  public:
    cse498::Vertex AddVertex(std::string id, T data, std::optional<double> X = std::nullopt, std::optional<double> Y = std::nullopt);
    cse498::Vertex RemoveVertex(std::string id);
    cse498::Vertex &GetVertex(std::string id);

    cse498::Edge AddEdge(std::string v1_id, std::string v2_id, bool bidirectional = false);
    cse498::Edge AddEdge(cse498::Vertex &v1, cse498::Vertex &v2, bool bidirectional = false);
    cse498::Edge RemoveEdge(cse498::Edge &edge);
    cse498::Edge RemoveEdge(std::string const &edge_id);
    cse498::Edge GetEdge(std::string const &edge_id);
    std::optional<cse498::Edge> GetEdge(cse498::Vertex &from, cse498::Vertex &to);
    std::optional<cse498::Edge> GetEdge(std::string from_id, std::string to_id);

    void ToFile(std::fstream &s);
    static Graph FromFile(std::fstream &s);
  };
}
