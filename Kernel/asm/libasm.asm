GLOBAL outb
GLOBAL inb
GLOBAL cpuVendor
GLOBAL testSysCall
GLOBAL getRegisters

section .text
	
inb:
    mov rdx, rdi
	in al, dx
	ret

outb:
    mov rdx,rdi
    mov rax,rsi
    out dx,al
    ret

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
    ret

section .bss
registers resq 17
