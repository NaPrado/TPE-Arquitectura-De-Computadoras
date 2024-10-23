GLOBAL sys_call

GLOBAL random

random:
    mov rax, 1
    mov rdi, 1
    mov rsi, msg
    mov rdx, 11

    int 80h

    ret

sys_call:

    int 80h

    ret

sys_Write:
	mov rax, 1
	mov rdi, 1
	mov rsi, msg
	mov rdx, 13
	int 80h
    ret

section .rodata
msg db "Hello world", 0