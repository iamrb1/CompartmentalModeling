# Feedback on initial specifications

## `ArgManager` class

I think this write-up makes sense.  I recommend you try to come up with some use-cases for it.

For example:
```
int main(int argc, char *argv[]) {
  ArgManager args(argc, argv);  // Pass input args into manager for processing.
  if (args.Has("-h") || args.Has("--help")) {
    PrintHelp();
    exit(0);
  }
  // Set a default output filename, then Test if one was specified in the
  // command line arguments sent in.
  std::string out_filename="default_output.dat";
  if (args.Has("-o")) out_filename = args.GetOption("-o");
}
```

This would have you have the member function `Has` to check if a string is present in the input arguments, as well as `GetOption` which would find a string return the NEXT string as the value of that option.

The above would make it MUCH easier to write a command-line program with lots of options.

Unless there are particular tests you can do for `argc` and `argv` validity, I think you'll need to assume that the user is passing them through directly from the command line.  The user errors are spot on.

## `CommandLine` class

The text below is from our discussion on Discord; it highlights the main ideas for how this class works.

Let's imagine that I'm a user of the CommandLine class as part of an interface for a program where I'm making a text adventure game.  Here's something I might do:

```
// Create a set of functions 

void ShowInventory(const std::string & args) {
  // ... code to print out the current inventory...
  // ... if args list item names, perhaps give more information about them ..
}

void Command_Move(const std::string & args) {
  // ... change where the user is based on the args, e.g. "north" or "west" or "up" ...
  // ... print info about the new location ...
}

void Command_Look(const std::string & args) {
  // ... identify the item looked at and give more details about it ...
}

int main() {
  CommandLine cmdl;
  // Add the commands for the user.
  cmdl.Add("inventory", ShowInventory);
  cmdl.Add("move", Command_Move);
  cmdl.Add("look", Command_Look);

  // And some shortcuts...
  cmdl.Add("i", ShowInventory);
  cmdl.Add("m", Command_Move);
  cmdl.Add("l", Command_Look);
  cmdl.Add("inv", ShowInventory);

  // Now, let's start the playing!
  cmdl.Run();  // This should take user inputs, calling the correct functions as needed.
}
```

## `ErrorManager` class

You seem to have a solid idea for how this class should work, and a good starting point on the interface.  Don't worry about catching any errors with the class; this is just a helper class for programmers to report errors if they find them; the class doesn't need to identify the error itself.

## `StaticString` class

Keeping strings in range shouldn't be too difficult.  All you need to do is have a `resize` member function that always checks to ensure that the specified size is not larger than the static limit on size.

I do recommend add a `ToString()` member function that will convert to a traditional string.  That will help you test these and pass them around to places that don't know how to work with static strings, until you have all of the pieces working.  For this initial version, try to just get the basic functionality up and running.  If you decide to do an advanced version of this class, there is a LOT of extra stuff we can choose to add in.


## `StringSet`

I know in my original description I said that you could get this working with either regular `std::string` or your own `cse::StaticString`, but now that I'm looking through the plans I would actually encourage you to initially use `std::string`; you can change over later.

The reason for this is that you will need to have many different functionalities implemented in `StaticString` before it will be easy to use here.  For some simple examples, in order to use a binary tree (`std::set`), you would need to have `operator<` and `operator==`.  If you want to use a hash table (`std::unordered_set`) you would need to build a hashing function for StaticString.  And so on.  None of these are hard to do, but you'll hit one challenge after another if you use `StaticString` that you won't have to worry about with `std::string`.

That said, I do think that trying it with `StaticString` will give you good insights into all of the details that need to be taken care of in a class.

For the functions that you are talking about implementing, I don't have a good feel for exactly what interface you're thinking about.  Like what argument will `Filter` look take?  If I were going to do it, I'd probably make it take a function to call on everything in the list.

```
void Filter( std::function<bool(std::string & in)> filter ) {
  // ... run each string through the filter, keeping only those that return true...
}
```

But there are lots of other options too.
