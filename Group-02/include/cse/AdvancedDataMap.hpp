/**
* @file DataMap.h
* @author Rahul Baragur
*/

#pragma once

#include <any>
#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <typeinfo>
#include <unordered_map>

namespace cse {

class [[maybe_unused]] AdvDataMap {
 private:
  /// Inspired by Dr.Ofria's in class Any implementation (refactored to use unique ptrs)
  class Any {
   private:
    struct Base {
      virtual ~Base() = default;
      [[nodiscard]] virtual std::unique_ptr<Base> clone() const = 0;
    };

    template <typename T>
    struct Val : Base {
      T value;

      explicit Val(T value) : value(value){};

      [[nodiscard]] std::unique_ptr<Base> clone() const override {
        return std::make_unique<Val<T>>(value);
      }
    };

    std::unique_ptr<Base> value_ptr;

   public:
    Any() = default;

    Any(const Any& in) {
      if (in.value_ptr) {
        value_ptr = in.value_ptr->clone();
      } else {
        value_ptr = nullptr;
      }
    }

    Any(Any&& in) noexcept {
      value_ptr = std::move(in.value_ptr);
      in.value_ptr = nullptr;
    }

    template <typename T>
    explicit Any(T value) {
      value_ptr = std::make_unique<Val<T>>(value);
    }

    Any& operator=(const Any& in) {
      if (in.value_ptr) {
        value_ptr = in.value_ptr->clone();
      } else {
        value_ptr = nullptr;
      }
      return *this;
    }

    template <typename T>
    Any& operator=(T value) {
      value_ptr = std::make_unique<Val<T>>(value);
      return *this;
    }

    template <typename T>
    [[nodiscard]] const T& get() const {
      using derive_t = Val<T>;
      auto* derive_ptr = dynamic_cast<derive_t*>(value_ptr.get());
      assert(derive_ptr != nullptr && "Wrong type requested from what is contained within Any");
      return derive_ptr->value;
    }
  };

  std::unordered_map<std::string, Any> m_map;

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
  [[maybe_unused]] inline void insert(const std::string& name, const T& val) {
    // https://stackoverflow.com/questions/3692954/add-custom-messages-in-assert custom messages in assert trick I have implemented within
    assert(!contains(name) && "Key already exists within DataMap");
    m_map[name] = Any(val);
  }

  template <typename T>
  inline T get(const std::string& name) {
    if (!m_map.contains(name)) {
      m_map[name] = Any(T{});
    }
    return m_map[name].get<T>();
  }

  template <typename T>
  [[maybe_unused]] inline T at(const std::string& name) const {
    assert(contains(name) && "Key does not exist in DataMap");
    return m_map.at(name).get<T>();
  }

  /**
   * Converts a value into a string using a stringstream
   * @tparam T templated value
   * @param name key of associated value
   * @return std::string of value
   */
  template <typename T>
  [[maybe_unused]] inline std::string to_string(const std::string& name) {
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
  [[maybe_unused]] inline void erase(const std::string& name) {
    assert(contains(name) && "Key does not exist in DataMap");
    m_map.erase(name);
  }

  /**
    * Clear the AdvDataMap
    */
  [[maybe_unused]] inline void clear() {
    assert(!empty() && "Datamap is not empty");
    m_map.clear();
  }

  /**
    * Gives the size of the map
    * @return unsigned long map size
    */
  [[maybe_unused]] [[nodiscard]] inline size_t size() const {
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
  [[nodiscard]] [[maybe_unused]] inline size_t count(const std::string& name) const {
    return m_map.count(name);
  }

  /**
   * Operator overload for []
   * @param name
   * @return
   */
  Any& operator[](const std::string& name) {
    if (!m_map.contains(name)) {
      m_map[name] = Any(0);
    }
    return m_map[name];
  }
};
}  // namespace cse