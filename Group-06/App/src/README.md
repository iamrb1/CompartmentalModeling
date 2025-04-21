# WORDLANG

## Welcome to the WORDLANG:

WordLang is a simple scripting language that allows to operate on a list of words. Lists can be created either from a default word database, loaded from previously saved lists by user, or combined from existing lists.

To run WordLang, change your directory in your terminal to `Group-06\App\src\`
under src directory run the `make` command to compile the program. Make file will
create an executable named `.\app.exe` which you can run it on your terminal by writing
`.\app.exe` this should start the program. 

```
cd Group-06\App\src
make
.\app.exe
```

### Word File Addition ###
If you want to add your own datafiles, make sure to locate them under `Group-06\App\database\`.
When you want to use your own word file specify it in the LIST LOAD command with the file type extension.
***Make sure that all the words are line separated in your file. Check the current files for referance.***


## COMMANDS TO RUN:

**<listname>** is a simple string command such as list1, list2 to the terminal.
**“filename/word database”**, if quotes denoted below for inputs, make sure to use them in the terminal as well.

**LOAD**
Load words from the file into a list of word. 
By default when a list created it is set as current working list.
```
LIST list1 = LOAD "my_test_file.txt"
```

**COMBINED**
Combines lists into one main list.
```
LIST list1 = LOAD "my_test_file1.txt"
LIST list2 = LOAD "my_test_file2.txt"
LIST list3 = LOAD "my_test_file3.txt"

LIST result = COMBINED list1 list2 list3
```

**DIFFERENCE**
Differences lists into one main list. Combines lists into one main list by only items are not same.
By default when a list created it is set as current working list.
```
LIST list1 = LOAD "my_test_file1.txt"
LIST list2 = LOAD "my_test_file2.txt"
LIST list3 = LOAD "my_test_file3.txt"

LIST result = DIFFERENCE list1 list2 list3
```

**INTERSECTION**
Intersect lists into one main list. Combines lists into one main list by only items are same.
By default when a list created it is set as current working list.
```
LIST list1 = LOAD "my_test_file1.txt"
LIST list2 = LOAD "my_test_file2.txt"
LIST list3 = LOAD "my_test_file3.txt"

LIST result = INTERSECTION list1 list2 list3
```

**COPY**
Copies a list to another list.
```
LIST list1 = LOAD "my_test_file1.txt"

LIST coppied = COPY list1
```

**SET_CURRENT**
Sets the current working lists for operations and searches.
```
LIST list1 = LOAD "my_test_file1.txt"
LIST list2 = LOAD "my_test_file2.txt"
LIST list3 = LOAD "my_test_file3.txt"

SET_CURRENT list1 list2
```

**ADD**
Adds the given words to the list specified. 
```
LIST list1 = LOAD "my_test_file1.txt"

ADD list1 "test words are listed as space separated"
```

**SAVE**
Saves the given list into file name specified, creates if file doesn't exists. 
```
LIST list1 = LOAD "my_test_file1.txt"

SAVE list "result_file.txt"
```

### Applying Paterns ###

**LENGTH**
Sets the current length of words in current lists set.
If '*' has been provided resets the length restriction.
```
LIST list1 = LOAD "my_test_file1.txt"

LENGTH = 5
LENGTH = *
```

**CONTAINS_ANY**
Filters the lists in current that each word must have at least one or more of letters given in contains_any.
```
LIST list1 = LOAD "my_test_file1.txt"

CONTAINS_ANY "aeiou"
```

CONTAINS_ANY “collection of letters” and CONTAINS_ALL “collection of letters” // orginize list based on given letters all in it or some 
NOT_CONTAINS “collection of letters” // orginize list based on given letters are not in the words in the list
GET “pattern”  // orginize the list based on pattern ‘_’ for a character, ‘*’ for 0 or more character
PRINT <number>  // prints results starting from top until number reached or ALL to print all results
RESET <list name> // resets the list to its original state

	Game Specifics:
 WORDLE(“word inputted “, “results of wordle”)  // For the results user inputs G(Green), Y(yellow), or B(Black). It returns top 10 possible words in the current list.
