/**
 * @file StateGrid.h
 * @author Dominik Leisinger
 *
 *
 */

#pragma once

#include <string>
#include <vector>
#include <map>

namespace cse {
class StateGrid {
 private:
  ///Rows in grid
  int m_rows = 0;
  ///Rows in grid
  int m_cols = 0;
  ///Grid to represent game map
  std::vector<std::string> m_grid;
  ///Map to represent the name and (future) properties of each state <<< Soon to be DataMap type
  std::map<char, std::vector<std::string>> m_dictionary = {
      {' ', {"EmptySpace", "Open"} },
      {'#', {"Wall", "Closed"}},
      {'X', {"Enemy", "Open"}},
      {'0', {"Exit", "Open"}},
      {'P', {"Player", "Closed"}}
  };

 public:
  [[maybe_unused]] explicit StateGrid(const std::string& diff);

  void choose_map(const std::string& diff);

  void display_grid();

  bool set_state(std::pair<int, int> move, std::pair<int, int> agent);

  char get_state(int row, int col);

  std::vector<std::string> define_state(char state);

  bool validate_position(std::pair<int, int> move);

  std::vector<std::pair<int,int>> find_moves(int row, int col);
};
}  // namespace cse