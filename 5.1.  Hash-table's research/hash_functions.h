#pragma once

#include <cstdint>
#include <cstring>
#include "config.h"

class ConstantHash
{
public:
    ConstantHash() = default;
    ~ConstantHash() = default;

    unsigned int operator()(char* string)
    {
        return TABLE_SIZE / 2;
    }
};

class FirstByteHash
{
public:
    FirstByteHash() = default;
    ~FirstByteHash() = default;

    unsigned int operator()(char* string)
    {
        return *string;
    }
};

class StrlenHash
{
public:
    StrlenHash() = default;
    ~StrlenHash() = default;

    unsigned int operator()(char* string)
    {
        return strlen(string);
    }
};

class ASCIILenHash
{
public:
    ASCIILenHash() = default;
    ~ASCIILenHash() = default;

    unsigned int operator()(char* string)
    {
        int sum = 0;
        int len = strlen(string);

        while (*string)
        {
            sum += *string;
            ++string;
        }

        return sum / len;
    }
};

class ASCIIXorRotate
{
public:
    ASCIIXorRotate() = default;
    ~ASCIIXorRotate() = default;

    unsigned int operator()(char* string)
    {
        if (!*string)
        {
            return 0;
        }

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

        return hash;
    }
};

class LibrarianHash
{
public:
    LibrarianHash() = default;
    ~LibrarianHash() = default;

    unsigned int operator()(char* string)
    {
        return std::hash<char*>{}(string);
    }
};

class CRC32Hash
{
public:
    CRC32Hash() = default;
    ~CRC32Hash() = default;

    unsigned int operator()(char* string)
    {
        int length = strlen(string);
        unsigned long crc_table[256];
        unsigned long crc;

        for (int i = 0; i < 256; i++)
        {
            crc = i;
            for (int j = 0; j < 8; j++)
                crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
            crc_table[i] = crc;
        };

        crc = 0xFFFFFFFFUL;

        while (length--)
            crc = crc_table[(crc ^ *string++) & 0xFF] ^ (crc >> 8);

        return (crc ^ 0xFFFFFFFFUL) % TABLE_SIZE;
    }
};

class MurmurHash2
{
public:
    MurmurHash2() = default;
    ~MurmurHash2() = default;

    unsigned int operator()(char* string)
    {
        int length = strlen(string);

        const unsigned int m = 0x5bd1e995;
        const unsigned int seed = 0;
        const int r = 24;

        unsigned int h = seed ^ length;

        const unsigned char* data = (const unsigned char*) string;
        unsigned int k;

        while (length >= 4)
        {
            k = data[0];
            k |= data[1] << 8;
            k |= data[2] << 16;
            k |= data[3] << 24;

            k *= m;
            k ^= k >> r;
            k *= m;

            h *= m;
            h ^= k;

            data += 4;
            length -= 4;
        }

        switch (length)
        {
            case 3:
                h ^= data[2] << 16;
            case 2:
                h ^= data[1] << 8;
            case 1:
                h ^= data[0];
                h *= m;
        };

        h ^= h >> 13;
        h *= m;
        h ^= h >> 15;

        return h;
    }
};

class JenkinsHash
{
public:
    JenkinsHash() = default;
    ~JenkinsHash() = default;

    unsigned int operator()(char* string)
    {
        uint32_t i = 0;
        uint32_t len = strlen(string);
        uint32_t hash = 0;

        while (i != len)
        {
            hash += string[i++];
            hash += hash << 10;
            hash ^= hash >> 6;
        }

        hash += hash << 3;
        hash ^= hash >> 11;
        hash += hash << 15;

        return hash;
    }
};
