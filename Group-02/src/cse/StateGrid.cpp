/**
 * @file StateGrid.cpp
 * @author Dominik Leisinger
 */

#include "cse/StateGrid.h"
#include <cassert>
#include <iostream>
#include <map>
#include <utility>
#include "cse/StateGridPosition.h"
namespace cse {

///NOTE: Used ChatGPT for syntax for this overload
///Chat link: https://chatgpt.com/share/67d227cf-5f50-8001-86f4-494a0f71a7ea
std::ostream& operator<<(std::ostream& os, const cse::AuditedVector<std::string>& grid) {
  for(const auto& row : grid)
  {
    os << row << "\n";
  }
  return os;
}
/**
 * @brief Displays the grid to user
 */
void StateGrid::display_grid() {
  assert(!m_grid.empty() && "m_grid is empty and cannot display");
  std::cout << &m_grid;
}
/**
 * @brief Sets the specific position to occupied by agent
 * @param new_position pair containing new agent spot
 * @param agent pair containing current agent position
 */
bool StateGrid::set_state(Point new_position) {
  auto agent = m_position.get_object_position();
  assert(!m_grid.empty() && m_grid[static_cast<size_t>(agent.x_position)][static_cast<size_t>(agent.y_position)] == 'P');

  if (validate_position({new_position.x_position, new_position.y_position})) {
    m_grid[static_cast<size_t>(new_position.x_position)][static_cast<size_t>(new_position.y_position)] = 'P';
    m_grid[static_cast<size_t>(agent.first)][static_cast<size_t>(agent.second)] = ' ';
    return true;
  }
  return false;
}

/**
 * Returns property information about a specific state
 * @param state queried state
 * @return vector of strings holding info about state
 */
std::vector<std::string> StateGrid::define_state(char state) {
  auto possible_state = m_dictionary.find(state);
  assert(possible_state != m_dictionary.end() && "This state is not in the map!");
  return m_dictionary.at(state);
}
///REVIEW COMMENT: A review comment was made to add std::optional for this function in the case where no
/// StateGrid state was found, but I do not believe this necessary as the assert checks
/// if the row,col is within bounds which will guarantee a StateGrid state.
/**
 * @brief Returns the state of a grid position
 * @param row row of position
 * @param col col of position
 * @return state of (row,col) position
 */
char StateGrid::get_state(Point statepos) {
  assert(statepos.x_position < m_rows && statepos.y_position < m_cols && "This is not inside the grid");
  return m_grid[static_cast<size_t>(statepos.x_position)][static_cast<size_t>(statepos.y_position)];
}

/**
 * @brief Validates if a position can be occupied by agent
 * @param move std::pair containing row,col of position to be validated
 * @return T/F of position validity
 */
bool StateGrid::validate_position(std::pair<int, int> move) {
  assert(move.first < m_rows && move.second < m_cols && "This move is out of bounds");
  char validate = m_grid[move.first][move.second];
  return (m_dictionary.find(validate) != m_dictionary.end() && m_dictionary.at(validate)[1]=="Open");
}

/**
 * @brief finds all possible moves for agent from (row,col) position
 * @return vector of pairs of possible move directions
 */
std::vector<Point> StateGrid::find_moves() {
  double row = m_position.x_position;
  double col = m_position.y_position;
  assert(row < m_rows && col < m_cols && "This is not inside the grid");
  std::vector<Point> moves = {};
  std::vector<std::pair<int, int>> poss_moves = {{(row + 1), col}, {(row - 1), col}, {row, (col + 1)}, {row, (col - 1)}};
  for (auto move : poss_moves) {
    if (validate_position(move)) {
      moves.push_back(Point(move.first,move.second));
    }
  }
  return moves;
}
/**
 * @brief sets m_grid to specified map
 * @param diff string to choose map of specified difficulty
 */
void StateGrid::load_map(const std::string& diff) {
  std::map<std::string, cse::AuditedVector<std::string>> maps = {
      {"test", {"#####", "# P #", "##X##", "## ##", "#0  #", "#####"}}}; ///Could add functionality to load in a map from separate file
  if (maps.find(diff) != maps.end()) {
    m_grid = maps[diff];
  } else {                                          ///<< If wrong map input, default to test map
    m_grid = maps["test"];
  }
  m_cols = static_cast<int>(m_grid[0].size());
  m_rows = static_cast<int>(m_grid.size());
  m_position.set_object_position({1,2});            ///< Until we implement more maps, hard coding original agent pos
}

}  // namespace cse