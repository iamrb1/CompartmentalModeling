/**
* @file AdvancedDataMap.h
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
#include <vector>
#include <algorithm>

namespace cse {

/// compile time concept checks listed below
template <typename T>
constexpr bool has_sizeof = requires([[maybe_unused]] T & value) {
  sizeof(T);
};

template <typename T>
constexpr bool uses_to_string = requires(T & value) {
  std::to_string(value);
};

template <typename T>
constexpr bool has_to_string = requires(T & value) {
  value.ToString();
};

/**
 * Advanced version of DataMap class
 * Features include: Custom memory management (custom any class)
 * Type erased values paired to std::string keys
 * Can add both values and references to DataMap
 * Concept checks and type checks
 */
class [[maybe_unused]] AdvDataMap {
 private:
  /// Inspired by Dr.Ofria's in class Any implementation (refactored to use unique ptrs)
  class Any {
   private:
    struct Base {
      virtual ~Base() = default;
      [[nodiscard]] virtual std::unique_ptr<Base> clone() const = 0;
      [[nodiscard]] virtual const std::type_info& type() const = 0;
      [[maybe_unused]] [[nodiscard]] virtual bool is_ref() const = 0;
      [[maybe_unused]] virtual void set() = 0;
      [[nodiscard]] virtual bool is_numeric() const = 0;
      [[nodiscard]] virtual bool is_conv_to_string() const = 0;
    };

    template <typename T>
    struct Val : Base {
      /// store the value
      T value;
      /// flag to see if a reference is to be added to map
      bool ref_flag;
      /// ptr to the value added if initialized elsewhere in code
      T* ref_ptr;
      /// checks if value is numeric in constructor
      static constexpr bool numeric = std::is_arithmetic_v<T>;
      /// checks if value is convertible to string in constructor
      static constexpr bool convert = std::convertible_to<T, std::string>;

      /**
       * Constructor for Val (Any internal)
       * @param value type erased value to be added
       * @param flag true if reference false if not
       * @param ref ptr to member variable value if flag is true
       */
      explicit Val(T value, bool ref_flag = false, T* ref = nullptr) : value(value), ref_flag(ref_flag), ref_ptr(ref) {}

      /**
       * Clone function for copy assignment and constructor
       * @return
       */
      [[nodiscard]] std::unique_ptr<Base> clone() const override {
        return std::make_unique<Val<T>>(value, ref_flag, ref_ptr);
      }

      /**
       * Get type of the value
       * @return
       */
      [[nodiscard]] const std::type_info& type() const override {
        return typeid(T);
      }

      /**
       * Return the flag if a reference
       * @return
       */
      [[nodiscard]] bool is_ref() const override {
        return ref_flag;
      }

      /**
       * Dereference the ref_ptr and set it to value when the original is changed
       */
      [[maybe_unused]] void set() override {
        assert(ref_ptr && "ref_ptr is null");
        value = *ref_ptr;
      }

      /**
       * Getter for numeric
       * @return
       */
      [[nodiscard]] bool is_numeric() const override {
        return numeric;
      }

      /**
       * Getter for convert
       * @return
       */
      [[nodiscard]] bool is_conv_to_string() const override {
        return convert;
      }
    };

    /**
     * Unique_ptr to the type erased value
     */
    std::unique_ptr<Base> value_ptr;

   public:
    /**
     * Default constructor
     */
    Any() = default;

    /**
     * Copy constructor
     * clone the value ptr if exists otherwise set to nullptr
     * @param in source any to copy
     */
    Any(const Any& in) {
      if (in.value_ptr) {
        value_ptr = in.value_ptr->clone();
      } else {
        value_ptr = nullptr;
      }
    }

    /**
     * Move constructor
     * transfer ownership from from source to this
     * @param in
     */
    Any(Any&& in) noexcept : value_ptr(std::move(in.value_ptr)) { in.value_ptr = nullptr; }

    /**
     * Templated Any constructor
     * has defaults to false and nullptr for insert method of advdatamap
     * sets value_ptr from parameters
     * @tparam T type of value
     * @param value value to be stored
     * @param flag if reference or not
     * @param ref_ptr pointer to value if it is a reference
     */
    template <typename T>
    Any(T value, bool flag = false, T* ref_ptr = nullptr) {  // NOLINT(*-explicit-constructor)
      /// I dont want this to be marked explicit
      value_ptr = std::make_unique<Val<T>>(value, flag, ref_ptr);
    }

