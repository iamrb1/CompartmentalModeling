## <span id="group-8" style="color: blue">Group 8: Turing</span>

**Members**: Joanna Rodriguez Zamarron, Ryon Baldwin-Williams, Kelsi Elliott, Hannah Dale, and Caden McDonald

**Shared Interests**: Productivity / Social Media

**Notable Strengths**: Software Engineering, User Interfaces, Web Development

### Classes to Build

| Class               | Description |
| ------------------- | ----------- |
| **`AnnotatedWrapper`** | A base class that provides "annotating" features to any derived class.  A simple form of annotations would be tags associated with the class. |
| **`TagManager`**    | A pseudo-container to track string tags on entires and be able to quickly identify all entries associated with a particular tag. |
| **`DynamicString`** | An interspersed mix of [`std::string`s](https://en.cppreference.com/w/cpp/string/basic_string) and functions (presumably stored using [`std::function`](https://en.cppreference.com/w/cpp/utility/functional/function)).  It should have a `ToString()` function that will append each of these together, calling the functions and making the results into strings.  Functions will be called each time conversions occur, so the string can dynamically change over time.  For example, if a string has a time stamp in it, the current time would be printed.  Or you could track the current value of a variable: `DynamicString str("x = ", x);`. |
| **`RandomAccessSet`** | The [`std::set`](https://en.cppreference.com/w/cpp/container/set) class is supposed to rapidly tell whether an object instance is a member, as well as step through members, and add or remove members.  A random_access set should also keep an array back-end for when stepping through entries.  If you use an array-style index, you should be able to access a consistent element (unless you modify the contents of the set),  |
| *Audited Pointer* | Template class that behaves like a raw pointer outside of "DEBUG" mode, but does extra correctness checks when debugging.  For example once a pointer is deleted, it should not allow you to dereference it (or delete it a second time).  What a program terminates, it should check to make sure that all AuditedPointers have been deleted. You can either override C++'s `new` and `delete` or use `New()` and `Delete()` style member functions.  I also recommend a stand-along `MakeAudited` template function that works similarly to [`std::make_shared()`](https://en.cppreference.com/w/cpp/memory/shared_ptr/make_shared). |

### Application Suggestion

A **Social To-Do List**.  Many to-do lists exist online, and some can even be shared, but this would be a simple to use system for managing groups of people, perhaps planning an event or trying to complete a coding project for a CS course.  A user invites others onto the lists and then every can lay-claim to todo items.  Even better is if each todo item can be expanded into a sub-list, so that bigger projects can always be broken down into smaller pieces.  You should also be able to tag items and then search for all items with a given tag.

Suggested **challenges**: Adding security to a system like this would be valuable.  Another option would be to add an old-fashioned AI-helper that identifies todo items that are important an have sat around to long.  If you mark how long you expect items to take, it could even suggest taking a bigger item and breaking it down into a smaller list as an action you might want to do.

