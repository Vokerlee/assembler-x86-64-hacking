# Research of hash functions, based on hash table with chaining method 

This lab investigates occupancy rate of different hash functions.
The size of hash table is 1009 (simple number), the number of words is 24000.

## Constant hash

This function always returns index 1009 / 2 = 504.
In this case the variance of the distribution is `D = 570860`.

<img src="Data//Constant hash.png" alt="drawing" width="440"/>

## Strlen hash

Lets develop our function. Add some unique input data characteristic, for example the length of the data.

In this case the variance of the distribution is `D = 63139`. That's better.

<img src="Data//Strlen hash.png" alt="drawing" width="440"/>

## Sum of ASCII numbers hash

It seems, that if we summarize the ASCII codes of all bytes in the data, it will be much better.

Calculate again the variance of the distribution: `D = 49370`. Better expected!

<img src="Data//ASCIILen hash.png" alt="drawing" width="440"/>

## First symbol hash

In two las pictures we can see, that almost all the data values have similar hashes, because the lengths of the words are similar, so the ASCII sum.

Lets check for fun, what will be, if the we make the hash the first byte of the data:

`D = 49370`. And there was no need to be clever!

<img src="Data//FirstByte hash.png" alt="drawing" width="440"/>


