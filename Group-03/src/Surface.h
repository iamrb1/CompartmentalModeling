// Surface.h
#pragma once

#include <vector>
#include <memory>
#include <string>
#include <utility>
#include "Circle.h"
#include "Sector.h"

namespace cse {

class Surface {
public:
    Surface(int width, int height, int sector_size);

    // add and move
    void add_circle(std::shared_ptr<Circle> circle);
    void move_circle(std::shared_ptr<Circle> circle, double x_pos, double y_pos);

    // update grid buckets
    void update();

    // returns: { "add" | "delete" | "nothing", circlePtr }
    std::pair<std::string, std::shared_ptr<Circle>> check_collision();

    // remove a circle from whatever sector it currently sits in
    void remove_circle(std::shared_ptr<Circle> circle);

private:
    int surface_width;
    int surface_height;
    int surface_size;
    int sector_width;
    int sector_height;

    // 2D grid of sectors
    std::vector<std::vector<Sector>> sectors;
};

} // namespace cse