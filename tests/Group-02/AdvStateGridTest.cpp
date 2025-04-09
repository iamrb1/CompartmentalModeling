/**
 * @file AdvStateGridTest.cpp
 * @author Dominik Leisinger, Matthew Hawkins
 */

#include <cse/StateGrid.hpp>
#include <cse/StateGridPosition.hpp>
#include <algorithm>
#include <catch2/catch.hpp>
#include <stdexcept>
#include <string>
#include <vector>

// Helper: Convert AuditedVector<string> to std::vector<string>
std::vector<std::string> toStdVector(const cse::AuditedVector<std::string>& av) {
  return std::vector<std::string>(av.begin(), av.end());
}

TEST_CASE("Testing load_map returns a valid map") {
  cse::StateGrid grid("test");

  // Agent 'P' should be at row 1, col 2.
  REQUIRE(grid.get_state(cse::Point(1, 2)) == 'P');

  // Exit '0' is at row 4, col 1.
  REQUIRE(grid.get_state(cse::Point(4, 1)) == '0');

  // Enemy 'X' is at row 2, col 2.
  REQUIRE(grid.get_state(cse::Point(2, 2)) == 'X');

  // Row 0 is "#####"
  // See Group-02/test.csv for the test grid layout
  std::string row0;
  for (int col = 0; col < 5; col++) {
    row0.push_back(grid.get_state(cse::Point(0, col)));
  }
  REQUIRE(row0 == "#####");
}

TEST_CASE("Testing find_properties returns correct values") {
  cse::StateGrid grid("test");

  SECTION("Testing initial position") {
    auto props = grid.find_properties();
    REQUIRE(props.find("Down") != props.end());
    REQUIRE(props.find("Left") != props.end());
    REQUIRE(props.find("Right") != props.end());
    REQUIRE(props.find("Up") == props.end());

    REQUIRE(toStdVector(props.at("Down")) == std::vector<std::string>{"Enemy", "Open"});
    REQUIRE(toStdVector(props.at("Left")) == std::vector<std::string>{"EmptySpace", "Open"});
    REQUIRE(toStdVector(props.at("Right")) == std::vector<std::string>{"EmptySpace", "Open"});
  }

  SECTION("Testing after movement") {
    REQUIRE(grid.set_state(cse::Point(2, 2)) == true);
    auto props = grid.find_properties();
    REQUIRE(props.find("Up") != props.end());
    REQUIRE(props.find("Down") != props.end());
    REQUIRE(props.find("Left") == props.end());
    REQUIRE(props.find("Right") == props.end());

    REQUIRE(toStdVector(props.at("Up")) == std::vector<std::string>{"EmptySpace", "Open"});
    REQUIRE(toStdVector(props.at("Down")) == std::vector<std::string>{"EmptySpace", "Open"});
  }
}

TEST_CASE("Testing set_condition and remove_condition functioning correctly") {
  cse::StateGrid grid("test");

  auto props_space = grid.define_state(' ');
  REQUIRE(std::find(props_space.begin(), props_space.end(), "Open") != props_space.end());

  grid.remove_condition(' ', "Open");
  auto new_props_space = grid.define_state(' ');
  REQUIRE(std::find(new_props_space.begin(), new_props_space.end(), "Open") == new_props_space.end());

  grid.set_condition('X', "Open", "Closed");
  auto props_enemy = grid.define_state('X');
  REQUIRE(std::find(props_enemy.begin(), props_enemy.end(), "Open") == props_enemy.end());
  REQUIRE(std::find(props_enemy.begin(), props_enemy.end(), "Closed") != props_enemy.end());
}

TEST_CASE("Testing validate_position at boundaries") {
  cse::StateGrid grid("test");

  REQUIRE_FALSE(grid.validate_position({0, 0}));

  REQUIRE(grid.validate_position({1, 1}));

  REQUIRE_FALSE(grid.validate_position({5, 4}));
}

TEST_CASE("Testing modify_all_cells modifies all cells") {
  cse::StateGrid grid("test");
  // Modify all '#' to 'W'
  grid.modify_all_cells([](int, int, char& cell) {
    if (cell == '#') {
      cell = 'W';
    }
  });
  // Check that no '#' remain in the grid.
  for (int row = 0; row < 6; ++row) {
    for (int col = 0; col < 5; ++col) {
      REQUIRE(grid.get_state(cse::Point(row, col)) != '#');
    }
  }
}

