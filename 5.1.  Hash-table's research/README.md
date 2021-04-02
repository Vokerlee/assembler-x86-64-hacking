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

## Strlen hash

It seems, that if we summarize the ASCII codes of all bytes in the data, it will be much better.

Claculate again the variance of the distribution: `D = 63139`. Better expected!

<img src="Data//ASCIIlen hash.png" alt="drawing" width="440"/>
