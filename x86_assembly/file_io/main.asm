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
