/**
 * @file StaticString.hpp
 * @author Mehmet Efe Caylan
 * @author Will Crawford
 * @author Ivan Bega
 * @author Emil Rahn Siegel
 * @author Orhan Aydin
 * @brief StaticString Class implementation file.
 * @version 0.2
 * @date 2025-02-06
 *
 */

#ifndef CSE_STATIC_STRING_HPP_
#define CSE_STATIC_STRING_HPP_

#include <cassert>
#include <cstring>
#include <iomanip>
#include <iterator>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace cse {

/**
 * @brief StaticString Class that provides faster string operations by limiting
 * the static size.
 *
 * StaticString class offers a compatible version of std::string class; a fixed
 * sized array provides string operations.
 *
 * @tparam MaxSize Static size_t value to limit our string's size excluding the
 * null terminator at the end.
 */
template <std::size_t MaxSize>
class StaticString {
 public:
  /**
   * @brief Iterator to the beginning of static string (begin iterator)
   * @return Iterator pointing to beginning of static string
   */
  constexpr char* begin() { return mString; }

  /**
   * @brief Iterator to the end of static string (end iterator)
   * @return Iterator pointing past the end of static string
   */
  constexpr char* end() { return mString + mCurrentSize; }

  /**
   * @brief Const iterator to the beginning of static string (begin iterator)
   * @return Const iterator pointing to beginning of static string
   */
  constexpr const char* cbegin() const { return mString; }

  /**
   * @brief Const iterator to the end of static string (end iterator)
   * @return Const iterator pointing past the end of static string
   */
  constexpr const char* cend() const { return mString + mCurrentSize; }

  /// @brief Constant value for StaticString npos, not found value.
  static constexpr std::size_t npos = static_cast<std::size_t>(-1);

  /*----------------------CONSTRUCTORS--------------------------------------*/

  /**
   * @brief Construct a new Static String object
   *
   * If the given string is empty then initialize it as empty.
   */
  constexpr StaticString() noexcept : mCurrentSize(0) {
    mString[0] = null_terminator;
  }

  /**
   * @brief Construct a new Static String object with a non-empty string
   *
   * This constructor initializes the StaticString object by copying characters
   * from the provided objects `std::string`, `std::string_view`, or `const
   * char*` into the fixed size character array. It ensures that no more than
   * MaxSize characters are copied and appends a null terminator at the end.
   *
   * @attention Make sure to check string provided is shorter than maximum size
   * defined.
   *
   * @tparam T A type that is implicitly convertible to `std::string_view`, such
   * as `const char*`, `std::string`, or `std::string_view`.
   * @param cstr Templated basic string to construct StaticString.
   */
  template <typename T, typename = std::enable_if_t<
                            std::is_convertible_v<T, std::string_view>>>
  constexpr StaticString(const T& cstr) : mCurrentSize(0) {
    while (cstr[mCurrentSize] != null_terminator && mCurrentSize < MaxSize) {
      mString[mCurrentSize] = cstr[mCurrentSize];
      ++mCurrentSize;
    }

    // The provided string is longer than MaxSize defined.
    assert(mCurrentSize <= MaxSize &&
           "Error: Input string exceeds StaticString defined maximum size.");

    // Initialize the last location of the string to null terminator
    mString[mCurrentSize] = null_terminator;
    mString[MaxSize] = null_terminator;
  }

  /**
   * @brief Construct a new StaticString object from another StaticString object
   *
   * @param staticString Static String object to be copied.
   */
  constexpr StaticString(const StaticString& staticString) noexcept
      : mCurrentSize(staticString.mCurrentSize) {
    std::copy(staticString.mString, staticString.mString + MaxSize + 1,
              mString);
  }

  /**
   * @brief Construct a new templated Static String object copying from another
   * StaticString.
   *
   * To be able to copy form another object, new templated StaticString object
   *  must be equal or greater sized in terms of static limit defined.
   *
   * @tparam newMaxSize New allocated objects maximum size value
   * @param staticString Source staticString object
   */
  template <std::size_t newMaxSize>
  constexpr StaticString(const StaticString<newMaxSize>& staticString) {
    // Check if the string that we will copy is larger than MaxSize defined
    assert(staticString.length() <= MaxSize &&
           "Error: The length of copied string exceeds static limit defined.");

    std::copy(staticString.get_str(),
              staticString.get_str() + staticString.length(), mString);

    mCurrentSize = staticString.length();
    mString[mCurrentSize] = null_terminator;
  }

  /*----------------------------OPERATORS-----------------------------------*/

  /**
   * @brief Allows to implicitly convert StaticString to std::string_view.
   *
   * @return std::string_view StringView of the StaticString.
   */
  constexpr operator std::string_view() const noexcept {
    // DeepSeek has been used to generate this function
    return std::string_view(mString, mCurrentSize);
  }

