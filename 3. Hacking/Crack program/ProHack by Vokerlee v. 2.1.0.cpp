#include "hack.h"

extern const char* hello_message;
extern const char* end_message;

int main() 
{
    size_t program_size = 0;
    char* program = read_file("S:\\Doc\\Projects\\Danya_hack\\HACKPASS.COM", &program_size);

    unsigned long long hash = hash_calc(program, program_size);

    PlaySound(TEXT("song.wav"), NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);

    if (hash == cracked_hash) 
    {
        print_message("Program is already cracked!!\n");
        delay(12000);
        return 0;
    }

    if (hash != normal_hash) 
    {
        print_message("Program is modified. Please check version\n");
        delay(12000);
        return 0;
    }

    print_message(hello_message);
    delay(3000);


    //                      PROGRAM CRACKING
    //====================================================================
    program[6] = 144;
    program[7] = 144;
    program[8] = 144;
    program[9] = 235;
    program[11] = 144;
    program[12] = 144;

    program[66] = 74;

    program[75] = 195;
    program[76] = 90;
    //====================================================================



    write_file("S:\\Doc\\Projects\\Danya_hack\\HACKPASS.COM", program_size, program);

    print_status_bar();
    print_message(end_message);

    delay(12000);

    return 0;
}

