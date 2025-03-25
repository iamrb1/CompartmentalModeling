#pragma once
#include <fstream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "../Util/Util.hpp"
#include "Edge.hpp"
#include "GraphExceptions.hpp"
#include "Vertex.hpp"

namespace cse {

  template <typename VERTEX_DATA_T> class Graph : public FileSerializable {
  private:
    std::unordered_map<std::string, std::shared_ptr<Vertex<VERTEX_DATA_T>>>
        vertices{}; ///< Collection of vertices in the graph, mapped by vertex ID
    std::unordered_map<std::string, std::shared_ptr<Edge<VERTEX_DATA_T>>>
        edges{}; ///< Collection of edges in the graph, mapped by edge ID

    // File parsing helpers
    void ParseSection(std::istream &f, const std::string &expected_section);
    void ParseVertices(std::istream &f, size_t indent_level);
    void ParseEdges(std::istream &f, size_t indent_level);

  protected:
    /** Gets the type name of the graph for serialization */
    std::string GetTypeName() const override { return "GRAPH"; }
    /** Gets the ID of the graph (empty string as graphs don't have IDs) */
    std::string GetId() const override { return ""; }
    /** Sets the ID of the graph (no-op as graphs don't have IDs) */
    void SetId(std::string) override {}
    std::vector<std::pair<std::string, SerializableProperty>> GetPropertyMap() override;
    void FromFile(std::istream &f, size_t prefix_size) override;

  public:
    Graph() {};
    Graph(std::istream &f) { FromFile(f, 0); }
    Vertex<VERTEX_DATA_T> &AddVertex(std::string const id, VERTEX_DATA_T data, double X = 0.0, double Y = 0.0);
    Vertex<VERTEX_DATA_T> &AddVertex(std::string const id, double X = 0.0, double Y = 0.0);
    Vertex<VERTEX_DATA_T> &GetVertex(std::string const &id) const;
    void RemoveVertex(std::string const id);
    bool HasVertex(std::string id) const { return vertices.find(id) != vertices.end(); };

    Edge<VERTEX_DATA_T> &AddEdge(std::string const v1_id, std::string const v2_id, double const &weight = 0.0);
    Edge<VERTEX_DATA_T> &AddEdge(Vertex<VERTEX_DATA_T> const &v1, Vertex<VERTEX_DATA_T> const &v2,
                                 double const &weight = 0.0);
    Edge<VERTEX_DATA_T> &GetEdge(std::string const &edge_id) const;
    Edge<VERTEX_DATA_T> &GetEdge(Vertex<VERTEX_DATA_T> const &from, Vertex<VERTEX_DATA_T> const &to) const;
    Edge<VERTEX_DATA_T> &GetEdge(std::string const &from_id, std::string const &to_id);
    void RemoveEdge(std::string const &edge_id);
    void RemoveEdge(Edge<VERTEX_DATA_T> const &edge);

    bool IsConnected(Vertex<VERTEX_DATA_T> const &v1, Vertex<VERTEX_DATA_T> const &v2) const;
    bool IsConnected(std::string const &v1_id, std::string const &v2_id) const;
  };

  // Function Implementations

  /**
   * Adds a new vertex to the graph
   * @param id Unique identifier for the vertex
   * @param data Vertex data
   * @param X X-coordinate position
   * @param Y Y-coordinate position
   * @return Reference to the created vertex
   * @throws runtime_error if vertex ID already exists
   */
  template <typename VERTEX_DATA_T>
  Vertex<VERTEX_DATA_T> &Graph<VERTEX_DATA_T>::AddVertex(std::string const id, VERTEX_DATA_T data, double X, double Y) {
    if (HasVertex(id)) {
      throw vertex_already_exists_error(id);
    }

    auto v = std::make_shared<Vertex<VERTEX_DATA_T>>(id, data, X, Y);
    vertices[id] = v;
    return *vertices[id];
  }

  /**
   * Adds a new vertex to the graph
   * @param id Unique identifier for the vertex
   * @param X X-coordinate position
   * @param Y Y-coordinate position
   * @return Reference to the created vertex
   * @throws runtime_error if vertex ID already exists
   */
  template <typename VERTEX_DATA_T>
  Vertex<VERTEX_DATA_T> &Graph<VERTEX_DATA_T>::AddVertex(std::string const id, double X, double Y) {
    if (HasVertex(id)) {
      throw vertex_already_exists_error(id);
    }

    auto v = std::make_shared<Vertex<VERTEX_DATA_T>>(id, X, Y);
    vertices[id] = v;
    return *vertices[id];
  }

  /**
   * Retrieves a vertex from the graph
   * @param id ID of the vertex to retrieve
   * @return Reference to the vertex
   * @throws out_of_range if vertex doesn't exist
   */
  template <typename VERTEX_DATA_T>
  Vertex<VERTEX_DATA_T> &Graph<VERTEX_DATA_T>::GetVertex(std::string const &id) const {
    if (vertices.find(id) == vertices.end()) {
      throw vertex_not_found_error(id);
    }
    return *(vertices.at(id));
  }

