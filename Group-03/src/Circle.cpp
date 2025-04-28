// Circle.cpp : implementation of the Circle class
#include "Circle.h"
#include <cstdlib>
#include <ctime>

Circle::Circle(double x, double y, double radius,
               double baseSpeed, CircleType type,
               int initialEnergy, int reproduceThreshold)
    : x_(x), y_(y), radius_(radius),
      dx_(0), dy_(0),
      baseSpeed_(baseSpeed), speed_(baseSpeed),
      proximityRadius_(radius * 2),
      type_(type), energy_(initialEnergy), initialEnergy_(initialEnergy),
      regenerating_(false), speedBoost_(false),
      eatingCounter_(0), reproduceThreshold_(reproduceThreshold) { //chatgpt
    if (radius <= 0) {
        throw std::invalid_argument("Circle radius must be positive");
    }
    // Seed random once for movement direction
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        seeded = true;
    }
    // Initialize random direction unit vector
    double angle = (std::rand() / static_cast<double>(RAND_MAX)) * 2 * M_PI;
    dx_ = std::cos(angle);
    dy_ = std::sin(angle);
}

double Circle::getX() const noexcept          { return x_; }
double Circle::getY() const noexcept          { return y_; }
double Circle::getRadius() const noexcept     { return radius_; }
double Circle::getSpeed() const noexcept      { return speed_; }
int    Circle::getEnergy() const noexcept     { return energy_; }
CircleType Circle::getType() const noexcept   { return type_; }
bool   Circle::isRegenerating() const noexcept { return regenerating_; }
bool   Circle::hasSpeedBoost() const noexcept { return speedBoost_; }
bool   Circle::canRepopulate() const noexcept { return eatingCounter_ >= reproduceThreshold_; }

void Circle::setPosition(double x, double y) noexcept { x_ = x; y_ = y; }

void Circle::setRadius(double radius) {
    if (radius <= 0) {
        throw std::invalid_argument("Circle radius must be positive");
    }
    radius_ = radius;
    proximityRadius_ = radius_ * 2;
}

void Circle::setType(CircleType type) noexcept {
type_ = type;
}

void Circle::move(double width, double height) {
    x_ += dx_ * speed_;
    y_ += dy_ * speed_;
    if (x_ - radius_ < 0 || x_ + radius_ > width) dx_ = -dx_;
    if (y_ - radius_ < 0 || y_ + radius_ > height) dy_ = -dy_;
}

void Circle::decreaseEnergy(int amount) {
    if (!regenerating_ && energy_ > 0) {
        energy_ -= amount;
        if (energy_ <= 0) {
            energy_ = 0;
            regenerating_ = true;
        }
        updateSpeed();
    }
}

void Circle::regenerateEnergy(int amount) {
    if (regenerating_) {
        energy_ += amount;
        if (energy_ >= initialEnergy_) {
            energy_ = initialEnergy_;
            regenerating_ = false;
        }
        updateSpeed();
    }
}

void Circle::updateSpeed() noexcept {
double energyFactor = static_cast<double>(energy_) / initialEnergy_;
speed_ = baseSpeed_ * energyFactor * (speedBoost_ ? 1.5 : 1.0);
}

void Circle::checkProximity(const Circle& other) {
    double rSum = radius_ + other.radius_;
    bool close     = SquaredDistance(x_, y_, other.x_, other.y_) < rSum * rSum;
    bool different = (type_ != other.type_);
    speedBoost_    = (close && different);
    updateSpeed();
}

bool Circle::overlaps(const Circle& a, const Circle& b) noexcept {
double rSum = a.radius_ + b.radius_;
return SquaredDistance(a.x_, a.y_, b.x_, b.y_) < rSum * rSum;
}

void Circle::eatPrey(const Circle& prey) {
    if (type_ == CircleType::Predator && prey.type_ == CircleType::Prey) {
        ++eatingCounter_;
        if (eatingCounter_ >= reproduceThreshold_) {
            eatingCounter_ = 0;
            // Flag for repopulation; simulation logic can handle spawning
        }
    }
}