    /**
     * Copy assignment operator for any
     * similar logic to copy constructor
     * @param in source any to copy
     * @return
     */
    Any& operator=(const Any& in) {
      if (in.value_ptr) {
        value_ptr = in.value_ptr->clone();
      } else {
        value_ptr = nullptr;
      }
      return *this;
    }

    /**
     * Assignment operator
     * assign new value to the old any object
     * @tparam T type of assigned value
     * @param value value to be assigned
     * @return Any reference
     */
    template <typename T>
    Any& operator=(T value) {
      value_ptr = std::make_unique<Val<T>>(value);
      return *this;
    }

    /**
     * Get stored value of type T from DataMap
     * @tparam T type
     * @return reference T of the value returned
     */
    template <typename T>
    [[nodiscard]] const T& get() const {
      using derive_t = Val<T>;
      auto* derive_ptr = dynamic_cast<derive_t*>(value_ptr.get());
      /// get raw pointer from value_ptr(unique ptr) and dynamically cast it (Base*) to a pointer of Val<T>
      assert(derive_ptr != nullptr && "Wrong type requested from what is contained within Any");
      if (derive_ptr->is_ref()) {
        derive_ptr->set();
        /// if the ptr is a reference as specified by the flag, i can set the value to the updated
        /// value by dereferencing the ptr
      }
      return derive_ptr->value;
    }

    /**
     * Get the type of the stored any object
     * @return const type_info reference
     */
    [[nodiscard]] const std::type_info& type() const {
      assert(value_ptr && "Any is empty");
      return value_ptr->type();
    }

    /**
     * Check if the any stored value is a reference
     * @return bool
     */
    [[nodiscard]] bool is_ref() const {
      assert(value_ptr && "Any is empty");
      return value_ptr->is_ref();
    }

    /**
     * Check if the any stored value is a numeric value
     * @return bool
     */
    [[nodiscard]] bool is_numeric() const {
      assert(value_ptr && "Any is empty");
      return value_ptr->is_numeric();
    }

    /**
     * Check if the any stored value is convertible to string
     * @return bool
     */
    [[nodiscard]] bool is_conv_to_string() const {
      assert(value_ptr && "Any is empty");
      return value_ptr->is_conv_to_string();
    }
  };

  /// Underlying map object built upon to create DataMap
  std::unordered_map<std::string, Any> m_map;

 public:
  /**
   * Default constructor
   */
  AdvDataMap() = default;

  /**
   * Constructor to allow initializer list for AdvDataMap
   * @param initial std::initializer list of a pair of string and any
   */
  [[maybe_unused]] AdvDataMap(std::initializer_list<std::pair<std::string, Any>> initial) {
    for (const auto& pair : initial) {
      m_map[pair.first] = pair.second;
    }
  }

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

  /**
    * Check if key is within the DataMap
    * @param name std::string name to be found
    * @return bool if key exists or not
     *
     * (For grading purposes of initial specs, this function function was renamed from keyContains to better fit naming
     * convention)
    */
  [[nodiscard]] inline bool contains(const std::string& name) const {
    return (m_map.find(name) != m_map.end());
  }

  /**
  * Insert key value pair into DataMap
   *
   * (For grading purposes of advanced specs, this function was renamed from set to insert to better match the standard
   * library)
  * @tparam T any value
  * @param name key name to be inserted
  * @param val value to be associated with key within DataMap
  */
  template <typename T>
  [[maybe_unused]] inline void insert(const std::string& name, const T& val) {
    // https://stackoverflow.com/questions/3692954/add-custom-messages-in-assert custom messages in assert trick I have implemented within
    assert(!contains(name) && "Key already exists within DataMap");
    m_map[name] = Any(val);
  }

  /**
   * Insert a reference into the DataMap that can change if the original thing pointed to changes
   * @tparam T any value
   * @param name key name to be inserted
   * @param val reference to be associated with key within DataMap
   */
  template <typename T>
  [[maybe_unused]] inline void add_ref(const std::string& name, T& val) {
    assert(!contains(name) && "Key already exists within DataMap");
    m_map[name] = Any(val, true, &val);
  }

