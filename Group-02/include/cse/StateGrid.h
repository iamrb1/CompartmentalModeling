/**
 * @file StateGrid.h
 * @author Dominik Leisinger
 *
 *
 */

#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>
#include "AuditedVector.h"
#include "DataMap.hpp"
#include "StateGridPosition.h"

namespace cse {

/**
 * StateDictionary
 * Struct to hold each StateGrid state, along with any (and future) properties it has
 */
struct StateDictionary {
  std::map<char, cse::AuditedVector<std::string>> m_state_dictionary;

  StateDictionary() {
    m_state_dictionary = {{' ', {"EmptySpace", "Open"}},
                          {'#', {"Wall", "Closed"}},
                          {'X', {"Enemy", "Open"}},
                          {'0', {"Exit", "Open"}},
                          {'P', {"Player", "Closed"}}};
  }
  /**
   * This function searches for a state in m_grid
   * @param searchchar char that is being searched for
   * @return bool stating if the state was found
   */
  bool find(char searchchar)
  {
    return m_state_dictionary.find(searchchar) != m_state_dictionary.end();
  }
  /**
   * This function checks if a state can be traversed into
   * @param searchchar state to be queried
   * @return bool stating if the state is traversable
   */
  bool traversable(char searchchar)
  {
    return m_state_dictionary.at(searchchar)[1] == "Open";
  }
  /**
   * This function returns property information about a state in m_grid
   * @param searchchar The state being queried
   * @return AuditedVector of properties for queried state
   */
  cse::AuditedVector<std::string> get_info(char searchchar)
  {
    return m_state_dictionary.at(searchchar);
  }
  /**
   * This function can alter or remove properties from states in m_grid
   * @param changestate The state to be changed
   * @param property The property to be changed/removed
   * @param changeprop An optional variable, that can contain the new property or when not present, indicates removal
   * The use of std::nullopt was suggested to me by ChatGPT
   */
  void change_property(char changestate, std::string& property, std::optional<std::string> changeprop = std::nullopt)
  {
    auto it = m_state_dictionary.find(changestate);
    if (it == m_state_dictionary.end()) {
      return;
    }

    cse::AuditedVector<std::string>& properties = m_state_dictionary.at(changestate);
    auto found = std::find(properties.begin(), properties.end(), property);

    if (found != properties.end()) {
      if(changeprop)
      {
        *found = std::move(*changeprop);
      }
      else
      {
        properties.erase(found);
      }

    }
  }
};

class StateGrid {
 private:
  ///Rows in grid
  int m_rows = 0;
  ///Rows in grid
  int m_cols = 0;
  ///StateGridPosition to represent Agent location
  cse::StateGridPosition m_position;
  ///Grid to represent game map
  cse::AuditedVector<std::string> m_grid;
  ///StateDictionary object that holds all property info about StateGrid states
  StateDictionary m_dictionary;


 public:

  /**
   * Default StateGrid Constructor (No arguments)
   */
  StateGrid()
  {
    load_map("test");
  }

  /**
 * @brief Contructor for StateGrid object
 * @param diff string representing requested difficulty
 */
  explicit StateGrid(const std::string& diff) {
    load_map(diff);
  }

  void load_map(const std::string& diff);

  friend std::ostream& operator<<(std::ostream& os, const cse::AuditedVector<std::string>& grid);

  void display_grid();
  ///REVIEW COMMENT: I had a comment about my use of std::pairs instead of a dedicated Point struct,
  /// like in SGPos, but this currently would not work with indexing into m_grid with doubles,
  /// so in advanced version I will include a better looking struct that will coincide with SGPos
  bool set_state(Point new_position);

  char get_state(Point statepos);

  cse::AuditedVector<std::string> define_state(char state);

  void set_condition(char changestate, std::string property, std::string changeprop);

  void remove_conditions(char changestate, std::string property);

  std::map<std::string, cse::AuditedVector<std::string>> find_properties();

  bool validate_position(std::pair<double, double> move);

  std::vector<Point> find_moves();

  void modify_all_cells(const std::function<void(int, int, char&)>& func);

};
}  // namespace cse