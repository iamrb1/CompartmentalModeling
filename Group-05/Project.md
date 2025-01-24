## <span id="group-5" style="color: blue">Group 5: Hamilton</span>

**Members**: Max Krawec, Calen Green, Pedro Mitkiewicz, Shahaab Ali, and Muhammad Asif Masood

**Shared Interests**: Data Processing

**Notable Strengths**: Algorithms, Databases, Web Development

### Classes to Build

| Class               | Description |
| ------------------- | ----------- |
| **`Datum`**         | A simple data object that can be either a string or double, based on how it was last set.  Should have `AsString()` and `AsDouble()` member functions to convert if needed, as well as `IsString()` and `IsDouble()` to check the current type.  Basic operators should be implemented as well with intuitive behavior. You should also come up with rules for how to handle edge cases (for example, if `AsDouble()` is called on a string that can't easily be converted to a double, should it use 0? NaN (a special double value representing "not-a-number")? Or throw an exception?).|
| **`ReferenceVector`** | Similar to an [`std::vector`](https://en.cppreference.com/w/cpp/container/vector), but keeps pointers under the hood.  This allows entries in the vector to all be treated like references instead of direct objects.  You will likely need to return a [proxy class](https://stackoverflow.com/questions/994488/what-is-proxy-class-in-c) when the vector is indexed into in order to redirect appropriately. |
| **`DataGrid`**      | A DataGrid should manage a 2D grid of Datum values (strings or doubles). The user should be able to request a row or column to manipulate. Consider storing the grid as a collection of rows and then returnning a ReferenceVector when a column is requested. |
| **`CSVfile`**       | A file manager that tracks a DataGrid.  When a DataGrid is output, make sure to properly sanitize any strings.  When it is loaded, handle string vs. double types logically. |
| **`ExpressionParser`** | A tool to read in a string and convert it into a function object.  It should take an equation (as a string) and convert it to a function that takes a container as input that it uses to draw values from. For example, you might design it so that the string "{3} + {7}" would produce a function that takes a vector as an argument and adds together the contents of indices 3 and 7, returning the result. |

### Application Suggestion

A **CSV-manipulation app**.  This program would load in files in a comma-separated-values (CSV) format and allow the user to perform manipulations.  For example they might remove rows or columns, merge files, or reorder the tables. The use should be able to also write short equations (that you could process using the ExpressionParser above) to create new columns.

Suggested **challenges**: Setup scripts in order to process data in a step-by-step fashion.  Alternatively, you could make a much more powerful expression parser to all for some spreadsheet-level functionality.  Python's [Pandas](https://pandas.pydata.org/docs/user_guide/index.html) library might also provide some inspiration for many more advanced features that would be useful in a system like this.

