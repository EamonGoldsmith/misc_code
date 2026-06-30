# RV32I assembly
.section .text

.globl _start

_start:
    # UART base address (QEMU virt machine)
    li t0, 0x10000000

    # Load address of the string
    la t1, hello_str

loop:
    # Load byte from string
    lb t2, 0(t1)
    beqz t2, halt  # Exit if null terminator

    # Write byte to UART
    sb t2, 0(t0)

    # Increment string pointer
    addi t1, t1, 1
    j loop

halt:
    j halt

.section .rodata
hello_str:
    .string "Hello, RISC-V RV32I!\n"
