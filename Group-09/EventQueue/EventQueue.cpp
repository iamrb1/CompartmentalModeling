/**
 * @file EventQueue.cpp
 *
 * @author Owen Haiar
 *
 */

#include <iostream>
#include "EventQueue.h"

void EventQueue::add(const Event &e) {
  heap.push(e);
  eventCount++;
}

std::optional<Event> EventQueue::remove(const Event &e) {
  std::priority_queue<Event, std::vector<Event>, EventCompare> temp;
  std::optional<Event> removed;

  // Move all elements except the one to be removed to temp
  while (!heap.empty()) {
	if (heap.top() != e) temp.push(heap.top());
	else {
	  removed = heap.top();
	  eventCount--;
	}
	heap.pop();
  }

  // Reassign
  heap = std::move(temp);

  return removed;
}

Event EventQueue::pop() {
  auto e = heap.top();
  heap.pop();
  eventCount--;
  return e;
}

Event EventQueue::peek() {
  return heap.top();
}

void EventQueue::print() {
  std::priority_queue<Event, std::vector<Event>, EventCompare> temp = heap;

  for (size_t i = 0; !temp.empty(); i++) {
	auto e = temp.top();
	std::cout << "#" << i << ":"
	          << " ID: " << e.getID()
			  << " Time: " << e.getTime()
			  << " Data: " << e.getData()
			  << std::endl;
	temp.pop();
  }
}


