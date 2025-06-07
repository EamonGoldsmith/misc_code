; todo-list program for x86 32bit assembly
; read command line args
; usage: ./todo [list, add, check, finished]

; list, print all unfinished tasks
; add, add a new unfinished task to the list
; check, mark a task as finished
; finished, print all finished tasks

#include "defs.s"

section .bss
	uinput_len equ 24	; 24 length for user name
	uinput resb uinput_len

section .data
	usage db 'usage: ./todo [list, add, check, finished]', 10, 0
	usage_len equ $ - greet

	filename db 'todo.txt', 0

section .text
	global _start

_start:
	; hello world
	mov eax, 4	; write()
	mov ebx, 1	; stdout fd
	mov ecx, usage
	mov edx, usage_len
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
	mov ecx, usage
	mov edx, usage_len
	int 0x80

	; close file
	mov eax, 6
	mov ebx, [file_descriptor]
	int 0x80

	; exit
	mov eax, 1	; exit()
	int 0x80
