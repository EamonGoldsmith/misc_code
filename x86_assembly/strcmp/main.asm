%include "defs.s"

section .data
	a db 'string', 0
	b db 'string', 0
	res db 'a', 10, 0

section .text
	global _start

_start:
	; compare strings
	mov rdi, a
	mov rsi, b

	call strcmp
	
	; print output
	add rax, '0'
	mov [res], rax
	mov [res + 1], 10
	mov [res + 2], 0
	print res, 3

exit:
	mov rax, SYS_EXIT
	syscall

strcmp:
	xor rax, rax
.loop:
	mov dl, [rdi + rax]
	mov bl, [rsi + rax]
	cmp dl, bl
	jne .mismatch
	cmp dl, 0
	je .done
	inc rax
	jmp .loop
.mismatch:
	mov rax, 0
.done:
	ret



