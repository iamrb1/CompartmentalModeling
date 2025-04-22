/**
 * @author Hannah Dale
 * @file DynamicString.h
 * @brief Declaration of the DynamicString class.
 */
#pragma once

#include <functional>
#include <string>
#include <vector>
#include <variant>

/**
 * @class DynamicString
 * @brief A class that holds both static and dynamic string segments, constructing
 *        a full string on demand by concatenating all segments.
 */
namespace cse {

class DynamicString {
public:
    /// Alias to represent a dynamic text segment
    using str_fun_t = std::function<std::string()>;

    /// Constructors
    DynamicString();                                // Default
    DynamicString(const std::string &staticStr);    // Construct with one static segment
    DynamicString(str_fun_t func);                  // Construct with one dynamic segment

    /// Builds and returns the combined string
    std::string ToString() const;

    /// Append overloads
    DynamicString& Append(const std::string &staticStr);
    DynamicString& Append(str_fun_t func);

    /// Operator+ overloads
    friend DynamicString operator+(const DynamicString &lhs, const std::string &rhs);
    friend DynamicString operator+(const DynamicString &lhs, str_fun_t rhs);

    void Clear();

private:
    /// The container that holds both string and function segments
    std::vector<std::variant<std::string, str_fun_t>> mSegments;
};


/************************************
 * Inline Method Implementations
 *  
 * We mark these methods inline so that, when this non‑template
 * class is defined in a header and included by multiple TUs,
 * the linker will merge duplicate definit
 ************************************/
 inline DynamicString::DynamicString()
 : mSegments() { }

inline DynamicString::DynamicString(const std::string &s) {
 mSegments.emplace_back(s);
}

inline DynamicString::DynamicString(str_fun_t f) {
 mSegments.emplace_back(f);
}

inline std::string DynamicString::ToString() const {
 std::string result;
 for (auto &segment : mSegments) {
     std::visit([&result](auto &segValue) {
         using segType = std::decay_t<decltype(segValue)>;
         if constexpr (std::is_same_v<segType, std::string>) {
             result += segValue;
         } else {
             result += segValue();
         }
     }, segment);
 }
 return result;
}

inline DynamicString &DynamicString::Append(const std::string &staticStr) {
 mSegments.emplace_back(staticStr);
 return *this;
}

inline DynamicString &DynamicString::Append(str_fun_t func) {
 mSegments.emplace_back(func);
 return *this;
}

inline DynamicString operator+(const DynamicString &lhs, const std::string &rhs) {
 DynamicString result = lhs;
 result.Append(rhs);
 return result;
}

inline DynamicString operator+(const DynamicString &lhs, DynamicString::str_fun_t rhs) {
 DynamicString result = lhs;
 result.Append(rhs);
 return result;
}

inline void DynamicString::Clear() {
    mSegments.clear();
}

} // namespace cse