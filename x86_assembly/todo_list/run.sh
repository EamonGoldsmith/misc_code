nasm -g -f elf64 -o main.o main.asm
ld -o todo main.o -g
./todo list