  /**
   * @brief Assignment operator.
   *
   * Copies the content from another StaticString.
   *
   * @param staticString StaticString to be assigned.
   * @return StaticString& Reference to this StaticString object.
   */
  StaticString& operator=(const StaticString& staticString) {
    // Avoid assigning to itself
    if (this == &staticString) {
      return *this;
    }

    // Check if the string that we will copy is larger than MaxSize defined
    assert(staticString.length() <= MaxSize &&
           "Error: The length of copied string exceeds static limit defined.");
    std::copy(staticString.mString,
              staticString.mString + staticString.length(), mString);

    mCurrentSize = staticString.mCurrentSize;
    mString[mCurrentSize] = null_terminator;
    return *this;
  }

  /**
   * @brief Assignment operator from StaticString.
   *
   * New string to be assigned must be shorter than size limit or exact length
   * with maximum size limit. When a StaticString assigned to a nullptr,
   * it will automatically clear itself and will be empty
   * as well as empty string initialization.
   *
   * @tparam T The templated type of basic string convertible to
   * std::string_view
   * @param str Basic string to be assigned.
   * @return StaticString& Reference to this StaticString object.
   */
  template <typename T, typename = std::enable_if_t<
                            std::is_convertible_v<T, std::string_view>>>
  constexpr StaticString& operator=(const T& str) {
    std::string_view view = str;

    if (view.size() > MaxSize) {
      throw std::out_of_range(
          "Invalid size for assign input, input string is too long.");
    }

    std::copy(view.begin(), view.end(), mString);
    mCurrentSize = view.size();
    mString[mCurrentSize] = null_terminator;

    // Rest of the string is cleared since this is a new string initialization.
    for (std::size_t i = mCurrentSize + 1; i <= MaxSize; ++i) {
      mString[i] = null_terminator;
    }

    return *this;
  }

  /**
   * @brief Overload of nullptr assign for StaticString.
   *
   * Nullptr assign will lead to clear the buffer of the string and assign
   * all charaters to null terminator.
   *
   * @return StaticString& Reference to this StaticString object.
   */
  constexpr StaticString& operator=(std::nullptr_t) noexcept {
    clear();
    return *this;
  }

  /**
   * @brief Index operator to get value in the specific index of string.
   *
   * Index operator only used to get a copy of character on the given index.
   *
   * @see set() to change a character on a given index.
   *
   * @param index size_t The index of character.
   * @return Indexed char value.
   */
  inline char operator[](std::size_t index) const {
    // Indexed value is out of range or
    // trying to index to the null terminator at the end
    if (index >= mCurrentSize) {
      throw std::out_of_range("Index value is out of range");
    }
    return mString[index];
  }

  /**
   * @brief Equality comparison with another StaticString.
   *
   * @param rhs The StaticString to compare with.
   * @return true if both strings are equal.
   * @return false if both strings are not equal.
   */
  bool operator==(const StaticString& rhs) const noexcept {
    return mCurrentSize == rhs.mCurrentSize &&
           std::strcmp(mString, rhs.mString) == 0;
  }

  /**
   * @brief Equality comparison with a C-string.
   *
   * @param rhs The C-string to compare with.
   * @return true if both strings are equal.
   * @return false if both strings are not equal.
   */
  bool operator==(const char* rhs) const {
    assert(rhs != nullptr && "Nullptr comparisons not a legal comparison");
    if (!rhs) return false;
    return std::strcmp(mString, rhs) == 0;
  }

  /**
   * @brief Less-than comparison with another StaticString.
   *
   * @param rhs The StaticString to compare with.
   * @return true if this string is lexicographically less than.
   * @return false if this string is lexicographically greater than.
   */
  bool operator<(const StaticString& rhs) const noexcept {
    return std::strcmp(mString, rhs.mString) < 0;
  }

  /**
   * @brief Less-than comparison with a C-string.
   *
   * @param rhs The StaticString to compare with.
   * @return true if this string is lexicographically less than.
   * @return false if this string is lexicographically greater than.
   */
  bool operator<(const char* rhs) const {
    if (!rhs) return false;
    assert(rhs != nullptr && "Nullptr comparisons not a legal comparison");
    return std::strcmp(mString, rhs) < 0;
  }

  /**
   * @brief Greater-than comparison with another StaticString.
   *
   * @param rhs The StaticString to compare with.
   * @return true if this string is lexicographically greater than.
   * @return false if this string is lexicographically less than.
   */
  bool operator>(const StaticString& rhs) const noexcept {
    return std::strcmp(mString, rhs.mString) > 0;
  }

  /**
   * @brief Greater-than comparison with a C-string.
   *
   * @param rhs The StaticString to compare with.
   * @return true if this string is lexicographically greater than.
   * @return false if this string is lexicographically less than.
   */
  bool operator>(const char* rhs) const {
    if (!rhs) return true;
    assert(rhs != nullptr && "Nullptr comparisons not a legal comparison");
    return std::strcmp(mString, rhs) > 0;
  }

  /**
   * @brief Less-than-or-equal-to comparison with another StaticString.
   *
   * @param rhs The StaticString to compare with.
   * @return true if this string is lexicographically less than or equal to rhs.
   * @return false if this string is lexicographically greater than rhs.
   */
  bool operator<=(const StaticString& rhs) const noexcept {
    return std::strcmp(mString, rhs.mString) <= 0;
  }

