GLOBAL sys_call

sys_call:
    push rdi
    push rsi
    push rcx
    push rdx
    push r8
    push r9
    push r10

	mov rax, rdi
	mov rdi, rsi
	mov rsi, rdx
	mov rdx, rcx
    mov r10, r8
    mov r8, r9

    int 80h

    pop rdi
    pop rsi
    pop rcx
    pop rdx
    pop r8
    pop r9
    pop r10

    ret