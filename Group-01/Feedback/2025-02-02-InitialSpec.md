# Feedback on initial specifications

Overall: Some good ideas in here; your group seemed the most lost on the first one (`Distribution`) so I provided the most feedback on that one.

## `Distribution` class

You didn't provide any actual function names that you would be developing to indicate how this class would be used; your document is currently just vague generalities about what you will be implementing.  Similarly the related classes that you list seem haphazard with no indication of how you think they are connected.

You're right that `WeightedSet` could be used in the implementation of Distribution, though you have other options as well.  WeightedSet will have a lot of overhead to allow the weights to change on a regular basis with fast updating, while your probabilities will stay fixed once a Distribution is created.  That said, it could be a fast way to get this working.

One possible way for you to implement this class is with a `vector<double> probs` where the values at each index provide the probability associated with that index and add up to one.  You can then pair that with `vector<double> cumulative_probs` which will track the cumulative probabilities.  I'll get back to why that is helpful.

First, let's talk about how we might use a distribution.  Let's imagine a simple scenario where we want to create a Binomial distribution with p=0.5 and N=40.  This situation would be like asking: If we flipped a fair coin 40 times, how many would come up heads?  (For the results, See the red dots in the first images here: https://en.wikipedia.org/wiki/Binomial_distribution).

The wikipedia pages gives you some good info on calculating this in general, but in brief, if each of the `N` attempts has a `p` probability of success then to calculate the probability of EXACTLY k successes, you would need:

The probability of `k` events succeeding: `p^k`
The probability of the remaining `N-k` events failing: `(1-p)^(N-k)`
The number of different ways that this can happen, which is `N` choose `k` or: `N!/(k!(N-k)!)`

Multiplying these factors together, the probability of `k` successes is:
      `p^k * (1-p)^(N-k) * N!/(k!(N-k)!)`

(I can talk more about the probability theory behind this if you need; I'm not sure how much you've had in classes).

If I were going to use your `Distribution` class in code, I might do something like:

```
Distribution dist;
dist.BuildBinomial(40, 0.5);

// Print the probability of getting exactly 15 successes.
std::cout << dist.GetProb(15) << std::endl; // 0.03658
```

Now, I promised to get back around to why you might want `cumulative_probs`.  For one, this might be something nice to be able to return (e.g., `dist.GetCumulativeProb(15)` would tell us the probability of 15 _or fewer_ successes).  BUT if you want to be able to randomly draw from a distribution, you just need to pick a uniform random number between 0.0 and 1.0 and match it to where this distribution's cumulative probability crosses that value.  That would allow you to do a binary search through the cumulative probabilities and find the result in log time.

Hopefully this helps!  For the purposes of the initial version of this class, you should plan to have a couple of distributions, but one of them can be an easy one.  Maybe Binomial and Uniform?  If you choose this class for more advanced versions there are a LOT of good distributions to consider.

## `Graph` class

Overall, solid ideas for what a `Graph` class might looks like.  I'm not sure how the similar classes are related in some cases (some kind of an explanation would have been helpful), but hopefully that will be clear from the interface you put together.

The base class specifications show that you have a good picture for where to take this class moving forward.

## `GraphPosition`

Good writeup.  While some of the similar classes you list are more about classes you will use, I appreciate the links to why you listed each of them and agree with the logic.

Solid thought was placed into the functions that you will be implementing in order to make this a useful class.  Also, I like the level of analysis that went into identifying the challenges.  Regarding issues like "Handling cases where the graph is modified while it’s being traversed could be difficult if that is something that’s allowed.", you should be able to decide how this works.  It is fair to require that the graph does not change during traversal, but I can also imagine advanced features that you may decide to add to facilitate such possibilities.

## `Random`

A nice, clean description for a random class.  You should feel free to use some of the other (good) random implementations to power this random class; I think the class will make it all much easier to use than the current standard library tools.  Hopefully this will be a straight-forward implementation.

## `WeightedSet`

Good description.  I would recommend that you have some more functions, however.

`double TotalWeight()` would allow you to know the cumulative weight of everything in the set.
`double Weight(size_t id)` would give you the weight of just item `id`.
`size_t At(double position)` would return the ID at a given position in the weighted set.

So, with the above, if I added:
Item 0: Weight 3.3
Item 1: Weight 20.1
Item 2: Weight 0.5
Item 3: Weight 9.2

Total weight would be 33.1.  If I requested the weight of id 1, it would give me back 20.1.  And if I did an `At(23.5)` it would scan through the set and see that position 23.5 hits inside of item 2, so it would return a 2.

The beauty of this system is that random works with it naturally -- you just pick a `double` between `0.0` and `TotalWeight()`, and then use that to index into `At()` and it will give you a proper random item based on weight.

I hope all of this helps!
