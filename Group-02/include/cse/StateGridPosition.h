/**
 * @file StateGridPosition.h
 * @author Bram Hogg
 *
 *
 */

#pragma once

#ifndef CSEG2_INCLUDE_CSE_STATEGRIDPOSITION_H
#define CSEG2_INCLUDE_CSE_STATEGRIDPOSITION_H

namespace cse {
    ///Point object for the position
    struct Point {
    ///The position of the object
    double x = 0;
    double y = 0;
};

class StateGridPosition {

private:

    ///The orientation in degrees
    double mObjectOrientation = 0;

    Point mObjectPosition;

    Point mCentralPosition;

public:
    Point createPoint(int x, int y) {
        Point p;
        p.x = x;
        p.y = y;
        return p;
    }

    Point GetGridPosition() { return mCentralPosition; }

    Point GetObjectPosition() { return mObjectPosition; }

    void SetObjectPosition(double x, double y) { mObjectPosition = createPoint(x, y); }

    double GetObjectOrientation() { return mObjectOrientation; }

    void SetObjectOrientation(double orientation) { mObjectOrientation = orientation; }

    Point CalculateObjectPosition();
    };
}

#endif //CSEG2_INCLUDE_CSE_STATEGRIDPOSITION_H
