#include "../../../Group-01/Graph/Graph.hpp"

#include <memory>
#include <sstream>
#include <vector>

#include "../../../Group-01/Graph/Edge.hpp"
#include "../../../Group-01/Graph/GraphExceptions.hpp"
#include "../../../Group-01/Graph/Vertex.hpp"
#include "../../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../Utils/TestUtils.hpp"

using Catch::Matchers::WithinAbs;

TEST_CASE("Test cse::Graph (Bidirectional)", "[base]") {
  // Use std::string as the vertex data type
  using BidirectionalGraph = cse::Graph<std::string, true>;

  BidirectionalGraph graph;

  // Test adding vertices
  auto &v1 = graph.AddVertex("id1", "Vertex1 Data");
  auto &v2 = graph.AddVertex("id2", "Vertex2 Data");
  CHECK(graph.GetVertex("id1").GetId() == "id1");
  CHECK(graph.GetVertex("id2").GetId() == "id2");

  CHECK_THROWS_AS(graph.AddVertex("id1", "Duplicate Data"), std::runtime_error);

  // Test removing vertices
  graph.RemoveVertex("id1");
  CHECK_THROWS_AS(graph.GetVertex("id1"), cse::vertex_not_found_error);

  // Test adding edges - same vertex being readded
  auto v1_2 = graph.AddVertex("id1", "Vertex1 Data");
  graph.AddEdge("id1", "id2", 0.0);

  CHECK(graph.IsConnected(v1_2, v2));
  CHECK(graph.IsConnected(v2, v1_2));
  CHECK(graph.IsConnected("id1", "id2"));
  CHECK(graph.IsConnected("id2", "id1"));

  // Testing Adding Edges by reference
  auto &v4 = graph.AddVertex("id4", "Vertex4 Data");
  auto &v5 = graph.AddVertex("id5", "Vertex5 Data");

  CHECK(!graph.IsConnected(v1_2, v4));
  CHECK(!graph.IsConnected(v4, v5));
  CHECK(!graph.IsConnected(v4, v4));

  auto &e2 = graph.AddEdge(v4, v5, 0.0);
  CHECK(graph.IsConnected(v4, v5));
  CHECK(graph.IsConnected(v5, v4));

  // Testing removing edges
  auto &v4_v5_edge = graph.GetEdge(v4.GetId(), v5.GetId());
  REQUIRE_THAT(v4_v5_edge.GetWeight(),
               WithinAbs(0, cse_test_utils::FLOAT_DELTA));

  graph.RemoveEdge(e2);
  CHECK(!graph.IsConnected(v4, v5));
  CHECK(!graph.IsConnected(v5, v4));
  CHECK_THROWS_AS(graph.GetEdge(v4.GetId(), v5.GetId()), std::runtime_error);
  CHECK_THROWS_AS(v4.GetEdge(v5), std::runtime_error);

  auto &e3 = graph.AddEdge("id1", "id2", 2);
  CHECK(graph.IsConnected(v1_2, v2));
  CHECK(graph.IsConnected(v2, v1));
  REQUIRE_THAT(e3.GetWeight(), WithinAbs(2, cse_test_utils::FLOAT_DELTA));
}

// Same test case as above but with a not bidirectional graph
TEST_CASE("Test cse::Graph", "[base]") {
  // Use std::string as the vertex data type
  using TestGraph = cse::Graph<std::string>;

  TestGraph graph;

  // Test adding vertices
  auto &v1 = graph.AddVertex("id1", "Vertex1 Data");
  auto &v2 = graph.AddVertex("id2", "Vertex2 Data");
  CHECK(graph.GetVertex("id1").GetId() == "id1");
  CHECK(graph.GetVertex("id2").GetId() == "id2");

  CHECK_THROWS_AS(graph.AddVertex("id1", "Duplicate Data"),
                  cse::vertex_already_exists_error);

  // Test removing vertices
  graph.RemoveVertex("id1");
  CHECK_THROWS_AS(graph.GetVertex("id1"), cse::vertex_not_found_error);

  // Test adding edges
  v1 = graph.AddVertex("id1", "Vertex1 Data");
  graph.AddEdge("id1", "id2", 0.0);

  CHECK(graph.IsConnected(v1, v2));
  CHECK(!graph.IsConnected(v2, v1));
  CHECK(graph.IsConnected("id1", "id2"));
  CHECK(!graph.IsConnected("id2", "id1"));

  // Testing Adding Edges by reference
  auto &v4 = graph.AddVertex("id4", "Vertex4 Data");
  auto &v5 = graph.AddVertex("id5", "Vertex5 Data");

  CHECK(!graph.IsConnected(v1, v4));
  CHECK(!graph.IsConnected(v4, v5));
  CHECK(!graph.IsConnected(v4, v4));

  auto &e2 = graph.AddEdge(v4, v5, 0.0);
  CHECK(graph.IsConnected(v4, v5));
  CHECK(!graph.IsConnected(v5, v4));

  // Testing removing edges
  auto &v4_v5_edge = graph.GetEdge(v4.GetId(), v5.GetId());
  REQUIRE_THAT(v4_v5_edge.GetWeight(),
               WithinAbs(0, cse_test_utils::FLOAT_DELTA));

  graph.RemoveEdge(e2);

  CHECK(!graph.IsConnected(v4, v5));
  CHECK(!graph.IsConnected(v5, v4));
  CHECK_THROWS_AS(graph.GetEdge(v4.GetId(), v5.GetId()),
                  cse::edge_not_found_error);
  CHECK_THROWS_AS(v4.GetEdge(v5), cse::edge_not_found_error);

  auto &e3 = graph.AddEdge("id1", "id2", 2);
  CHECK(graph.IsConnected(v1, v2));
  CHECK(!graph.IsConnected(v2, v1));
  REQUIRE_THAT(e3.GetWeight(), WithinAbs(2, cse_test_utils::FLOAT_DELTA));
}

