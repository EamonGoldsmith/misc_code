; Greeter program for x86 32bit assembly
; User is prompted to enter a name.
; If the name is 5 characters long or less respond "What a nice name"
; If the name is more than 5 characters, respond "That's a silly name"
; 
; Demonstrates usage of:
; - stack
; - memory
; - allocation 
; - function calls

SYS_WRITE equ 4
SYS_READ equ 3
SYS_EXIT equ 1
STDIN equ 0
STDOUT equ 1

section .bss
	uinput_len equ 24	; 24 length for user name
	uinput resb uinput_len

section .data
	greet db 'Hello, I am an assembly program.', 10, 0
	greet_len equ $ - greet

	question db 'What is your name?', 10, 0
	question_len equ $ - question

	responseA db 'What a nice name.', 10, 0
	responseA_len equ $ - responseA

	responseB db 'Thats a silly name!', 10, 0
	responseB_len equ $ - responseB

section .text
	global _start

_start:
	; say hello
	mov edx, greet_len
	mov ecx, greet
	mov ebx, STDOUT
	mov eax, SYS_WRITE
	int 0x80

	; ask for name
	mov ecx, question
	mov edx, question_len
	mov ebx, STDOUT
	mov eax, SYS_WRITE
	int 0x80
	
	; get name
	mov edx, uinput_len
	mov ecx, uinput
	mov ebx, STDIN
	mov eax, SYS_READ
	int 0x80

	; check name
	mov edx, uinput_len

nice_name:
	; make call to function
	push responseA_len
	push responseA
	call responseA_func
	add esp, 8
	jmp end

silly_name:
	; make call to function
	push responseB_len
	push responseB
	call responseB_func
	add esp, 8

end:
	; exit
	mov eax, 1
	int 0x80

responseA_func:
	; setup stack
	push ebp
	mov ebp, esp

	; get arguments
	mov eax, [ebp + 8]
	mov ebx, [ebp + 12]

	; print response
	mov edx, responseA_len
	mov ecx, responseA
	mov ebx, STDIN
	mov eax, SYS_WRITE
	int 0x80

	; return
	mov esp, ebp
	pop ebp

	ret

responseB_func:
	; setup stack
	push ebp
	mov ebp, esp

	; get arguments
	mov eax, [ebp + 8]
	mov ebx, [ebp + 12]

	; print response
	mov edx, responseB_len
	mov ecx, responseB
	mov ebx, STDIN
	mov eax, SYS_WRITE
	int 0x80

	; return
	mov esp, ebp
	pop ebp

	ret
