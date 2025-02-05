/**
 * @file StateGrid.cpp
 * @author Dominik Leisinger
 */

#include <cse/StateGrid.h>

namespace cse{

/**
 * @brief Contructor for StateGrid object
 * @param rows rows in grid map
 * @param cols cols in grid map
 */
StateGrid::StateGrid(size_t rows, size_t cols, std::string difficulty) {}


/**
 * @brief Displays the grid to user
 */
void StateGrid::DisplayGrid() {}

/**
 * @brief Sets the specific position to occupied by agent
 * @param row row of position
 * @param col col of position
 */
void StateGrid::Setstate(int row, int col) {}

/**
 * @brief Returns the state of a grid position
 * @param row row of position
 * @param col col of position
 * @return state of (row,col) position
 */
std::string StateGrid::Getstate(int row, int col) {
  return std::string();
}
/**
 * @brief Validates if a position can be occupied by agent
 * @param row row of position
 * @param col col of position
 * @return T/F of position validity
 */
bool StateGrid::Validateposition(int row, int col) {
  return false;
}

/**
 * @brief finds all possible moves for agent from (row,col) position
 * @param row row of position
 * @param col col of position
 * @return std::string of possible move directions
 */
std::string StateGrid::Findmoves(int row, int col) {
  return "std::string()";
}
}