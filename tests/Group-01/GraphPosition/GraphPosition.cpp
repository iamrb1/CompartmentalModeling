#include "../../../Group-01/GraphPosition/GraphPosition.hpp"

#include <sstream>
#include <vector>

#include "../../../Group-01/Graph/Graph.hpp"
#include "../../../third-party/Catch/single_include/catch2/catch.hpp"

using Catch::Matchers::WithinAbs;

TEST_CASE("GraphPosition Constructor Tests", "[GraphPosition]") {
  cse::Graph<std::string> graph;

  SECTION("Valid GraphPosition Initialization") {
    auto &v1 = graph.AddVertex("A", "Vertex A Data");
    REQUIRE_NOTHROW(cse::GraphPosition<std::string>(graph, &v1));
  }

  SECTION("Invalid GraphPosition Initialization with nullptr Vertex") {
    REQUIRE_THROWS_AS(cse::GraphPosition<std::string>(graph, nullptr),
                      std::invalid_argument);
  }
}

TEST_CASE("GraphPosition Getters and Setters Tests", "[GraphPosition]") {
  cse::Graph<std::string> graph;
  auto &v1 = graph.AddVertex("A", "Vertex A Data");
  auto &v2 = graph.AddVertex("B", "Vertex B Data");

  cse::GraphPosition<std::string> pos(graph, &v1);

  SECTION("GetCurrentVertex returns the correct vertex") {
    REQUIRE(pos.GetCurrentVertex() == v1);
  }

  SECTION("SetCurrentVertex updates current vertex") {
    pos.SetCurrentVertex(v2);
    REQUIRE(pos.GetCurrentVertex() == v2);
  }

  SECTION("SetCurrentVertex marks the new vertex as visited") {
    pos.SetCurrentVertex(v2);
    REQUIRE(!pos.IsVisited(pos.GetCurrentVertex()));
  }
}

TEST_CASE("GraphPosition Visited Vertices Tests", "[GraphPosition]") {
  cse::Graph<std::string> graph;
  auto &v1 = graph.AddVertex("A", "Vertex A Data");
  auto &v2 = graph.AddVertex("B", "Vertex B Data");
  auto &v3 = graph.AddVertex("C", "Vertex C Data");

  graph.AddEdge("A", "B");
  graph.AddEdge("B", "C");

  cse::GraphPosition<std::string> pos(graph, &v1);

  SECTION("MarkVisited and IsVisited work correctly") {
    pos.MarkVisited(v1);
    pos.MarkVisited(v2);
    REQUIRE(pos.IsVisited(v1));
    REQUIRE(pos.IsVisited(v2));
    REQUIRE(!pos.IsVisited(v3));
  }
}

TEST_CASE("GraphPosition Depth and Breadth-First Search Traversal Tests", "[GraphPosition]") {
  cse::Graph<std::string> graph;
  auto &v1 = graph.AddVertex("A", "Vertex A Data");
  auto &v2 = graph.AddVertex("B", "Vertex B Data");
  auto &v3 = graph.AddVertex("C", "Vertex C Data");
  auto &v4 = graph.AddVertex("D", "Vertex D Data");

  graph.AddEdge("A", "B");
  graph.AddEdge("A", "C");
  graph.AddEdge("B", "D");

  cse::GraphPosition<std::string> pos(graph, &v1);

  SECTION("DFS follows deep traversal first") {
    std::vector<cse::Vertex<std::string> *> order{&v4, &v2, &v3, &v1};
    size_t ind = 0;
    while ((bool)++pos) {
      auto &v = *(order.at(ind));
      CHECK(pos.GetCurrentVertex() == v);
      ind++;
    }
  }

  SECTION("BFS follows breadth traversal first") {
    std::vector<cse::Vertex<std::string> *> order{&v1, &v2, &v3, &v4};
    pos.SetTraversalMode(cse::TraversalModes::BFS<std::string>());
    pos.ResetTraversal(v1);
    size_t ind = 0;
    while ((bool)++pos) {
      auto &v = *(order.at(ind));
      CHECK(pos.GetCurrentVertex() == v);
      ind++;
    }
  }
}