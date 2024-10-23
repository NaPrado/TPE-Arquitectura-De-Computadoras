GLOBAL sys_call

GLOBAL random
GLOBAL sys_Write

random:
    mov rax, 1
    mov rdi, 1
    mov rsi, msg
    mov rdx, 11

    int 80h

    ret

sys_call:
    push rdi
    push rsi
    push rcx
    push rdx
    push r8
    push r9
    push r10

	mov rax, rdi    ; 1er param -> rax
	mov rdi, rsi    ; 2er param -> rdi
	mov rsi, rdx    ; 3er param -> rsi
	mov rdx, rcx    ; 4er param -> rdx
    mov r10, r8
    mov r8, r9

    int 80h

    pop r10
    pop r9
    pop r8
    pop rdx
    pop rcx
    pop rsi
    pop rdi
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