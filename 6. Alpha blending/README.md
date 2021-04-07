# Alpha blending with SSE optimization

The purpose of this program is to compose two photos to place the cat on the path in the forest:)
In fact, the program is able to place every bmp photo on another photo.

## How bmp files work
To write the program it is necessary to learn how bmp files store:
1. Firstly it has special header:
```C++
struct BITMAPFILEHEADER
{
    uint16_t bfType      = 0;
    uint32_t bfSize      = 0;
    uint16_t bfReserved1 = 0;
    uint16_t bfReserved2 = 0;
    uint32_t bfOffBits   = 0;
};
```
2. `bfSize` is the size of the photo and `bfOffBits` is the address, where info about all pixels is (relatively to the start of the header).
3. After that short header bmp file contains special information about photo:
```C++
struct BITMAPINFO
{
    uint32_t bV5Size          = 0;
    uint32_t bV5Width         = 0;
    uint32_t bV5Height        = 0;
    uint16_t bV5Planes        = 0;
    ...
```

So the class `BMPFile`, containing all this info, was created.

## How to compose photos

To compose photos the standard algorithm of alpha blending is applied: 

<img src="Photos//Scheme.png" alt="drawing" width="600"/>

What concerns new alpha value, we do it possible maximum. To boost the program eexecution SSE instruction are used to handle all 3 colours of pixel at the same time.
Doing this actions 10000 times, figure out that SSE program work 2.38 time faster than without SSE.

Here is the result of the program:

<p align="center">
    <img src="Photos//composed.jpg" alt="drawing" width="600"/>
<\p>

