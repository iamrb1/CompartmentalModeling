// Circle.cpp : implementation of the Circle class
#include "Circle.h"
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <cmath>

namespace cse {
    
/**
 * Constructor for Circle
 * 
 * Creates a circle with specified position, size, and behavior characteristics.
 * Initializes movement in a random direction as a unit vector (dx_, dy_).
 * Implementation for seeding and randomization suggested by ChatGPT
 * 
 * @param x X-coordinate of circle center
 * @param y Y-coordinate of circle center
 * @param radius Radius of the circle (must be positive)
 * @param baseSpeed Base movement speed when at full energy
 * @param type Whether this circle is a predator or prey
 * @param initialEnergy Starting energy level for the circle
 * @param reproduceThreshold Number of prey eaten before reproduction is possible
 * @throws std::invalid_argument if radius is not positive
 */
Circle::Circle(double x, double y, double radius,
               double baseSpeed, CircleType type,
               int initialEnergy, int reproduceThreshold)
    : x_(x), y_(y), radius_(radius),
      dx_(0), dy_(0),
      baseSpeed_(baseSpeed), speed_(baseSpeed),
      proximityRadius_(radius * 2),
      type_(type), energy_(initialEnergy), initialEnergy_(initialEnergy),
      regenerating_(false), speedBoost_(false),
      eatingCounter_(0), reproduceThreshold_(reproduceThreshold) {

    if (radius <= 0) {
        throw std::invalid_argument("Circle radius must be positive");
    }

    // Seed random number generator once for the entire application
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        seeded = true;
    }

    // Initialize random movement direction as a unit vector
    const double pi = 3.141592;
    double angle = (std::rand() / static_cast<double>(RAND_MAX)) * 2 * pi;
    dx_ = std::cos(angle); // x-component of direction vector
    dy_ = std::sin(angle); // y-component of direction vector
}

// Returns internal state values
double Circle::getX() const noexcept          { return x_; }
double Circle::getY() const noexcept          { return y_; }
double Circle::getRadius() const noexcept     { return radius_; }
double Circle::getSpeed() const noexcept      { return speed_; }
int    Circle::getEnergy() const noexcept     { return energy_; }
CircleType Circle::getType() const noexcept   { return type_; }
bool   Circle::isRegenerating() const noexcept { return regenerating_; }
bool   Circle::hasSpeedBoost() const noexcept { return speedBoost_; }

/**
 * Checks if circle can reproduce
 * @return true if the circle has eaten enough prey to reproduce
 */
bool   Circle::canRepopulate() const noexcept { return eatingCounter_ >= reproduceThreshold_; }

/**
 * Updates the circle's position
 * Includes validation to prevent NaN or infinite values
 * 
 * @param x New x-coordinate
 * @param y New y-coordinate
 */
void Circle::setPosition(double x, double y) noexcept { 
    if (std::isnan(x) || std::isnan(y) || std::isinf(x) || std::isinf(y))
    {
        x_ = 0.0;
        y_ = 0.0;
        assert(false && "Circle::setPosition received invalid coordinates");
        return;
    }
    x_ = x; 
    y_ = y;
}

/**
 * Sets a new radius for the circle
 * Also updates the proximity radius which is used for interaction detection
 * 
 * @param radius New radius value (must be positive)
 * @throws std::invalid_argument if radius is not positive
 */
void Circle::setRadius(double radius) {
    if (radius <= 0) {
        throw std::invalid_argument("Circle radius must be positive");
    }
    radius_ = radius;
    proximityRadius_ = radius_ * 2;
}

/**
 * Changes the circle's type (predator or prey)
 * 
 * @param type New type value
 */
void Circle::setType(CircleType type) noexcept {
    type_ = type;
}

/**
 * Updates the circle's position based on its direction and speed
 * Handles boundary collisions by reversing direction component
 * 
 * @param width Width of the simulation area
 * @param height Height of the simulation area
 */
void Circle::move(double width, double height) {
    x_ += dx_ * speed_;
    y_ += dy_ * speed_;
    if (x_ - radius_ < 0 || x_ + radius_ > width) dx_ = -dx_;
    if (y_ - radius_ < 0 || y_ + radius_ > height) dy_ = -dy_;
}

/**
 * Decreases the circle's energy by the specified amount
 * Sets regenerating state when energy reaches zero
 * 
 * @param amount Amount to decrease energy by
 * @throws std::invalid_argument if amount is negative
 */
void Circle::decreaseEnergy(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Cannot decrease energy by negative amount");
    }
    
    if (!regenerating_ && energy_ > 0) {
        energy_ -= amount;
        
        if (energy_ <= 0) {
            energy_ = 0;
            regenerating_ = true;
        }
        
        updateSpeed();
    }
}

/**
 * Increases the circle's energy during regeneration phase
 * Exits regeneration state when energy is fully restored
 * 
 * @param amount Amount to increase energy by
 */
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

/**
 * Updates the circle's speed based on current energy level and boost status
 * Speed is proportional to energy and gets a 50% boost when near different type
 */
void Circle::updateSpeed() noexcept {
    double energyFactor = static_cast<double>(energy_) / initialEnergy_;
    speed_ = baseSpeed_ * energyFactor * (speedBoost_ ? 1.5 : 1.0);
}

/**
 * Checks proximity to another circle
 * Activates speed boost if close to a different type of circle
 * 
 * @param other The other circle to check proximity against
 */
void Circle::checkProximity(const Circle& other) {
    double rSum = radius_ + other.radius_;
    bool close = SquaredDistance(x_, y_, other.x_, other.y_) < rSum * rSum;
    bool different = (type_ != other.type_);
    speedBoost_ = (close && different);
    
    updateSpeed();
}

/**
 * Static method to check if two circles overlap
 * 
 * @param a First circle
 * @param b Second circle
 * @return true if circles overlap, false otherwise
 */
bool Circle::overlaps(const Circle& a, const Circle& b) noexcept {
    double rSum = a.radius_ + b.radius_;
    return SquaredDistance(a.x_, a.y_, b.x_, b.y_) < rSum * rSum;
}

/**
 * Handles predator eating prey interaction
 * Increments eating counter and tracks reproduction readiness
 * 
 * @param prey The prey circle being eaten
 */
void Circle::eatPrey(const Circle& prey) {
    if (type_ == CircleType::Predator && prey.type_ == CircleType::Prey) {
        ++eatingCounter_;
        
        if (eatingCounter_ >= reproduceThreshold_) {
            eatingCounter_ = 0;
        }
    }
}

}