  /**
   * @brief Less-than-or-equal-to comparison with a C-string.
   *
   * @param rhs The StaticString to compare with.
   * @return true if this string is lexicographically less than or equal to rhs.
   * @return false if this string is lexicographically greater than rhs.
   */
  bool operator<=(const char* rhs) const {
    if (!rhs) return false;
    assert(rhs != nullptr && "Nullptr comparisons not a legal comparison");
    return std::strcmp(mString, rhs) <= 0;
  }

  /**
   * @brief Greater-than-or-equal-to comparison with another StaticString.
   *
   * @param rhs The StaticString to compare with.
   * @return true if this string is lexicographically greater than or equal rhs.
   * @return false if this string is lexicographically less than rhs.
   */
  bool operator>=(const StaticString& rhs) const noexcept {
    return std::strcmp(mString, rhs.mString) >= 0;
  }

  /**
   * @brief Greater-than-or-equal-to comparison with a C-string.
   *
   * @param rhs The StaticString to compare with.
   * @return true if this string is lexicographically greater than or equal rhs.
   * @return false if this string is lexicographically less than rhs.
   */
  bool operator>=(const char* rhs) const {
    if (!rhs) return true;
    assert(rhs != nullptr && "Nullptr comparisons not a legal comparison");
    return std::strcmp(mString, rhs) >= 0;
  }

  /*--------------------------FUNCTIONS-------------------------------------*/

  /**
   * @brief Return the maximum size of the string.
   *
   * @return std::size_t static size of the string.
   */
  constexpr std::size_t size() const noexcept { return MaxSize; }

  /**
   * @brief Return the current length of the string.
   *
   * @return std::size_t Length of the string.
   */
  constexpr std::size_t length() const noexcept { return mCurrentSize; }

  /**
   * @brief Get the stored string in StaticString.
   *
   * @return const char* A pointer to the character array.
   */
  constexpr const char* get_str() const noexcept { return mString; }

  /**
   * @brief Get the stored string in StaticString.
   *
   * @return char* A pointer to the character array.
   */
  constexpr char* get_str() noexcept { return mString; }

  /**
   * @brief Converts the StaticString to an std::string.
   *
   * @return std::string The equivalent std::string.
   */
  std::string to_string() const { return std::string(mString, mCurrentSize); }

  /**
   * @brief Returns a string_view of the current StaticString content.
   *
   * @return std::string_view A view of the current string data.
   */
  constexpr std::string_view view() const noexcept {
    return std::string_view(mString, mCurrentSize);
  }

  /**
   * @brief Finds the first occurrence of a character, string, string_view,
   *  char* in the string.
   *
   * @tparam T Templated types are string, string_view, char*
   * @param str Templated parameter to searched in the string
   * @return std::size_t The index of the character if found;
   * if not found StaticString::npos.
   */
  template <typename T, typename = std::enable_if_t<
                            std::is_convertible_v<T, std::string_view>>>
  constexpr std::size_t find(const T& str) const noexcept {
    // std::string, char*, char[N] are all convertible to string_view, thus
    // We convert templated input to convert into string_view to perform find
    std::string_view currentView(mString, mCurrentSize);
    std::string_view size(str);

    if (size.empty() || size.length() > mCurrentSize) return npos;

    return currentView.find(size);
  }

  /**
   * @brief Finds the first occurrence of a character in the string.
   *
   * @param ch Character to be searched
   * @return std::size_t index of the character found
   */
  constexpr std::size_t find(char ch) const noexcept {
    for (std::size_t i = 0; i < mCurrentSize; ++i) {
      if (mString[i] == ch) return i;
    }
    return npos;
  }

  /**
   * @brief Sets the given character at given index.
   *
   * Indexing starts from 0; if the MaxSize is 10 then last possible index is 9.
   * If inserted character is a null terminator, StaticString will behave as
   * the rest of the string will be ignored and won't be able to recover it.
   *
   * @attention If '\0' is explicitly set as a character,
   * rest of the string will be terminated.
   *
   * @param index size_t Index to be changed.
   * @param character Char value to be assigned.
   * @throws std::out_of_range If the index is invalid.
   */
  void set(const size_t& index, const char& character) {
    if (index >= mCurrentSize || index >= MaxSize) {
      throw std::out_of_range("Index value is out of range");
    }

    // If user explicitly tries to cut their strings with null terminator
    // Handle the size appropriately
    if (character == '\0') {
      mCurrentSize = index;
    }
    mString[index] = character;
  }

  /*--------------------------- STRING OPERATIONS ----------------------------*/

