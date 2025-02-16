#include "Edge.hpp"
#include "Graph.hpp"

void cse::Edge::CreateFromFile(std::istream &f, size_t indent_level, Graph &graph) {
  std::string line;
  std::getline(f, line);
  auto [key, id] = FileUtil::SeparateKeyValue(line);
  if (key != "EDGE") {
    throw std::runtime_error("Invalid type: " + key);
  }

  auto props = ParseProperties(f, indent_level);
  auto from = graph.GetVertex(props.from_id);
  auto to = graph.GetVertex(props.to_id);
  graph.AddEdge(from, to, props.bidirectional);
}