; directives --------------------------------------------------
DEFAULT ABS

; constants ---------------------------------------------------
LF equ 10
NULL equ 0
TRUE equ 1
FALSE equ 0
EXIT_SUCCESS equ 1

O_RDONLY equ 00
O_WRONLY equ 01
O_RDWR equ 02

; unix syscalls (64 bit) --------------------------------------
SYS_WRITE equ 1
SYS_READ equ 0
SYS_OPEN equ 2
SYS_LSEEK equ 8
SYS_CLOSE equ 3
SYS_FORK equ 57
SYS_EXECVE equ 59
SYS_WAIT_ID equ 247
SYS_EXIT equ 60

; buffer numbers
STDIN equ 0
STDOUT equ 1

; seek options
SEEK_SET equ 0	; set to offset
SEEK_CUR equ 1	; add offset to current position
SEEK_END equ 2	; end of file plus offset bytes
SEEK_DATA equ 3 ; ?
SEEK_HOLE equ 4 ; !?

; macros ------------------------------------------------------

%macro string 2+
	%1 db %2
	%1_len equ $ - %1
%endmacro

%macro prologue 0
	push rbp		; push stack
	mov rbp, rsp	; reset after push
%endmacro

%macro return 0
	mov rsp, rbp
	pop rbp
	ret
%endmacro

%macro print_str 1
	print %1, %1_len
%endmacro

%macro print 2
	write STDOUT, %1, %2
%endmacro

%macro read 3
	mov rax, SYS_READ
	mov rdi, [%1]
	mov rsi, %2
	mov rdx, %3
	syscall
%endmacro

%macro write 3
	mov rax, SYS_WRITE 
	mov rdi, %1		; fd
	mov rsi, %2		; ptr
	mov rdx, %3		; length
	syscall
%endmacro

%macro write_str 2
	write [%1], %2, %2_len
%endmacro

%macro fopen 4
	mov rax, SYS_OPEN
	mov rdi, %1
	mov rsi, %2		; mode
	or rsi, 0x400
	mov rdx, %3		; permissions
	syscall
	mov [%4], rax
%endmacro

%macro fclose 1
	mov rax, SYS_CLOSE
	mov rdi, [%1]
	syscall
%endmacro

%macro fseek_end 1
	mov rax, SYS_LSEEK
	mov rdi, [%1]
	mov rsi, 0			; offset
	mov rdx, SEEK_END	; whence
	syscall
%endmacro

; fancy macros ------------------------------------------------

%macro print_lit 1+
    section .rodata
        %%str: db %1
        %%len: equ $ - %%str
    section .text
        mov rax, SYS_WRITE
        mov rdi, STDOUT
        mov rsi, %%str
        mov rdx, %%len
        syscall
%endmacro
