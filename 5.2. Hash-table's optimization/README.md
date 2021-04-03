# Hash-table's optimization
Let's optimize the  program, that was considered in the [last chapter](https://github.com/Vokerlee/Assembly/tree/main/5.1.%20%20Hash-table's%20research) of hash-table's research. But, first, let's look at a little theory.

## What's happening
The work algorithm of hash-table:
1. Reading words from some source.
2. Calculating the hash-number of this words.
3. This hash-number is the index in massive of hash-table.
4. Appropriate value put in the massive.
5. If some words have the same hash (that means the same index in massive), collision appears. To solve this problem lists are used. This picture explains everything:

<img src="Readme pictures//Hashtable.png" alt="drawing" width="600"/>
