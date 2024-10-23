GLOBAL cpuVendor
GLOBAL testSysCall

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
