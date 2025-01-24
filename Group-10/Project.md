## <span id="group-10" style="color: blue">Group 10: Bellman</span>

**Members**: Alex Mueller, Blake Crimmins, Amery Johnson, Devon FoxElster, and Ephraim Bennett

**Shared Interests**: Science / Social Media

**Notable Strengths**: A.I. / Algorithms / Web Development

### Classes to Build

| Class               | Description |
| ------------------- | ----------- |
| **`StaticVector`**  | Like [`std::vector`](https://en.cppreference.com/w/cpp/container/vector), but a maximum vector size is specified in the template parameter and all memory is allocated at object creation.  Size cannot go over the static limit, but it will likely be much faster than `std::vector` in many instances. |
| **`MemoFunction`** | A function wrapper that automatically caches inputs to output -- if the same input is used a second time, it should immediately return the output rather than call the function again.  Start by making MemoFunction handle a single input, but for a harder challenge you can set it up to handle functions with multiple parameters.  You could also add an eviction policy if you want to limit the number of values stored (typically you would remove the oldest stored values, but you may also want to remove values due to collisions in the hash table.) |
| **`ComboManager`** | A tool to manage combinations of items from a container.  Should be able to step through all possible combinations, or do branching with each included / excluded in order to facilitate bounding and backtracking. It should have some helper function, such as pre-calculating the total number of combination possible. |
| **`BruteForceOptimizer`** | A tool to explore all possible combinations of a set of inputs to maximize a value function.  Should handle backtracking and bounding (if the value function is NaN, backtrack, include a second MAX value function to help bounding) or other optimization techniques. |
| **`MemoryFactory`** | A simple memory manager where collections of the same type are stored to be given out as requested (more should get allocated when an existing supply is exhausted).  When a new object of the factory type is created, the factory should provide the memory for it.  When an object is deleted, the factory should reclaim the memory to be handed out again.  Since object sizes are fixed in C++, this provides a faster way of allocating object if you know you are going to need to do frequent allocations and freeing of memory. |

### Application Suggestion

A **Citizen-Science web console**.  It can be challenging to build a citizen-science application where the data is fed back in to the central hub in a useful manner.  You will build a tool to simplify this for people interested in developing such an application.  To demonstrate the tool, pick an NP-Complete problem that you build a solver for (using the tools above) and then a website where problem instances can be placed, along with a spot for people to run it.  Some good options might be instances of the Knapsack problem, Satisfiability, or Maximum Independent Set.  When a user solves and instance of the problem locally, the results should be included on the site.

Suggested **challenges**: One fun challenge would be to attach the citizen-science console to a more interesting problem, perhaps one you find online or (even better) one of the projects put together by one of the other groups;  group 3 (a physics-based cellular automaton) or group 7 (ecology simulator) might work particularly well, assuming they use the suggested project.
