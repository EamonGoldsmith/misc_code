; todo-list program for x86 64bit assembly
; read command line args
; usage: ./todo [list, add, check]

; list, print all tasks
; add, add a new unfinished task to the list
; check, mark a task as finished

%include "defs.s"

TASK_DESC_LEN equ 200

section .bss
	file_descriptor resd 1
	argv resd 1
	result resd 1
	file_char resd 1
	file_line resd TASK_DESC_LEN + 2
	uinput resd TASK_DESC_LEN

section .data
	string usage, 'usage: ./todo [list, add, check]', 10, 0
	string filename, 'db.txt', 0

	cmd_list db 'list', 0
	cmd_add db 'add', 0
	cmd_check db 'check', 0

section .text
	global _start

_start:
	cmp [rsp], 2
	jge	read_args

	; incorrect number of args
	print_lit 'Incorrect args, provide a command', 10
	print_str usage
	jmp exit

read_args:
	; examine the second argument, the command
	mov rdi, [rsp + 16]

	; check list
	mov rsi, cmd_list
	call strcmp
	cmp rax, 0
	jg command_list

	; check add
	mov rsi, cmd_add
	call strcmp
	cmp rax, 0
	jg command_add

	; check check
	mov rsi, cmd_check
	call strcmp
	cmp rax, 0
	jg command_check

	; command not found
	print_lit 'Unrecognised command, provide one of', 10
	print_str usage
	jmp exit

command_list:
	fopen filename, O_RDONLY, 0666, file_descriptor
	print_lit 'All tasks:', 10, 0

.print_task_loop:
	; read first byte [unfinished/finished]
	read file_descriptor, file_char, 1

	; print checkbox
	cmp [file_char], 0
	jne .print_checked_box
	print_lit '[ ] ', 0
	jmp .print_task_id
.print_checked_box:
	print_lit '[X] ', 0

.print_task_id:
	; read first byte [task id]
	read file_descriptor, file_char, 1

	; convert to ascii
	mov rax, [file_char]
	add rax, '0'
	mov [file_char], rax

	; print task id
	print file_char, 1
	print_lit '. ', 0

.print_task_desc:
	; read byte of description
	read file_descriptor, file_char, 1

	; check call failed
	cmp rax, 0
	je .done

	; check null byte
	cmp [file_char], 0
	je .print_next_task

	; print byte
	print file_char, 1
	jmp .print_task_desc

.print_next_task:
	print_lit 10, 0
	jmp .print_task_loop

.done:
	print_lit 10, 0
	fclose file_descriptor
	jmp exit

command_add:
	; open file in append mode, don't truncate
	fopen filename, O_WRONLY, 0666, file_descriptor

	; ask for new task
	print_lit 'Enter task description: ', 10, 0

	; read new task name
	mov rax, SYS_READ
	mov rdi, STDIN
	mov rsi, uinput
	mov rdx, TASK_DESC_LEN
	syscall

	; write spacer byte
	mov [file_char], 0
	write [file_descriptor], file_char, 1

	; write 0 byte for [unfinished/finished]
	mov [file_char], 0
	write [file_descriptor], file_char, 1

	; write id byte
	mov [file_char], 3
	write [file_descriptor], file_char, 1

	; find length of task desc
	mov rdi, uinput
	call strlen
	sub rax, 1	; remove newline
	mov [result], rax

	; write task description
	write [file_descriptor], uinput, [result]

	; null terminator
	mov [file_char], 0
	write file_descriptor, file_char, 1

	print_lit 'Your task has been added.', 10, 0
	fclose file_descriptor
	jmp exit

command_check:
	print_lit 'Task to check [number]: ', 10, 0

	; read task id
	mov rax, SYS_READ
	mov rdi, STDIN
	mov rsi, uinput
	mov rdx, 1
	syscall

	print_lit 'You selected: '

	mov rbx, [uinput]
	sub rbx, '0' ; convert to int

	fopen filename, O_RDONLY, 0666, file_descriptor
.count_task_loop:
	
	; count first byte [unfinished/finished]
	read file_descriptor, file_char, 1
	; count first byte [task id]
	read file_descriptor, file_char, 1
.count_task_desc:
	; read byte of description
	read file_descriptor, file_char, 1
	; check call failed
	cmp rax, 0
	je .done
	; check null byte
	cmp [file_char], 0
	je .count_next_task
.count_next_task:
	inc rbx
	jmp .count_task_loop
.done:
	fclose file_descriptor
	jmp exit



exit:
	mov rax, SYS_EXIT
	syscall

strlen:
	xor rax, rax
.loop:
	cmp byte [rdi + rax], 0
	je .done
	inc rax
	jmp .loop
.done:
	ret

strcmp:
	xor rax, rax
.loop:
	mov dl, [rdi + rax]
	mov bl, [rsi + rax]
	cmp dl, bl
	jne .mismatch
	cmp dl, 0
	je	.done
	inc rax
	jmp .loop
.mismatch:
	mov rax, 0
.done:
	ret

