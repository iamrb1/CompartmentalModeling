#include "../../third-party/Catch/single_include/catch2/catch.hpp"

#include "../../Group-03/src/Sector.h"
#include "../../Group-03/src/Surface.h"
#include "../../Group-03/src/Circle.h"

using cse::Sector;
using cse::Surface;
using cse::Circle;
using cse::CircleType;
using std::make_shared;

TEST_CASE("SectorTest 1: Add", "[Sector]") {
    Sector sector(0, 0);
    auto circle = make_shared<Circle>(
        0.0, 0.0,   
        1.0,        
        1.0,        
        CircleType::Prey
    );
    sector.add_circle(circle);
    CHECK(sector.circles.size() == 1);
}

TEST_CASE("SectorTest 2: Remove", "[Sector]") {
    Sector sector(0, 0);
    auto circle = make_shared<Circle>(0.0, 0.0, 1.0, 1.0, CircleType::Prey);
    sector.add_circle(circle);
    sector.remove_circle(circle);
    CHECK(sector.circles.empty());
}

TEST_CASE("SurfaceTest 1: AddCircle", "[Surface]") {
    Surface surface(100, 100, 10);
    auto circle = make_shared<Circle>(0.0, 0.0, 1.0, 1.0, CircleType::Predator);
    surface.add_circle(circle);
    CHECK(circle->getX() == Approx(0.0));
    CHECK(circle->getY() == Approx(0.0));
}

TEST_CASE("SurfaceTest 2: MoveCircle", "[Surface]") {
    Surface surface(100, 100, 10);
    auto circle = make_shared<Circle>(0.0, 0.0, 1.0, 1.0, CircleType::Predator);
    surface.add_circle(circle);
    surface.move_circle(circle, 15.0, 15.0);
    CHECK(circle->getX() == Approx(15.0));
    CHECK(circle->getY() == Approx(15.0));
}

TEST_CASE("SurfaceTest 3: Move un-added circle", "[Surface]") {
    Surface surface(100, 100, 10);
    auto circle = make_shared<Circle>(5.0, 5.0, 1.0, 1.0, CircleType::Prey);

    REQUIRE_NOTHROW(surface.move_circle(circle, 20.0, 30.0));
    CHECK(circle->getX() == Approx(20.0));
    CHECK(circle->getY() == Approx(30.0));
}
