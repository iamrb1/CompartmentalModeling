/**
 * @file StateGridPosition.h
 * @author Bram Hogg
 *
 *
 */

#include <cse/StateGrid.h>

#pragma once

#ifndef CSEG2_INCLUDE_CSE_STATEGRIDPOSITION_H
#define CSEG2_INCLUDE_CSE_STATEGRIDPOSITION_H

namespace cse
{
///Point object for the position
struct Point
{
    ///The position of the object
    double x = 0;
    double y = 0;
};

class StateGridPosition
{

private:

    ///The orientation in degrees
    double object_orientation = 0;

    Point object_position;

    Point central_position;

    ///The state grid this position is referring to
    StateGrid *state_grid = nullptr;

public:
    Point createPoint(int x, int y)
    {
        Point p;
        p.x = x;
        p.y = y;
        return p;
    }

    ///Get the stategrid this position is associated with
    StateGrid *Getstategrid() { return state_grid; }

    ///Possibly getState, but can be done in only necessary places as well

    ///Get central position of the stategrid
    Point Getgridposition() { return central_position; }

    ///Get the position of the object this stategrid is associated with
    Point Getobjectposition() { return object_position; }

    ///Set position of object this stategrid is associated with
    void Setobjectposition(double x, double y) { object_position = createPoint(x, y); }

    double Getobjectorientation();

    ///Set the object's orientation, 0-359 degrees
    void Setobjectorientation(double orientation) { object_orientation = orientation; }

    ///Calculates the object's position, still working on specs
    Point Calculateobjectposition();
};
}

#endif //CSEG2_INCLUDE_CSE_STATEGRIDPOSITION_H
