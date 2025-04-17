// Circle.cpp : implementation of the Circle class
#include "Circle.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <numbers>


// Constructor
Circle::Circle(double x, double y, double radius, double baseSpeed, double speed, const std::string& circleType)
    : x_(x), y_(y), radius_(radius), baseSpeed_(baseSpeed), speed_(speed), circleType_(circleType), energy_(100), 
    regen_(false), repopulate_(false), eatingCounter_(0), proximityRadius_(radius * 2), speedBoost_(false) {

    if (radius <= 0) {
        throw std::invalid_argument("Radius must be positive");
    }
    if (circleType_ == "blue") {
        repopulate_ = true;
    }

    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        seeded = true;
    }

    double angle = (std::rand() % 360) * 3.14159265358979323846 / 180.0;
    dx_ = std::cos(angle);
    dy_ = std::sin(angle);
}

// Destructor
Circle::~Circle() {}

// Getter for X coordinate
double Circle::getX() const {
    return x_;
}

// Getter for Y coordinate
double Circle::getY() const {
    return y_;
}

// Getter for radius
double Circle::getRadius() const {
    return radius_;
}

// Getter for speed
double Circle::getSpeed() const {
    return speed_;
}

// Getter for energy
int Circle::getEnergy() const {
    return energy_;
}

// Getter for circle type
std::string Circle::getCircleType() const {
    return circleType_;
}

// Getter for energy regeneration
bool Circle::getRegen() const {
    return regen_;
}

// Getter for speed boost
bool Circle::getSpeedBoost() const {
    return speedBoost_;
}

// Getter for base speed
double Circle::getBaseSpeed() const {
    return baseSpeed_;
}

// Getter for characteristic
std::string Circle::getCharacteristic() const {
    return characteristic_;
}

// Setter for position
void Circle::setPosition(double x, double y) {
    x_ = x;
    y_ = y;
}

// Setter for radius
void Circle::setRadius(double radius) {
    if (radius <= 0) {
        throw std::invalid_argument("Radius must be positive");
    }
    radius_ = radius;
}

// Setter for Speed
void Circle::setSpeed(int speed) {
    speed_ = speed;
}

// Setter for circle type
void Circle::setCircleType(const std::string& circleType) {
    circleType_ = circleType;
}

// Check if this circle overlaps with another circle
bool Circle::overlaps(const Circle& other) const {
    double dx = x_ - other.x_;
    double dy = y_ - other.y_;
    double distance = std::sqrt(dx * dx + dy * dy);
    return distance < (radius_ + other.radius_);
}

// Miscellaneous characteristics
void Circle::setCharacteristic(const std::string& characteristic) {
    characteristic_ = characteristic;
}

void Circle::move(double width, double height) {
    // Update position
    x_ += dx_ * speed_;
    y_ += dy_ * speed_;

    // Bounce off left/right walls
    if (x_ - radius_ < 0) {
        x_ = radius_;
        dx_ *= -1;
    }
    else if (x_ + radius_ > width) {
        x_ = width - radius_;
        dx_ *= -1;
    }

    // Bounce off top/bottom walls
    if (y_ - radius_ < 0) {
        y_ = radius_;
        dy_ *= -1;
    }
    else if (y_ + radius_ > height) {
        y_ = height - radius_;
        dy_ *= -1;
    }
}


// Decreases energy of the circle
// If energy is 0, set regen to true and speed to 0
// If energy is less than 0, set it to 0
// If energy is greater than 0, update speed
void Circle::decreaseEnergy(int energy) {
    if (energy_ > 0 && !regen_) {
        energy_ -= energy;
       
        if (energy_ <= 0) {
            regen_ = true;
            speed_ = 0;
            energy_ = 0;
        }
        updateSpeed();
    }
}

// Increases energy of the circle
// If energy is greater than initial energy, set regen to false and speed to base speed
// If energy is less than initial energy, update speed
void Circle::regenEnergy(int energy) {
    if (regen_) {
        energy_ += energy;
        if (energy_ >= initialEnergy) {
            regen_ = false;
            speed_ = baseSpeed_;
        }
    }
}

// Updates the speed of the circle based on energy and speed boost
void Circle::updateSpeed() {
    if (speedBoost_) {
        speed_ = baseSpeed_ * (energy_ / 100.0) * speedBoost;
    } else {
        speed_ = baseSpeed_ * (energy_ / 100.0);
    }
}

// Check if the circle is in proximity to another circle
// If the distance is less than the proximity radius and the circle types are different, set speed boost to true
void Circle::checkProximity(const Circle& other) {
    auto calculateDistance = [this](const Circle& other) {
        double dx = x_ - other.x_;
        double dy = y_ - other.y_;
        return std::sqrt(dx * dx + dy * dy);
    };

    double distance = calculateDistance(other);

    auto updateSpeedBoost = [this](bool boost) {
        speedBoost_ = boost;
        updateSpeed();
    };

    if (distance < proximityRadius_ && circleType_ != other.circleType_) {
        updateSpeedBoost(true);
    } else {
        updateSpeedBoost(false);
    }
}

// Getter to check if a circle can repopulate
bool Circle::canRepopulate() const {
    return repopulate_;
}

// If a predator circle touches a prey circle, the prey circle is eaten
// If the predator circle eats 5 prey circles, it can repopulate
void Circle::eatPreyCircle() {
    if (circleType_ == "red") {
        eatingCounter_++;
        if (eatingCounter_ >= reproduceNumber) {
            repopulate_ = true;
            eatingCounter_ = 0;
        }
    }
}


