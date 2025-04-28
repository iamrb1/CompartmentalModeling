// Circle.h : interface of the Circle class
// Circle.h
#pragma once
#include <stdexcept>
#include <cmath>


namespace cse {

// Represents the two possible roles of a circle in the simulation
enum class CircleType { Predator, Prey };

class Circle {
public:
    static constexpr int kDefaultInitialEnergy     = 1000;
    static constexpr int kDefaultReproduceThreshold = 5;

    // Constructs a circle at (x, y) with given radius, speed, and type.
    // Optionally specify initial energy and reproduce threshold.
    Circle(double x,
           double y,
           double radius,
           double baseSpeed,
           CircleType type,
           int initialEnergy       = kDefaultInitialEnergy,
           int reproduceThreshold  = kDefaultReproduceThreshold);
    ~Circle() = default;

    // Accessors
    double       getX() const noexcept;
    double       getY() const noexcept;
    double       getRadius() const noexcept;
    double       getSpeed() const noexcept;
    int          getEnergy() const noexcept;
    CircleType   getType() const noexcept;
    bool         isRegenerating() const noexcept;
    bool         hasSpeedBoost() const noexcept;
    bool         canRepopulate() const noexcept;

    // Mutators
    void setPosition(double x, double y) noexcept;
    void setRadius(double radius);
    void setType(CircleType type) noexcept;

    // Simulation behaviors
    void move(double width, double height);
    void decreaseEnergy(int amount);
    void regenerateEnergy(int amount);
    void checkProximity(const Circle& other);
    void eatPrey(const Circle& prey);

    static bool overlaps(const Circle& a, const Circle& b) noexcept;

    bool overlaps(const Circle& other) const noexcept { return overlaps(*this, other); }

private:
    // Position and geometry
    double x_;
    double y_;
    double radius_;

    // Movement direction unit vector
    double dx_;
    double dy_;

    // Movement speeds
    double baseSpeed_;
    double speed_;
    double proximityRadius_;

    // Role and state
    CircleType type_;
    int        energy_;
    int        initialEnergy_;
    bool       regenerating_;

    // Predation
    bool speedBoost_;
    int  eatingCounter_;
    int  reproduceThreshold_;

    // Squared distance
    static double SquaredDistance(double x1, double y1, double x2, double y2) noexcept {
        double dx = x1 - x2;
        double dy = y1 - y2;
        return dx*dx + dy*dy;
    }

    // Update speed based on current energy and boost state
    void updateSpeed() noexcept;
};
}