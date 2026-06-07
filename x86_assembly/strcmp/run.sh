nasm -g -f elf64 -o main.o main.asm
ld -o strcmp main.o -g
./strcmp