  /**
   * Check if the value associated with the key specified is a reference
   * @param name key name to be checked
   * @return bool
   */
  [[maybe_unused]] [[nodiscard]] inline bool is_ref(const std::string& name) const {
    assert(contains(name) && "Key does not exist in DataMap");
    return m_map.at(name).is_ref();
  }

  /**
   * Emulate behavior of the [] operator as in std::unordered map
   * Get value from key std::string
   * If key does not exist, set key to defaulted any templated value constructor
   * @tparam T templated value returned
   * @param name std::string
   * @return T
   */
  template <typename T>
  inline T get(const std::string& name) {
    if (!m_map.contains(name)) {
      m_map[name] = Any(T{});
    }
    assert(m_map[name].type() == typeid(T) && "Wrong type requested from what is contained within DataMap for value");
    return m_map[name].get<T>();
  }

  /**
  * .at method for finding where key is
  * @tparam T templated value returned
  * @param name std::string
  * @return
  */
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
    /// refactored Dr.Ofria]s version to work with AdvDataMap
    assert(contains(name) && "Key does not exist in DataMap");
    T value = get<T>(name);
    if constexpr (std::convertible_to<T, std::string>) {
      return value;
    } else if constexpr (uses_to_string<T>) {
      return std::to_string(value);
    } else if constexpr (has_to_string<T>) {
      return value.ToString();
    } else {
      std::stringstream ss;
      ss << value;
      return ss.str();
      /// If passed in type not compartible with concept checks, manually create a stringstream
      /// and pass in the value returning the string
    }
  }

  /**
   * Check if value associated with key is the same type as the type specified
   * @tparam T type to be checked agaisnt
   * @param name key name
   * @return bool
   */
  template <typename T>
  [[maybe_unused]] inline bool is_type(const std::string& name) {
    assert(contains(name) && "Key does not exist in DataMap");
    return m_map.at(name).type() == typeid(T);
  }

  /**
   * Check if value associated with key is a numeric value
   * @param name key name
   * @return bool
   */
  [[maybe_unused]] inline bool is_numeric(const std::string& name) {
    assert(contains(name) && "Key does not exist in DataMap");
    return m_map.at(name).is_numeric();
  }

  /**
   * Check if value associated with key is convertible to string
   * @param name key name
   * @return bool
   */
  [[maybe_unused]] inline bool is_conv_to_string(const std::string& name) {
    assert(contains(name) && "Key does not exist in DataMap");
    return m_map.at(name).is_conv_to_string();
  }

  /**
   * Get memory size of the value associated with a key
   * @tparam T type of value
   * @param name key name
   * @return size_t memory size
   */
  template <typename T>
  [[maybe_unused]] inline size_t get_mem_size(const std::string& name) {
    assert(contains(name) && "Key does not exist in DataMap");
    if constexpr (has_sizeof<T>) {
      return sizeof(T);
    }
    return sizeof(m_map.at(name));
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
    return m_map.find(name) != m_map.end();
  }

  /**
   * Operator overload for []
   * @param name
   * @return
   */
  Any& operator[](const std::string& name) {
    if (!m_map.contains(name)) {
      m_map[name] = Any(0);
      /// create a default value if called with no assignment ex data_map["key"]
    }
    return m_map[name];
  }

  template <typename T>
  [[maybe_unused]] [[nodiscard]] inline std::vector<std::string> get_type_keys() const {
    /// original method created by myself, refactored to utilizing a lambda within a for each
    /// instead of a loop utilizing assistance by gpt4o
    std::vector<std::string> names = {};
    if (m_map.empty()) {
      return names;
    }
    std::for_each(m_map.begin(), m_map.end(), [&names](const auto& pair) {
      if (pair.second.type() == typeid(T)) {
        names.push_back(pair.first);
      }
    });
    /// use for_each and specify a lambda within as a filter to grab desired keys
    /// previous implementation was to use a for range loop instead of for_each
    /// now captures names within the lambda and performs operation to push the string back within vector if type match
    return names;
  }
};
}  // namespace cse