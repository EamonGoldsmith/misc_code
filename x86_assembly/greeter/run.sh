nasm -f elf -o main.o main.asm -g -F dwarf
ld -m elf_i386 -o main main.o
./main