TEST_CASE("Test cse::Graph - To file", "Export to file") {
  using TestGraph = cse::Graph<std::string>;

  TestGraph graph;
  // Test adding vertices
  auto &v1 = graph.AddVertex("id1", "Vertex1 Data");
  auto &v2 = graph.AddVertex("id2", "Vertex2 Data", 1.5);
  graph.AddEdge(v1, v2);
  std::stringstream s;

  graph.ToFile(s);

  std::vector<std::string> lines{
      "GRAPH:",       "  Vertices:",      "    VERTEX:id2",
      "      X:1.5",  "      Y:0",        "    VERTEX:id1",
      "      X:0",    "      Y:0",        "",
      "  Edges:",     "    EDGE:id1-id2", "      from:id1",
      "      to:id2", "      weight:0",   ""};
  // REQUIRE(cse_test_utils::CheckForStringFile(lines, s));
}

TEST_CASE("Test cse::Graph - From file", "Read from file") {
  using TestGraph = cse::Graph<std::string>;

  std::vector<std::string> lines{
      "GRAPH:",       "  Vertices:",      "    VERTEX:id2",
      "      X:1.5",  "      Y:0",        "    VERTEX:id1",
      "      X:1",    "      Y:1",        "",
      "  Edges:",     "    EDGE:id1-id2", "      from:id1",
      "      to:id2", "      weight:0",   ""};
  std::stringstream s;
  cse_test_utils::BuildFileFromVector(lines, s);

  TestGraph graph(s);
  CHECK(graph.GetVertex("id1").GetId() == "id1");
  REQUIRE_THAT(graph.GetVertex("id1").GetX(),
               WithinAbs(1, cse_test_utils::FLOAT_DELTA));
  REQUIRE_THAT(graph.GetVertex("id1").GetY(),
               WithinAbs(1, cse_test_utils::FLOAT_DELTA));
  REQUIRE_THAT(graph.GetVertex("id2").GetX(),
               WithinAbs(1.5, cse_test_utils::FLOAT_DELTA));
  REQUIRE_THAT(graph.GetVertex("id2").GetY(),
               WithinAbs(0, cse_test_utils::FLOAT_DELTA));
  CHECK(graph.GetVertex("id2").GetId() == "id2");
  CHECK(graph.IsConnected("id1", "id2"));
  CHECK(!graph.IsConnected("id2", "id1"));

  lines = {"GRAPH:",
           "  Vertices:",
           "    VERTEX:id2",
           "      X:1.5",
           "      Y:0",
           "    VERTEX:id1",
           "      X:1",
           "      Y:1",
           "",
           "  Edges:",
           ""};
  s.str(std::string());
  cse_test_utils::BuildFileFromVector(lines, s);

  graph = TestGraph(s);
  CHECK(graph.GetVertex("id1").GetId() == "id1");
  REQUIRE_THAT(graph.GetVertex("id1").GetX(),
               WithinAbs(1, cse_test_utils::FLOAT_DELTA));
  REQUIRE_THAT(graph.GetVertex("id1").GetY(),
               WithinAbs(1, cse_test_utils::FLOAT_DELTA));
  REQUIRE_THAT(graph.GetVertex("id2").GetX(),
               WithinAbs(1.5, cse_test_utils::FLOAT_DELTA));
  REQUIRE_THAT(graph.GetVertex("id2").GetY(),
               WithinAbs(0, cse_test_utils::FLOAT_DELTA));
  CHECK(graph.GetVertex("id2").GetId() == "id2");
  CHECK(!graph.IsConnected("id1", "id2"));
  CHECK(!graph.IsConnected("id2", "id1"));
}

