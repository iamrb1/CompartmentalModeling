# Feedback on initial specifications

## `StaticVector` class

This looks good.  Don't worry about getting every single vector function implemented; just hit some of the key ones.  And don't worry about doing any of the more "advanced" stuff, like doing anything with extra template parameters (they normally let you control things like how vectors allocate extra space, etc.)

I recommend you add a `.ToVector()` member function to covert your StaticVector into a regular `std::vector`.  This will be useful in some circumstances where a function call is limited to std::vector, but it will also help a LOT when testing.


## `MemoFunction` class

Looks like a good analysis.  For your initial version, you don't worry about functions with multiple parameters -- that can be an addition if you decide to do an advanced version of this class.

I don't know that you need a calculate function -- the `operator()` should be sufficient.  It should check the cache and return a value of found OR call the function (saving the result in the cache) if not found.


## `ComboManager` class

I hadn't realized that there was a `std::next_permutation`, but that does seem like good inspiration.

Instead of a `MakeCombinations` function (or, perhaps, in addition to), I would go with something more similar to the standard library.  Perhaps `.NextCombo()` and `.PrevCombo()` to step through.  I can imaging situation where you might want to step through a billion combinations, which you can do in reasonable time in C++, but I wouldn't want to pre-generate all of them if there were that many.  Just step through and use them as you need them.  That said, you do note that you could return them as a generator, so this might be what you were thinking anyway.

As you step through combinations, I would probably just treat them as indices that you use to, for example, know which entries in an array or vector you should be working with.


## `BruteForceOptimizer` class

You are right to recognize that a clean implementation for this one is going to be tricky - it's probably the hardest of the classes that your team is working with.

I'd recommend that you focus on getting it working with a SINGLE NP-Complete search problem.  Knapsack is a good choice, as would be set cover, minimum dominating set, STIG, or any other one where you're trying to find the optimal subset of items that minimizes or maximizes a goal.

While efficiency will clearly be important, don't worry about it until you get the functionality in place.  And many of the efficiency optimization you should do only if you pick this class as one to build a more advanced version of.

## `MemoryFactory` class

Looks like you put good thought into this one too.  Some of the functions that you list seem like they would be internal, helper functions, such as `ExtendSpace()` (which should be called automatically any time `Allocate()` doesn't have memory available), but that's good.

I would probably called `Deallocate()` simply `Free()` for simplicity, but that's a style choice.

When you allocate, you should be able to pass in constructor arguments.  I can show you how to do that with variable numbers of arguments, but it's fine if you want to start with managing classes that have default constructors.