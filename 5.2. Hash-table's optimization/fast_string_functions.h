#pragma once
#include "config.h"

inline size_t ultra_strlen(const char* str)
{
    unsigned long* chunk = reinterpret_cast<unsigned long*>(const_cast<char*>(str));
    int res = 0;

    __asm
    {
            xor ecx, ecx
            mov esi, chunk
            xor ebx, ebx
            not ebx

        fast_len_loop:
            cmp ecx, 6
            je fast_len_loop_end

            mov eax, [esi]
            xor eax, ebx
            inc ecx

            cmp eax, ebx
            jne fast_len_loop

        fast_len_loop_end:
            shl ecx, 3
            mov res, ecx
    }

    return res;
}

inline size_t fast_strlen(const char* str)
{
    unsigned int counter = 0;
    const __m256i* ptr = (__m256i*) str;
    __m256i zero = _mm256_setzero_si256();

    while (1) 
    {
        __m256i mask = _mm256_cmpeq_epi8(*ptr, zero);

        if (!_mm256_testz_si256(mask, mask)) 
            return (counter * 32) + strlen((char*)ptr);

        counter++;
        ptr++;
    }
}

inline bool ultra_strcmp(char* str1, char* str2)
{
    unsigned long* chunk_arr1 = reinterpret_cast<unsigned long*>(str1);
    unsigned long* chunk_arr2 = reinterpret_cast<unsigned long*>(str2);
    unsigned long int res = 0;

    __asm
    {
            mov edi, chunk_arr1
            mov esi, chunk_arr2
            mov ecx, 6

        cmp_loop:
            test ecx, ecx
            je cmp_loop_end

            dec ecx
            mov eax, [edi]
            mov ebx, [esi]
            xor eax, ebx

            add esi, 4
            add edi, 4

            test eax, eax
            je cmp_loop

        cmp_loop_end:
            mov res, eax
    }

    return res;
}

int fast_strcmp(const char* s1, const char* s2) 
{
    if (s1 == s2) 
        return 0;

    __m128i* ptr1 = reinterpret_cast<__m128i*>(const_cast<char*>(s1));
    __m128i* ptr2 = reinterpret_cast<__m128i*>(const_cast<char*>(s2));

    for (; ; ptr1++, ptr2++) 
    {

        const __m128i a = _mm_loadu_si128(ptr1);
        const __m128i b = _mm_loadu_si128(ptr2);

        const uint8_t mode =
            _SIDD_UBYTE_OPS |
            _SIDD_CMP_EQUAL_EACH |
            _SIDD_NEGATIVE_POLARITY |
            _SIDD_LEAST_SIGNIFICANT;

        if (_mm_cmpistrc(a, b, mode))
        {
            const auto idx = _mm_cmpistri(a, b, mode);

            const uint8_t b1 = (reinterpret_cast<char*>(ptr1))[idx];
            const uint8_t b2 = (reinterpret_cast<char*>(ptr2))[idx];

            if (b1 < b2)
                return -1;
            else if (b1 > b2)
                return +1;
            else
                return 0;
        }
        else if (_mm_cmpistrz(a, b, mode))
            break;
    }

    return 0;
}