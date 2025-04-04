/**
 * @file StringSet.hpp
 * @author Orhan Aydin
 * @author Emil Rahn-Siegel
 * @author Ivan Bega
 * @brief Header file for StringSet class
 */

#ifndef CSE_STRING_SET_HPP_
#define CSE_STRING_SET_HPP

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <random>
#include <ranges>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>
namespace cse {

/**
 * @brief A container(unordered_set) with a set of strings and various methods
 * like adding, removing, checking existence, set operations and filtering
 * to simplify using the container.
 *
 */
template <class T = std::string>
class StringSet {
 private:
  /// @brief Unordred set of string that as underlying data structure
  std::unordered_set<T> mElements;

 public:
  /// @brief Define an iterator type for string set (unordered set)
  using iterator = std::unordered_set<T>::iterator;

  /// @brief Define const iterator
  using const_iterator = std::unordered_set<T>::const_iterator;

  /**
   * @brief Default constructor for a StringSet
   *
   */
  StringSet() = default;

  /**
   * @brief Move constructor
   *
   * @param other StringSet to move from
   */
  /// https://www.geeksforgeeks.org/move-constructors-in-c-with-examples/
  StringSet(StringSet<T> &&other) noexcept
      : mElements(std::move(other.mElements)) {}

  /**
   * @brief Move assignment operator
   *
   * @param other StringSet to move from
   * @return StringSet& A reference to the assigned StringSet
   */
  /// https://www.geeksforgeeks.org/move-constructors-in-c-with-examples/
  StringSet &operator=(StringSet<T> &&other) noexcept {
    mElements = std::move(other.mElements);
    return *this;
  }

  /**
   * @brief Iterator to the first element of string set (begin iterator)
   * @return Iterator pointing to begining of string set
   */
  iterator begin() { return mElements.begin(); }

  /**
   * @brief Iterator to the end of string set (end iterator)
   * @return Iterator pointing past the last element of string set
   */
  iterator end() { return mElements.end(); }

  /**
   * @brief Iterator to the first element of string set (const)
   * @return Const iterator pointing to beginning of string set
   */
  const_iterator begin() const { return mElements.begin(); }

  /**
   * @brief Iterator to the end of string set (const)
   * @return Const Iterator pointing past the last element of string set
   */
  const_iterator end() const { return mElements.end(); }

  /**
   * @brief Add string into the string set
   * @param str Added to string set
   */
  void insert(const T &str) { mElements.insert(str); }

  /**
   * @brief Add vector of strings into the string set
   * @param stringVector  Vector of strings
   */
  void insert(const std::vector<T> &stringVector) {
    for (const T &str : stringVector) {
      mElements.insert(str);
    }
  }

  /**
   * @brief Erase specific string from string set
   * @param str Deleted from string set
   */
  void erase(const T &str) {
    assert(!str.empty() && "Erasing empty string");
    mElements.erase(str);
  }

  /**
   * @brief Make string set empty
   */
  void clear() noexcept { mElements.clear(); }

  /**
   * @brief Get the size of set
   * @return Size of string set
   */
  size_t size() const { return mElements.size(); }

  /**
   * @brief Check if string set is empty
   * @return True if the set is empty, false otherwise
   */
  bool empty() const { return mElements.empty(); }

  /**
   * @brief Check if two string sets are equal
   * @param other The second set
   * @return true Both sets contain same elements
   * @return false Sets do not contain exactly the same strings
   */
  bool operator==(const StringSet &other) const {
    return mElements == other.mElements;
  }

  /**
   * @brief Check if two string sets are not equal
   * @param other The second set
   * @return true Sets do not contain exactly the same strings
   * @return false Both sets contain same elements
   */
  bool operator!=(const StringSet &other) const {
    return mElements != other.mElements;
  }

  /**
   * @brief Check if a specific string in the set or not
   * @param str that needs to be checked
   * @return 0 or 1. If str in the set returns 1 otherwise 0
   */
  size_t count(const T &str) const { return mElements.count(str); }

  /**
   * @brief Convert set to vector
   * @return Vector of strings
   */
  std::vector<T> to_vector() const {
    return std::vector<T>(mElements.begin(), mElements.end());
  }

  /**
   * @brief Swap the elements of sets
   * @param other The second set
   */
  void Swap(StringSet &other) noexcept {
    std::swap(mElements, other.mElements);
  }

  StringSet Union(const StringSet &other) const;

  StringSet intersection(const StringSet &other) const;

  StringSet difference(const StringSet &other) const;

  void filter(const std::function<bool(const T &str)> filter, bool erase);

  void retain_filter(const std::function<bool(const T &str)> filter);

  void remove_filter(const std::function<bool(const T &str)> filter);

  bool is_subset(const StringSet &other) const;

