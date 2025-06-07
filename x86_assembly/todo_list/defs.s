; constants
LF equ 10
NULL equ 0
TRUE equ 1
FALSE equ 0
EXIT_SUCCESS equ 1

; syscall numbers
SYS_WRITE equ 4
SYS_READ equ 3
SYS_OPEN equ 5
SYS_LSEEK equ 19
SYS_CLOSE equ 6
SYS_FORK equ 2
SYS_WAITPID equ 7
SYS_EXIT equ 1

; buffer numbers
STDIN equ 0
STDOUT equ 1
