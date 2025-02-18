#include "Graph.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>

namespace cse {
  Graph::Graph(std::istream &f) {
    FromFile(f, 0);
  }

  /**
   * TODO @lspecht: Add integration with GraphPosition
   * TODO @lspecht: Add GetAllEdges from vertex
   * TODO @lspecht: Add data parameter
   */
  std::shared_ptr<cse::Vertex> cse::Graph::AddVertex(std::string id, double X, double Y) {
    if (vertices.find(id) != vertices.end()) {
      throw std::runtime_error("Vertex already exists: " + id);
    }
    auto v = std::make_shared<cse::Vertex>(id, X, Y);
    vertices[id] = v;
    return v;
  }

  std::shared_ptr<cse::Vertex> cse::Graph::GetVertex(std::string const &id) const {
    if (vertices.find(id) == vertices.end()) {
      throw std::out_of_range("Vertex does not exist: " + id);
    }
    return vertices.at(id);
  }

  std::shared_ptr<cse::Vertex> cse::Graph::RemoveVertex(std::string id) {
    auto it = vertices.find(id);
    if (it == vertices.end()) {
      throw std::out_of_range("Vertex does not exist: " + id);
    }
    auto removedVertex = it->second;
    vertices.erase(it);
    return removedVertex;
  }

  void Graph::ValidateVerticesExist(const std::string &v1_id, const std::string &v2_id) const {
    if (vertices.find(v1_id) == vertices.end() || vertices.find(v2_id) == vertices.end()) {
      throw std::out_of_range("One or both vertices do not exist.");
    }
  }

  std::shared_ptr<Edge> Graph::CreateEdge(const std::string &edge_id, const std::string &v1_id,
                                          const std::string &v2_id, bool bidirectional) {
    auto v1 = vertices[v1_id];
    auto v2 = vertices[v2_id];
    return bidirectional ? std::make_shared<BidirectionalEdge>(edge_id, v1, v2)
                         : std::make_shared<Edge>(edge_id, v1, v2);
  }

  std::weak_ptr<cse::Edge> cse::Graph::AddEdge(std::string v1_id, std::string v2_id, bool bidirectional) {
    ValidateVerticesExist(v1_id, v2_id);

    std::string edge_id = v1_id + "-" + v2_id;
    auto edge = CreateEdge(edge_id, v1_id, v2_id, bidirectional);

    GetVertex(v1_id)->AddEdge(edge);
    edges[edge_id] = edge;
    return edge;
  }

  std::weak_ptr<cse::Edge> cse::Graph::AddEdge(std::shared_ptr<cse::Vertex> &v1, std::shared_ptr<cse::Vertex> &v2,
                                               bool bidirectional) {
    return AddEdge(v1->GetId(), v2->GetId(), bidirectional);
  }

  void cse::Graph::RemoveEdge(std::string const &edge_id) {
    auto it = edges.find(edge_id);
    if (it == edges.end()) {
      throw std::out_of_range("Edge does not exist: " + edge_id);
    }
    auto edge = std::move(it->second);
    edges.erase(it);
  }

  void Graph::RemoveEdge(std::weak_ptr<cse::Edge> edge) {
    if (edge.expired()) {
      throw std::out_of_range("Edge does not exist");
    }
    auto sh = edge.lock();
    RemoveEdge(sh->GetId());
  }

  std::weak_ptr<cse::Edge> cse::Graph::GetEdge(std::string const &edge_id) const {
    if (edges.find(edge_id) == edges.end()) {
      throw std::out_of_range("Edge does not exist.");
    }
    auto edge_ptr = edges.at(edge_id);
    return edge_ptr;
  }

  std::weak_ptr<cse::Edge> cse::Graph::GetEdge(std::shared_ptr<cse::Vertex> const from,
                                               std::shared_ptr<cse::Vertex> const to) const {
    return GetEdge(from->GetEdge(to)->GetId());
  }

  std::weak_ptr<cse::Edge> cse::Graph::GetEdge(std::string from_id, std::string to_id) {
    return GetEdge(GetVertex(from_id), GetVertex(to_id));
  }

  bool Graph::IsConnected(std::shared_ptr<cse::Vertex> const &v1, std::shared_ptr<cse::Vertex> const &v2) const {
    try {
      auto e = GetEdge(v1, v2);
      auto e_sh = e.lock();
      return e_sh->IsConnected(v1, v2);
    } catch (std::runtime_error) {
      // If there is a runtime error, the edge does not exist
    }

    return false;
  }

  bool Graph::IsConnected(std::string const &v1_id, std::string const &v2_id) const {
    return IsConnected(GetVertex(v1_id), GetVertex(v2_id));
  }

  void Graph::ParseVertices(std::istream &f, size_t indent_level) {
    std::string line;
    while (FileUtil::CheckPrefixSize(f, indent_level + 2)) {
      auto vertex = std::make_shared<Vertex>(f, indent_level + 2);
      vertices[vertex->GetId()] = vertex;
    }
  }

  void Graph::ParseEdges(std::istream &f, size_t indent_level) {
    while (FileUtil::CheckPrefixSize(f, indent_level + 2)) {
      Edge::CreateFromFile(f, indent_level + 2, *this);
    }
  }

  void Graph::ParseSection(std::istream &f, const std::string &expected_section) {
    std::string line;
    std::getline(f, line);
    auto [section_key, _] = FileUtil::SeparateKeyValue(line);
    if (section_key != expected_section) {
      throw std::runtime_error("Expected " + expected_section + " section, got: " + section_key);
    }
  }

  void Graph::FromFile(std::istream &f, size_t) {
    std::string line;
    std::getline(f, line);
    auto [key, value] = FileUtil::SeparateKeyValue(line);
    if (key != GetTypeName()) {
      throw std::runtime_error("Invalid type: " + key);
    }

    ParseSection(f, "Vertices");
    ParseVertices(f, 2);

    std::getline(f, line); // Skip empty line
    ParseSection(f, "Edges");
    ParseEdges(f, 2);
  }

  std::vector<std::pair<std::string, SerializableProperty>> Graph::GetPropertyMap() {
    std::vector<std::pair<std::string, SerializableProperty>> properties;

    auto verticesWriter = [this](std::ostream &s) {
      s << "\n";
      for (auto const &[id, vertex] : vertices) {
        vertex->ToFile(s, 2);
      }
    };

    auto edgesWriter = [this](std::ostream &s) {
      s << "\n";
      for (auto const &[id, edge] : edges) {
        edge->ToFile(s, 2);
      }
    };

    // Handler not needed since we override FromFile
    properties.emplace_back("Vertices", SerializableProperty(verticesWriter, [](const std::string &) {}));
    properties.emplace_back("Edges", SerializableProperty(edgesWriter, [](const std::string &) {}));
    return properties;
  }
} // namespace cse
