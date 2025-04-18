#pragma once

#include <fstream>
#include <memory>
#include <string>

#include "../third-party/json/json.hpp"
#include "Edge.hpp"
#include "Graph.hpp"
#include "Vertex.hpp"

namespace cse {

/**
 * Used ChatGPT to assist adding support to
 * Import and Export a Graph in JSON
 */
template <typename VERTEX_DATA_T, bool IS_BIDIRECTIONAL = false>
class GraphJson {
 private:
  Graph<VERTEX_DATA_T, IS_BIDIRECTIONAL> &graph;

 public:
  /**
   * Constructor
   * @param g The graph
   */
  GraphJson(Graph<VERTEX_DATA_T, IS_BIDIRECTIONAL> &g) : graph(g) {}

  /**
   * Exports the graph to a JSON
   * @return JSON of the graoh
   */
  std::string ExportToJson() const {
    nlohmann::json j;

    // Export vertices
    j["vertices"] = nlohmann::json::array();
    auto vertices = graph.GetVertices();
    for (auto v : vertices) {
      nlohmann::json vertex;
      vertex["id"] = v->GetId();
      vertex["data"] = v->GetData();
      vertex["x"] = v->GetX();
      vertex["y"] = v->GetY();
      j["vertices"].push_back(vertex);
    }

    // Export edges
    j["edges"] = nlohmann::json::array();
    auto edges = graph.GetEdges();
    for (auto e : edges) {
      nlohmann::json edge;
      edge["id"] = e->GetId();
      edge["from"] = e->GetFrom().GetId();
      edge["to"] = e->GetTo().GetId();
      edge["weight"] = e->GetWeight();
      j["edges"].push_back(edge);
    }

    return j.dump(4);
  }

  /**
   * Exports the graph to a JSON
   * @param filename Path to the output file
   * @return true if successful, false otherwise
   */
  bool ExportToFile(const std::string &filename) const {
    try {
      std::ofstream file(filename);
      if (!file.is_open()) {
        return false;
      }

      file << ExportToJson();
      file.close();
      return true;
    } catch (const std::exception &) {
      return false;
    }
  }

  /**
   * Imports a graph from a JSON
   * @param jsonStr JSON to import
   * @return true if successful, false otherwise
   */
  bool ImportFromJson(const std::string &jsonStr) {
    try {
      nlohmann::json j = nlohmann::json::parse(jsonStr);

      // Clear graph
      graph.ClearGraph();

      // Import vertices
      if (j.contains("vertices") && j["vertices"].is_array()) {
        for (const auto &vertex : j["vertices"]) {
          try {
            graph.AddVertex(vertex["id"], vertex["data"], vertex["x"],
                            vertex["y"]);
          } catch (const cse::vertex_already_exists_error &e) {
            // Skip duplicate vertices
          } catch (const std::exception &e) {
            // Skip vertices with invalid data
          }
        }
      }

      // Import edges
      if (j.contains("edges") && j["edges"].is_array()) {
        for (const auto &edge : j["edges"]) {
          try {
            graph.AddEdge(edge["from"], edge["to"], edge["weight"]);
          } catch (const cse::vertex_not_found_error &e) {
            // Skip edges with missing vertices
          } catch (const cse::edge_connection_error &e) {
            // Skip edges with connection issues
          } catch (const std::exception &e) {
            // Skip edges otherwise
          }
        }
      }

      return true;
    } catch (const nlohmann::json::parse_error &e) {
      // JSON parsing failed
      return false;
    } catch (const std::exception &e) {
      return false;
    }
  }

  /**
   * Imports a graph from a JSON
   * @param filename Path to the file
   * @return true if successful, false otherwise
   */
  bool ImportFromFile(const std::string &filename) {
    try {
      std::ifstream file(filename);
      if (!file.is_open()) {
        // File could not be opened
        return false;
      }

      std::string jsonStr((std::istreambuf_iterator<char>(file)),
                          std::istreambuf_iterator<char>());
      file.close();

      return ImportFromJson(jsonStr);
    } catch (const std::ios_base::failure &e) {
      // File I/O error
      return false;
    } catch (const std::exception &e) {
      return false;
    }
  }
};

}  // namespace cse