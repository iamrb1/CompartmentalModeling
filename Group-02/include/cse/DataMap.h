/**
* @file DataMap.h
* @author Rahul Baragur
*/

#pragma once

#include <any>
#include <cassert>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <unordered_map>

namespace cse {

/**
* DataMap Class
*
*/
class DataMap {
 private:
  std::unordered_map<std::string, std::any> m_map;

 public:
  /**
   * Default constructor
   */
  DataMap() = default;

  DataMap(std::initializer_list<std::pair<std::string, std::any>> initial);

  /**
  * Insert key value pair into DataMap
  * @tparam T any value
  * @param name key name to be inserted
  * @param val value to be associated with key within DataMap
  */
  template <typename T>
  void insert(const std::string& name, const T& val) {
    assert(!contains(name) && "Key already exists within DataMap");
    m_map[name] = val;
  }

  std::any& operator[](const std::string& key);

  /**
  * .at method for finding where key is
  * @tparam T
  * @param name
  * @return
  */
  template <typename T>
  T at(const std::string& name) {
    if (contains(name)) {
      const std::any& val = m_map[name];
      if (val.type() == typeid(T)) {
        return std::any_cast<T>(m_map[name]);
      }
      throw(std::runtime_error("Wrong type requested from what is contained within DataMap for key: " + name));
    }
    throw(std::out_of_range("Key does not exist within DataMap"));
  }

  bool contains(const std::string& name);

  /**
    * Delete a key from the map
    * @param name key to be deleted
    */
  void key_delete(const std::string& name) {
    assert(contains(name) && "Key does not exist in DataMap");
    m_map.erase(name);
  }

  /**
    * Clear the DataMap
    */
  void clear() {
    assert(!empty() && "Datamap is not empty");
    m_map.clear();
  }

  /**
    * Gives the size of the map
    * @return unsigned long map size
    */
  size_t size() {
    assert(m_map.size() >= 0 && "Size of DataMap is less than 0");
    return m_map.size();
  }

  /**
    * Emptys the DataMap
    */
  bool empty() {
    return m_map.empty();
  }

  /**
    * Returns the count of the keys within the map
    * @param name key to be found
    * @return unsigned long # of keys
    */
  size_t count(const std::string& name) {
    return m_map.count(name);
  }
};
}  // namespace cse