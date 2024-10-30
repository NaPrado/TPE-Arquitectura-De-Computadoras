GLOBAL cpuVendor
GLOBAL testSysCall
GLOBAL getRegisters

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret
; testSysCall:
; 	mov rax,6
; 	mov rdi, 0xFF0000
; 	mov rsi, 70
; 	mov rdx,70
; 	int 80h
; 	mov rax,6
; 	mov rdi, 0x00FF00
; 	mov rsi, 80
; 	mov rdx,70
; 	int 80h
	

; 	mov rax, 35
; 	mov rdi, 3
; 	int 80h

; 	mov rax, 1
; 	mov rdi, 1
; 	mov rsi, msg
; 	mov rdx, 13
; 	int 80h

; 	mov rax, 5
; 	mov rdi, 20
; 	mov rsi, 20 
; 	int 80h
	
; 	mov rax, 1
; 	mov rdi, 2
; 	mov rsi, msg
; 	mov rdx, 13
; 	int 80h

; 	mov rax, 5
; 	mov rdi, 20
; 	mov rsi, 30 
; 	int 80h

; 	mov rax, 1
; 	mov rdi, 1
; 	mov rsi, msg
; 	mov rdx, 13
; 	int 80h
; 	ret
; msg db "Hello, World!", 0

getRegisters:
    mov qword [registers], rax
    mov rax, registers
    mov qword [rax+8], rbx
    mov qword [rax+16], rcx
    mov qword [rax+24], rdx
    mov qword [rax+32], rdi
    mov qword [rax+40], rsi
    mov qword [rax+48], rsp
    mov qword [rax+56], rbp
    mov qword [rax+72], r8
    mov qword [rax+80], r9
    mov qword [rax+88], r10
    mov qword [rax+96], r11
    mov qword [rax+104], r12
    mov qword [rax+112], r13
    mov qword [rax+120], r14
    mov qword [rax+128], r15
    ; mov qword [rax+136], rip
    ret

section .bss
registers resq 17
