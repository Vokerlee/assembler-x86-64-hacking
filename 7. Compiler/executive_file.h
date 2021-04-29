#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <windows.h>

const size_t CODE_CHAR  = IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ;
const size_t DATA_CHAR  = IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE | IMAGE_SCN_CNT_INITIALIZED_DATA;
const size_t IDATA_CHAR = IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ;

const size_t IMAGE_BASE = 0x00400000;
const size_t ENTRY_POINT_ADDR = 0x1000;

const size_t VRT_SIZE = 0x5000;
const size_t SIZE_RAW = 0x1000;

const size_t DATA_START   = VRT_SIZE * 2 + ENTRY_POINT_ADDR;
const size_t IMPORT_START = VRT_SIZE + ENTRY_POINT_ADDR;

bool create_exe(const char filename[]);

void fill_dos_header(IMAGE_DOS_HEADER* dos_header);

void fill_NT_header(IMAGE_NT_HEADERS* NT_header, int NUM_OF_SEC = 3);
void fill_NT_file_header(IMAGE_FILE_HEADER* NT_file_header, int NUM_OF_SEC);
void fill_NT_optional_header(IMAGE_OPTIONAL_HEADER* NT_optional_header);

void fill_section_header(IMAGE_SECTION_HEADER* section, const char sec_name[], size_t vrt_size, size_t vrt_addr,
    size_t size_raw_data, size_t ptr_raw_data, size_t charac);

struct import_name
{
    WORD Hint;
    const CHAR* Name;
    size_t size;

    import_name(const char name[], size_t size, WORD hnt = 0) :
        Hint(hnt),
        Name(name),
        size(size)
    {}

    void fill(const char name[], size_t size, WORD hnt = 0)
    {
        Name = name;
        this->size = size;
        Hint = hnt;
    }

    size_t put_in_file(FILE* file)
    {
        if (file == nullptr)
            return 0;

        fwrite(&Hint, sizeof(WORD), 1, file);
        fwrite(Name, sizeof(char), size, file);

        char zero = 0;
        fwrite(&zero, sizeof(zero), 1, file); // feature of the end

        return size;
    }
};

class imprt_tabel
{
private:
    size_t size;
    IMAGE_IMPORT_DESCRIPTOR* table;
    import_name* name_imp;
    size_t names_size;
    IMAGE_THUNK_DATA64* thunk;
    size_t align_size;

public:
    imprt_tabel(size_t sz = 5) :
        size(sz + 1),
        name_imp(nullptr),
        names_size(0),
        align_size(0)
    {
        table    = (IMAGE_IMPORT_DESCRIPTOR*) calloc(sz + 1, sizeof(IMAGE_IMPORT_DESCRIPTOR));
        name_imp = (import_name*) calloc(sz, sizeof(import_name));
        thunk    = (IMAGE_THUNK_DATA64*) calloc(sz, sizeof(thunk[0]));
    }

    void fill_def_names(void)
    {
        name_imp[0].fill("ExitProcess", 11);
        name_imp[1].fill("WriteConsoleA", 13);
        name_imp[2].fill("ReadConsoleA", 12);
        name_imp[3].fill("ToDec", 5);
        name_imp[4].fill("ad6scanf", 8);

        for (size_t i = 0; i < size - 1; ++i)
            names_size += name_imp[i].size + 1 + 2;
    }

    bool fiil_default(void)
    {
        fill_def_names();

        size_t tbl_size = sizeof(table[1]) * size;
        size_t thunks_size = sizeof(thunk[1]) * (size - 1);
        size_t names_start = IMPORT_START + tbl_size + thunks_size;
        size_t fst_thunk_start = names_start + names_size;
        size_t name_rva = fst_thunk_start + thunks_size;
        size_t cur_name_size = 0;

        for (size_t i = 0; i < size - 1; ++i)
        {
            table[i].OriginalFirstThunk = IMPORT_START + tbl_size + i * sizeof(thunk[1]);
            table[i].FirstThunk = fst_thunk_start + i * sizeof(thunk[1]);
            table[i].Name = name_rva;
            thunk[i].u1.AddressOfData = (names_start + cur_name_size);
            cur_name_size += name_imp[i].size + 1 + 2;
        }

        return true;
    }

    bool put_in_file(FILE* file) // default
    {
        if (file == nullptr)
            return false;

        const size_t dll_name_size = 11;
        const size_t stub_size = 1 + SIZE_RAW - (sizeof(table[0]) * size + 2 * sizeof(thunk[1]) * (size - 1) + names_size + dll_name_size);
        char* stub = new char[stub_size] {0};

        fwrite(table, sizeof(table[0]), size, file);

        fwrite(thunk, sizeof(thunk[0]), size - 1, file);

        for (size_t i = 0; i < size - 1; ++i)
            name_imp[i].put_in_file(file);

        fwrite(thunk, sizeof(thunk[0]), size - 1, file);

        fprintf(file, "sfasmlib.dll\0");

        fwrite(stub, sizeof(stub[0]), stub_size, file);

        delete[] stub;
        return true;
    }

    unsigned get_proc_addr(size_t num)
    {
        return sizeof(table[0]) * size + sizeof(thunk[0]) * (size - 1) + names_size + num * sizeof(thunk[0]);
    }

    ~imprt_tabel(void)
    {
        if (table != nullptr)
        {
            free(table);
            table = nullptr;
        }
        if (name_imp != nullptr)
        {
            free(name_imp);
            name_imp = nullptr;
        }
        if (thunk != nullptr)
        {
            free(thunk);
            thunk = nullptr;
        }
        size = 0;
    }
};

const unsigned char DOS_STUB[] =
{
    0x0E, 0x1F, 0xBA, 0x0E, 0x00, 0xB4,
    0x09, 0xCD, 0x21, 0xB8, 0x01, 0x4C,
    0xCD, 0x21, 0x54, 0x68, 0x69, 0x73,
    0x20, 0x70, 0x72, 0x6F, 0x67, 0x72,
    0x61, 0x6D, 0x20, 0x63, 0x61, 0x6E, 
    0x6E, 0x6F, 0x74, 0x20, 0x62, 0x65,
    0x20, 0x72, 0x75, 0x6E, 0x20, 0x69,
    0x6E, 0x20, 0x44, 0x4F, 0x53, 0x20,
    0x6D, 0x6F, 0x64, 0x65, 0x2E, 0x0D,
    0x0D, 0x0A, 0x24, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x5D, 0x5C,
    0x6D, 0xC1, 0x19, 0x3D, 0x03, 0x92,
    0x19, 0x3D, 0x03, 0x92, 0x19, 0x3D,
    0x03, 0x92, 0x97, 0x22, 0x10, 0x92,
    0x1E, 0x3D, 0x03, 0x92, 0xE5, 0x1D,
    0x11, 0x92, 0x18, 0x3D, 0x03, 0x92,
    0x52, 0x69, 0x63, 0x68, 0x19, 0x3D,
    0x03, 0x92, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};