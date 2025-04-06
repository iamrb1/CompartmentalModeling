#pragma once

#include <vector>
#include <memory> 
#include "Circle.h"

namespace cse{
class Sector {
    public:
        int x, y;                       // Sector coordinates
        std::vector<std::shared_ptr<Circle>> circles;   // Circles in the sector

        Sector(int x, int y);
        void add_circle(std::shared_ptr<Circle> circle);
        void remove_circle(std::shared_ptr<Circle> circle);

        // New accessor methods
        int get_circle_count() const;
        std::shared_ptr<Circle> get_circle_at(size_t index) const;
};
}