#include "../../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../../Group-01/Graph/Vertex.hpp"
#include "../../../Group-01/Graph/Edge.hpp"
#include "../../../Group-01/Graph/GraphExceptions.hpp"
#include "../Utils/TestUtils.hpp"

#include <vector>
#include <sstream>

using Catch::Matchers::WithinAbs;

TEST_CASE("Test cse::Vertex", "[base]") {
  // Using string as the vertex data type
  cse::Vertex<std::string> v("id1", "vertex_data");

  REQUIRE_THAT(v.GetX(), WithinAbs(0, cse_test_utils::FLOAT_DELTA));
  REQUIRE_THAT(v.GetY(), WithinAbs(0, cse_test_utils::FLOAT_DELTA));
  CHECK(v.GetId() == "id1");

  std::ostringstream os;
  os << v;
  CHECK(os.str() == "Vertex(id1)");
}

TEST_CASE("Test cse::Vertex - Edges", "[base] - Edge") {
    auto v1 = std::make_shared<cse::Vertex<std::string>>("id1", "data1");
    auto v2 = std::make_shared<cse::Vertex<std::string>>("id2", "data2");
    auto v3 = std::make_shared<cse::Vertex<std::string>>("id3", "data3");

    // Single direction edge
    auto e1 = std::make_shared<cse::Edge<std::string>>("edge1", v1, v2);
    v1->AddEdge(e1);

    // Fix: Pass references instead of dereferenced shared_ptr
    CHECK(v1->IsConnected(*v2));  // Correct
    CHECK(!v1->IsConnected(*v3)); // Correct
}

TEST_CASE("Test cse::Vertex - To file", "Export to file") {
  using VertexT = cse::Vertex<std::string>;
  
  auto v1 = std::make_shared<VertexT>("id1", "vertex_data");
  std::stringstream s;
  v1->ToFile(s, 1);
  
  std::vector<std::string> lines{"  VERTEX:id1", "    X:0", "    Y:0"};
  REQUIRE(cse_test_utils::CheckForStringFile(lines, s));
}

TEST_CASE("Test cse::Vertex - From file", "Read from file") {
  using VertexT = cse::Vertex<std::string>;

  std::vector<std::string> lines{"  VERTEX:id1", "    X:1", "    Y:1"};
  std::stringstream s;
  cse_test_utils::BuildFileFromVector(lines, s);

  auto v1 = std::make_shared<VertexT>(s, 2);
  
  REQUIRE_THAT(v1->GetX(), WithinAbs(1, cse_test_utils::FLOAT_DELTA));
  REQUIRE_THAT(v1->GetY(), WithinAbs(1, cse_test_utils::FLOAT_DELTA));
  CHECK(v1->GetId() == "id1");
}

// The tests below were generated with the help of AI
TEST_CASE("Test cse::Vertex - Getters and Setters", "[base]") {
    using VertexT = cse::Vertex<std::string>;
    
    auto v1 = std::make_shared<VertexT>("id1", "data1", 1.0, 2.0);
    
    CHECK(v1->GetId() == "id1");
    CHECK(v1->GetData() == "data1");
    REQUIRE_THAT(v1->GetX(), WithinAbs(1.0, cse_test_utils::FLOAT_DELTA));
    REQUIRE_THAT(v1->GetY(), WithinAbs(2.0, cse_test_utils::FLOAT_DELTA));
    CHECK(v1->GetShowId() == true);
    
    v1->SetData("new_data");
    v1->SetShowId(false);
    
    CHECK(v1->GetData() == "new_data");
    CHECK(v1->GetShowId() == false);
}

TEST_CASE("Test cse::Vertex - Edge Management", "[base]") {
    using VertexT = cse::Vertex<std::string>;
    
    auto v1 = std::make_shared<VertexT>("id1", "data1");
    auto v2 = std::make_shared<VertexT>("id2", "data2");
    auto v3 = std::make_shared<VertexT>("id3", "data3");
    
    auto e1 = std::make_shared<cse::Edge<std::string>>("edge1", v1, v2);
    auto e2 = std::make_shared<cse::Edge<std::string>>("edge2", v1, v3);
    
    v1->AddEdge(e1);
    v1->AddEdge(e2);
    
    CHECK(v1->GetEdges().size() == 2);
    CHECK(v1->GetEdge(*v2)->GetId() == "edge1");
    CHECK(v1->GetEdge("id3")->GetId() == "edge2");
    
    v1->RemoveEdge("id2");
    CHECK(v1->GetEdges().size() == 1);
    CHECK_THROWS_AS(v1->GetEdge(*v2), cse::edge_not_found_error);
}