  /**
   * @brief Appends a basic string type objects to the end of the current
   * string.
   *
   * Appends the given basic string objects to the end of StaticString, if
   * the maxsize is not exceeded.
   *
   * @tparam T The templated type of basic string convertible to
   * std::string_view
   * @throw out_of_range If the resulting operation exceeds the static limit
   * out_of_range error is thrown.
   * @param str Basic string type object to be appended.
   */
  template <typename T, typename = std::enable_if_t<
                            std::is_convertible_v<T, std::string_view>>>
  void append(const T& str) {
    std::string_view temp_view = str;

    // assert((mCurrentSize + cstr_length <= MaxSize) &&
    // "Appending string exceeds maximum size");
    if (mCurrentSize + temp_view.size() > MaxSize) {
      throw std::out_of_range(
          "Static limit exceeded, appended string must be within limits "
          "defined");
    }

    std::copy(temp_view.data(), temp_view.data() + temp_view.size(),
              mString + mCurrentSize);

    // Update the length.
    mCurrentSize += temp_view.size();
    mString[mCurrentSize] = null_terminator;
  }

  /**
   * @brief Appends a character to the end of the current string.
   *
   * @param character The character to append.
   * @throws std::out_of_range If the resulting string would exceed
   * the maximum size.
   */
  void append(const char& character) {
    // Validate that given string fits into our string within static limit.
    if (mCurrentSize + 1 > MaxSize) {
      throw std::out_of_range(
          "Static limit exceeded, appended string must be within limits "
          "defined");
    }

    // Update the length and set the character.
    mString[mCurrentSize] = character;
    mCurrentSize++;
    mString[mCurrentSize] = null_terminator;
  }

  /**
   * @brief Concatenates another StaticString onto this StaticString object.
   *
   * @param rhs The StaticString to concatenate.
   * @return StaticString& Reference to this StaticString.
   * @throws std::out_of_range If the resulting string would exceed
   * the maximum size.
   */
  StaticString& concat(const StaticString& rhs) {
    // Validate that given string fits into our string within static limit.
    if (mCurrentSize + rhs.mCurrentSize > MaxSize) {
      throw std::out_of_range(
          "Static limit exceeded, concatenated string must be within MaxSize");
    }

    std::copy(rhs.mString, rhs.mString + rhs.mCurrentSize + 1,
              mString + mCurrentSize);

    // Update the length.
    mCurrentSize += rhs.mCurrentSize;
    mString[mCurrentSize] = null_terminator;
    return *this;
  }

  /**
   * @brief Extracts a substring as a std::string_view.
   *
   * Substring function returns std::string_view,
   * read only access to given range of substring.
   *
   * @attention Minus indexing is not allowed.
   *
   * @param start The starting index (inclusive).
   * @param end The ending index (exclusive).
   * @return std::string_view Representing the substring.
   * @throws std::out_of_range If the resulting string would exceed
   * the maximum size.
   */
  std::string_view substr(const std::size_t& start,
                          const std::size_t& end) const {
    // Validate that given string fits into our string within static limit.
    if (start > end || end > mCurrentSize) {
      throw std::out_of_range("Invalid substring request");
    }
    return std::string_view(mString + start, end - start);
  }

  /**
   * @brief Insert provided string to the given index and shift the rest.
   *
   * Insert adds given string to starting from the given index such that
   * StaticString s("123456789");
   * s.insert("Test", 0); -> "Test123456789"
   * Allowed types for insert is char, string, char*, string_view
   *
   * @tparam T Templated type
   * @param str String to be inserted
   * @param index size_t index to insert string.
   */
  template <typename T, typename = std::enable_if_t<
                            std::is_convertible_v<T, std::string_view>>>
  void insert(const T& str, const std::size_t& index) {
    std::string_view temp_view = str;

    if (index > mCurrentSize || mCurrentSize + temp_view.size() > MaxSize) {
      throw std::out_of_range(
          "Static limit exceeded, inserted string must be within MaxSize with "
          "current string size.");
    }

    // Move the curent characters to create space for insert
    std::memmove(mString + index + temp_view.size(), mString + index,
                 mCurrentSize - index);

    std::memcpy(mString + index, temp_view.data(), temp_view.size());

    mCurrentSize += temp_view.size();
    mString[mCurrentSize] = null_terminator;
  }

  /**
   * @brief Insert provided string to the given index and shift the rest.
   *
   * Overload for char as parameter.
   * Insert adds given string to starting from the given index such that
   * StaticString s("123456789");
   * s.insert('T', 0); -> "T123456789"
   * Allowed types for insert is char, string, char*, string_view
   *
   * @param chr charater to be inserted
   * @param index size_t index to insert character
   */
  void insert(const char& chr, const std::size_t& index) {
    if (index > mCurrentSize || mCurrentSize + 1 > MaxSize) {
      throw std::out_of_range(
          "Static limit exceeded, inserted string must be within MaxSize with "
          "current string size.");
    }

    // Move the curent characters to create space for insert
    std::memmove(mString + index + 1, mString + index, mCurrentSize - index);

    mString[index] = chr;
    ++mCurrentSize;
    mString[mCurrentSize] = null_terminator;
  }

