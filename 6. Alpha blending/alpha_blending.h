#pragma once

#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <cassert>
#include <ctime>
#include <iostream>

#include <emmintrin.h>
#include <pmmintrin.h>
#include <xmmintrin.h>
#include <smmintrin.h>

const int BYTES_PER_PIXEL     = 4;
const unsigned char MAX_ALPHA = 255;
const int MAX_ALPHA_POW       = 8;

// Here is the structure of BMP file
#pragma pack(push, 2)
struct CIEXYZ
{
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t z = 0;
};

struct CIEXYZTRIPLE
{
    CIEXYZ ciexyzRed   = {};
    CIEXYZ ciexyzGreen = {};
    CIEXYZ ciexyzBlue  = {};
};

struct BITMAPFILEHEADER
{
    uint16_t bfType      = 0;
    uint32_t bfSize      = 0;
    uint16_t bfReserved1 = 0;
    uint16_t bfReserved2 = 0;
    uint32_t bfOffBits   = 0;
};

struct BITMAPINFO
{
    uint32_t bV5Size          = 0;
    uint32_t bV5Width         = 0;
    uint32_t bV5Height        = 0;
    uint16_t bV5Planes        = 0;
    uint16_t bV5BitCount      = 0;
    uint32_t biV5Compression  = 0;
    uint32_t bV5SizeImage     = 0;
    uint32_t bV5XPelsPerMeter = 0;
    uint32_t bV5YPelsPerMeter = 0;
    uint32_t bV5ClrUsed       = 0;
    uint32_t bV5ClrImportant  = 0;
    uint32_t bV5RedMask       = 0;
    uint32_t bV5GreenMask     = 0;
    uint32_t bV5BlueMask      = 0;
    uint32_t bV5AlphaMask     = 0;
    CIEXYZTRIPLE bV5Endpoints = {};
    uint32_t bV5GammaRed      = 0;
    uint32_t bV5GammaGreen    = 0;
    uint32_t bV5GammaBlue     = 0;
    uint32_t bV5Intent        = 0;
    uint32_t bV5ProfileData   = 0;
    uint32_t bV5ProfileSize   = 0;
    uint32_t bV5Reserved      = 0;
};

struct BMPHeader
{
    BITMAPFILEHEADER file_header;
    BITMAPINFO       info_header;
};
#pragma pack(pop)
// End of structure

class BMPFile 
{
private:
    std::unique_ptr<unsigned char[]> data_;
    unsigned char* bitmap_;

    int size_ = 0;
    BMPHeader header_ = {};

    class file_closer
    {
    public:
        file_closer() = default;
        ~file_closer() = default;

        void operator()(FILE* file_pointer)
        {
            if (file_pointer)
                fclose(file_pointer);
        }
    };

public:
    BMPFile() noexcept : size_(0) {};
    ~BMPFile() = default;

    BMPFile(const BMPFile& other) = delete;

    BMPFile& operator=(const BMPFile& other) = delete;

    BMPFile(BMPFile&& other) noexcept
    {
        std::swap(*this, other);
    }

    BMPFile& operator=(BMPFile&& other) noexcept
    {
        std::swap(*this, other);
        return *this;
    }

    BMPFile(const char* filename)
    {
        auto bmp_file = std::unique_ptr<FILE, file_closer>(fopen(filename, "rb"), file_closer());

        if (!bmp_file.get())
            throw std::runtime_error("This file does not exist!");

        fseek(bmp_file.get(), 0, SEEK_END);
        size_ = ftell(bmp_file.get());
        fseek(bmp_file.get(), 0, SEEK_SET);

        data_ = std::unique_ptr<unsigned char[]>(new unsigned char[size_]());
        fread(data_.get(), sizeof(unsigned char), size_, bmp_file.get());

        memcpy(&header_, data_.get(), sizeof(BMPHeader));
        bitmap_ = data_.get() + header_.file_header.bfOffBits;
    }

    int size() const noexcept
    {
        return size_;
    }

    int width() const noexcept
    {
        return header_.info_header.bV5Width;
    }

    int height() const noexcept
    {
        return header_.info_header.bV5Height;
    }

    const unsigned char* data() const noexcept
    {
        return data_.get();
    }

    void save_to_file(const char* filename)
    {
        auto bmp_file = std::unique_ptr<FILE, file_closer>(fopen(filename, "wb"), file_closer());
        fwrite(data_.get(), sizeof(unsigned char), size_, bmp_file.get());
    }

    void compose_alpha(const BMPFile& other, int x, int y)
    {
        if (x + other.width() > width() || y + other.height() > height())
            throw std::runtime_error("Can not place the picture: it is too big!!!");

        for (int i = 0; i < other.height(); ++i)
        {
            for (int j = 0; j < other.width(); ++j)
            {
                int src_position  = (i * other.width() + j) * BYTES_PER_PIXEL;       // wherefrom get
                int dst_position  = ((y + i) * width() + (x + j)) * BYTES_PER_PIXEL; // whither to place

                unsigned char src_alpha = other.bitmap_[src_position + 3];           // get alpha byte from source picture

                // Load pixels data to the first 32 bits of vectors
                int src_pixel      = *(reinterpret_cast<int*>(other.bitmap_ + src_position));
                int* dst_pixel_ptr = reinterpret_cast<int*>(bitmap_ + dst_position);
                __m128i src_colors = _mm_cvtsi32_si128(src_pixel);
                __m128i dst_colors = _mm_cvtsi32_si128(*dst_pixel_ptr);
                 
                __m128i src_alpha_vec = _mm_set1_epi32(src_alpha);                   // load vector of 4 instances of 8-bit alpha
                __m128i alpha_vec     = _mm_set1_epi32(MAX_ALPHA - src_alpha);

                // Mask for rearraging RGB colours in pixel data
                // 0x80 = 0b10000000 for zeroing, others are indeces of pixels
                __m128i mask_expand_bytes = _mm_setr_epi8(0, 0x80, 0x80, 0x80, 1, 0x80, 0x80, 0x80, 2, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80);
                dst_colors = _mm_shuffle_epi8(dst_colors, mask_expand_bytes);
                src_colors = _mm_shuffle_epi8(src_colors, mask_expand_bytes);

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

                *dst_pixel_ptr = _mm_cvtsi128_si32(res_colors);
                bitmap_[dst_position + 3] = MAX_ALPHA;
            }
        }
    }

    friend void swap(BMPFile& first, BMPFile& second) noexcept
    {
        std::swap(first.data_, second.data_);
        std::swap(first.size_, second.size_);
        std::swap(first.header_, second.header_);
        std::swap(first.bitmap_, second.bitmap_);
    }
};


