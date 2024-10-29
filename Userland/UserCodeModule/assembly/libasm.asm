GLOBAL sys_call
GLOBAL _hlt
GLOBAL _cli
GLOBAL _sti

sys_call:

    mov rax, rdi
    mov rdi, rsi
    mov rsi, rdx
    mov rdx, rcx
    mov r8, r10

    int 80h

    ret

;espera a que ocurra una interrupcion 
_hlt:
	sti
	hlt
	ret
;desabilita interrupciones de Hardware
_cli:
	cli
	ret

;habilita interrupciones de Hardware
_sti:
	sti
	ret

section .rodata
msg db "Hello world", 0