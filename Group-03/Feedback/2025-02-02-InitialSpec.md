# Feedback on initial specifications

## `FixedPoint` class

Good analysis of existing classes and needed functionality; `std::ratio` is a particularly relevant choice to model this class on.  I don't think a straight-forward version of this class shouldn't have any huge obstacles to its implementation; the biggest challange will be if you try to make it faster than `double` for many circumstances, but worrying about those issues would be a more advanced challenge for this C++ class (if your team picks it as one of the options to make a more complicated version of).

## `FunctionSet` class

Solid writeup.  I think your plans for how this class will work and how to implement it are well thought through.  On your list of challenges, I think the first one is the only one to worry about in the initial implementation.  For the others: (2) You should use std::function to store each individual function.  (3) I don't think the first version needs to do anything special with exception handling, and (4) This is definitely an issue to worry about only if you want to make a more advanced version of this C++ class.

## `Circle` class

Straight-forward description.  For functions to implement, I would explicitly include a `Distance()` function to measure the distance between the centers of two circles.  If this distance is less than the sum of their radii, then the circles overlap; if it's greater, they do not overlap, allowing that function to be implemented more easily.  Distance is normally calculated as:

```
 sqrt( xdiff * xdiff + ydiff * ydiff )
```

Where `xdiff` and `ydiff` simply represent the differences in x and y between the points your measuring the distance between.

If you want an EASY way for a speedup, I recommend also implementing `SquareDistance()`.  It turns out that square root is actually pretty slow to calculate (relatively to other operation), but squaring a value is much faster.  You can compare `SquareDistance()` against the square of the sum of the radii to know if two circles overlap or not, without ever taking a square root.

Regarding the challenges, if you want to do this in a web interface, the HTML5 canvas should be able to handle all of the circles you need -- I've done thousands of circles at once, all moving independently, with no issues.  I'm happy to show off some of this in class if you like!

## `Surface` class

This class definition was a bit light, but the separate conversation we had about QuadTrees versus full sector grids shows that you are thinking through the issue.  Depending on the size of the worlds and the number of items in it, I think this project will run much faster than you expect (that's the beauty of C++ -- I've done this in both C++ and JavaScript and the speed difference is shocking!)

## `DataTracker` class

Solid analysis of this class.  The initial implementation should be nice and straight-forward.
