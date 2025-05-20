section .text
	global _start

_start:
	; label
	mov eax, 4	; write()
	mov ebx, 1	; stdout fd
	mov ecx, msg
	mov edx, len
	int 0x80

	; load args
	mov eax, '4'
	sub eax, '0'
	mov edx, '3'
	sub edx, '0'

	; call function
	call sum
	mov ecx, eax

	; print result
	mov eax, 4
	mov ebx, 1
	mov edx, 1
	int 0x80

	; exit
	mov eax, 1	; exit()
	int 0x80

sum:	
	; eax + edx
	mov eax, ecx
	add eax, edx
	add eax, '0'
	ret

section .data
	msg db 'The sum is:', 10, 0
	len equ $ - msg

section .bss
	res resb 1 
