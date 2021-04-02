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

<img src="Data//Strlen hash.png" alt="drawing" width="400"/>

## Sum of ASCII numbers hash

It seems, that if we summarize the ASCII codes of all bytes in the data, it will be much better.

Calculate again the variance of the distribution: `D = 49370`. Better expected!

<img src="Data//ASCIILen hash.png" alt="drawing" width="440"/>

## First symbol hash

In two last pictures we can see, that almost all the data values have similar hashes, because the lengths of the words are similar, so the ASCII sum.

Lets check for fun, what will be, if the we make the hash the first byte of the data:

`D = 22304`. In some times less, than the previous variances!!! And there was no need to be clever!

<img src="Data//FirstByte hash.png" alt="drawing" width="440"/>

## Xor rotate (rol) hash

Stop playing children's games. Now everything is serious. Consider the following code of hash-calculating:
```C++
    uint32_t hash = 0;
    
    while (*string)
    {
        hash = hash ^ (*string);
        uint32_t oldest_bit = hash & 0x80000000;
        oldest_bit >>= 31;
        hash <<= 1;
        hash |= oldest_bit;
        ++string;
    }
```
This case diffres from all previous cases. Now we have good hash wuth `D = 35.12`.

<img src="Data//ASCIIXor hash.png" alt="drawing" width="440"/>