TEST_CASE("Test cse::Graph - From advanced file", "Complex graph") {
  using TestGraph = cse::Graph<std::string>;

  TestGraph graph;
  // Test adding vertices
  auto &v1 = graph.AddVertex("id1", "Vertex1 Data");
  auto &v2 = graph.AddVertex("id2", "Vertex2 Data", 1.5);
  auto &v3 = graph.AddVertex("id3", "Vertex3 Data");

  graph.AddEdge(v1, v2);
  graph.AddEdge(v1, v3, 2);
  std::stringstream s;

  graph.ToFile(s);

  TestGraph destinationGraph(s);
  CHECK(destinationGraph.GetVertex("id1").GetId() == "id1");
  CHECK(destinationGraph.GetVertex("id2").GetId() == "id2");
  CHECK(destinationGraph.GetVertex("id3").GetId() == "id3");

  CHECK(destinationGraph.IsConnected("id1", "id2"));
  CHECK(!destinationGraph.IsConnected("id2", "id1"));
  CHECK(destinationGraph.IsConnected("id1", "id3"));
  CHECK(!destinationGraph.IsConnected("id3", "id1"));
  auto e = graph.GetEdge("id1", "id3");
  REQUIRE_THAT(e.GetWeight(), WithinAbs(2, cse_test_utils::FLOAT_DELTA));
}

TEST_CASE("Test cse::Graph - Check removing Vertex removes related edges",
          "Remove Vertex") {
  using TestGraph = cse::Graph<std::string>;

  TestGraph graph;
  // Test adding vertices
  auto &v1 = graph.AddVertex("id1", "Vertex1 Data");
  auto &v2 = graph.AddVertex("id2", "Vertex2 Data", 1.5);
  auto &v3 = graph.AddVertex("id3", "Vertex3 Data");
  auto &v4 = graph.AddVertex("id4", "Vertex4 Data");

  graph.AddEdge(v1, v2);
  graph.AddEdge(v1, v3, 2);
  graph.AddEdge(v2, v4);
  graph.AddEdge(v3, v4);

  graph.RemoveVertex(v1.GetId());
  CHECK_THROWS_AS(graph.GetEdge("id1_id2"), cse::edge_not_found_error);

  graph.RemoveVertex(v4.GetId());
  CHECK_THROWS_AS(graph.GetEdge("id2_id4"), cse::edge_not_found_error);
  CHECK_THROWS_AS(graph.GetEdge("id3_id4"), cse::edge_not_found_error);

  CHECK_THROWS_AS(v2.GetEdge("id4"), cse::edge_not_found_error);
  CHECK_THROWS_AS(v3.GetEdge("id4"), cse::edge_not_found_error);
}

TEST_CASE("Test cse::Graph - Cycle", "HasCycle functionality") {
  using TestGraph = cse::Graph<std::string>;

  TestGraph graph;
  // Test adding vertices
  auto v1 = graph.AddVertex("id1", "Vertex1 Data");
  auto v2 = graph.AddVertex("id2", "Vertex2 Data");
  auto v3 = graph.AddVertex("id3", "Vertex3 Data");
  auto v4 = graph.AddVertex("id4", "Vertex4 Data");

  SECTION("Graph with super simple cycle") {
    // Graph with cycle
    graph.AddEdge(v1, v2);
    graph.AddEdge(v2, v1);
    REQUIRE(graph.HasCycle());
  }

  SECTION("Graph with simple cycle") {
    // Graph with cycle
    graph.AddEdge(v1, v2);
    graph.AddEdge(v2, v3);
    graph.AddEdge(v3, v4);
    graph.AddEdge(v4, v1);
    REQUIRE(graph.HasCycle());
  }

  SECTION("Graph with no simple cycle") {
    // Graph with cycle
    graph.AddEdge(v1, v2);
    graph.AddEdge(v2, v3);
    graph.AddEdge(v3, v4);
    REQUIRE(!graph.HasCycle());
  }

  SECTION("Graph with no more complex graph") {
    auto &v5 = graph.AddVertex("id5");

    // Graph with cycle
    graph.AddEdge(v1, v2);
    graph.AddEdge(v2, v3);
    graph.AddEdge(v2, v5);
    graph.AddEdge(v5, v4);
    graph.AddEdge(v3, v4);
    graph.AddEdge(v4, v1);
    REQUIRE(graph.HasCycle());
  }
}
