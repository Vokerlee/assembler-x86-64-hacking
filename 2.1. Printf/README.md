# Printf assembler implementation

This directory contains a `printformat` function's implementation, written in `nasm`.

It is similar to usual `printf` function of `stdio.h` library. But has some differences (all `printf` features, not mentioned here, are not supported in `printformat`):

1. `printforamt` supports format specifiers:
    * `%s` is a string
    * `%d` is an `int` number
    * `%x` is a hexadecimal number
    * `%b` is a binary number
    * `%o` is an octal number
    * `%c` is a char number
    * `%%` is `%`
2. There some escape sequences:
    * `\n` is a new line
    * `\t` is a tab
    * `\v` is a vertical tab
    * `\\` is `\`

Instead of slow comparisons there is a jump table with handling-addresses.

Here is the example of usage:

```C++
extern "C" void printformat (const char*, ...);

int main()
{
    printformat("Try to execute:\n");
    printformat("I %s %x %d%% %c %b \nI %s %x %d%% %c %b \n", "love", 3802, 100, '!', 127, "love", 3802, 100, '!', 127);
    
    return 0;
}
```