  /**
   * @brief Removes characters of a range from the StaticString.
   *
   * Removes charaters within a valid range of indexes, start index is inclusive
   * while end index is exclusive.
   *
   * @param start size_t Start index of the range (inclusive).
   * @param end size_t End index of the range (exclusive).
   */
  void remove(const size_t& start, const size_t& end) {
    if (start >= end || end > mCurrentSize) {
      throw std::out_of_range(
          "Index is out of bound , start and end index for remove must be "
          "withing static limits.");
    }
    size_t length = end - start;

    std::memmove(mString + start, mString + end, mCurrentSize - end);

    mCurrentSize -= length;
    mString[mCurrentSize] = null_terminator;
  }

  /**
   * @brief Finds every occurrence of a character, string, string_view,
   *  char* in the string.
   *
   * FindAll searches trough the StaticString and returns all occurance of
   * searched string in the StaticString as the indexes to the start of them.
   *
   * @tparam T Templated types are string, string_view, char*
   * @param str Templated parameter to be searched in the string
   * @return std::vector<std::size_t> A vector of the indexes of the character
   * if found; if not found empty vector.
   */
  template <typename T, typename = std::enable_if_t<
                            std::is_convertible_v<T, std::string_view>>>
  constexpr std::vector<std::size_t> findAll(const T& str) const noexcept {
    std::string_view size(str);

    // A list of all of the indexes found
    std::vector<size_t> indexesFound = std::vector<size_t>{};

    for (std::size_t i = 0; i <= mCurrentSize - size.length(); ++i) {
      std::size_t j = 0;
      for (; j < size.length(); ++j) {
        if (mString[i + j] != str[j]) break;
      }
      if (j == size.length()) {
        indexesFound.push_back(i);
      }
    }
    return indexesFound;
  }

  /**
   * @brief Finds every occurrence of a character.
   *
   * FindAll searches trough the StaticString and returns all occurance of
   * searched character in the StaticString as the indexes of it.
   *
   * @param ch Character to be searched
   * @return std::vector<std::size_t> A vector of the indexes of the character
   * if found; if not found empty vector.
   */
  constexpr std::vector<std::size_t> findAll(char ch) const noexcept {
    std::vector<std::size_t> indexesFound = std::vector<size_t>{};

    for (std::size_t i = 0; i < mCurrentSize; ++i) {
      if (mString[i] == ch) {
        indexesFound.push_back(i);
      };
    }
    return indexesFound;
  }

  /**
   * @brief Finds the first occurrence of a character, string, string_view,
   *  char* in the string, in reverse order.
   *
   * @tparam T Templated types are string, string_view, char*
   * @param str Templated parameter to searched in the string
   * @return std::size_t The index of the character if found;
   * if not found StaticString::npos.
   */
  template <typename T, typename = std::enable_if_t<
                            std::is_convertible_v<T, std::string_view>>>
  constexpr std::size_t rFind(const T& str) const noexcept {
    // std::string, char*, char[N] are all convertible to string_view, thus
    // We convert templated input to convert into string_view to perform rfind
    std::string_view size(str);

    for (std::size_t i = mCurrentSize - size.length(); i > 0; --i) {
      std::size_t j = 0;
      for (; j < size.length(); ++j) {
        if (mString[i + j] != str[j]) break;
      }
      if (j == size.length()) return i;
    }
    return npos;
  }

  /**
   * @brief Finds the first occurrence of a character in the string, in reverse
   * order.
   *
   * @param ch Character to be searched
   * @return std::size_t index of the character found, StaticString::npos if
   * not found.
   */
  constexpr std::size_t rFind(char ch) const noexcept {
    for (std::size_t i = mCurrentSize; i > 0; --i) {
      if (mString[i - 1] == ch) return i - 1;
    }
    return npos;
  }

  /**
   * @brief Finds the first occurrence of a character, string, string_view,
   * char* in the string, in reverse search order.
   *
   * @param str Templated parameter to searched in the string
   * @return std::vector<std::size_t> A vector of the indexes of the character
   * if found; if not found empty vector.
   */
  template <typename T, typename = std::enable_if_t<
                            std::is_convertible_v<T, std::string_view>>>
  constexpr std::vector<std::size_t> rFindAll(const T& str) const noexcept {
    // std::string, char*, char[N] are all convertible to string_view, thus
    // We convert templated input to convert into string_view to perform find
    std::string_view size(str);

    std::vector<size_t> indexesFound = std::vector<size_t>{};

    for (std::size_t i = 0; i <= mCurrentSize - size.length(); ++i) {
      std::size_t j = 0;
      for (; j < size.length(); ++j) {
        if (mString[i + j] != str[j]) break;
      }
      if (j == size.length()) {
        indexesFound.insert(indexesFound.begin(), i);
      }
    }
    return indexesFound;
  }

  /**
   * @brief Finds the first occurrence of a character in the string,
   * in reverse search order.
   *
   * @param ch Character to be searched
   * @return std::vector<std::size_t> A vector of the indexes of the character
   * if found; if not found empty vector.
   */
  constexpr std::vector<std::size_t> rFindAll(char ch) const noexcept {
    std::vector<std::size_t> indexesFound = std::vector<size_t>{};

    for (std::size_t i = 0; i < mCurrentSize; ++i) {
      if (mString[i] == ch) {
        indexesFound.insert(indexesFound.begin(), i);
      };
    }
    return indexesFound;
  }

