#pragma once

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