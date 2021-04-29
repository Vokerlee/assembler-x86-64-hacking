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
                int src_position = (i * other.width() + j)       * BYTES_PER_PIXEL;   // wherefrom get
                int dst_position = ((y + i) * width() + (x + j)) * BYTES_PER_PIXEL;   // whither to place

                for (int k = 0; k < 3; k++)
                {
                    unsigned char src_color = other.bitmap_[src_position + k];
                    unsigned char dst_color = bitmap_[dst_position + k];

                    unsigned char src_alpha = other.bitmap_[src_position + 3];        // get alpha byte from source picture
                    unsigned char src_alpha_neg = MAX_ALPHA - src_alpha;

                    short int res_color = src_color * src_alpha + dst_color * src_alpha_neg;

                    bitmap_[dst_position + k] = res_color >> MAX_ALPHA_POW;
                }

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