  /**
   * @brief Replaces all instances of the string given with the new string.
   *
   * @attention When replacing two strings, make sure not to exceed the static
   * limit defined.
   *
   * @tparam T1 Templated types are string, string_view, char*
   * @tparam T2 Templated types are string, string_view, char*
   * @param str1 String to be replaced.
   * @param str2 String to replace with.
   */
  template <
      typename T1, typename T2,
      typename = std::enable_if_t<std::is_convertible_v<T1, std::string_view> &&
                                  std::is_convertible_v<T2, std::string_view>>>
  void replace(const T1& str1, const T2& str2) {
    std::vector<size_t> index = findAll(str1);
    std::string_view size(str1);
    std::string_view size2(str2);

    // Since the lengths might be different we calculate difference to align
    // index
    int diff = static_cast<int>(size2.size()) - static_cast<int>(size.size());

    if (index.size() * size2.length() > MaxSize) {
      throw std::out_of_range("replace exceeds the size allowed");
    }

    for (int i = 0; i < (int)index.size(); i++) {
      remove(index[i] + (i * diff), index[i] + size.length() + (i * diff));
      insert(str2, index[i] + (i * diff));
    }
  }

  /**
   * @brief Replaces all instances of the character given with new character.
   *
   * @param chr Character to be replaced.
   * @param rplc Character to replace with.
   */
  void replace(char chr, char rplc) {
    for (size_t i = 0; i < mCurrentSize; ++i) {
      if (mString[i] == chr) {
        mString[i] = rplc;
      }
    }
  }

  /**
   * @brief Replaces all instances of the string given with the new character.
   *
   * @attention When replacing different type and size, make sure to not
   * exceed static limit defined in the StaticString.
   *
   * @tparam T Templated types are string, string_view, char*
   * @param str1 String to be replaced.
   * @param chr Character to be replace with.
   */
  template <typename T, typename = std::enable_if_t<
                            std::is_convertible_v<T, std::string_view>>>
  void replace(const T& str1, char chr) {
    std::vector<size_t> index = findAll(str1);
    std::string_view size(str1);

    // Since the lengths are different we calculate difference to align index
    int diff = 1 - static_cast<int>(size.length());

    for (std::size_t i = 0; i < index.size(); i++) {
      remove(index[i] + (i * diff), index[i] + size.length() + (i * diff));
      insert(chr, index[i] + (i * diff));
    }
  }

  /**
   * @brief Replaces all instances of the character given with the new string.
   *
   * @attention When replacing different type and size, make sure to not
   * exceed static limit defined in the StaticString.
   *
   * @tparam T Templated types are string, string_view, char*
   * @param chr Character to be replaced.
   * @param str String to be replaced.
   */
  template <typename T, typename = std::enable_if_t<
                            std::is_convertible_v<T, std::string_view>>>
  void replace(char chr, const T& str) {
    std::vector<size_t> index = findAll(chr);
    std::string_view size(str);

    if (index.size() * size.length() > MaxSize) {
      throw std::out_of_range("replace exceeds the size allowed");
    }

    // Since the lengths are different we calculate difference to align index
    int diff = static_cast<int>(size.length()) - 1;

    for (std::size_t i = 0; i < index.size(); i++) {
      remove(index[i] + (i * diff), index[i] + size_t(1) + (i * diff));
      insert(str, index[i] + (i * diff));
    }
  }

  /**
   * @brief Applies a given function to each character in the StaticString.
   *
   * This is a utility function and can be used to perform operations or
   * custom changes over the string using your own functions.
   * Foreach iterates over each charater and applies changes to each character
   * if advised.
   *
   * @tparam Func The function type to be applied to each character.
   * @param lambda A function or lambda that accepts a character.
   */
  template <typename Func>
  void foreach (Func lambda) {
    for (size_t i = 0; i < mCurrentSize; ++i) {
      lambda(mString[i]);
    }
  }

  /**
   * @brief Transforms the StaticString by applying a function to each character
   *
   * This is not an in place function, changes made on StaticString by the
   * function will be applied to a copy of the current object, and
   * new modified object will be returned.
   *
   * @tparam Func The function type to be applied to each character.
   * @param lambda A function or lambda that accepts a character.
   * @return StaticString A new transformed StaticString.
   */
  template <typename Func>
  [[nodiscard]] StaticString transform(Func lambda) const {
    StaticString result(*this);

    for (std::size_t i = 0; i < result.length(); ++i) {
      result.get_str()[i] = lambda(result.get_str()[i]);
    }
    return result;
  }

  /**
   * @brief Counts the occurrences of each character in the StaticString.
   *
   * @return std::unordered_map<char, std::size_t> A map of character to its
   * count.
   */
  constexpr std::unordered_map<char, std::size_t> char_count() const noexcept {
    std::unordered_map<char, std::size_t> counts;

    for (std::size_t i = 0; i < mCurrentSize; ++i) {
      ++counts[mString[i]];
    }
    return counts;
  }

