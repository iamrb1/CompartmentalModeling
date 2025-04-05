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
    REQUIRE_NOTHROW(cse::GraphPosition<std::string>(graph, v1));
  }
}

TEST_CASE("GraphPosition Getters and Setters Tests", "[GraphPosition]") {
  cse::Graph<std::string> graph;
  auto &v1 = graph.AddVertex("A", "Vertex A Data");
  auto &v2 = graph.AddVertex("B", "Vertex B Data");

  cse::GraphPosition<std::string> pos(graph, v1);

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

  cse::GraphPosition<std::string> pos(graph, v1);

  SECTION("MarkVisited and IsVisited work correctly") {
    pos.MarkVisited(v1);
    pos.MarkVisited(v2);
    REQUIRE(pos.IsVisited(v1));
    REQUIRE(pos.IsVisited(v2));
    REQUIRE(!pos.IsVisited(v3));
  }
}

/**
 * Citation: There was use of AI (Github Copilot) to generate the test cases for
 * A* algotithm
 */
TEST_CASE("GraphPosition Depth and Breadth-First Search Traversal Tests",
          "[GraphPosition]") {
  cse::Graph<std::string> graph;

  // Set up vertices with positions
  auto &v1 = graph.AddVertex("A", "Vertex A Data", 0, 0);
  auto &v2 = graph.AddVertex("B", "Vertex B Data", 1, 1);
  auto &v3 = graph.AddVertex("C", "Vertex C Data", 0, 2);
  auto &v4 = graph.AddVertex("D", "Vertex D Data", 2, 2);

  // Add weighted edges
  auto e1 = graph.AddEdge("A", "B", 1.414);
  auto e2 = graph.AddEdge("A", "C", 2.0);
  auto e3 = graph.AddEdge("B", "D", 1.414);
  cse::GraphPosition<std::string> pos(graph, v1);

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

  auto e4 = graph.AddEdge("C", "D", 2);
  SECTION("A* finds optimal path to destination - Simple") {
    // Path should be A -> B -> D as it's shorter than A -> C -> D
    std::vector<cse::Vertex<std::string> *> expectedOrder{&v1, &v2, &v4};
    pos.SetTraversalMode(cse::TraversalModes::AStar<std::string>(v4));
    pos.ResetTraversal(v1);

    size_t ind = 0;
    while ((bool)++pos) {
      auto &v = *(expectedOrder.at(ind));
      CHECK(pos.GetCurrentVertex() == v);
      ind++;
    }
    CHECK(ind == expectedOrder.size());
  }

  SECTION("A* Path that does not exist") {
    // Path should be A -> B -> D as it's shorter than A -> C -> D
    std::vector<cse::Vertex<std::string> *> expectedOrder{&v4};
    pos.ResetTraversal(v4);
    pos.SetTraversalMode(cse::TraversalModes::AStar<std::string>(v1));

    size_t ind = 0;
    while ((bool)++pos) {
      auto &v = *(expectedOrder.at(ind));
      CHECK(pos.GetCurrentVertex() == v);
      ind++;
    }
    CHECK(ind == expectedOrder.size());
  }

  SECTION("A* Path on a grid") {
    cse::Graph<std::string> graph;

    const int GRID_SIZE = 5;

    // Generates a grid for the graph
    for (int x = 0; x < GRID_SIZE; x++) {
      for (int y = 0; y < GRID_SIZE; y++) {
        std::string v_id = std::to_string(x) + "_" + std::to_string(y);
        graph.AddVertex(v_id, x, y);
      }
    }

    for (int x = 0; x < GRID_SIZE; x++) {
      for (int y = 0; y < GRID_SIZE; y++) {
        std::string v_id_1 = std::to_string(x) + "_" + std::to_string(y);
        std::string v_id_2;
        bool isXRightEdge = x == (GRID_SIZE - 1);
        bool isYBottomEdge = y == (GRID_SIZE - 1);
        bool isXLeftEdge = x == 0;
        bool isYTopEdge = y == 0;

        if (!isXRightEdge) {
          v_id_2 = std::to_string(x + 1) + "_" + std::to_string(y);
          graph.AddEdge(v_id_1, v_id_2, 1);
          graph.AddEdge(v_id_2, v_id_1, 1);
        }
        if (!isXLeftEdge) {
          v_id_2 = std::to_string(x - 1) + "_" + std::to_string(y);
          graph.AddEdge(v_id_1, v_id_2, 1);
          graph.AddEdge(v_id_2, v_id_1, 1);
        }
        if (!isYBottomEdge) {
          v_id_2 = std::to_string(x) + "_" + std::to_string(y + 1);
          graph.AddEdge(v_id_1, v_id_2, 1);
          graph.AddEdge(v_id_2, v_id_1, 1);
        }
        if (!isYTopEdge) {
          v_id_2 = std::to_string(x) + "_" + std::to_string(y - 1);
          graph.AddEdge(v_id_1, v_id_2, 1);
          graph.AddEdge(v_id_2, v_id_1, 1);
        }
        if (!isXRightEdge && !isYBottomEdge) {
          v_id_2 = std::to_string(x + 1) + "_" + std::to_string(y + 1);
          graph.AddEdge(v_id_1, v_id_2, 1.41);
          graph.AddEdge(v_id_2, v_id_1, 1.41);
        }
        if (!isXLeftEdge && !isYTopEdge) {
          v_id_2 = std::to_string(x - 1) + "_" + std::to_string(y - 1);
          graph.AddEdge(v_id_1, v_id_2, 1.41);
          graph.AddEdge(v_id_2, v_id_1, 1.41);
        }
      }
    }

    // Expected order is the order items are visited in
    std::vector<std::string> expectedOrder{"0_0", "1_1", "2_2", "3_3", "4_4"};
    // Final path is the best path found by a* using the heuristic
    std::vector<std::string> finalPath{"0_0", "1_1", "2_2", "3_3", "4_4"};
    cse::GraphPosition<std::string> pos(graph, graph.GetVertex("0_0"));
    auto origin = graph.GetVertex("0_0");
    auto destination = graph.GetVertex("4_4");
    size_t ind = 0;

    SECTION("Initial full matrix") {
      pos.ResetTraversal(origin);
      pos.SetTraversalMode(
          cse::TraversalModes::AStar<std::string>(destination));

      while ((bool)++pos) {
        auto v_id = expectedOrder.at(ind);
        CHECK(pos.GetCurrentVertex().GetId() == v_id);
        ind++;
      }
      CHECK(ind == expectedOrder.size());
    }

    SECTION("Traverse matrix with simple obstacle ") {
      origin = graph.GetVertex("0_0");
      destination = graph.GetVertex("4_4");
      graph.RemoveVertex("1_1");
      /**
       * O O O O O
       * O X O O O
       * O O O O O
       * O O O O O
       * O O O O O
       */
      pos.ResetTraversal(origin);
      pos.SetTraversalMode(
          cse::TraversalModes::AStar<std::string>(destination));

      pos.TraverseGraph();

      finalPath = {"0_0", "0_1", "1_2", "2_3", "3_4", "4_4"};
      auto traversalPath = pos.GetTraversalPath();
      ind = 0;
      for (auto v_pointer : traversalPath) {
        CHECK(v_pointer->GetId() == finalPath.at(ind));
        ind++;
      }
    }

    SECTION("Traverse matrix with full more obstacles") {
      auto &origin = graph.GetVertex("0_0");
      auto &destination = graph.GetVertex("4_4");
      pos.ResetTraversal(origin);
      pos.SetTraversalMode(
          cse::TraversalModes::AStar<std::string>(destination));

      // Adds additional obstacles
      graph.RemoveVertex("1_1");
      graph.RemoveVertex("2_1");
      graph.RemoveVertex("3_1");
      graph.RemoveVertex("4_1");
      /**
       * O O O O O
       * O X X X X
       * O O O O O
       * O O O O O
       * O O O O O
       */
      expectedOrder = {"0_0", "0_1", "1_0", "1_2", "2_3", "3_4", "4_4"};
      finalPath = {"0_0", "0_1", "1_2", "2_3", "3_4", "4_4"};

      // Check it visits nodes in the correct order
      while ((bool)++pos) {
        auto v_id = expectedOrder.at(ind);
        CHECK(pos.GetCurrentVertex().GetId() == v_id);
        ind++;
      }
      CHECK(ind == expectedOrder.size());

      // Checks the final path is correct
      auto traversalPath = pos.GetTraversalPath();
      ind = 0;
      for (auto v_pointer : traversalPath) {
        CHECK(v_pointer->GetId() == finalPath.at(ind));
        ind++;
      }
    }

    SECTION("Traverse matrix with full more complex obstacle") {
      origin = graph.GetVertex("4_0");
      destination = graph.GetVertex("0_4");
      pos.ResetTraversal(origin);
      pos.SetTraversalMode(
          cse::TraversalModes::AStar<std::string>(destination));

      // Add diagonal for testing
      graph.AddEdge("1_0", "0_1", 1.41);
      graph.AddEdge("0_1", "1_0", 1.41);

      // Adds additional obstacles
      graph.RemoveVertex("1_1");
      graph.RemoveVertex("2_1");
      graph.RemoveVertex("3_1");
      graph.RemoveVertex("4_1");

      graph.RemoveVertex("0_3");
      graph.RemoveVertex("1_3");
      graph.RemoveVertex("2_3");
      graph.RemoveVertex("3_3");
      /**
       * O 1 1 1 1
       * 1 X X X X
       * O 1 1 1 O
       * X X X X 1
       * 1 1 1 1 1
       */
      expectedOrder = {
          "4_0", "3_0", "2_0", "1_0", "0_1", "0_2", "0_0", "1_2", "2_2",
          "3_2", "4_2", "4_3", "4_4", "3_4", "2_4", "1_4", "0_4",
      };

      finalPath = {"4_0", "3_0", "2_0", "1_0", "0_1", "1_2", "2_2",
                   "3_2", "4_3", "4_4", "3_4", "2_4", "1_4", "0_4"};

      ind = 0;
      // Check it visits nodes in the correct order
      while ((bool)++pos) {
        auto v_id = expectedOrder.at(ind);
        CHECK(pos.GetCurrentVertex().GetId() == v_id);
        ind++;
      }

      // Checks the final path is correct
      auto traversalPath = pos.GetTraversalPath();
      ind = 0;
      for (auto v_pointer : traversalPath) {
        CHECK(v_pointer->GetId() == finalPath.at(ind));
        ind++;
      }
    }
  }
}