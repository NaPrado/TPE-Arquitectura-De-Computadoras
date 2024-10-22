GLOBAL sys_call

sys_call:
    push r15
    mov r15, r8
    mov r8, r9
    mov r9, r10
	mov r10, r8
    mov r8, r15
    pop r15
	mov rdx, rcx
	mov rsi, rdx
	mov rdi, rsi
	mov rax, rdi

    int 80h

    ret