  /**
   * Removes a vertex from the graph
   * @param id ID of the vertex to remove
   * @throws out_of_range if vertex doesn't exist
   */
  template <typename VERTEX_DATA_T> void Graph<VERTEX_DATA_T>::RemoveVertex(std::string const id) {
    auto it = vertices.find(id);
    if (it == vertices.end()) {
      std::cout << "Did not find vertex to remove" << std::endl;
      throw vertex_not_found_error(id);
    }

    std::vector<std::string> edgesToRemove;
    for (auto it = edges.begin(); it != edges.end();) {
      std::shared_ptr<Edge<VERTEX_DATA_T>> edge = it->second;
      if (edge->GetTo().GetId() == id) {
        edge->GetFrom().RemoveEdge(edge->GetTo().GetId());
        edgesToRemove.push_back(edge->GetId());
      } else if (edge->GetFrom().GetId() == id) {
        edgesToRemove.push_back(edge->GetId());
      }
      ++it;
    }

    for (auto edgeId : edgesToRemove) {
      RemoveEdge(edgeId);
    }

    vertices.erase(it);
  }

  /**
   * Adds a new edge to the graph using vertex IDs
   * @param v1_id Source vertex ID
   * @param v2_id Destination vertex ID
   * @param weight Edge weight
   * @return Weak pointer to the created edge
   */
  template <typename VERTEX_DATA_T>
  Edge<VERTEX_DATA_T> &Graph<VERTEX_DATA_T>::AddEdge(std::string const v1_id, std::string const v2_id,
                                                     double const &weight) {
    if (!HasVertex(v1_id) || !HasVertex(v2_id)) {
      throw edge_connection_error("Both vertices must exist to create an edge");
    }

    std::string edge_id = v1_id + "-" + v2_id;
    auto v1 = vertices[v1_id];
    auto v2 = vertices[v2_id];
    auto edge = std::make_shared<Edge<VERTEX_DATA_T>>(edge_id, v1, v2, weight);

    v1->AddEdge(edge);
    edges[edge_id] = edge;
    return *edge;
  }

  /**
   * Adds a new edge to the graph using vertex pointers
   * @param v1 Source vertex
   * @param v2 Destination vertex
   * @param weight Edge weight
   * @return Weak pointer to the created edge
   */
  template <typename VERTEX_DATA_T>
  Edge<VERTEX_DATA_T> &Graph<VERTEX_DATA_T>::AddEdge(Vertex<VERTEX_DATA_T> const &v1, Vertex<VERTEX_DATA_T> const &v2,
                                                     double const &weight) {
    return AddEdge(v1.GetId(), v2.GetId(), weight);
  }

  /**
   * Gets an edge from the graph by ID
   * @param edge_id ID of the edge to retrieve
   * @return Weak pointer to the edge
   * @throws out_of_range if edge doesn't exist
   */
  template <typename VERTEX_DATA_T>
  Edge<VERTEX_DATA_T> &Graph<VERTEX_DATA_T>::GetEdge(std::string const &edge_id) const {
    if (edges.find(edge_id) == edges.end()) {
      throw edge_not_found_error(edge_id);
    }
    auto edge_ptr = edges.at(edge_id);
    return *edge_ptr;
  }

  /**
   * Gets an edge between two vertices
   * @param from Source vertex
   * @param to Destination vertex
   * @return Weak pointer to the edge
   */
  template <typename VERTEX_DATA_T>
  Edge<VERTEX_DATA_T> &Graph<VERTEX_DATA_T>::GetEdge(Vertex<VERTEX_DATA_T> const &from,
                                                     Vertex<VERTEX_DATA_T> const &to) const {
    return GetEdge(from.GetEdge(to)->GetId());
  }

  /**
   * Gets an edge between two vertices using their IDs
   * @param from_id Source vertex ID
   * @param to_id Destination vertex ID
   * @return Weak pointer to the edge
   */
  template <typename VERTEX_DATA_T>
  Edge<VERTEX_DATA_T> &Graph<VERTEX_DATA_T>::GetEdge(std::string const &from_id, std::string const &to_id) {
    return GetEdge(GetVertex(from_id), GetVertex(to_id));
  }

  /**
   * Removes an edge from the graph by ID
   * @param edge_id ID of the edge to remove
   * @throws out_of_range if edge doesn't exist
   */
  template <typename VERTEX_DATA_T> void Graph<VERTEX_DATA_T>::RemoveEdge(std::string const &edge_id) {
    auto it = edges.find(edge_id);
    if (it == edges.end()) {
      throw edge_not_found_error(edge_id);
    }
    it->second.reset();
    edges.erase(it);
  }

