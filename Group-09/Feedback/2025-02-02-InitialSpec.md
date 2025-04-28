# Feedback on initial specifications

## `EventQueue` class

You're right that this is a pretty standard, straight-forward class that shouldn't be too hard to implement.  Your description looks good.

## `EventManager` class

Good thought about functions and error conditions.  I think you're right on about challenges as well, though one of the other challenges could be if you end up with a backlog of events such that the manager can't keep up, though that might just sort itself out naturally.

## `Image` class

This class requires a more sophisticated interface, and you have identified that and some of the issues you're likely to deal with.  My guess is that you'll have more come up as you try to build the interface, but should be able to deal with them as they do.

Hopefully the demonstration of Emscripten helped you see how you'll be able to manage JS from C++, but I'm happy to help work through some trickier issues that come up if you get stuck.

## `TextBox` class

Once again, nicely analyzed.  One of the other class projects you point out, "RichText", does seem like it could be very useful for the sorts of conversions that you're looking to do.  I'm not sure yet if that group is going to do HTML (they didn't specify explicitly in their writeup), but they did refer to `TextBox` as a related class project saying "Our data structure is ideal for this use case, we should cater to this class." so you should definitely talk.

## `WebLayout` class

I think you are correct in identifying this as one of the trickier implementations, especially since it will be so dependent on getting some of the other classes working.  You can try this one out with per-made stubs to get it going.  I don't think you'll have to worry about sockets and other direct web issues; most of that should automatically be handled on the HTML / generate code side, but if issues do come up, don't hesitate to ask.
