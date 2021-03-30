#include "hack.h"

const char* end_message = "100%\nCongratulations! Program cracked!\n\n"    \
"You can donat me to support poor student!\n";

const char* hello_message = "Danya_hack program cracking will be started in 3 seconds...\n";

char* read_file (const char* file_name, size_t* program_size)
{
    assert(file_name);

    FILE* program_file = fopen(file_name, "rb");
    assert(program_file);

    fseek(program_file, 0, SEEK_END);
    size_t file_size = ftell(program_file);
    fseek(program_file, 0, SEEK_SET);

    char* program = (char*) calloc(file_size, sizeof(char));
    assert(program);

    fread(program, 1, file_size, program_file);
    fclose(program_file);

    *program_size = file_size;
    return program;
}

unsigned long long hash_calc (void* buffer, size_t buffer_size)
{
    assert(buffer);

    char* norm_buffer = (char*) buffer;
    unsigned long long hash = 0;

    for (int i = 0; i < buffer_size; ++i)
        hash = hash * 133419 + (unsigned char) norm_buffer[i] + 931384623;

    return hash;
}

void write_file (const char* file_name, size_t program_size, char* program)
{
    assert(file_name);
    assert(program);

    FILE* program_file = fopen(file_name, "wb");
    assert(program_file);

    fwrite(program, program_size, sizeof(char), program_file);
    fclose(program_file);
}

void delay (int number_of_miliseconds)
{
    int msec = number_of_miliseconds;
    clock_t start_time = clock();

    while (clock() < start_time + msec);
}

void print_status_bar()
{
    int stars_number = PROGRESS_NUMBER;

    print_message("Program is cracking:\n");

    for (int i = 0; i < stars_number + 1; ++i) 
    {
        printf("*");

        delay(300);
    }

    printf("\n");
}

void print_message (const char* message)
{
    size_t message_length = strlen(message);

    for (int i = 0; i < message_length; ++i)
    {
        putc(message[i], stdout);
        delay(100);
    }
}