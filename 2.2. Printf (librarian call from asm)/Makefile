all: printf
	./printf

printf: printf.o
	gcc -no-pie -o printf printf.o

printf.o: printf.asm
	nasm -w+all -f elf64 printf.asm -o printf.o
