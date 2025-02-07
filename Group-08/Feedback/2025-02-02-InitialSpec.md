# Feedback on initial specifications

## `AnnotatedWrapper` class

Key ideas seem good and clear thought was put into specifications.  Having the flexibility of the tags being associated with any data is nice, though it will add some extra challenges to the implementation.  You might want to take a look at `DataMap` in Group 02 for a related class.

My biggest concern with the current implementation plan is that it really caters to the power use, who may want some very complex data associated with these tags. The `std::any` class is powerful, but requires you to specify types whenever you use it. Having a simple version of the interface available as well, might be nice.  That said, feel free to get your current version working and then we can round it out further if you decide this is one of the classes that you want to expand.


## `TagManager` class

Good analyses for this class.  I'd need to see an example use case to have a full sense of how the interface works, but it's looking solid to me.

## `DynamicString` class

This looks like a nice writeup too.  One of the trickiest parts of this class is going to be how you internally represent data that is a combination of strings and function results.  I see two obvious solutions, though I am sure there are more possibilities.  In both cases you probably want to define:

```
using str_fun_t = std::function<std::string()>; // Function that returns a string.
```

1. You could use a vector of `std::variant<std::string, std_fun_t>` and deal with each entry based on the type it is.

2. You could use a pair of vectors, one of `std::string`, and the other of `std_fun_t` and alternate between the two.

You would start in the `std::string` vector (with an empty string).  If a string gets appended, you append it there.  If a function gets appended, you put it in the function array and move on to the next string being at the end.  It's more bookkeeping, but might be easier?

Also, for the interface, I recommend overloading the `+` operator rather than using `Append`, though no reason that you can't have both.

## `RandomAccessSet` class

I fully understand your concerns about how to get started with this class.  I recommend you start by just using an `std::vector` behind the scenes and build on the set-like interface.  If you need to
keep brute-force searching through the vector, that's okay -- don't worry about speed.

Once you have a working system, you can speed it up by adding an additional "helper" data structure.


## Audited pointer

This is a particularly challenging class.  Let me start by suggesting that you do NOT override `new` and `delete` right away (that can always be done later) and don't worry about this new pointer type being used automatically.  You should think of this in the same sort of way as you think about `std::shared_ptr` and all of it's helpers.

Step 1: Come up with a good name for the template type that's short so it's easy to use a lot..  Maybe `Aptr`?  Assuming that's what we call it, instead of a `int *` we would use `Aptr<int>` which would behave in the same way, but it will allow us to do checks.

Step 2: Build a template class and constructor that allows you to pass in a raw pointer for it to manage.

```
template <typename TYPE>
class Aptr {
private:
  TYPE * ptr = nullptr;   ///< The raw pointer associated with this Ptr object.
public:
  Aptr() = default;
  Aptr(const Aptr &) = default;
  Aptr(TYPE * in_ptr) : ptr(in_ptr) { }
};
```

Step 3: When building a new object with an audited pointer, you will likely need to call a helper function rather than the regular `new`, similar to how you call `std::make_shared()` for shared pointers in the standard library.  Something like:

```
/// Create a new Aptr of the target type; use the args in the constructor.
template <typename T, typename... ARGS>
[[nodiscard]] Aptr<T> MakeAptr(ARGS &&... args) {
  auto ptr = new T(std::forward<ARGS>(args)...); // Build the raw pointer
  return Aptr<T>(ptr, true);                     // Convert to audited pointer
}
```

Now, if you wanted to create a pointer to a new `Widget` that needs an `int` and a `string` in the constructor, you might create it with:

```
auto widget_ptr = MakeAptr(5, "abc");
```

And it should work.

Step 4: Make sure you can delete a raw pointer.  For simplicity, rather than overloading the `delete` operator, I recommend that in the initial version you simply create a `Delete()` member function that needs to be run to handle it.

Step 5: Implement the core operators that pointers need.  At least `operator*` and `operator->`.  Possibly some helper functions like `Raw()` that simply gives you back the raw pointer in case it's needed for some kind of function call.  (An auto-cast to raw would be good too; we can talk about those.)

Step 6: Add in any tests that you want to perform when you are in debug mode.  Go for things that are easy.  For example, you can include a 'status' flag inside of a pointer to indicate if it is "allocated", "unallocated" (i.e., no pointers been placed here yet), or "freed".  If something has been freed, make sure it doesn't get used or freed again until a new value has been assigned to it.

There are LOT of places where you'll be able to do more, but this is already a LOT more work than I was originally thinking (hence me providing so much scaffolding above).

For example, technically pointers may point to arrays, so more work would be needed to handle that, both to create the arrays (perhaps with a `MakeAptrArray` helper function) and other operators (such as `operator[]`), but that wouldn't be needed unless you made an advanced version of this class.

One of the BIG areas that you would need to work with (in a more advanced version of this class!!) is that multiple pointers may all point to the same memory.  If so, you would need to see if ANY of those freed the memory so that you would know that none of the others should do anything with it any more.  Similar, this would be needed in order to be able to warn the programmer if they forgot to free the memory at all.

Lots of other "advanced" features:
- void pointers should have a host of special rules, if we even want to allow them.
- type casting should be possible
- pointers should be able to handle shifts to base classes automatically
- the `operator->*` should work
(and on and on)

But if you do pick this class to build a more advanced version of, we can talk through all of these options some more and pick what you'd want to do.
