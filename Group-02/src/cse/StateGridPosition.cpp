/**
 * @file StateGridPosition.cpp
 * @author Bram Hogg
 */

#include <cse/StateGridPosition.h>

namespace cse
{
    Point StateGridPosition::CalculateObjectPosition()
    {
        ///Calculate the position of the object and return the point
        Point defaultObjectPosition;
        defaultObjectPosition.x = 0;
        defaultObjectPosition.y = 0;
        return defaultObjectPosition;
    }

}