#include "../../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../../Group-01/Graph/Vertex.hpp"
#include "../../../Group-01/Graph/Edge.hpp"
#include "../../../Group-01/Graph/GraphExceptions.hpp"
#include "../Utils/TestUtils.hpp"

#include <vector>
#include <sstream>

using Catch::Matchers::WithinAbs;

TEST_CASE("Test cse::Edge - Basic", "[base]") {
    using TestVertex = cse::Vertex<std::string>;
    using TestEdge = cse::Edge<std::string>;

    auto v1 = std::make_shared<TestVertex>("id1", "Vertex1 Data");
    auto v2 = std::make_shared<TestVertex>("id2", "Vertex2 Data");

    auto edge = std::make_shared<TestEdge>("edge1", v1, v2, 2.5);

    CHECK(edge->GetId() == "edge1");
    CHECK(&edge->GetFrom() == v1.get());
    CHECK(&edge->GetTo() == v2.get());
    REQUIRE_THAT(edge->GetWeight(), WithinAbs(2.5, cse_test_utils::FLOAT_DELTA));
    CHECK(edge->GetShowWeight() == true);
}

TEST_CASE("Test cse::Edge - Connectivity", "[base]") {
    using TestVertex = cse::Vertex<std::string>;
    using TestEdge = cse::Edge<std::string>;

    auto v1 = std::make_shared<TestVertex>("id1", "Data1");
    auto v2 = std::make_shared<TestVertex>("id2", "Data2");
    auto v3 = std::make_shared<TestVertex>("id3", "Data3");

    auto edge = std::make_shared<TestEdge>("edge1", v1, v2);

    CHECK(edge->IsConnected(*v1, *v2));
    CHECK(!edge->IsConnected(*v2, *v1));  // Not bidirectional
    CHECK(!edge->IsConnected(*v1, *v3));
    CHECK(!edge->IsConnected(*v3, *v2));
}

TEST_CASE("Test cse::Edge - To file", "[file]") {
    using TestVertex = cse::Vertex<std::string>;
    using TestEdge = cse::Edge<std::string>;

    auto v1 = std::make_shared<TestVertex>("id1", "Data1");
    auto v2 = std::make_shared<TestVertex>("id2", "Data2");
    auto edge = std::make_shared<TestEdge>("edge1", v1, v2, 1.5);

    std::stringstream s;
    edge->ToFile(s, 1);

    std::vector<std::string> lines{
        "  EDGE:edge1",
        "    from:id1",
        "    to:id2",
        "    weight:1.5"
    };
    REQUIRE(cse_test_utils::CheckForStringFile(lines, s));
}