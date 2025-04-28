## <span id="group-6" style="color: blue">Group 6: Lamport</span>

**Members**: Emil Rahn-Siegel, Will Crawford, Ivan Bega, Mehmet Efe Caylan, and Orhan Aydin

**Shared Interests**: Games / Data Processing / Scripting

**Notable Strengths**: Algorithms, Databases

### Classes to Build

| Class               | Description |
| ------------------- | ----------- |
| **`ArgManager`**    | A tool to take in the "argc" and "argv" values supplied by the commend line and convert them into flags and settings.  Once option is to set up the ArgManager to load in all of the command-line options (and how many arguments each takes) before you pass in argc and argv, then options can be triggered automatically and a "--help" (often shortened to '-h') can automatically generate a list of all of the options.  More advanced versions might also handle URL query params for web application or interface with config files. |
| **`CommandLine`**   | A call for managing a command-line process.  Programmers should be able to specify a set of known commands and which functions should be called if those commands are triggered.  Such a setup would allow end users to provide traditional command-line inputs and this object automatically takes care of the routing it to the correct functionality.  Ideally it would include extra tools like history tracking and state tracking. |
| **`ErrorManager`**  | A set of tools to manage errors that occur, providing a clear message to the user (perhaps even color-coding? or appearing in a web browser). You should be able to configure the class to specify an alternative action to take when an error occurs (for example, a command-line app might want to intercept the error to use as feedback). You should also be able to have multiple levels of errors (at least separate warnings and fatal errors) |
| **`StaticString`**  | Like an [`std::string`](https://en.cppreference.com/w/cpp/string/basic_string), but a maximum number of characters is set up front with all memory allocated at once.  Size cannot go over the static limit, but makes for fast class when manipulating many strings. |
| **`StringSet`**     | A container with a set of strings and various methods to simplify using the container.  You can choose to set this up with regular `std::string` objects or with StaticString, or else you can make it a template so that the programmer can decide which type to use inside of it. The container should be able to handle operations like filter (to keep only strings that meet a certain criterion), filter out (to remove strings that meet a criterion) and well as union, intersection, and difference between two different StringSets. |

### Application Suggestion

A **Word-manipulation Language**. Build a simple scripting language that allows you to load words from a word-list file and then filter those words by sequence, length, etc.  This language should be useful for creating or solving word games.  For example, if you were trying to solve a Wordle, you could filter the list to words of length 5, and that have or do not have letters and particular positions or at all in a word.  This system should also make it easy to make crossword puzzle or play games like Scrabble. You should use your CommandLine class to use this language interactively.  Note that this project is similar to a language Dr. Ofria started to make in CSE 450, but that project didn't get far.

Suggested **challenges**: Add some puzzle-specific features into the language.  For example, providing an exact result after a Wordle guess and automatically filtering the possibilities down (Or asking it what the next best word to guess would be given the current word list remaining.) For Scrabble, it could know letter scores, so if you get it available letters it could help you find the maximum scoring word.
