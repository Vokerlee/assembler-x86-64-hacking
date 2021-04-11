# Hash-table's optimization
Let's optimize the program, that was considered in the [last chapter](https://github.com/Vokerlee/Assembly/tree/master/5.1.%20%20Hash-table's%20research) of hash-table's research. But, first, let's look at a little theory.

## What's happening
The work algorithm of hash-table:
1. Reading words from some source.
2. Calculating a hash-number of these words.
3. This hash-number is the index in array of our hash-table.
4. The appropriate value is put in array.
5. If some words have the same hash (that means the same index in array), collision appears. To solve this problem lists are used. This picture explains everything:

<img src="Readme pictures//Hashtable.png" align="center" alt="drawing" width="600"/>

## Code analysis
Okey. Let's create an imitation of some useful work:
```C++
int kek = 0;
for (int i = 0; i < N_SAMPLES; ++i)
{
    int word_index = i % N_WORDS;
    kek = kek + i + table.contains(words + word_index * WORD_SIZE);
}

printf("Kek = %d", kek);
```
To research code we will use CRC32 hash function. Using time-profiler, get the following time-distribution of the program:

<img src="Readme pictures//Step1.png" alt="drawing" width="800"/>

Wow wow wow! Function `CRC32Hash::operator()` takes 98% of program execution! We need to fix it as soon as possible!

## Code optimization №1

First of all let's look at the `CRC32Hash::operator()` function. We have:

```C++
class CRC32Hash
{
public:
    CRC32Hash() = default;
    ~CRC32Hash() = default;

    unsigned int operator()(char* string)
    {
        int length = strlen(string);
        unsigned long crc = 0xEDB88320UL;

        unsigned long crc_table[256];

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

        return (crc ^ 0xFFFFFFFFUL);
    }
};
```

It is awful! CRC-table `C++ unsigned long crc_table[256]` counts every time! Again and again! 
Urgently fix it by counting the table only one time:

```C++
class CRC32Hash
{
public:
    unsigned long crc = 0xEDB88320UL;
    unsigned long crc_table[256];

    CRC32Hash()
    {
        for (int i = 0; i < 256; i++)
        {
            crc = i;

            for (int j = 0; j < 8; j++)
                crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;

            crc_table[i] = crc;
        };
    }

    ~CRC32Hash() = default;

    unsigned int operator()(char* string)
    {
        int length = strlen(string);

        crc = 0xFFFFFFFFUL;

        while (length--)
            crc = crc_table[(crc ^ *string++) & 0xFF] ^ (crc >> 8);

        return (crc ^ 0xFFFFFFFFUL);
    }
};
```

Now we have:

<img src="Readme pictures//Step2.png" alt="drawing" width="800"/>

## Code optimization №2
But it not all! If think better, the idea to use intrinsics optimization come:

```C++
class CRC32Hash
{
public:
    CRC32Hash() = default;
    ~CRC32Hash() = default;

    unsigned int operator()(char* string)
    {
        int length = strlen(string);
        unsigned long crc = 0xFFFFFFFFUL;
        size_t iters = length / sizeof(uint32_t);

        for (size_t i = 0; i < iters; ++i)
        {
            crc = _mm_crc32_u32(crc, *(const uint32_t*)string);
            string += sizeof(uint32_t);
        }

        return crc;
    }
};
```

<img src="Readme pictures//Step3.png" alt="drawing" width="800"/>

Cool. Now let's turn on debug regime to know how other functions work:

<img src="Readme pictures//Debug.png" alt="drawing" width="800"/>

## Code optimization №3 (fail)

The first functions here are our purpose to boost. It's easy to notice that out hash function owns `strlen` call. We can get rig of it, but let's try to optimize it with AVX2 for the sake of interest:

```C++
inline size_t fast_strlen(const char* str)
{
    unsigned int counter = 0;
    const __m256i* ptr = (__m256i*) str;
    __m256i zero = _mm256_setzero_si256();

    while (1) 
    {
        __m256i mask = _mm256_cmpeq_epi8(*ptr, zero);

        if (!_mm256_testz_si256(mask, mask)) 
            return (counter * 32) + simple_strlen((char*)ptr);

        counter++;
        ptr++;
    }
}
```

Unfortunately, it dosen't help:

<img src="Readme pictures//Step4.png" alt="drawing" width="800"/>

The reason is that we have short words and it is silly to use handler of 32 words in 1 pass. All other variants will slow down the function.

## Code optimization №3 2.0 (fail again)

Do not despair! There is a sense to do the following:

```asm
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
```

But this time we lose again...

<img src="Readme pictures//Step5.png" alt="drawing" width="800"/>

## Code optimization №3 3.0 (use trick)

Okey. It seems that librarian `strlen` is fast enough. So exclude this function, because it is the best decision:

```C++
class CRC32Hash
{
public:
    CRC32Hash() = default;
    ~CRC32Hash() = default;

    unsigned int operator()(char* string)
    {
        unsigned long crc = 0xFFFFFFFFUL;

        for (; *string != 0; string += sizeof(uint32_t))
            crc = _mm_crc32_u32(crc, *(const uint32_t*)string);
     
        return crc;
    }
};
```
The result is:

<img src="Readme pictures//Step7.png" alt="drawing" width="800"/>

## Code optimization №4 (fail...)

We still have `List<char *>::contains` function, where the other function `hash_table<char *, CRC32Hash>::contains` also rests.

Consider it closer:

```C++
bool contains(T value) const noexcept
{
    Node<T>* cur = back_;

    while (cur && strcmp(cur->value, value) != 0)
        cur = cur->prev;

    return cur ? true : false;
}
```

And this time function `strcmp` challenge us! (We have to win). Сall to mind, that SSE and AVX2 are unuseful in our case. So we have the last chance to boost the program.
Actions are already familiar:

```asm
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
```

Oh, shit... Here we go again.

<img src="Readme pictures//Step6.png" alt="drawing" width="800"/>

## Summarizing
We have lost several battles against the compiler, but not the war!

Taking into consideration, that compiler has O2 optimizatoion we got `99,1835782%` boost of the program execution! (Or 122 times)