TEST_CASE("Test cse::Vertex - Edge Cleanup", "[base]") {
    using VertexT = cse::Vertex<std::string>;
    
    auto v1 = std::make_shared<VertexT>("id1", "data1");
    auto v2 = std::make_shared<VertexT>("id2", "data2");
    
    {
        auto e1 = std::make_shared<cse::Edge<std::string>>("edge1", v1, v2);
        v1->AddEdge(e1);
        CHECK(v1->GetEdges().size() == 1);
    } // e1 goes out of scope
    
    v1->CleanupExpiredEdges();
    CHECK(v1->GetEdges().empty());
}

TEST_CASE("Test cse::Vertex - Construction", "[base]") {
    using VertexT = cse::Vertex<std::string>;
    
    auto v1 = std::make_shared<VertexT>("id1", "data1");
    auto v2 = std::make_shared<VertexT>("id2", "data2", 1.0, 2.0);
    auto v3 = std::make_shared<VertexT>("id3", 3.0, 4.0);
    
    CHECK(v1->GetData() == "data1");
    REQUIRE_THAT(v1->GetX(), WithinAbs(0.0, cse_test_utils::FLOAT_DELTA));
    
    CHECK(v2->GetData() == "data2");
    REQUIRE_THAT(v2->GetX(), WithinAbs(1.0, cse_test_utils::FLOAT_DELTA));
    
    CHECK(v3->GetData() == "");
    REQUIRE_THAT(v3->GetX(), WithinAbs(3.0, cse_test_utils::FLOAT_DELTA));
}

TEST_CASE("Test cse::Vertex - Error Cases", "[base]") {
    using VertexT = cse::Vertex<std::string>;
    
    auto v1 = std::make_shared<VertexT>("id1", "data1");
    
    CHECK_THROWS_AS(v1->RemoveEdge("nonexistent"), cse::edge_not_found_error);
    CHECK_THROWS_AS(v1->GetEdge("nonexistent"), cse::edge_not_found_error);
}

TEST_CASE("Test cse::Vertex - Integer Data Type", "[types]") {
    using IntVertex = cse::Vertex<int>;
    
    auto v1 = std::make_shared<IntVertex>("v1", 42);
    auto v2 = std::make_shared<IntVertex>("v2", 100, 1.5, 2.5);
    
    CHECK(v1->GetData() == 42);
    CHECK(v2->GetData() == 100);
    REQUIRE_THAT(v2->GetX(), WithinAbs(1.5, cse_test_utils::FLOAT_DELTA));
    
    v1->SetData(123);
    CHECK(v1->GetData() == 123);
    
    // Test edge connection with int vertices
    auto e1 = std::make_shared<cse::Edge<int>>("e1", v1, v2);
    v1->AddEdge(e1);
    CHECK(v1->IsConnected(*v2));
}

TEST_CASE("Test cse::Vertex - Double Data Type", "[types]") {
    using DoubleVertex = cse::Vertex<double>;
    
    auto v1 = std::make_shared<DoubleVertex>("v1", 3.14);
    auto v2 = std::make_shared<DoubleVertex>("v2", 2.718);
    
    REQUIRE_THAT(v1->GetData(), WithinAbs(3.14, cse_test_utils::FLOAT_DELTA));
    REQUIRE_THAT(v2->GetData(), WithinAbs(2.718, cse_test_utils::FLOAT_DELTA));
    
    v1->SetData(1.414);
    REQUIRE_THAT(v1->GetData(), WithinAbs(1.414, cse_test_utils::FLOAT_DELTA));
    
    // Test file serialization with double vertices
    std::stringstream s;
    v1->ToFile(s, 1);
    std::vector<std::string> lines{"  VERTEX:v1", "    X:0", "    Y:0"};
    REQUIRE(cse_test_utils::CheckForStringFile(lines, s));
}