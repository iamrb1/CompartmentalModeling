/**
* @file DataMap.h
* @author Rahul Baragur
*/

#pragma once

#include <any>
#include <cassert>
#include <memory>
#include <sstream>
#include <string>
#include <typeinfo>
#include <unordered_map>

namespace cse {

class AdvDataMap {
 private:
  /// Inspired by Dr.Ofria's in class Any implementation
  struct Base {
    virtual ~Base() = default;
  };

  template <typename T>
  struct Val : Base {
    T value;

    explicit Val(T value) : value(value){};
  };

  std::unordered_map<std::string, std::unique_ptr<Base>> m_map;

 public:
  /**
   * Default constructor
   */
  AdvDataMap() = default;

  /**
   * Copy constructor
   * @param other AdvDataMap to be copied
   */
  AdvDataMap(const AdvDataMap& other) = default;

  /**
   * Assignment operator
   * @param other AdvDataMap to be copied
   * @return reference to AdvDataMap
   */
  AdvDataMap& operator=(const AdvDataMap& other) = default;

  [[nodiscard]] inline bool contains(const std::string& name) const {
    return (m_map.find(name) != m_map.end());
  }

  template <typename T>
  inline void insert(const std::string& name, const T& val) {
    // https://stackoverflow.com/questions/3692954/add-custom-messages-in-assert custom messages in assert trick I have implemented within
    assert(!contains(name) && "Key already exists within DataMap");
    m_map[name] = std::make_unique<Val<T>>(val);
  }

  template <typename T>
  inline T get(const std::string& name) {
    if (!m_map.contains(name)) {
      m_map[name] = std::make_unique<Val<T>>(T{});
    }
    /// GPT 4 assisted me with the bottom line of code
    /// I prompted GPT to show how to successfully safely downcast using dynamic_cast as taught in class
    /// We get the raw pointer from the std::unique_ptr using .get and then utilize dynamic_cast to case
    /// Base * into Val *. auto keyword is for conciseness
    auto* holder = dynamic_cast<Val<T>*>(m_map[name].get());
    assert(holder != nullptr && "Wrong type requested from what is contained within DataMap for value");
    return holder->value;
  }

  template <typename T>
  inline T at(const std::string& name) const {
    assert(contains(name) && "Key does not exist in DataMap");
    auto* holder = dynamic_cast<Val<T>*>(m_map.at(name).get());
    assert(holder != nullptr && "Wrong type requested from what is contained within DataMap for value");
    return holder->value;
  }

  /**
   * Converts a value into a string using a stringstream
   * @tparam T templated value
   * @param name key of associated value
   * @return std::string of value
   */
  template <typename T>
  inline std::string to_string(const std::string& name) {
    assert(contains(name) && "Key does not exist in DataMap");
    T value = get<T>(name);
    std::stringstream ss;
    ss << value;
    return ss.str();
  }

  /**
    * Delete a key from the map
    * @param name key to be deleted
    */
  inline void erase(const std::string& name) {
    assert(contains(name) && "Key does not exist in DataMap");
    m_map.erase(name);
  }

  /**
    * Clear the AdvDataMap
    */
  inline void clear() {
    assert(!empty() && "Datamap is not empty");
    m_map.clear();
  }

  /**
    * Gives the size of the map
    * @return unsigned long map size
    */
  [[nodiscard]] inline size_t size() const {
    return m_map.size();
  }

  /**
    * Emptys the AdvDataMap
    */
  [[nodiscard]] inline bool empty() const {
    return m_map.empty();
  }

  /**
    * Returns the count of the keys within the map
    * @param name key to be found
    * @return unsigned long # of keys
    */
  [[nodiscard]] inline size_t count(const std::string& name) const {
    return m_map.count(name);
  }
};
}  // namespace cse