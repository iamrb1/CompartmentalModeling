/**
 * @file StateGridPosition.h
 * @author Bram Hogg
 *
 *
 */

#pragma once


#include <optional>
#include <string>


class StateGrid;

namespace cse {
///Point object for the position
struct Point {
  ///The position of the object
  double x_position = 0;
  double y_position = 0;

  bool operator==(const Point& other) const {
    return x_position == other.x_position && y_position == other.y_position;
  }
};

class StateGridPosition {

 private:
  ///The orientation in degrees
  double m_object_orientation;

  Point m_object_position;

  Point m_central_position;

  ///The state grid this position is referring to
  std::optional<StateGrid*> m_state_grid = nullptr;

 public:
  StateGridPosition();

//  static Point create_point(double x, double y);

  ///Get the stategrid this position is associated with
  std::optional<StateGrid*> get_state_grid() { return m_state_grid; }

  ///Possibly getState, but can be done in only necessary places as well

  ///Get central position of the stategrid
  Point get_grid_position() { return m_central_position; }

  ///Get the position of the object this stategrid is associated with
  Point get_object_position() { return m_object_position; }

  ///Set position of object this stategrid is associated with
  void set_object_position(double x, double y);

  ///Overloaded in case we want to use pairs or individual points
    void set_object_position(std::pair<int, int> agent);

  double get_object_orientation();

  std::string compare_direction(Point newmove);

  ///Set the object's orientation, 0-359 degrees
  void set_object_orientation(double orientation) { m_object_orientation = orientation; }

  ///Calculates the object's position, still working on specs
  [[nodiscard]] Point calculate_object_position();
};
}  // namespace cse