  void substring_filter(const T &substring);

  void size_filter(int size);

  StringSet search(const T &wildcard);

  int count_occurrence(const T &substring);

  StringSet random_sample(
      int sampleSize, const std::function<bool(const T &)> filter =
                          [](const T &) { return true; });

  std::tuple<int, double, T, T> statistics();

  StringSet symmetric_difference(const StringSet &other) const;

  StringSet union_with(const StringSet &other,
                       const std::function<bool(const T &str)> filter);

  std::set<T> sort(
      const std::function<bool(const T &str, const T &str2)> comparator) const;
};

/**
 * @brief Output stream operator to print set
 * @param os output stream to write
 * @param set String set
 * @return os
 */
template <typename T>
std::ostream &operator<<(std::ostream &os, const StringSet<T> &set) {
  for (const auto &str : set) {
    os << str << " ";
  }
  return os;
}

/**
 * @brief Union of two sets
 * @param other The second set
 * @return A new set that contains unique elements from both sets
 */
template <typename T>
cse::StringSet<T> cse::StringSet<T>::Union(const StringSet<T> &other) const {
  StringSet<T> result;

  for (const auto &str : mElements) {
    result.insert(str);
  }

  for (const auto &str : other.mElements) {
    result.insert(str);
  }

  return result;
}

/**
 * @brief Intersection of two sets
 * @param other The second set
 * @return A set of common values in 2 sets
 */
template <typename T>
cse::StringSet<T> cse::StringSet<T>::intersection(
    const StringSet<T> &other) const {
  StringSet<T> result;

  for (const auto &str : mElements) {
    if (other.mElements.find(str) != other.mElements.end()) {
      result.insert(str);
    }
  }

  return result;
}

/**
 * @brief Operates differentiation between two string sets
 * @param other the second set
 * @return the difference set
 */
template <typename T>
cse::StringSet<T> cse::StringSet<T>::difference(
    const cse::StringSet<T> &other) const {
  StringSet<T> result;

  // Iterate through every element in mElements
  for (const auto &str : mElements) {
    if (other.mElements.find(str) == other.mElements.end()) {
      result.insert(str);
    }
  }

  return result;
}

/**
 * @brief Delete or keep elements based on filter and boolean parameter
 * @param filter Custom filter function
 * @param erase Boolean, RetainFilter function calls with false, RemoveFilter
 * function calls with true
 */
template <typename T>
void cse::StringSet<T>::filter(const std::function<bool(const T &)> predicate,
                               bool erase) {
  std::vector<T> filteredElements;

  for (const auto &str : mElements) {
    if (erase == predicate(str)) {
      filteredElements.push_back(str);
    }
  }

  for (const auto &str : filteredElements) {
    mElements.erase(str);
  }
}

/**
 * @brief Keep elements in string set based on custom filter
 * @param filter custom function
 */
template <typename T>
void cse::StringSet<T>::retain_filter(
    const std::function<bool(const T &)> predicate) {
  filter(predicate, false);
}

/**
 * @brief Delete elements based on custom filter
 * @param filter custom function
 */
template <typename T>
void cse::StringSet<T>::remove_filter(
    const std::function<bool(const T &)> predicate) {
  filter(predicate, true);
}

/**
 * @brief Checks if a current set is a subset of another set
 *
 * @param other Other set that is a superset
 * @return true Current set is a subset of other set
 * @return false Current set is not a subset of other set
 */
template <typename T>
bool cse::StringSet<T>::is_subset(const StringSet<T> &other) const {
  // Prepare a lambda that checks if captured element is in the other set
  // Used DeepSeek to assist with this lambda
  auto otherPredicate = [&other](const T &element) {
    return other.count(element) == 1;
  };

  // use all_of in ranges to loop through each element in mElements
  return std::ranges::all_of(mElements, otherPredicate);
}

/**
 * @brief Removes words from a current StringSet that contain a substring
 * @param substring Substring to check against
 */
template <typename T>
void cse::StringSet<T>::substring_filter(const T &substring) {
  // this function was written with the help from DeepSeek
  if (substring.empty()) return;  // do not do anything for an empty substring

  // Lambda that will be used in RemoveFilter to filter words that we need
  auto substringPredicate = [substring](const T &element) {
    std::string_view element_view(element);
    std::string_view substr_view(substring);
    return element_view.find(substr_view) != std::string_view::npos;
  };

  remove_filter(substringPredicate);
}
/**
 * @brief Search for a string with wildcard
 * @param wildcard Wildcard string
 * @return StringSet<T> Set of strings that match the wildcard
 */
template <typename T>
cse::StringSet<T> cse::StringSet<T>::search(const T &wildcard) {
  StringSet<T> result;

  // Helper function to match a string with a wildcard pattern
  auto matches = [](const T &str, const T &pattern) {
    auto it_str = str.cbegin();
    auto it_pat = pattern.cbegin();

    while (it_str != str.cend() && it_pat != pattern.cend()) {
      if (*it_pat == '*') {
        if (++it_pat == pattern.cend()) {
          return true;
        }
        while (it_str != str.cend() && *it_str != *it_pat) {
          ++it_str;
        }
      } else if (*it_pat == '?' || *it_pat == *it_str) {
        ++it_str;
        ++it_pat;
      } else {
        return false;
      }
    }

    return it_str == str.cend() && it_pat == pattern.cend();
  };

  // Iterate through elements and add matching ones to result
  for (const auto &element : mElements) {
    if (matches(element, wildcard)) {
      result.insert(element);
    }
  }

  return result;
}
/** 
 * @brief Count the number of occurrences of a string in the set
 * @param substring The substring to search for
 * @return The number of occurrences of the substring
 */
template <typename T>
int cse::StringSet<T>::count_occurrence(const T &substring) {
  int totalCount = 0;

  for (const auto &element : mElements) {
    size_t pos = element.find(substring);
    if (pos != T::npos) {
      ++totalCount;
    }
  }

  return totalCount;
}
/**
 * @brief Randomly sample a subset of the set
 * @param sampleSize The number of elements to sample
 * @param filter A function that returns true if an element should be included
 * in the sample defaults to all elements
 * @return A new set containing a random sample of the elements
 */
template <typename T>
cse::StringSet<T> cse::StringSet<T>::random_sample(
    int sampleSize, const std::function<bool(const T &)> filter) {
  StringSet<T> result;
  if (sampleSize <= 0) {
    return result;
  }

  std::vector<T> elements;
  for (const auto &element : mElements) {
    if (filter(element)) {
      elements.push_back(element);
    }
  }

  if (static_cast<std::unordered_set<T>::size_type>(sampleSize) >
      elements.size()) {
    sampleSize = elements.size();
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(elements.begin(), elements.end(), gen);

  for (int i = 0; i < sampleSize; ++i) {
    result.insert(elements[i]);
  }

  return result;
}
/** 
 * @brief Calculate statistics about the set
 * @return A tuple containing the number of strings, the average length of the
 * strings, the longest string, and the shortest string
 */
template <typename T>
std::tuple<int, double, T, T> cse::StringSet<T>::statistics() {
  int numStrings = size();
  if (numStrings == 0) {
    return std::make_tuple(0, 0.0, "", "");
  }

  int totalLength = 0;
  T longestString = "";
  T shortestString = *mElements.begin();

  for (const auto &str : mElements) {
    int length = static_cast<int>(str.length());
    totalLength += length;
    if (length > static_cast<int>(longestString.length())) {
      longestString = str;
    }
    if (length < static_cast<int>(shortestString.length())) {
      shortestString = str;
    }
  }

  double avgLength = static_cast<double>(totalLength) / numStrings;

  return std::make_tuple(numStrings, avgLength, longestString, shortestString);
}

/**
 * @brief Removes strings of length not equal to size
 *
 * @param length length of strings that will be retained
 */
template <typename T>
void cse::StringSet<T>::size_filter(int length) {
  assert(length > 0);

  auto sizePredicate = [length](const T &element) {
    return element.length() == static_cast<std::size_t>(length);
  };

  retain_filter(sizePredicate);
}

/**
 * @brief Check both sets and return not common elements from both sets
 * @param other the second set
 * @return Symmetric Difference set
 */
template <typename T>
cse::StringSet<T> cse::StringSet<T>::symmetric_difference(
    const cse::StringSet<T> &other) const {
  StringSet<T> result1;
  StringSet<T> result2;
  StringSet<T> finalResult;

  result1 = difference(other);
  result2 = other.difference(*this);

  finalResult = result1.Union(result2);

  return finalResult;
}

/**
 * @brief Union two sets if their elements meet the custom filter condition
 * @param other the second set
 * @param filter custom function(condition)
 * @return Union of two sets based on the custom filter
 */
template <typename T>
cse::StringSet<T> cse::StringSet<T>::union_with(
    const cse::StringSet<T> &other,
    const std::function<bool(const T &)> filter) {
  StringSet<T> result = Union(other);
  result.retain_filter(filter);
  return result;
}

/**
 * @brief Sort the StringSet<T>
 * @param comparator sorting condition
 * @return sorted set
 */
template <typename T>
std::set<T> cse::StringSet<T>::sort(
    const std::function<bool(const T &str, const T &str2)> comparator) const {
  std::vector<T> sortedElements = to_vector();
  std::sort(sortedElements.begin(), sortedElements.end(), comparator);
  return std::set<T>(sortedElements.begin(), sortedElements.end());
}
}  // namespace cse

#endif