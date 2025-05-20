nasm -f elf -o main.o main.asm
ld -m elf_i386 -s -o main main.o
./main
