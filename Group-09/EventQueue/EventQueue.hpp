/**
 * @file EventQueue.h
 *
 * @author Owen Haiar
 */

#pragma once

#include <optional>
#include <queue>
#include <unordered_set>
#include <iostream>
#include <format>
#include <cassert>
#include "../Event/Event.hpp"

namespace cse {

template<typename... Args>
/**
 * @brief Compares two events by time for use in priority queue
 */
struct EventCompare {
  bool operator()(const Event<Args...> &a, const Event<Args...> &b) const {
	return a.getTime() > b.getTime();
  }
};

/**
 * @brief A min-heap queue for storing Events.
 */
template<typename... Args>
class EventQueue {
 public:
  EventQueue() = default;
  ~EventQueue() = default;

  /**
   * @brief Adds an event to the EventQueue
   * @param e The event to add
   */
  void add(const Event<Args...> &e) {
	if (!ids_.contains(e.getID())) {
	  heap_.push(e);
	  ids_.insert(e.getID());
	  eventCount_++;
	} else {
	  auto msg = std::format("Error adding event: Event ID {} already exists in EventQueue", e.getID());
	  throw std::invalid_argument(msg);
	}
  }

  /**
   * @brief Removes an event from the EventQueue
   * @param e The event to remove
   * @return The removed event, if it was found
   */
  std::optional<Event<Args...>> remove(const Event<Args...> &e) {
	std::priority_queue<Event<Args...>, std::vector<Event<Args...>>, EventCompare<Args...>> temp;
	std::optional<Event<Args...>> removed;

	// Move all elements except the one to be removed to temp
	while (!heap_.empty()) {
	  if (heap_.top() != e) temp.push(heap_.top());
	  else {
		removed = heap_.top();
		ids_.erase(e.getID());
		eventCount_--;
	  }
	  heap_.pop();
	}

	// Reassign
	heap_ = std::move(temp);

	assert(!removed.has_value() || removed.value().getID() == e.getID());
	return removed;
  }

  /**
   * @brief Pops the event at the top of the EventQueue
   * @return The event at the top of the EventQueue
   */
  Event<Args...> pop() {
	assert(eventCount_ > 0); // Ensure there is an event to pop (heap is not empty)
	auto e = heap_.top();
	heap_.pop();
	eventCount_--;
	ids_.erase(e.getID());
	return e;
  }

  /**
   * @brief Returns the event at the top of the EventQueue, without removing
   * @return The event at the top of the EventQueue
   */
  Event<Args...> peek() const {
	assert(eventCount_ > 0); // Ensure there is an event to pop (heap is not empty)
	return heap_.top();
  }

  /**
   * @brief Updates an event in the EventQueue
   * @details The updated event must have the same ID as the event currently in the queue
   * @param e The event to update
   * @throws std::invalid_argument If an event with a matching ID is not found
   */
  void update(const Event<Args...> &e) {
	auto removed = this->remove(e);
	if (removed.has_value()) {
	  assert(removed.value().getID() == e.getID()); // Reinsert same ID with updated time
	  this->add(e);
	} else {
	  auto msg = std::format("Error updating event: Event ID {} not found in EventQueue", e.getID());
	  throw std::invalid_argument(msg);
	}
  }

  /**
   * @brief Prints the contents of the EventQueue to stdout
   */
  void print() const {
	std::priority_queue<Event<Args...>, std::vector<Event<Args...>>, EventCompare<Args...>> temp = heap_;

	for (size_t i = 0; !temp.empty(); i++) {
	  auto e = temp.top();
	  std::cout << "#" << i << ":"
				<< " ID: " << e.getID()
				<< " Time: " << e.getTime()
				<< std::endl;
	  temp.pop();
	}
  }

  [[nodiscard]] size_t size() const { return eventCount_; }

 private:
  std::priority_queue<Event<Args...>, std::vector<Event<Args...>>, EventCompare<Args...>> heap_; // Min heap
  std::unordered_set<int> ids_; // Set of all ID currently in use
  size_t eventCount_ = 0; // Number of events currently in the queue
};

}

