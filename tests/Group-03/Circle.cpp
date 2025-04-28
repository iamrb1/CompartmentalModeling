#define CATCH_CONFIG_MAIN
#include "../../third-party/Catch/single_include/catch2/catch.hpp"

#include "../../Group-03/src/Circle.h"
#include "../../Group-03/src/Surface.h"

using cse::Circle;
using cse::CircleType;

constexpr double kRadius    = 4.0;
constexpr double kBaseSpeed = 2.0;
constexpr int    kSurfW     = 100;
constexpr int    kSurfH     = 100;
constexpr int    kSectorSz  = 10;

TEST_CASE("Circle Initialization", "[Circle]") {
    Circle circle(5.0, 10.0, 3.0, 2.0, CircleType::Predator);
    CHECK(circle.getX()      == Approx(5.0));
    CHECK(circle.getY()      == Approx(10.0));
    CHECK(circle.getRadius() == Approx(3.0));

    CHECK_THROWS_AS(circle.setRadius(0), std::invalid_argument);
}

TEST_CASE("Circle gets a random direction", "[Circle]") {

    Circle c(0, 0, 3.0, 2.0, CircleType::Prey);

    double x0 = c.getX(),
           y0 = c.getY();
    c.move(100.0, 100.0);
    CHECK( !(c.getX() == Approx(x0) && c.getY() == Approx(y0)) );
}

TEST_CASE("canRepopulate logic", "[Circle]") {
    Circle predator(0, 0, 3.0, 2.0, CircleType::Predator);
    CHECK_FALSE(predator.canRepopulate());

    for (int i = 0; i < Circle::kDefaultReproduceThreshold; ++i) {
        predator.eatPrey(predator); 
    }
    CHECK_FALSE(predator.canRepopulate());
}

TEST_CASE("Invalid Circle construction", "[Circle]") {
    CHECK_THROWS_AS(Circle(0, 0, -1.0, 2.0, CircleType::Prey), std::invalid_argument);
    CHECK_THROWS_AS(Circle(0, 0,  0.0, 2.0, CircleType::Prey), std::invalid_argument);

    CHECK_NOTHROW(Circle(-10.0, -20.0, 5.0, 2.0, CircleType::Prey));
}

TEST_CASE("Surface add & move circle", "[Surface]") {
    cse::Surface surf(100, 100, 10);
    auto circle = std::make_shared<Circle>(15.0, 25.0, 5.0, 2.0, CircleType::Prey);

    surf.add_circle(circle);
    CHECK(circle->getX() == Approx(15.0));
    CHECK(circle->getY() == Approx(25.0));

    surf.move_circle(circle, 35.0, 45.0);
    CHECK(circle->getX() == Approx(35.0));
    CHECK(circle->getY() == Approx(45.0));
}



TEST_CASE("Circle: basic construction & accessors", "[Circle]") {
    Circle c(10.0, 15.0, kRadius, kBaseSpeed, CircleType::Predator);
    CHECK(c.getX()      == Approx(10.0));
    CHECK(c.getY()      == Approx(15.0));
    CHECK(c.getRadius() == Approx(kRadius));
    CHECK(c.getSpeed()  == Approx(kBaseSpeed));
    CHECK(c.getType()   == CircleType::Predator);
    CHECK_FALSE(c.isRegenerating());
    CHECK_FALSE(c.hasSpeedBoost());
}

TEST_CASE("Circle: setType & setPosition", "[Circle]") {
    Circle c(0, 0, 1.0, 1.0, CircleType::Prey);
    c.setType(CircleType::Predator);
    CHECK(c.getType() == CircleType::Predator);

    c.setPosition(5.5, 6.5);
    CHECK(c.getX() == Approx(5.5));
    CHECK(c.getY() == Approx(6.5));
}

TEST_CASE("Circle: reject non-positive radius", "[Circle]") {
    CHECK_THROWS_AS(
        Circle(0, 0, 0.0, 1.0, CircleType::Prey),
        std::invalid_argument
    );
    CHECK_THROWS_AS(
        Circle(0, 0, -1.0, 1.0, CircleType::Prey),
        std::invalid_argument
    );

    Circle c(0, 0, 1.0, 1.0, CircleType::Prey);
    CHECK_THROWS_AS(c.setRadius(0.0), std::invalid_argument);
}

TEST_CASE("Circle: energy lifecycle updates speed", "[Circle][Energy]") {
    Circle c(0, 0, 3.0, 2.0, CircleType::Predator);
    int fullEnergy = Circle::kDefaultInitialEnergy;

    REQUIRE(c.getEnergy() == fullEnergy);

    c.decreaseEnergy(static_cast<int>(fullEnergy * 0.4));
    CHECK(c.getEnergy() == Approx(fullEnergy * 0.6));
    CHECK(c.getSpeed() < kBaseSpeed);

    c.decreaseEnergy(fullEnergy);
    CHECK(c.getEnergy() == 0);
    CHECK(c.isRegenerating());
    CHECK(c.getSpeed() == Approx(0.0));

    c.regenerateEnergy(fullEnergy / 2);
    CHECK(c.getEnergy() == fullEnergy / 2);
    CHECK(c.isRegenerating());
    CHECK(c.getSpeed() >= 0.0);

    double prevSpeed = c.getSpeed();
    c.decreaseEnergy(10);
    CHECK(c.getSpeed() <= prevSpeed);
}

