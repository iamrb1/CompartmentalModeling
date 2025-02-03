#include <utility>

/**
 * @file Event.h
 *
 * @author Owen Haiar
 */

#ifndef CSE498_SPRING2025_GROUP_09_EVENT_EVENT_H_
#define CSE498_SPRING2025_GROUP_09_EVENT_EVENT_H_

struct Event {
 private:
  int id;
  int time;
  std::string data;
 public:
  Event(int id, int time, std::string data) : id(id), time(time), data(std::move(data)) {}
  int getID() const { return id; }
  int getTime() const { return time; }
  std::string getData() const { return data; }
  bool operator==(const Event& other) const { return id == other.id; }
  bool operator!=(const Event& other) const { return id != other.id; }
};

#endif //CSE498_SPRING2025_GROUP_09_EVENT_EVENT_H_
