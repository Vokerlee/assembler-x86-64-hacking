all: printf
	./printf

printf: main.o printf.o 
	g++ -no-pie -o printf main.o printf.o
	# ld main.o printf.o -o printf

printf.o: printf.asm
	nasm -w+all -f elf64 printf.asm -o printf.o

main.o: main.cpp
	g++ -c -Wall main.cpp -o main.o