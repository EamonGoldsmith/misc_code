# A Single-Pass Custom Assembler
- **The Goal**: Write a program that reads a text file containing simple assembly instructions (Intel 8080) and outputs a raw binary file of instructions/machine code.

- **Skilled learned**: Robust file I/O, heavy use of Enums with data payloads, string parsing, and binary bitwise manipulation (std::fs::File, u8 arrays).

- **Core Requirements**:
Define a small instruction set using a Rust enum.
Parse lines like MOV R1, 0x05 or ADD R1, R2 using basic string splitting or the nom crate if you want to learn parser combinators.
Translate those enums into their exact byte representations and write the raw sequence out to a .bin file.

- **Bonus Stretch**: Implement a symbol table to handle basic labels and jumps (e.g., JMP loop_start), which will require a two-pass architecture to resolve addresses.
