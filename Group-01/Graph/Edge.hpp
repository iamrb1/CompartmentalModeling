#pragma once
#include <iostream>
#include <memory>
#include <string>

#include "../Util/Util.hpp"
#include "GraphExceptions.hpp"
#include "Vertex.hpp"

namespace cse {
template <typename VERTEX_DATA_T, bool IS_BIDIRECTIONAL>
class Graph;  // Forward declaration

template <typename VERTEX_DATA_T>
class Edge : public StreamSerializable {
 private:
  std::string id;
  std::shared_ptr<Vertex<VERTEX_DATA_T>> from;
  std::shared_ptr<Vertex<VERTEX_DATA_T>> to;
  double weight = 0.0;
  bool show_weight = true;  ///< Display the weight

  // Helper for parsing edge properties
  struct EdgeProperties {
    std::string from_id;
    std::string to_id;
    double weight;
  };

  static EdgeProperties ParseProperties(std::istream &f, size_t indent_level) {
    auto properties =
        FileUtil::GetProperties(f, indent_level + cse::BASE_INDENTATION);
    if (properties.size() != 3) {
      throw cse::file_format_error(
          "Error reading file. Must have correct parameter size");
    }
    if (properties.at(0).first != "from") {
      throw cse::file_format_error(
          "Error reading file. 'From' should be first property");
    }
    if (properties.at(1).first != "to") {
      throw cse::file_format_error(
          "Error reading file. 'To' should be second property");
    }
    if (properties.at(2).first != "weight") {
      throw cse::file_format_error(
          "Error reading file. 'Weight' should be third property");
    }

    return EdgeProperties{properties.at(0).second, properties.at(1).second,
                          std::stod(properties.at(2).second)};
  }

 protected:
  std::string GetTypeName() const override { return "EDGE"; }
  std::vector<std::pair<std::string, SerializableProperty>> GetPropertyMap()
      override {
    std::vector<std::pair<std::string, SerializableProperty>> properties;
    properties.emplace_back(
        "from",
        SerializableProperty([this](std::ostream &s) { s << from->GetId(); },
                             [](const std::string &) {}));
    properties.emplace_back(
        "to",
        SerializableProperty([this](std::ostream &s) { s << to->GetId(); },
                             [](const std::string &) {}));
    properties.emplace_back(
        "weight", SerializableProperty([this](std::ostream &s) { s << weight; },
                                       [](const std::string &) {}));
    return properties;
  }
  void SetId(std::string newId) override { id = newId; };

 public:
  virtual ~Edge() = default;
  Edge(const std::string id, std::shared_ptr<Vertex<VERTEX_DATA_T>> &from,
       std::shared_ptr<Vertex<VERTEX_DATA_T>> &to, double weight = 0.0)
      : id(id), from(from), to(to), weight(weight) {}
  Edge() = delete;

  virtual bool IsConnected(const Vertex<VERTEX_DATA_T> &v1,
                           const Vertex<VERTEX_DATA_T> &v2) {
    assert(from != nullptr);
    assert(to != nullptr);
    return v1 == *from && v2 == *to;
  };

  Vertex<VERTEX_DATA_T> &GetFrom() const { return *from; }
  Vertex<VERTEX_DATA_T> &GetTo() const { return *to; }
  std::string GetId() const override { return id; };
  double GetWeight() const { return weight; };
  bool GetShowWeight() const { return show_weight; }
  void SetShowWeight(bool show) { show_weight = show; }

  template <bool IS_BIDIRECTIONAL>
  static void CreateFromFile(std::istream &f, size_t indent_level,
                             Graph<VERTEX_DATA_T, IS_BIDIRECTIONAL> &graph);
};

// Function Implementations

/**
 * Creates an edge from file input and adds it to the graph
 * @param f Input stream to read from
 * @param indent_level The indentation level
 * @param graph The graph to add the edge to
 * @throws runtime_error if input format is invalid
 */
template <typename VERTEX_DATA_T>
template <bool IS_BIDIRECTIONAL>
void Edge<VERTEX_DATA_T>::CreateFromFile(
    std::istream &f, size_t indent_level,
    Graph<VERTEX_DATA_T, IS_BIDIRECTIONAL> &graph) {
  std::string line;
  std::getline(f, line);
  auto [key, id] = FileUtil::SeparateKeyValue(line);
  if (key != "EDGE") {
    throw file_format_error("Invalid type: " + key);
  }

  auto props = ParseProperties(f, indent_level);
  auto &from = graph.GetVertex(props.from_id);
  auto &to = graph.GetVertex(props.to_id);
  graph.AddEdge(from, to, props.weight);
}
}  // namespace cse
