# Feedback on initial specifications

## `DataMap` class

You seem to have a solid plan here for this implementation; the stackoverflow article should be able to get you most of the way there.  I agree that with `std::any`, you'll be able to get most of the tricky functionality out of the way, though if you decide to make a more advanced version of this class I'll encourage you to implement that part yourself.

For example, I'd love for you to be able to have a `ToString()` function that would return an `std::string` version of any saved value, even if you don't know the type on the callers end, BUT we have not yet covered the material for how to do that, so we'll save it for later.

Nitpicky point: careful with style consistency in function names.  Right now you have `set` and `Get` the differ in capitalization.  I'm fine with you choosing your own style, but make sure to be consistent.

You did a nice job identifying expected challenges and issues that you'll be able to explore more as you move forward.


## `StateGrid`

Looks like a good overview of the class, though it's not immediately clear to me what the proposed functions `Validate` or `FindMoves` do exactly (I can make some guesses, but the writeup didn't specify)

You mention the challenges of building the map. For simplicity, you should have functions to load and save StateGrids so you can pull them out of files easily.  If each state has an associated character, this will allow you to easily look at the file and know what the world looks like.  If you decide to build an advanced version of this class, you might add functions to make it easier to build maps from scratch (e.g., `GenerateMaze()` and others).


## `StateGridPosition`

As you point out, this can be a fairly simple class.  One decision that you may need to still make (at least it wasn't clear from your write up) is whether `StateGridPosition` (SGP) knows which `StateGrid` it is in (presumably, by keeping a pointer to it, or even a reference if possible) so that you can just manipulate the SGP to get StateGrid info.  If so, you should add many functions, like `GetStateGrid()` and `GetState()` (to know the state of this position it's indicating).

Alternatively, if SGP is just a helper class that does NOT know which grid it's in, then there should be a lot more `StateGrid` functions that that an SGP as an argument rather than (or probably in addition to?) ones that take explicit coordinates.  I definitely like the idea of being able to find the distance between two SGPs, and perhaps you can have some tools to help figure out things like "line of sight", but that would only be if you decide to make more advanced versions.


## `ActionMap`

This is a solid specification for this class.  The interface looks good and the error analysis seems well thought trough.  Your list of challenges, in particular, highlight the real difficulties with this implementation, if it is to be done as flexibly (and to be as useful!) as possible.  In your initial implementation you may choose to have a fixed set of arguments in a given `ActionMap` so that you don't need to deal with type erasure and the associated error-catching challenges at this early stage (though if you are comfortable with it, feel free to do so.)

For other class projects that are similar, I'm only realizing now that I'm reading all of these, but `ActionMap` has a lot of similarities with group 6's `CommandLine` object, which redirects command-line "commands" to appropriate functions.  They're not identical (and have different use cases), but I think the underlying implementations could resemble each other some.

## `AuditedVector`

Making your vector class behavior like std::vector outside of debut mode is nice and easy.  Your code should look something like this:

```
namespace cse {

#ifdef NDEBUG  // Standard way of testing if we are NOT in debug mode...
  using vector = std::vector;  // We take the implementation directly!

#else  // If we ARE in debug mode...
  template <typename T, typename... Ts>
  class vector : public std::vector<T,Ts...> {
    // Code that we are CHANGING from std::vector
  };

#endif

}  // End namespace cse
```

As you can see above, you can "cheat" with most of the vector functionality by simply wrapping the std::vector class.  You can even do your checks and then just return the original code.

You should look through the standard vector member functions and decide which ones your are going to audit.  For the simple version of this class, feel free to ignore iterators; we can talk about how to do those if you decide to do the more advanced version (though it's not THAT hard).

In the meantime, there are a number of functions you might want to audit.  I'll give you an example of what one implementation might look like:

```
  T & back() {
    assert(size() > 0);    // No "back" element in an empty vector.
    return std::vector<T,Ts...>::back();
  }
```

So, basically, you make sure that it is safe to do a back, and then just call the original version of it.  And if you get tired of typing `std::vector<T,Ts...>` all the time to refer to the original version, you can just include, somewhere near the top of the definition of your class, something like `using base_t = std::vector<T,Ts...>;` and then you would only have had to write `return base_t::back();` above.

Note that the is also a `const` version of back() and you would need to do both other them.
