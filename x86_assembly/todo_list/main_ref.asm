; todo-list program for x86 32bit assembly
; read command line args
; usage: ./todo [list, add, check, finished]

; list, print all unfinished tasks
; add, add a new unfinished task to the list
; check, mark a task as finished
; finished, print all finished tasks

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
	dec eax ; remove newline
	cmp eax, 5
	jg silly_name

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

section .text
	global _start

section .bss
	file_descriptor resd 1

section .data
	msg db 'Hello,', 'World!', 10, 0
	len equ $ - msg

	text db 'This is new text!!', 10, 0
	text_len equ $ - text

	filename db 'example.txt', 0

_start:
	; hello world
	mov eax, 4	; write()
	mov ebx, 1	; stdout fd
	mov ecx, msg
	mov edx, len
	int 0x80

	; open file
	mov eax, 5	; open()
	mov ebx, filename
	mov ecx, 01	; mode
	or ecx, 02000h
	mov edx, 0666	; permissions
	int 0x80
	mov [file_descriptor], eax

	; seek the end of file
	mov eax, 19
	mov ebx, [file_descriptor]
	mov ecx, 0
	mov edx, 2
	int 0x80

	; write to file
	mov eax, 4	; write()
	mov ebx, [file_descriptor]
	mov ecx, text
	mov edx, text_len
	int 0x80

	; close file
	mov eax, 6
	mov ebx, [file_descriptor]
	int 0x80

	; exit
	mov eax, 1	; exit()
	int 0x80