  /**
   * @brief Splits the given StaticString based on delimiter defined.
   *
   * Splits the given StaticStrings into smaller staticString objects from
   * delimiter provided and returns a vector of new StaticString objects
   * splitted from delimiter.
   *
   * @param delimiter Character The delimiter to split StaticString.
   * @return std::vector<StaticString<MaxSize>> Collection of splitted
   * StaticString objects.
   */
  [[nodiscard]] constexpr std::vector<StaticString<MaxSize>> split(
      char delimiter) const noexcept {
    std::vector<StaticString<MaxSize>> result;
    StaticString<MaxSize> new_string;

    for (std::size_t i = 0; i < mCurrentSize; i++) {
      if (mString[i] == delimiter) {
        // Make sure to not add empty string, avoid begin with delimiter
        if (!new_string.empty()) {
          result.push_back(new_string);
        }
        new_string.clear();
      } else {
        new_string.append(mString[i]);
      }
    }

    result.push_back(new_string);
    // If the delimiter at the end results extra empty string in the vecto
    // Get rid of that
    if (!result.empty() && result.back().empty()) {
      result.pop_back();
    }
    return result;
  }

  /**
   * @brief Compares this StaticString to another using a custom comparator.
   *
   * Useful for lexicographical or conditional comparison beyond equality.
   * @attention Inorder to make comparison both StaticString objects must be
   * equal length for character wise comparison.
   *
   * If you decide to compare using both object not character wise, adjust
   * arguments of the lambda according to that. Make sure to use in caution.
   *
   * @note Comparing object wise can be used for pattern matching
   *
   * @tparam Func A comparator function taking two characters and returning a
   * bool.
   * @param rhs The StaticString to compare against.
   * @param lambda The comparator function.
   * @return true If all character comparisons return true.
   * @return false Otherwise.
   */
  template <typename Func>
  constexpr bool compare(const StaticString& rhs, Func lambda) const noexcept {
    // Branch based on Lambda parameters to compare character or object based.
    if constexpr (std::is_invocable_v<Func, char, char>) {
      if (mCurrentSize != rhs.mCurrentSize) return false;

      for (std::size_t i = 0; i < mCurrentSize; ++i) {
        if (!lambda(mString[i], rhs.mString[i])) {
          return false;
        }
      }
      return true;
    } else if constexpr (std::is_invocable_v<Func, StaticString,
                                             StaticString>) {
      return lambda(*this, rhs);
    } else {
      throw std::invalid_argument("Lambda provided is missing arguments.");
    }
  }

  /**
   * @brief Checks if the string is empty or not.
   *
   * @return true If the string is empty
   * @return false If the string is not empty
   */
  inline constexpr bool empty() const noexcept {
    if (mCurrentSize == 0) {
      return true;
    } else {
      return false;
    }
  }

  /**
   * @brief Swaps the values of a StaticString with other.
   *
   * Swap can be used to change values between two StaticString object, however,
   * they must align in terms of lengths. If the length of one exceeds the
   * static limit of other error is thrown.
   *
   * @tparam OtherSize Static limit of other object
   * @param other Other StaticString to be swapped.
   */
  template <std::size_t OtherSize>
  constexpr void swap(StaticString<OtherSize>& other) {
    if (mCurrentSize > other.size() || other.length() > MaxSize) {
      throw std::out_of_range(
          "Swap failed: Swap would exceed one of the StaticString's limit.");
    }
    StaticString<MaxSize> temp = *this;
    *this = other;
    other = temp;
  }

  /**
   * @brief Swaps the values of a StaticString with other conditionally.
   *
   * Swap can be used to change values between two StaticString object, however,
   * they must align in terms of lengths. If the length of one exceeds the
   * static limit of other error is thrown. Also, condition takes the two
   * objects iteselfs as the parameters.
   *
   * @tparam OtherSize Static limit of other object
   * @tparam Func Lamda function as the condition
   * @param other Other StaticString to be swapped.
   * @param lambda Conditional lambda.
   */
  template <std::size_t OtherSize, typename Func>
  void swap(StaticString<OtherSize>& other, Func lambda) {
    if (lambda(*this, other)) {
      if (mCurrentSize > other.size() || other.length() > MaxSize) {
        throw std::out_of_range(
            "Swap failed: one of the strings is too large.");
      }

      StaticString temp = *this;
      *this = other;
      other = temp;
    }
  }

  /**
   * @brief Erases all instances of the given string up to the specified amount.
   *
   * @tparam T Templated types are std::string, std::string_view, char*
   * @param str1 The string to erase.
   * @param amount How many instances of the strings should be erased
   * by default all instances.
   */
  template <typename T, typename = std::enable_if_t<
                            std::is_convertible_v<T, std::string_view>>>
  void erase(const T& str1, int amount = -1) {
    std::vector<size_t> index = findAll(str1);
    std::string_view size(str1);

    if (index.size() * size.length() > MaxSize) {
      throw std::out_of_range("erase exceeds the size allowed");
    }

    // If no default value given erase all instances.
    if (amount < 0) {
      amount = static_cast<int>(index.size());
    }

    int counter = 0;
    for (std::size_t i = 0; i < index.size(); i++) {
      if (counter <= amount) {
        // After remove indexes are shifted, to eliminate substract
        // amount removed and the length of removed string.
        remove(index[i] - (i * size.length()),
               index[i] + size.length() - (i * size.length()));
        counter++;
      }
    }
  }

