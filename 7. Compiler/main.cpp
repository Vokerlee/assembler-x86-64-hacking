#include "language_parsing.h"
#include "executive_file.h"
//#include <unistd.h>

int main()
{
    printf("Please, enter the file-name of your program you want to execute:\n");

    char *file_name = (char *) calloc(100, sizeof(char));
    assert(file_name);

    //scanf("%s", file_name);

    FILE *language = fopen("prog.txt", "rb");
    assert(language);

    lang_asm(language);

    fclose(language);
    free(file_name);

    printf("Everything is OK. Your program was executed\n");

    create_exe("test.exe");

    //system("pause");

    return 0;
}
