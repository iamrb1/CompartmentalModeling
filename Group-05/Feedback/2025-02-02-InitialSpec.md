# Feedback on initial specifications

## `Datum` class

Good ideas for implementation.

You shouldn't need `SetStringValue` and `SetDoubleValue` as functions.  Just make overrides of `operator=` that take `string`, `double`, and (of course) `Datum` itself as arguments.  The correct override will be called based on what you're setting it to.

## `ReferenceVector` class

It sounds like you decided to start with a very limited version of this class that can only keep references to `Datum` objects.  This is a fine plan, though I think you will quickly find that it won't be too hard to make a more general version.

Indexing should return `Datum &` (i.e., references), not `Datum *` (pointers).  They will be stored as pointers internally, but from the outside they should always look like references.

The function you need to have is `void SetReference(size_t index, Datum & var)`, in order to set what a position in the vector should refer to.  Under the hood it would then keep the pointer to var.  So you might have as the member variable `std::vector<Datum *> references;`.  When `var` gets passed in to `SetReference`, you need to save it's original location, so you do `references[index] = &var`.  When you need to return it, you want to convert it back to a reference.  So if you needed to return index `i`, you would return `*references[i]`, with the `*` doing the conversion to reference for you.

I'll talk about this more in `DataGrid` below.

As for the challenges, I think you are correct that you don't need to worry about the proxy objects at this time; they would only be needed for more esoteric functionality.

## `DataGrid` class

A `DataGrid` should not need to use a `ReferenceVector` internally -- it would be find to just use `std::vector` if you want.  To explain my comment in the original description, imagine that you created the `DataGrid` internals as: 

```
std::vector< std::vector< Datum > > data;
```

You can actually make this slightly easier to work with by breaking up the type:

```
using row_t = std::vector< Datum >;  // Each row is a vector of Datum.
std::vector< row_t > data;           // The table is a vector of rows.
```

So, if you want to implement a `GetRow()` member function that allows you to change the row (and indeed, this is what `operator[]` should do as well), you can use:

```
row_t & GetRow(size_t row_id) { return data[row_id]; }
```

But how would you implement a `GetCol()` function where you can modify the column too?

That's where a `ReferenceVector` comes in.  It will _refer_ to all of the correct positions in the grid.  Here's what the function might look like:

```
using col_t = ReferenceVector<Datum>;
col_t GetCol(size_t col_id) {
  const size_t num_rows = data.size();
  col_t out_col(num_rows);
  for (size_t row_id = 0; row_id < num_rows; ++row_id) {
    out_col.SetReference(row_id) = data[row_id][col_id];
  }
  return out_col;
}
```

The returned ReferenceVector will behave just like `std::vector<Datum>`, but will "refer" all changes back to the correct place in the original data grid.

Hopefully this makes more sense!


## `CSVfile` class

Assuming we get `DataGrid` working well, above, it seems like you have a much better feel for how to work with `CSVFile`.  You've already identified one aspect of this in challenges, but make sure you have a good, distinct way to store strings, even if they have quotes or other special characters in them.  This is a classic issue with Databases, so lots of good information is out there.


## ExpressionParser

I realize that this C++ class dips more into compilers than I had originally intended (for this group, at least).  To make it MUCH more straightforward, we will disallow multiple operators in a single expression.  Get it working with a SINGLE operator, so you don't need to worry about order of operations or anything complicated like that.  If you choose this C++ class to scale up in complexity, I can talk with you about different ways for getting more complex expressions to parse easily.  (Since it's only expressions with a fixed set of grammatical rules, even that won't be too bad).

Interestingly, even with this simplification, we will still have full functionality, it will just require more additional columns as we build out any complex operation, one step at a time.