TEST_CASE("Circle: move & bounce inside bounds", "[Circle][Move]") {
    Circle c(1.0, 1.0, 3.0, 2.0, CircleType::Prey);

    c.move(kSurfW, kSurfH);
    CHECK(c.getX() <= c.getRadius());
    CHECK(c.getY() <= c.getRadius());

    c.setPosition(kSurfW - c.getRadius() - 0.1, c.getY());
    c.move(kSurfW, kSurfH);
    CHECK(c.getX() <= kSurfW - c.getRadius());
}

TEST_CASE("Circle: overlap & proximity", "[Circle][Proximity]") {
    Circle predator(0, 0, 5.0, 2.0, CircleType::Predator);
    Circle prey(8.0, 0, 3.0, 2.0, CircleType::Prey);

    CHECK_FALSE(predator.overlaps(prey));

    prey.checkProximity(predator);
    CHECK_FALSE(prey.hasSpeedBoost());

    double x2 = predator.getRadius() + prey.getRadius() - 0.1;
    prey.setPosition(x2, 0);
    prey.checkProximity(predator);
    CHECK(prey.hasSpeedBoost());
}

TEST_CASE("Circle: predator reproduction threshold", "[Circle][Predator]") {
    Circle predator(0, 0, 5.0, 2.0, CircleType::Predator);
    Circle prey(1.0, 1.0, 1.0, 1.0, CircleType::Prey);

    REQUIRE_FALSE(predator.canRepopulate());
    for (int i = 0; i < Circle::kDefaultReproduceThreshold; ++i) {
        predator.eatPrey(prey);
    }
    CHECK_FALSE(predator.canRepopulate());
}

TEST_CASE("Sector: add & remove circle bookkeeping", "[Sector]") {
    cse::Sector s(0, 0);
    auto c1 = std::make_shared<Circle>(5.0, 5.0, 3.0, 2.0, CircleType::Prey);

    s.add_circle(c1);
    CHECK(s.circles.size() == 1);
    CHECK(s.circles[0] == c1);

    s.remove_circle(c1);
    CHECK(s.circles.empty());
}

TEST_CASE("Surface: store, move & report collisions", "[Surface]") {
    cse::Surface surf(kSurfW, kSurfH, kSectorSz);

    auto c1 = std::make_shared<Circle>(12.0, 18.0, 3.0, 2.0, CircleType::Prey);
    surf.add_circle(c1);
    surf.move_circle(c1, 32.0, 42.0);
    CHECK(c1->getX() == Approx(32.0));
    CHECK(c1->getY() == Approx(42.0));

    auto c2 = std::make_shared<Circle>(35.0, 42.0, 3.0, 2.0, CircleType::Prey);
    surf.add_circle(c2);
    auto res1 = surf.check_collision();
    CHECK(res1.first == "add");

    surf.remove_circle(c2);
    auto prey2 = std::make_shared<Circle>(12.0, 10.0, 3.0, 2.0, CircleType::Prey);
    surf.move_circle(c1, 10.0, 10.0);
    surf.add_circle(prey2);
    auto res2 = surf.check_collision();
    CHECK(res2.first  == "add");

}

TEST_CASE("Surface: edge coordinates land in correct sector", "[Surface][Edge]") {
    cse::Surface surf(kSurfW, kSurfH, kSectorSz);
    auto c = std::make_shared<Circle>(20.0, 19.9, 3.0, 2.0, CircleType::Predator);
    surf.add_circle(c);
    surf.move_circle(c, c->getRadius(), 50.0);
    CHECK(c->getX() >= c->getRadius());
}

TEST_CASE("Surface: extreme bottom-right corner safe", "[Surface][Edge]") {
    cse::Surface surf(kSurfW, kSurfH, kSectorSz);
    auto c = std::make_shared<Circle>(99.0, 99.0, 1.0, 2.0, CircleType::Prey);
    REQUIRE_NOTHROW(surf.add_circle(c));
}

TEST_CASE("Surface: two distinct pointers same coords", "[Surface][Edge]") {
    cse::Surface surf(kSurfW, kSurfH, kSectorSz);
    auto c1 = std::make_shared<Circle>(30.0, 30.0, 3.0, 2.0, CircleType::Prey);
    auto c2 = std::make_shared<Circle>(30.0, 30.0, 3.0, 2.0, CircleType::Prey);
    surf.add_circle(c1);
    surf.add_circle(c2);
    auto r = surf.check_collision();
    CHECK(r.first == "add");
}

TEST_CASE("Circle: energy never below zero", "[Circle][Edge]") {
    Circle c(0,0,3.0,2.0,CircleType::Predator);
    c.decreaseEnergy(Circle::kDefaultInitialEnergy * 2);
    CHECK(c.getEnergy() == 0);
}

TEST_CASE("Circle: no speed boost exactly at proximity radius", "[Circle][Edge]") {
    Circle a(0,0,3.0,2.0,CircleType::Prey);
    Circle b(a.getRadius()*2, 0.0, 3.0, 2.0, CircleType::Predator);
    a.checkProximity(b);
    CHECK_FALSE(a.hasSpeedBoost());
}