TEST_CASE("Testing find_moves near boundaries") {
  cse::StateGrid grid("test");
  std::vector<cse::Point> moves = grid.find_moves();
  std::vector<cse::Point> expectedMoves = {cse::Point(1, 1), cse::Point(1, 3), cse::Point(2, 2)};
  REQUIRE(moves.size() == expectedMoves.size());
  for (auto& expected : expectedMoves) {
    auto it = std::find(moves.begin(), moves.end(), expected);
    REQUIRE(it != moves.end());
  }
}

TEST_CASE("Testing set_state on invalid position") {
  cse::StateGrid grid("test");
  bool moved = grid.set_state(cse::Point(0, 0));
  REQUIRE_FALSE(moved);
  REQUIRE(grid.get_state(cse::Point(1, 2)) == 'P');
}

TEST_CASE("Testing multiple sequential moves") {
  cse::StateGrid grid("test");
  // Initially, agent is at (1,2)
  // Attempt to move the agent to (1,3) (valid) and then try to move further.
  bool moved = grid.set_state(cse::Point(1, 3));
  REQUIRE(moved == true);
  // Now, (1,2) should be empty and (1,3) should hold 'P'
  REQUIRE(grid.get_state(cse::Point(1, 2)) == ' ');
  REQUIRE(grid.get_state(cse::Point(1, 3)) == 'P');

  // Attempt a second move from (1,3) to (2,3). In the test grid, row 2 ("##X##")
  // at column 3 is '#', so the move should fail.
  moved = grid.set_state(cse::Point(2, 3));
  REQUIRE(moved == false);
  // The agent stays at (1,3)
  REQUIRE(grid.get_state(cse::Point(1, 3)) == 'P');
}

TEST_CASE("Testing find_moves returns empty when all neighbors are blocked") {
  cse::StateGrid grid("test");
  // Agent is initially at (1,2). Convert its neighboring traversable cells to walls.
  grid.modify_all_cells([](int row, int col, char& cell) {
    if ((row == 1 && (col == 1 || col == 3)) || (row == 2 && col == 2)) {
      cell = '#';
    }
  });
  auto moves = grid.find_moves();
  // Now no valid moves should be available.
  REQUIRE(moves.empty());
}

TEST_CASE("Testing copy constructor of StateGrid") {
  cse::StateGrid grid1("test");

  cse::StateGrid grid2 = grid1;

  REQUIRE(grid2.get_state(cse::Point(1, 2)) == grid1.get_state(cse::Point(1, 2)));
  REQUIRE(grid2.get_state(cse::Point(4, 1)) == grid1.get_state(cse::Point(4, 1)));

  grid1.set_state(cse::Point(1, 3));

  REQUIRE(grid2.get_state(cse::Point(1, 2)) == 'P');
  REQUIRE(grid2.get_state(cse::Point(1, 3)) != 'P');
}

TEST_CASE("Testing assignment operator of StateGrid") {
  cse::StateGrid grid1("test");
  cse::StateGrid grid2("test");

  grid2 = grid1;

  REQUIRE(grid2.get_state(cse::Point(1, 2)) == grid1.get_state(cse::Point(1, 2)));

  grid1.set_state(cse::Point(1, 3));

  REQUIRE(grid2.get_state(cse::Point(1, 2)) == 'P');
}

TEST_CASE("Testing validate_position on edge cells") {
  cse::StateGrid grid("test");

  // (0,0) is a wall so not traversable.
  REQUIRE_FALSE(grid.validate_position({0, 0}));
  // (5,4) is also a wall.
  REQUIRE_FALSE(grid.validate_position({5, 4}));
  // (1,1) is an empty space so should be traversable.
  REQUIRE(grid.validate_position({1, 1}));
}

TEST_CASE("Testing incorrect filepath throws exception") {
  REQUIRE_THROWS_AS((cse::StateGrid("nonexistent_file")), std::runtime_error);
}

TEST_CASE("Testing move of more than one space", "[StateGrid]") {
  cse::StateGrid grid("test");

  bool moved = grid.set_state(cse::Point(1, 4));
  REQUIRE_FALSE(moved);
  REQUIRE(grid.get_state(cse::Point(1,2)) == 'P');
}

