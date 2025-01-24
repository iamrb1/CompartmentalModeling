## <span id="group-2" style="color: blue">Group 2: Ritchie</span>

**Members**: Nitish Maindoliya, Dominik Leisinger, Rahul Baragur, Bram Hogg, and Matthew Hawkins

**Shared Interests**: Games / Productivity

**Notable Strengths**: Software Engineering, User Interfaces, Web Development

### Classes to Build

| Class               | Description |
| ------------------- | ----------- |
| **`DataMap`**       | A dynamic map that can match names to arbitrary data.  The user will need to provide the type it was set to when getting the value; the object should detect of wrong types are used.  For example, `Set("health", 45.0)` would set a variable named "health" to a double of value 45.0.  To get it again, you would need to use `Get<double>("health")` to make sure the type is know at compile time. Get should have an `assert` that is tripped if the wrong type is specified. |
| **`StateGrid`**     | A dynamic 2D grid where each position is one of a set of pre-defined states. Each state type should have a name, a symbol, and any other information the define that state (perhaps as a DataMap). For example, if a StateGrid was used to represent a maze, you might give it two types: a "wall" with the symbol '#' and the property "Hardness" set to 20, as well as an "open" state with the symbol ' ' (space) and no properties. |
| **`StateGridPosition`** | Track a single position (and orientation?) in the state grid to manage an individual agent moving through it. |
| **`ActionMap`**     | A map of strings to functions that can be triggered.  Can be useful in supplying a list of user actions when building a dynamic interface.  It should have member function like `AddFunction(std::string name, std::function<void()> fun);` and `Trigger(std::string name);` |
| *Audited Vector* | Functionality of [`std::vector`](https://en.cppreference.com/w/cpp/container/vector) but when compiled in debug mode, checks every index to ensure it is valid and in range; similar checks for iterators.  You can make your `cse::vector` derive from `std::vector` to get started (e.g., `template <typename T, typename... Ts> class vector : public std::vector<T,Ts...> { ... }`) and then add in a helper so you know the base type `using base_t = std::vector<T,Ts...>;`. In functions that you need to simply pass a call to the base type, this is easy to do: `size_t size() const { return base_t::size(); }` |

### Application Suggestion

A **productivity dungeon** where players adventure through a dungeon, but some of the boss battles are entries from your to-do list that you must complete in order to defeat them. Alternatively, you can place actual learning challenges in the dungeon and turn it into a gamified learning experience.

Suggested **challenges**: Web interface. Add enough features in the dungeon where there are many possible things for a player to do, including collecting items and using magic.
