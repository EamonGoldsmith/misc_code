; directives --------------------------------------------------
DEFAULT ABS

; constants ---------------------------------------------------
LF equ 10
NULL equ 0
TRUE equ 1
FALSE equ 0
EXIT_SUCCESS equ 1

; unix syscalls -----------------------------------------------
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

%macro print 2
	mov rax, SYS_WRITE
	mov rdi, STDOUT
	mov rsi, %1		; string ptr
	mov rdx, %2		; string length
	syscall
%endmacro

%macro print_str 1
	mov rax, SYS_WRITE
	mov rdi, STDOUT
	mov rsi, %1		; string
	mov rdx, %1_len ; string length
	syscall
%endmacro

%macro fopen 5
	mov rax, SYS_OPEN
	mov rdi, %1
	mov rsi, %2		; mode
	or rsi, %3
	mov rdx, %4		; permissions
	syscall
	mov [%5 + 0], rax
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

%macro fwrite 2
	mov rax, SYS_WRITE
	mov rdi, [%1]
	mov rsi, %2
	mov rdx, %2_len
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
