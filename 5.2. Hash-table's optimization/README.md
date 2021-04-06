# Hash-table's optimization
Let's optimize the  program, that was considered in the [last chapter](https://github.com/Vokerlee/Assembly/tree/main/5.1.%20%20Hash-table's%20research) of hash-table's research. But, first, let's look at a little theory.

## What's happening
The work algorithm of hash-table:
1. Reading words from some source.
2. Calculating a hash-number of these words.
3. This hash-number is the index in massive of our hash-table.
4. Appropriate value is put in the massive.
5. If some words have the same hash (that means the same index in massive), collision appears. To solve this problem lists are used. This picture explains everything:

<img src="Readme pictures//Hashtable.png" alt="drawing" width="600"/>

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

Wow wow wow! Function `CRC32Hash::operator()` takes 98% program execution! We need to fix it as soon as possible!

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
Urgently fix this by counting the table only 1 time:
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

But it not all! If think better, the idea to use intrinsics optimization come:



All the code can be rewritten to assembler, so the function `strlen` can be.
Using our super brain, create the same code in assemler:

```asm
inline unsigned int operator()(char* string)
{
    uint32_t len = fast_strlen(string);
    uint32_t hash = 0;

    __asm
    {
            mov eax, hash
            mov ecx, len
            mov esi, string

        jenkins_loop:
            cmp ecx, 0
            je jenkins_loop_end
            dec ecx

            xor ebx, ebx
            mov bl, [esi + ecx]
            add eax, ebx

            mov ebx, eax
            shl ebx, 10
            add eax, ebx

            mov ebx, eax
            shr ebx, 6
            xor eax, ebx

            jmp jenkins_loop

        jenkins_loop_end:
            mov ebx, eax
            shl ebx, 3
            add eax, ebx

            mov ebx, eax
            shr ebx, 11
            xor eax, ebx

            mov ebx, eax
            shl ebx, 15
            add eax, ebx

            mov hash, eax
    }

    return hash;
}
```
Here is the realization of `fast_strlen`:

```asm
inline int fast_strlen(char* str)
{
    unsigned long* chunk = reinterpret_cast<unsigned long*>(str);
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
## Code optimization №2

Now let's look at the `List<char *>::contains`:
```C++
bool contains(T value) const noexcept
{
    Node<T>* cur = back_;
    
    while (cur && strcmp(cur->value, value) != 0)
        cur = cur->prev;

    return cur ? true : false;
}
```

As we can see, the essential part of this function is `strcmp`. So let's rewrite it!

```asm
inline bool fast_strcmp(char* str1, char* str2)
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
## Summarizing
And now we are ready to test the program again! So let it be!

<img src="Readme pictures//Optimized times.png" alt="drawing" width="800"/>

WOW! We get `23%` boost of the program execution!
