# Graph Class - Original Specs

## Description

A class to manage a collection of vertices and edges; should be useful for conducting graph algorithms.
The class should be able to load from files, save to file, access connection information, and add or remove vertices and edges. It should also allow fast access to vertices and easy iteration through vertices and edges.
[_From CSE498_](https://cse498.github.io/project-details.html).

## Base Class Specs

- For some of the methods, it might be considerable to change Vertex to GraphPosition

```cpp
template <class T>
class Graph {
public:
  cse498::Vertex AddVertex(std::string id, T data, std::optional<double> X, std::optional<double> Y);
  cse498::Vertex RemoveVertex(std::string id);
  cse498::Vertex& GetVertex(std::string id);

  cse498::Edge AddEdge(std::string v1_id, std::string v2_id, bool bidirectional = false );
  cse498::Edge AddEdge(cse498::Vertex &v1, cse498::Vertex &v2, bool bidirectional = false );
  void RemoveEdge(cse498::Edge &edge);
  void RemoveEdge(std::string const& edge_id);
  cse498::Edge GetEdge(std::string const& edge_id);
  std::optional<cse498::Edge> GetEdge(cse498::Vertex &from, cse498::Vertex &to);
  std::optional<cse498::Edge> GetEdge(std::string from_id, std::string to_id );

  void ToFile(std::fstream s);
  static Graph FromFile(std::fstream s);
}
```

## Advanced class specs and changes

### List of major changes:

- Change from `BidirectionalEdge` into two one directional edges. This will be ideal for the scenario where we want to make a graph creator that can be more specific and have different weights for different directions of edges.
- Add weights to Edges
- Allow for templated data being stored in Vertices, storing information on them optionally.
  - This can get somewhat complex, as supporting any type on the vertex without templating the class could be somewhat hard.
- Integration with GraphPosition, specifically on traversal methods and algorithms to be implemented on it.
- Allow for representation of graphs in WASM and EmScriptem.
- Update error handling to use Graph specific errors
- For saving to and reading from files, we may support basic data types and even integrate with th existing file serializer function. But we will not aim to support any existing datatype

### Advanced class public interface:

```cpp
namespace cse {

  class Graph : public FileSerializable {
  public:
    Graph() {};
    Graph(std::istream &f);

    std::shared_ptr<cse::Vertex> GetVertex(std::string const &id) const;
    std::shared_ptr<cse::Vertex> RemoveVertex(std::string id);

    void RemoveEdge(std::string const &edge_id);
    void RemoveEdge(std::weak_ptr<cse::Edge> edge);

    std::weak_ptr<cse::Edge> GetEdge(std::string const &edge_id) const;
    std::weak_ptr<cse::Edge> GetEdge(std::shared_ptr<cse::Vertex> const from,
                                     std::shared_ptr<cse::Vertex> const to) const;
    std::weak_ptr<cse::Edge> GetEdge(std::string from_id, std::string to_id);

    bool IsConnected(std::shared_ptr<cse::Vertex> const &v1, std::shared_ptr<cse::Vertex> const &v2) const;
    bool IsConnected(std::string const &v1_id, std::string const &v2_id) const;

    // New/Changed methods
    /**
     * Changes how to go through the edges when traversing
     * Function can also be moved to GraphPosition
     */
    void SetNextTraversal(std::function(cse::Vertex(cse::Edges)));

    // Changed to support the addition of Value
    std::shared_ptr<cse::Vertex> AddVertex(std::string id, double X = 0.0, double Y = 0.0, std::optional<T> value = std::nullopt);

    // Changed to support adding weights
    std::weak_ptr<cse::Edge> AddEdge(std::string v1_id, std::string v2_id, double weigth = 0.0);
    std::weak_ptr<cse::Edge> AddEdge(std::shared_ptr<cse::Vertex> &v1, std::shared_ptr<cse::Vertex> &v2, , double weigth = 0.0);
    /**
     * Saves this into a JS Canvas
     * Maybe we could save to a file and JS use that file
     */
    void SaveToJS(/* Not sure what the argument type would be*/)

  };
} // namespace cse
```
