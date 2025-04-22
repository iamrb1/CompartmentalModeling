#define CATCH_CONFIG_MAIN
#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Group-03/src/Circle.h"
#include "../../Group-03/src/Surface.h"

TEST_CASE("CircleTest1", "CircleInitialization") {
    Circle circle(5.0, 10.0, 3.0, 2, 2, "red");
    CHECK(circle.getX() == 5.0);
    CHECK(circle.getY() == 10.0);
    CHECK(circle.getRadius() == 3.0);
    CHECK_THROWS_AS(circle.setRadius(0), std::invalid_argument);

    Circle c(0,0,3,2,2,"red");
    double mag = std::sqrt(c.dx_ * c.dx_ + c.dy_ * c.dy_);
    CHECK(mag > 0);

    Circle blue(0,0,3,2,2,"blue");
    CHECK(blue.canRepopulate());
}

TEST_CASE("CircleTest2", "CircleInvalidRadiusAndPositions") {
    CHECK_THROWS_AS(Circle(0.0, 0.0, -1.0, 2, 2, "red"), std::invalid_argument);
    CHECK_THROWS_AS(Circle(0.0, 0.0, 0.0, 2, 2, "red"), std::invalid_argument);
    CHECK_NOTHROW(Circle(-10.0, -20.0, 5.0, 2, 2, "blue"));
}

TEST_CASE("SurfaceTest1", "AddCircleToSurface") {
    cse::Surface surface(100, 100, 10);
    auto circle = std::make_shared<Circle>(15.0, 25.0, 5.0, 2, 2, "red");

    surface.add_circle(circle);

    CHECK(circle->getX() == 15.0);
    CHECK(circle->getY() == 25.0);
    CHECK(circle->getRadius() == 5.0);
}

TEST_CASE("SurfaceTest2", "MoveCircleOnSurface") {
    cse::Surface surface(100, 100, 10);
    auto circle = std::make_shared<Circle>(15.0, 25.0, 5.0, 2, 2, "red");

    surface.add_circle(circle);
    surface.move_circle(circle, 35.0, 45.0);

    CHECK(circle->getX() == 35.0);
    CHECK(circle->getY() == 45.0);
}


