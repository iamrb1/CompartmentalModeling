#include "../../third-party/Catch/single_include/catch2/catch.hpp"
#include "../../Group-03/src/Circle.h"
#include "../../Group-03/src/Surface.h"
#define CATCH_CONFIG_MAIN

using std::make_shared;
using std::shared_ptr;

namespace {

constexpr double kRadius     = 4.0;
constexpr double kBaseSpeed  = 2.0;
constexpr double kInitSpeed  = 2.0;

constexpr int    kSurfaceW   = 100;
constexpr int    kSurfaceH   = 100;
constexpr int    kSectorSize = 10;

}

TEST_CASE("Circle basic construction and mutators", "[Circle]") {

    Circle c(10.0, 15.0, kRadius, kBaseSpeed, kInitSpeed, "red");

    CHECK(c.getX()          == 10.0);
    CHECK(c.getY()          == 15.0);
    CHECK(c.getRadius()     == kRadius);
    CHECK(c.getBaseSpeed()  == kBaseSpeed);
    CHECK(c.getSpeed()      == kInitSpeed);
    CHECK(c.getCircleType() == "red");
    CHECK_FALSE(c.getRegen());
    CHECK_FALSE(c.getSpeedBoost());


    c.setSpeed(3);
    c.setCircleType("blue");
    c.setCharacteristic("Prey");

    CHECK(c.getSpeed()        == Approx(3));
    CHECK(c.getCircleType()   == "blue");
    CHECK(c.getCharacteristic()== "Prey");
}

TEST_CASE("Circle rejects invalid radius values", "[Circle]") {
    CHECK_THROWS_AS(Circle(0,0,0 ,kBaseSpeed,kInitSpeed,"red"), std::invalid_argument);
    CHECK_THROWS_AS(Circle(0,0,-1,kBaseSpeed,kInitSpeed,"red"), std::invalid_argument);
}



TEST_CASE("Energy life‑cycle updates speed correctly", "[Circle][Energy]") {
    Circle c(0,0,3,2,2,"red");
    const int full = c.initialEnergy;

    REQUIRE(c.getEnergy() == full);

    c.decreaseEnergy(full * 0.4);
    CHECK(c.getEnergy() == Approx(full * 0.6));
    CHECK(c.getSpeed()  < c.getBaseSpeed());

    c.decreaseEnergy(full);
    CHECK(c.getEnergy() == 0);
    CHECK(c.getRegen());
    CHECK(c.getSpeed() == 0);

    c.regenEnergy(full / 2);
    CHECK(c.getEnergy() == full / 2);
    CHECK(c.getRegen() == true);
    CHECK(c.getSpeed() >= 0);

    double spd = c.getSpeed();
    c.decreaseEnergy(10);
    CHECK(c.getSpeed() <= spd);
}



TEST_CASE("Circle moves and bounces inside bounds", "[Circle][Move]") {
    Circle c(1,1, 3, 2, 2, "red");
    c.move(kSurfaceW, kSurfaceH);
    CHECK(c.getX() >= c.getRadius());
    CHECK(c.getY() >= c.getRadius());

    c.setPosition(kSurfaceW - c.getRadius() - 0.1, c.getY());
    c.move(kSurfaceW, kSurfaceH);
    CHECK(c.getX() <= kSurfaceW - c.getRadius());
}


TEST_CASE("Overlap and proximity behaviour", "[Circle][Proximity]") {
    Circle predator( 0, 0, 5, 2, 2, "red");
    Circle prey     ( 8, 0, 3, 2, 2, "blue");

    CHECK_FALSE(predator.overlaps(prey));
    prey.checkProximity(predator);
    CHECK_FALSE(prey.getSpeedBoost());

    // Far away → boost disabled
    prey.setPosition(50,50);
    prey.checkProximity(predator);
    CHECK_FALSE(prey.getSpeedBoost());
}


TEST_CASE("Predator reproduction threshold", "[Circle][Predator]") {
    Circle predator(0,0,5,2,2,"red");

    REQUIRE_FALSE(predator.canRepopulate());
    for(int i=0; i<predator.reproduceNumber; ++i)
        predator.eatPreyCircle();

    CHECK(predator.canRepopulate());
}


TEST_CASE("Sector add & remove circle bookkeeping", "[Sector]") {
    cse::Sector s(0,0);
    auto c1 = make_shared<Circle>(5,5,3,2,2,"red");

    s.add_circle(c1);
    CHECK(s.get_circle_count() == 1);
    CHECK(s.get_circle_at(0)   == c1);

    s.remove_circle(c1);
    CHECK(s.get_circle_count() == 0);
}


TEST_CASE("Surface stores, moves and reports collisions", "[Surface]") {
    cse::Surface surf(kSurfaceW, kSurfaceH, kSectorSize);

    auto circ = make_shared<Circle>(12,18,3,2,2,"red");
    surf.add_circle(circ);

    surf.move_circle(circ, 32, 42);
    CHECK(circ->getX() == Approx(32));
    CHECK(circ->getY() == Approx(42));

    auto c2 = make_shared<Circle>(35,42,3,2,2,"red");   // will overlap circ
    surf.add_circle(c2);

    auto res = surf.check_collision();
    CHECK(res.first == "add");

    surf.remove_circle(c2);
    circ->setCircleType("red");
    surf.move_circle(circ,10,10);

    auto prey = make_shared<Circle>(12,10,3,2,2,"blue");
    surf.add_circle(prey);

    auto res2 = surf.check_collision();
    CHECK(res2.first == "delete");
    CHECK(res2.second == prey);
}

/// More edge cases
TEST_CASE("Edge‑case coordinates land in the correct sector", "[Surface][Edge]") {
    constexpr int kW = 100, kH = 100, kS = 10;
    cse::Surface surf(kW, kH, kS);

    auto c = make_shared<Circle>(20.0, 19.9, 3, 2, 2, "red");
    surf.add_circle(c);

    surf.move_circle(c, 0 + c->getRadius(), 50);
    CHECK( c->getX() >= c->getRadius() );
}

/// sector overflow guard: add at bottom‑right corner
TEST_CASE("Adding circle at extreme bottom‑right does not overflow grid",
          "[Surface][Edge]") {
    constexpr int kW = 100, kH = 100, kS = 10;
    cse::Surface surf(kW, kH, kS);

    auto c = make_shared<Circle>(99, 99, 1, 2, 2, "red");
    REQUIRE_NOTHROW( surf.add_circle(c) );
}

/// duplicate spatial location, different pointers
TEST_CASE("Two distinct pointers at identical coordinates coexist or reject as spec",
          "[Surface][Edge]") {
    cse::Surface surf(kSurfaceW, kSurfaceH, kSectorSize);

    auto c1 = make_shared<Circle>(30,30,3,2,2,"red");
    auto c2 = make_shared<Circle>(30,30,3,2,2,"red");

	surf.add_circle(c1);
	surf.add_circle(c2);
	auto res = surf.check_collision();
	CHECK(res.first == "add");
}
/// energy underflow guard
TEST_CASE("Energy never drops below zero", "[Circle][Edge]") {
    Circle c(0,0,3,2,2,"red");
    c.decreaseEnergy(c.initialEnergy * 2);
    CHECK( c.getEnergy() == 0 );
}

/// proximity threshold edge: exactly equal to proximity radius
TEST_CASE("No speed‑boost exactly at proximity radius", "[Circle][Edge]") {
    Circle a(0,0,3,2,2,"red");
    Circle b(a.getRadius()*2, 0, 3, 2, 2, "blue");

    a.checkProximity(b);
    CHECK_FALSE( a.getSpeedBoost() );
}



