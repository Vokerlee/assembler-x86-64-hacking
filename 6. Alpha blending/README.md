# Alpha blending with SSE optimization

The purpose of this program is to compose two photos to place the cat on the path in the forest:) (Alpha blending mechanism is used)
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

To compose the photos the standard algorithm of alpha blending is applied (composing two pictures with different alpha channels)

<img src="Photos//Scheme.png" alt="drawing" width="600"/>

It can been described with the following code (`src_colors` contains the pixel data RGBA in first 32 bits, similarly `dst_colors`):

```C++
// Mask for rearraging RGB colours in pixel data
// 0x80 = 0b10000000 for zeroing, others are indeces of pixels
__m128i mask_expand_bytes = _mm_setr_epi8(0, 0x80, 0x80, 0x80, 1, 0x80, 0x80, 0x80, 2, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80);
dst_colors   = _mm_shuffle_epi8(dst_colors, mask_expand_bytes);
src_colors   = _mm_shuffle_epi8(src_colors, mask_expand_bytes);

// Alpha blending multiplication
__m128i dest_colors_alpha_applied = _mm_mullo_epi32(dst_colors, alpha_vec);
__m128i src_colors_alpha_applied  = _mm_mullo_epi32(src_colors, src_alpha_vec);
__m128i res_colors = _mm_add_epi32(src_colors_alpha_applied, dest_colors_alpha_applied);

// To take only the lowest bits after multiplication
__m128i shift = _mm_setr_epi32(MAX_ALPHA_POW, 0, 0, 0);
res_colors    = _mm_sra_epi32(res_colors, shift);

// 0, 4, 8 - indeces to make the number RGBA model again
__m128i mask_compress_bytes = _mm_setr_epi8(0, 4, 8, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80);
res_colors = _mm_shuffle_epi8(res_colors, mask_compress_bytes);
```

What concerns new alpha value, we do it possible maximum. To boost the program execution SSE instructions are used to handle all 3 colours of pixel at the same time.
Doing this actions 10000 times, figure out that SSE program work `2.38` times faster than without SSE.

Here is the result of the program:

<p align="center">
    <img src="Photos//composed.jpg" alt="drawing" width="600"/>
</p>

