; Intel 8080 Assembly Language Example 2
; Toggles Bit 0 on I/O Port 05h with a busy wait loop.

        ORG     0100h       ; Start of program space

PORT_ID EQU     05h         ; Define the target I/O port address

START:  MVI     A, 01h      ; Load the turn-ON pattern (Bit 0 = 1)
        OUT     PORT_ID     ; Output to the hardware port -> LED ON
        CALL    DELAY       ; Wait for a moment

        MVI     A, 00h      ; Load the turn-OFF pattern (Bit 0 = 0)
        OUT     PORT_ID     ; Output to the hardware port -> LED OFF
        CALL    DELAY       ; Wait for a moment

        JMP     START       ; Repeat forever

; --- Busy-Wait Delay Subroutine ---
; Uses a nested 16-bit loop using the BC register pair.
; Adjust values depending on the CPU clock speed (e.g., 2MHz).
DELAY:  LXI     B, 0FFFFh   ; Load 16-bit register pair B and C with Max (65535)

D_LOOP: DCX     B           ; Decrement the BC register pair by 1
        
        ; The 8080 'DCX' instruction doesn't affect the zero flag!
        ; We have to manually check if BC has reached 0 by ORing B and C.
        MOV     A, B        ; Move B into Accumulator
        ORA     C           ; Logical OR with C. If both are 0, Zero Flag sets.
        
        JNZ     D_LOOP      ; If result is not zero, keep looping
        RET                 ; Delay finished, return to caller

        END                 ; End of assembly code
