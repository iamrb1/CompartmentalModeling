## <span id="group-1" style="color: blue">Group 1: Stroustrup</span>

**Members**:  Jerry Chen, George Faraj, Leonardo Specht, Kollin Bartz, and Tyler Pritchett

**Shared Interests**: Data Processing

**Notable Strengths**: Algorithms, Databases, Web Development

### Classes to Build

| Class               | Description |
| ------------------- | ----------- |
| **`Random`**        | A random number generator.  User should supply the range and numerical type and the object should return a uniform random value in that range. The constructor should take a random number seed.  Member functions could include `double GetDouble(double min, double max)` and `double GetInt(int min, int max)` (and perhaps variants), as well as `bool P(double probability)`, which you give the probability of "true" and it flips the appropriate weighted coin for you.  |
| **`WeightedSet`**   | Like an [`std::set`](https://en.cppreference.com/w/cpp/container/set), but each item has a numerical "weight" associated with it.  A user should be able to index into the set with a value between 0.0 and total weight of all items.  For example, if there are three items, A, B, and C, with associated weights 0.5, 3.1, and 1.5, then the total weight would be 5.1.  In that range, 0.0 to 0.5 would give you A, >0.5 through 3.6 would give you B, and >3.6 through 5.1 would give you C.  Randomly indexing in allows you to pull a weighted random value from the set.  The tricky part is making this fast even when there are many items in the set.
| **`Distribution`**  | Create or manage a discrete distribution of values that can be used to draw from whatever distribution you need (for example, a Binomial distribution). You can use `Random` and `WeightedSet` to build this class and make it quite useful  Pre-calculation of distributions can allow for fast random draws. |
| **`Graph`**         | A class to manage a collection of vertices and edges; should be useful for conducting graph algorithms.  The class should be able to load from files, save to file, access connection information, and add or remove vertices and edges. |
| **`GraphPosition`** | A position on the Graph object, useful for implementing more complex graph algorithms.  For example, it might be usable for iterating through each vertex during a graph traversal. |

### Application Suggestion

A **graph generator and analyzer**.  The user would be able to specify the type of graph they want, as well as the size and any other relevant parameters (for example, width and height in a grid, or number of edges in a random graph).  The app will create the graph and be able to provide information about some key properties of the graph, such as average degree of vertices, height of its minimum spanning tree, etc.  Distribution objects will be useful as you try to create small world networks or other graph types that have particular distributions of edges.

Suggested **challenges**: I'd recommend a web interface (to allow anyone to easily create graphs), but another good option would be more advanced graph algorithms for analyses.
