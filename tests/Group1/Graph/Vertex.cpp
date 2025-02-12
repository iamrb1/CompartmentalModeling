#include "../../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../../Group-01/Graph/Vertex.hpp"
#include "../../../Group-01/Graph/Edge.hpp"

#include <vector>
#include <sstream>

using Catch::Matchers::WithinAbs;

double const FLOAT_DELTA = 0.000001;

bool CheckForStringFile(std::vector<std::string> lines, std::stringstream &s)
{
  size_t i = 0;
  std::string line;
  while (std::getline(s, line))
  {
    CHECK(line == lines.at(i));
    i++;
  }

  CHECK(i == lines.size());
  return true;
}

void BuildFileFromVector(std::vector<std::string> const &lines, std::stringstream &s)
{
  for (auto line : lines)
  {
    s << line << "\n";
  }
}

TEST_CASE("Test cse::Vertex", "[base]")
{
  cse::Vertex v("id1");

  REQUIRE_THAT(v.GetX(), WithinAbs(0, FLOAT_DELTA));
  REQUIRE_THAT(v.GetY(), WithinAbs(0, FLOAT_DELTA));
  CHECK(v.GetId() == "id1");

  std::ostringstream os;
  os << v;
  CHECK(os.str() == "Vertex(id1)");
}

TEST_CASE("Test cse::Vertex - Edges", "[base] - Edge")
{
  auto v1 = std::make_shared<cse::Vertex>("id1");
  auto v2 = std::make_shared<cse::Vertex>("id2");
  auto v3 = std::make_shared<cse::Vertex>("id3");

  // Single direction edge
  auto e1 = std::make_shared<cse::Edge>("edge1", v1, v2);
  v1->AddEdge(e1);
  CHECK(v1->IsConnected(v2));
  CHECK(!v1->IsConnected(v3));

  std::shared_ptr<cse::Edge> e2 = std::make_shared<cse::BidirectionalEdge>("edge2", v1, v3);
  v1->AddEdge(e2);
  CHECK(v1->IsConnected(v3));
  CHECK(v3->IsConnected(v1));
}

TEST_CASE("Test cse::Vertex - To file", "Export to file")
{
  auto v1 = std::make_shared<cse::Vertex>("id1");
  std::stringstream s;
  v1->ToFile(s, "  ");
  std::vector<std::string> lines{"  VERTEX:id1", "    X:0", "    Y:0"};
  REQUIRE(CheckForStringFile(lines, s));
}

TEST_CASE("Test cse::Vertex - From file", "Read from file")
{
  // auto v1 = std::make_shared<cse::Vertex>("id1");
  // std::stringstream s;
  // v1->ToFile(s, "  ");
  // std::vector<std::string> lines{"  VERTEX:id1", "    X:0", "    Y:0"};
  // REQUIRE(CheckForStringFile(lines, s));
  // BuildFileFromVector
}
