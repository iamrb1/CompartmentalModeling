/**
 * @file StateGridPosition.cpp
 * @author Bram Hogg
 */

#include <cse/StateGridPosition.h>
#include <cassert>
#include <cmath>

namespace cse {

const int MAX_ROTATION = 360;

[[maybe_unused]] Point StateGridPosition::calculate_object_position() {
  ///Calculate the position of the object and return the point
  Point default_object_position;

  default_object_position.x_position = 0;
  default_object_position.y_position = 0;

  assert(m_state_grid == nullptr && "StateGrid is nullptr by default");

  return default_object_position;
}

///Get the orientation of the object, 0-359 degrees
[[maybe_unused]] double StateGridPosition::get_object_orientation() {
    m_object_orientation = std::fmod(m_object_orientation, MAX_ROTATION);
    if (m_object_orientation < 0) {
        m_object_orientation += MAX_ROTATION;
    }
  assert(m_object_orientation >= 0 && "orientation isn't negative");
  return m_object_orientation;
}

StateGridPosition::StateGridPosition() {
  Point p;
  p.x_position = 0;
  p.y_position = 0;
  m_central_position = p;
  m_object_orientation = 0;
  m_state_grid = nullptr;

  m_object_position = p;
  assert(m_object_position.x_position == 0 && m_object_position.y_position == 0 && "default object position is (0, 0)");
}

void StateGridPosition::set_object_position(std::pair<int, int> agent) {
    Point p;
    p.x_position = static_cast<double>(agent.first);
    p.y_position = static_cast<double>(agent.second);
    m_object_position = p;
}

void StateGridPosition::set_object_position(double x, double y) {
    Point p;
    p.x_position = x;
    p.y_position = y;
    m_object_position = p;
}
std::string StateGridPosition::compare_direction(const Point newmove) const {
    double row_diff = newmove.x_position - m_object_position.x_position;
    double col_diff = newmove.y_position - m_object_position.y_position;

    assert((row_diff == 0 || col_diff == 0) && "This move is not valid");

    if (row_diff == 0)
    {
        if (col_diff == -1) return "Left";
        if (col_diff == 1) return "Right";
    }
    else if (col_diff == 0)
    {
        if (row_diff == -1) return "Up";
        if (row_diff == 1) return "Down";
    }

    return "Invalid Move";  ///<< This should never be returned, assert statement covers all bases

}

//Point StateGridPosition::create_point(double x, double y) {
//    Point p;
//    p.x_position = x;
//    p.y_position = y;
//    return p;
//}

}  // namespace cse