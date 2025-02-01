/**
 * @file ActionMap.h
 * @author Nitish Maindoliya
 */

#pragma once

#include <functional>
#include <string>
#include <unordered_map>

namespace cse {
class ActionMap : public std::unordered_map<std::string, std::function<void()>> {
 public:
  void insert(const std::string& key, const std::function<void()>& value);

  void trigger(const std::string& key) const;

  void remove(const std::string& key);
};
}  // namespace cse
