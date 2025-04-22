#include "../../third-party/Catch/single_include/catch2/catch.hpp"

#include <vector>
#include "../../Group-03/src/Sector.h"
#include "../../Group-03/src/Surface.h"
#include "../../Group-03/src/Circle.h"
#define CATCH_CONFIG_MAIN
namespace cse {
    TEST_CASE("SectorTest 1", "Add") {
        Sector sector(0, 0);
        auto circle = std::make_shared<Circle>(0, 0, 1, 1, 1, "red");
        sector.add_circle(circle);
        CHECK(sector.circles.size() == 1);
    }

    TEST_CASE("SectorTest 2", "Remove") {
        Sector sector(0, 0);
        auto circle = std::make_shared<Circle>(0, 0, 1, 1, 1, "red");
        sector.add_circle(circle);
        sector.remove_circle(circle);
        CHECK(sector.circles.size() == 0);
    }

    TEST_CASE("SurfaceTest 1", "Add") {
        cse::Surface surface(100, 100, 10);
        auto circle = std::make_shared<Circle>(0, 0, 1, 1, 1, "red");
        surface.add_circle(circle);
        CHECK(circle->getX() == 0.0);
        CHECK(circle->getY() == 0.0);
    }

    TEST_CASE("SurfaceTest 2", "Move") {
        cse::Surface surface(100, 100, 10);
        auto circle = std::make_shared<Circle>(0, 0, 1, 1, 1, "red");
        surface.add_circle(circle);
        surface.move_circle(circle, 15, 15);
        CHECK(circle->getX() == 15.0);
        CHECK(circle->getY() == 15.0);
    }

}