  /**
   * Removes an edge from the graph using a weak pointer
   * @param edge Weak pointer to the edge to remove
   * @throws out_of_range if edge doesn't exist or is expired
   */
  template <typename VERTEX_DATA_T> void Graph<VERTEX_DATA_T>::RemoveEdge(Edge<VERTEX_DATA_T> const &edge) {
    RemoveEdge(edge.GetId());
  }

  /**
   * Checks if two vertices are connected by an edge
   * @param v1 First vertex
   * @param v2 Second vertex
   * @return true if vertices are connected or false otherwise
   */
  template <typename VERTEX_DATA_T>
  bool Graph<VERTEX_DATA_T>::IsConnected(Vertex<VERTEX_DATA_T> const &v1, Vertex<VERTEX_DATA_T> const &v2) const {
    try {
      auto e = GetEdge(v1, v2);
      return e.IsConnected(v1, v2);
    } catch (const edge_not_found_error &) {
      // If there is an edge_not_found_error, the edge does not exist
      return false;
    }
    return false;
  }

  /**
   * Checks if two vertices are connected by an edge using their IDs
   * @param v1_id First vertex ID
   * @param v2_id Second vertex ID
   * @return true if vertices are connected, false otherwise
   */
  template <typename VERTEX_DATA_T>
  bool Graph<VERTEX_DATA_T>::IsConnected(std::string const &v1_id, std::string const &v2_id) const {
    return IsConnected(GetVertex(v1_id), GetVertex(v2_id));
  }

  /**
   * Parses the vertices section from a file stream
   * @param f Input stream to read from
   * @param indent_level The indentation level
   */
  template <typename VERTEX_DATA_T> void Graph<VERTEX_DATA_T>::ParseVertices(std::istream &f, size_t indent_level) {
    std::string line;
    while (FileUtil::CheckPrefixSize(f, indent_level + cse::BASE_INDENTATION)) {
      auto vertex = std::make_shared<Vertex<VERTEX_DATA_T>>(f, indent_level + cse::BASE_INDENTATION);
      vertices[vertex->GetId()] = vertex;
    }
  }

  /**
   * Parses the edges section from a file stream
   * @param f Input stream to read from
   * @param indent_level The indentation level
   */
  template <typename VERTEX_DATA_T> void Graph<VERTEX_DATA_T>::ParseEdges(std::istream &f, size_t indent_level) {
    while (FileUtil::CheckPrefixSize(f, indent_level + cse::BASE_INDENTATION)) {
      Edge<VERTEX_DATA_T>::CreateFromFile(f, indent_level + cse::BASE_INDENTATION, *this);
    }
  }

  /**
   * Parses a section header from a file stream and validates it
   * @param f Input stream to read from
   * @param expected_section The expected section header
   * @throws runtime_error if the section header is invalid
   */
  template <typename VERTEX_DATA_T>
  void Graph<VERTEX_DATA_T>::ParseSection(std::istream &f, const std::string &expected_section) {
    std::string line;
    std::getline(f, line);
    auto [section_key, _] = FileUtil::SeparateKeyValue(line);
    if (section_key != expected_section) {
      throw file_format_error("Expected " + expected_section + " section, got: " + section_key);
    }
  }

  /**
   * Deserializes the graph from a file stream
   * @param f Input stream to read from
   * @param prefix_size The indentation level (unused)
   * @throws runtime_error if the file format is invalid
   */
  template <typename VERTEX_DATA_T> void Graph<VERTEX_DATA_T>::FromFile(std::istream &f, size_t) {
    std::string line;
    std::getline(f, line);
    auto [key, value] = FileUtil::SeparateKeyValue(line);
    if (key != GetTypeName()) {
      throw file_format_error("Invalid type: " + key);
    }

    ParseSection(f, "Vertices");
    ParseVertices(f, cse::BASE_INDENTATION);

    std::getline(f, line); // Skip empty line
    ParseSection(f, "Edges");
    ParseEdges(f, cse::BASE_INDENTATION);
  }

  /**
   * Gets the serializable properties for this graph
   * @return Vector of property pairs for serialization
   */
  template <typename VERTEX_DATA_T>
  std::vector<std::pair<std::string, SerializableProperty>> Graph<VERTEX_DATA_T>::GetPropertyMap() {
    std::vector<std::pair<std::string, SerializableProperty>> properties;

    auto verticesWriter = [this](std::ostream &s) {
      s << "\n";
      for (auto const &[id, vertex] : vertices) {
        vertex->ToFile(s, cse::BASE_INDENTATION);
      }
    };

    auto edgesWriter = [this](std::ostream &s) {
      s << "\n";
      for (auto const &[id, edge] : edges) {
        edge->ToFile(s, cse::BASE_INDENTATION);
      }
    };

    // Handler not needed since we override FromFile
    properties.emplace_back("Vertices", SerializableProperty(verticesWriter, [](const std::string &) {}));
    properties.emplace_back("Edges", SerializableProperty(edgesWriter, [](const std::string &) {}));
    return properties;
  }

} // namespace cse
