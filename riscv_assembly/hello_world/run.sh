# assemble
riscv64-linux-gnu-as -march=rv32i -mabi=ilp32 -o hello.o hello.s

# link
riscv64-linux-gnu-ld -T linker.ld -m elf32lriscv -o hello.elf hello.o

# convert to ELF
riscv64-linux-gnu-objcopy -O binary hello.elf hello.bin

# run in VM
qemu-system-riscv32 -machine virt -nographic -bios none -kernel hello.elf
