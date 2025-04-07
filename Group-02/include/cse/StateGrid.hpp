/**
 * @file StateGrid.h
 * @author Dominik Leisinger
 *
 *
 */

#pragma once

#include <functional>
#include <algorithm>
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
  std::map<char, AuditedVector<std::string>> m_state_dictionary;

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
  [[nodiscard]] bool find(char search_char) const {
    return m_state_dictionary.find(search_char) != m_state_dictionary.end();
  }
  /**
   * This function checks if a state can be traversed into
   * @param searchchar state to be queried
   * @return bool stating if the state is traversable
   */
  [[nodiscard]] bool traversable(char search_char) const {
    return m_state_dictionary.at(search_char)[1] == "Open";
  }
  /**
   * This function returns property information about a state in m_grid
   * @param searchchar The state being queried
   * @return AuditedVector of properties for queried state
   */
  [[nodiscard]] AuditedVector<std::string> get_info(char search_char) const {
    return m_state_dictionary.at(search_char);
  }
  /**
   * This function can alter or remove properties from states in m_grid
   * @param changestate The state to be changed
   * @param property The property to be changed/removed
   * @param changeprop An optional variable, that can contain the new property or when not present, indicates removal
   * The use of std::nullopt was suggested to me by ChatGPT
   */
  void change_property(char change_state, std::string& property, std::optional<std::string> change_prop = std::nullopt) {
    auto it = m_state_dictionary.find(change_state);
    if (it == m_state_dictionary.end()) {
      return;
    }

    AuditedVector<std::string>& properties = m_state_dictionary.at(change_state);
    auto found = std::find(properties.begin(), properties.end(), property);

    if (found != properties.end()) {
      if (change_prop) {
        *found = std::move(*change_prop);
      } else {
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
  StateGridPosition m_position;
  ///Grid to represent game map
  AuditedVector<std::string> m_grid;
  ///StateDictionary object that holds all property info about StateGrid states
  StateDictionary m_dictionary;

 public:
  /**
   * Default StateGrid Constructor (No arguments)
   */
  StateGrid() {
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

  friend std::ostream& operator<<(std::ostream& os, const AuditedVector<std::string>& grid);

  [[maybe_unused]] void display_grid() const;
  ///REVIEW COMMENT: I had a comment about my use of std::pairs instead of a dedicated Point struct,
  /// like in SGPos, but this currently would not work with indexing into m_grid with doubles,
  /// so in advanced version I will include a better looking struct that will coincide with SGPos
  [[maybe_unused]] bool set_state(Point new_position);

  [[nodiscard]] char get_state(Point state_pos) const;

  [[nodiscard]] AuditedVector<std::string> define_state(char state) const;

  [[maybe_unused]] void set_condition(char change_state, std::string property, std::string change_prop);

  [[maybe_unused]] void remove_condition(char change_state, std::string property);

  [[maybe_unused]] [[nodiscard]] std::map<std::string, AuditedVector<std::string>> find_properties() const;

  [[nodiscard]] bool validate_position(std::pair<double, double> move) const;

  [[nodiscard]] std::vector<Point> find_moves() const;

  [[maybe_unused]] void modify_all_cells(const std::function<void(int, int, char&)>& func);
};
}  // namespace cse