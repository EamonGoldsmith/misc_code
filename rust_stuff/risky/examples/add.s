; Intel 8080 Assembly Language Example 1
; This program adds two numbers (5 and 3) and stores the result.

        ORG     0100h       ; Start of the program (standard CP/M transient program area)

START:  MVI     A, 5        ; Move immediate value 5 into register A (Accumulator)
        MVI     B, 3        ; Move immediate value 3 into register B
        
        ADD     B           ; Add the contents of register B to register A (A = A + B)
        
        STA     SUM         ; Store the contents of register A into memory location 'SUM'
        
        HLT                 ; Halt the processor

; --- Data Segment ---
SUM:    DB      0           ; Reserve 1 byte for the result, initialized to 0

        END                 ; End of assembly code
