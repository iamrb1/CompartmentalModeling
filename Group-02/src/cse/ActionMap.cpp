/**
 * @file ActionMap.cpp
 * @author Nitish Maindoliya
 */

#include <cse/ActionMap.h>

namespace cse {
/**
 * @brief Insert a new action into the map.
 * @param key The key to insert the action with.
 * @param value The action to insert.
 */
void ActionMap::insert(const std::string& key, const std::function<void()>& value) {
  this->emplace(key, value);
}

/**
 * @brief Trigger an action by key.
 * @param key The key of the action to trigger.
 */
void ActionMap::trigger(const std::string& key) const {
  if (this->contains(key)) {
    this->at(key)();
  }
}

/**
 * @brief Remove an action by key.
 * @param key The key of the action to remove.
 */
void ActionMap::remove(const std::string& key) {
  if (this->contains(key)) {
    this->erase(key);
  }
}
}  // namespace cse
