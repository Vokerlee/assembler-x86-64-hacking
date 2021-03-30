extern "C" void printformat (const char*, ...);
//#include <stdlib.h>
//#include <stdio.h>

int main()
{
    printformat("Try to execute:\n");
    printformat("I %s %x %d%% %c %b \nI %s %x %d%% %c %b \n", "love", 3802, 100, '!', 127, "love", 3802, 100, '!', 127);
    
    return 0;
}
