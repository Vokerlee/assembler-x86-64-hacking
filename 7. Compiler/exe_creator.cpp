#include "executive_file.h"

bool create_exe(const char filename[])
{
    const size_t stub_size = SIZE_RAW;
    char stub[stub_size] = { 0 };

    size_t vrt_addr = ENTRY_POINT_ADDR;
    size_t vrt_size = VRT_SIZE;
    size_t size_raw = SIZE_RAW;
    size_t ptr_raw  = 0x400;

    FILE* exe = fopen(filename, "wb");
    if (exe == nullptr)
        return printf("Problem with openning %s file", filename), true;

    IMAGE_DOS_HEADER* dos_header = (IMAGE_DOS_HEADER*) calloc(1, sizeof(IMAGE_DOS_HEADER));
    fill_dos_header(dos_header);

    IMAGE_NT_HEADERS* NT_header  = (IMAGE_NT_HEADERS*) calloc(1, sizeof(IMAGE_NT_HEADERS));
    fill_NT_header(NT_header);

    IMAGE_SECTION_HEADER* text_section = (IMAGE_SECTION_HEADER*) calloc(1, sizeof(IMAGE_SECTION_HEADER));
    fill_section_header(text_section, ".text", vrt_size, vrt_addr, size_raw, ptr_raw, CODE_CHAR);

    IMAGE_SECTION_HEADER* data_section = (IMAGE_SECTION_HEADER*) calloc(1, sizeof(IMAGE_SECTION_HEADER));
    fill_section_header(data_section, ".idata", vrt_size, IMPORT_START, size_raw, ptr_raw + size_raw, IDATA_CHAR);

    IMAGE_SECTION_HEADER* import_data_section = (IMAGE_SECTION_HEADER*) calloc(1, sizeof(IMAGE_SECTION_HEADER));
    fill_section_header(import_data_section, ".data", vrt_size, DATA_START, size_raw, ptr_raw + 2 * size_raw, DATA_CHAR);

    fwrite(dos_header, sizeof(IMAGE_DOS_HEADER), 1, exe);
    fwrite(&DOS_STUB, sizeof(DOS_STUB[0]), sizeof(DOS_STUB) / sizeof(DOS_STUB[0]), exe);
    fwrite(NT_header, sizeof(IMAGE_NT_HEADERS), 1, exe);

    fwrite(text_section,        sizeof(IMAGE_SECTION_HEADER), 1, exe);
    fwrite(data_section,        sizeof(IMAGE_SECTION_HEADER), 1, exe);
    fwrite(import_data_section, sizeof(IMAGE_SECTION_HEADER), 1, exe);

    fwrite(stub, sizeof(stub[0]), 30 * 16, exe); // code section

    //fwrite(out.buf, sizeof(out.buf[0]), out.size, exe);

    imprt_tabel imp_tabel;
    imp_tabel.put_in_file(exe);

    fwrite(stub, sizeof(stub[0]), stub_size, exe);

    free(dos_header);
    free(NT_header);
    free(data_section);
    free(text_section);
    free(import_data_section);

    fclose(exe);

    return true;
}

void fill_dos_header(IMAGE_DOS_HEADER* dos_header)
{
    dos_header->e_magic    = 'ZM';
    dos_header->e_cblp     = 0x0090;
    dos_header->e_cp       = 0x0003;
    dos_header->e_cparhdr  = 0x0004;
    dos_header->e_minalloc = 0x0010;
    dos_header->e_maxalloc = 0xFFFF;
    dos_header->e_sp       = 0x00B8;
    dos_header->e_lfarlc   = 0x0040;
    dos_header->e_lfanew   = 0x00B0;
}

void fill_NT_header(IMAGE_NT_HEADERS* NT_header, int NUM_OF_SEC)
{
    NT_header->Signature = '\0EP\0';

    fill_NT_file_header(&(NT_header->FileHeader), NUM_OF_SEC);
    fill_NT_optional_header(&(NT_header->OptionalHeader));
}

void fill_NT_file_header(IMAGE_FILE_HEADER* NT_file_header, int NUM_OF_SEC)
{
    NT_file_header->Machine              = IMAGE_FILE_MACHINE_AMD64;
    NT_file_header->NumberOfSections     = static_cast<WORD>(NUM_OF_SEC),
    NT_file_header->TimeDateStamp        = static_cast<DWORD>(time(nullptr));
    NT_file_header->SizeOfOptionalHeader = sizeof(IMAGE_OPTIONAL_HEADER32);
    NT_file_header->Characteristics      = IMAGE_FILE_EXECUTABLE_IMAGE | IMAGE_FILE_32BIT_MACHINE;
}

void fill_NT_optional_header(IMAGE_OPTIONAL_HEADER* NT_optional_header)
{
    NT_optional_header->Magic                 = IMAGE_NT_OPTIONAL_HDR32_MAGIC;
    NT_optional_header->AddressOfEntryPoint   = ENTRY_POINT_ADDR;
    NT_optional_header->ImageBase             = IMAGE_BASE;
    NT_optional_header->SectionAlignment      = ENTRY_POINT_ADDR;
    NT_optional_header->FileAlignment         = 0x200;
    NT_optional_header->MajorSubsystemVersion = 4;
    NT_optional_header->SizeOfImage           = 2 * 0x8000;
    NT_optional_header->SizeOfHeaders         = 0x400;
    NT_optional_header->Subsystem             = IMAGE_SUBSYSTEM_WINDOWS_CUI;
    NT_optional_header->NumberOfRvaAndSizes   = 0x10;
    NT_optional_header->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = IMPORT_START;
}

void fill_section_header(IMAGE_SECTION_HEADER* section, const char sec_name[], size_t vrt_size, size_t vrt_addr,
                         size_t size_raw_data, size_t ptr_raw_data, size_t charac)
{
    sprintf((char* const)section->Name, "%s", sec_name);
    section->Misc.VirtualSize = vrt_size;

    section->VirtualAddress   = vrt_addr;
    section->SizeOfRawData    = size_raw_data;
    section->PointerToRawData = ptr_raw_data;
    section->Characteristics  = charac;
}