  /**
   * @brief Erases all instances of the given string up to the specified amount.
   *
   * @note Amount starts from the 0, which means if 0 is given as the amount,
   * the first occurance will be erased.
   *
   * @param ch Character to be erased from the string.
   * @param amount How many instances of the character should be erased.
   */
  void erase(char ch, int amount = -1) {
    std::vector<size_t> index = findAll(ch);

    if (amount < 0) {
      amount = static_cast<int>(index.size());
    }

    int counter = 0;
    for (std::size_t i = 0; i < index.size(); i++) {
      if (counter <= amount) {
        // After remove indexes are shifted, to eliminate substract
        // amount removed and the length of removed string.
        remove(index[i] - (i * 1), index[i] + size_t(1) - (i * 1));
        counter++;
      }
    }
  }

  /**
   * @brief Removes the whitespace from the end of static string.
   *
   * Trim removes both leading and trailing whitespaces in the string.
   */
  void trim() {
    std::size_t start = 0;
    std::size_t end = mCurrentSize;

    // Trim leading whitespace
    while (start < mCurrentSize &&
           (mString[start] == ' ' || mString[start] == '\t' ||
            mString[start] == '\r')) {
      ++start;
    }

    // Trim trailing whitespace
    while (end > start &&
           (mString[end - 1] == ' ' || mString[end - 1] == '\t' ||
            mString[end - 1] == '\r')) {
      --end;
    }

    if (start > 0 || end < mCurrentSize) {
      // Remove the trailing or leading white spaces from the string
      std::memmove(mString, mString + start, end - start);
      mCurrentSize = end - start;
      mString[mCurrentSize] = null_terminator;

      // Clear the remaining part of the string, make sure properly handled
      for (std::size_t i = mCurrentSize + 1; i <= MaxSize; ++i) {
        mString[i] = null_terminator;
      }
    }
  }

  /**
   * @brief Clears the StaticString string.
   *
   * Clears the StaticString and assigns backto empty string. StaticSize
   * remains same.
   *
   */
  constexpr void clear() noexcept {
    std::fill(mString, mString + MaxSize, '\0');
    mCurrentSize = 0;
  }

  /**
   * @brief Replace the character if the condition is met.
   *
   * @tparam Func Templated Lambda function.
   * @param chr Character to be replaced.
   * @param rplc Character to replace with.
   * @param func Conditional lambda.
   */
  template <typename Func>
  void replace_if(const char& chr, const char& rplc, Func func) {
    for (std::size_t i = 0; i < mCurrentSize; ++i) {
      if (mString[i] == chr && func(i)) {
        mString[i] = rplc;
      }
    }
  }

 private:
  /// @brief Constant value for StaticString null terminator used.
  static constexpr char null_terminator = '\0';

  /// @brief Array of characters stores the string with a predefined size.
  char mString[MaxSize + 1] = {null_terminator};  // Array of characters '\0'

  /// @brief size_t The current length of the string.
  std::size_t mCurrentSize = 0;  // Tracks the current length of the string.
};

/**
 * @brief << output stream operator implementation
 *
 * @tparam MaxSize Static limit size
 * @param oss Ostream to ouput
 * @param str StaticString object to get output
 * @return std::ostream& ostream Output string collected
 */
template <std::size_t MaxSize>
std::ostream& operator<<(std::ostream& oss, const StaticString<MaxSize>& str) {
  oss << str.get_str();
  return oss;
}

/**
 * @brief >> input stream operator implementation
 *
 * @tparam MaxSize Static limit size
 * @param iss Istream to get input
 * @param str StaticString object to assign input
 * @return std::istream& istream input collected
 */
template <std::size_t MaxSize>
std::istream& operator>>(std::istream& iss, StaticString<MaxSize>& str) {
  char buffer[MaxSize + 1];
  // Read input until MaxSize reached
  iss >> std::setw(MaxSize + 1) >> buffer;
  // Assign the temporary array to object
  str = buffer;
  return iss;
}

}  // namespace cse

namespace std {

/**
 * @brief This is a templated hasher class used to customized hash for
 * StaticString class to be used.
 *
 * @tparam MaxSize Limit defined for the StaticString
 */
template <std::size_t MaxSize>
struct hash<cse::StaticString<MaxSize>> {
  /**
   * @brief Hash function operator for StaticString.
   *
   * Allows StaticString to be used in hashed containers like
   * std::unordered_map.
   *
   * @param obj The StaticString object to hash.
   * @return std::size_t The hash value.
   */
  std::size_t operator()(const cse::StaticString<MaxSize>& obj) const noexcept {
    return std::hash<std::string_view>{}(obj.view());
  }
};

}  // namespace std

#endif  // CSE_STATIC_STRING_HPP_
