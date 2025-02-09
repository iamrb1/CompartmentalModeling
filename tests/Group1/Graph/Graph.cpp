#include "../../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../../Group-01/Graph/Vertex.hpp"
#include "../../../Group-01/Graph/Edge.hpp"
#include "../../../Group-01/Graph/Graph.hpp"

#include <vector>
#include <sstream>

using Catch::Matchers::WithinAbs;

TEST_CASE("Test cse::Graph", "[base]")
{
  cse::Graph graph;

  // Test adding vertices
  auto v1 = graph.AddVertex("id1");
  auto v2 = graph.AddVertex("id2");
  CHECK(graph.GetVertex("id1")->GetId() == "id1");
  CHECK(graph.GetVertex("id2")->GetId() == "id2");

  CHECK_THROWS_AS(graph.AddVertex("id1"), std::runtime_error);

  // Test removing vertices
  graph.RemoveVertex("id1");
  CHECK_THROWS_AS(graph.GetVertex("id1"), std::out_of_range);

  // Test adding edges
  v1 = graph.AddVertex("id1");
  auto e1 = graph.AddEdge("id1", "id2", false);
  CHECK(e1->IsConnected(v1, v2));
  CHECK(!e1->IsBidirectional());
  CHECK(!e1->IsConnected(v2, v1));

  // Testing Adding Edges by reference
  auto v4 = graph.AddVertex("id4");
  auto v5 = graph.AddVertex("id5");
  auto e2 = graph.AddEdge(v4, v5, false);
  // TODO @lspecht: Should check if nodes are connected via Graph Interface
  CHECK(e2->IsConnected(v4, v5));
  CHECK(!e2->IsBidirectional());
  CHECK(!e2->IsConnected(v5, v4));

  // Testing removing Edges
  CHECK(graph.GetEdge(v4->GetId(), v5->GetId()) != nullptr);
  graph.RemoveEdge(e2);
  CHECK_THROWS_AS(graph.GetEdge(v4->GetId(), v5->GetId()), std::out_of_range);
  CHECK_THROWS_AS(graph.RemoveEdge(e2), std::out_of_range);

  // TODO @lspecht: Getting a deleted edge should not work here
  std::cout << v4->GetEdge(v5).use_count() << std::endl;

  // Test bidirectional edges
  auto e3 = graph.AddEdge("id1", "id2", true);
  CHECK(e3->IsConnected(v1, v2));
  CHECK(e3->IsConnected(v2, v1));
  CHECK(e3->IsBidirectional());
}