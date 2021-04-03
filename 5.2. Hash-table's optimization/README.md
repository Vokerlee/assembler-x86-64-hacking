# Hash-table's optimization
Let's optimize the  program, that was considered in the [last chapter](https://github.com/Vokerlee/Assembly/tree/main/5.1.%20%20Hash-table's%20research) of hash-table's research. But, first, let's look at a little theory.

## What's happening
The work algorithm of hash-table:
1. Reading words from some source.
2. Calculating the hash-number of this words.
3. This hash-number is the index in massive of hash-table.
4. Appropriate value put in the massive.
5. If some words have the same hash (that means the same index in massive), collision appears. To solve this problem lists are used. This picture explains everything:

<img src="Readme pictures//Hashtable.png" alt="drawing" width="600"/>

## Code analysis
To research code we will use Jenkins hash function. Using time-profiler, get the following time-distribution:

<img src="Readme pictures//Initial times.png" alt="drawing" width="800"/>

We can see, that some of the slowest functions are `List<char *>::contains` and `JenkinsHash::operator()`.

## Code optimization

First of all let's rewrite `JenkinsHash::operator()` function. We have:

```C++
int operator()(char* string)
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
```
All the code can be rewritten to assembler, so the function `strlen` can be.
Using our super brain, create the same code in assemler:

```asm
inline int operator()(char* string)
{
    uint32_t len  = fast_strlen(string);
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




