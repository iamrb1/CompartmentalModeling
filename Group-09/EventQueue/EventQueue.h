/**
 * @file EventQueue.h
 *
 * @author Owen Haiar
 */

#ifndef CSE498_SPRING2025_GROUP_09_EVENTQUEUE_EVENTQUEUE_H_
#define CSE498_SPRING2025_GROUP_09_EVENTQUEUE_EVENTQUEUE_H_

#include <optional>
#include <queue>
#include "Event/Event.h"

struct EventCompare {
  bool operator()(const Event &a, const Event &b) {
	return a.getTime() < b.getTime();
  }
};

class EventQueue {
 public:
  EventQueue() = default;
  ~EventQueue() = default;
  void add(const Event&);
  std::optional<Event> remove(const Event&);
  Event pop();
  Event peek();
  void print();
 private:
  std::priority_queue<Event, std::vector<Event>, EventCompare> heap;
  int eventCount = 0;
};

#endif //CSE498_SPRING2025_GROUP_09_EVENTQUEUE_EVENTQUEUE_H_
