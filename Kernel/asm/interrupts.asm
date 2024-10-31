
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL regs_backup

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

GLOBAL _int80Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler

EXTERN irqDispatcher
EXTERN sysCallDispatcher
EXTERN exceptionDispatcher
EXTERN getStackBase

SECTION .text

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

%macro catchRegisters 0
    mov [regs_backup], rax
    mov rax, regs_backup
    mov [rax+8*1], rbx
    mov [rax+8*2], rcx
    mov [rax+8*3], rdx
    mov [rax+8*4], rdi
    mov [rax+8*5], rsi
    mov [rax+8*6], rsp
    mov [rax+8*7], rbp
    mov [rax+8*8], r8
    mov [rax+8*9], r9
    mov [rax+8*10], r10
    mov [rax+8*11], r11
    mov [rax+8*12], r12
    mov [rax+8*13], r13
    mov [rax+8*14], r14
    mov [rax+8*15], r15
    mov rbx, [rsp]      ; rip
    mov [rax+8*16], rbx
    mov rbx, [rsp+8]    ; cs
    mov [rax+8*17], rbx
    mov rbx, [rsp+16]   ; rflags
    mov [rax+8*18], rbx
%endmacro

%macro exceptionHandler 1
    catchRegisters

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

    call getStackBase
    mov [rsp+8*3], rax
    mov rax, userland
    mov [rsp], rax
    iretq
%endmacro


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

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	    0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5

_int80Handler:
	
	mov r9, r8
	mov r8, r10
	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi 
	mov rdi, rax
	
	call sysCallDispatcher

	iretq

;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Zero Division Exception
_exception6Handler:
	exceptionHandler 6

haltcpu:
	cli
	hlt
	ret

section .rodata
    userland equ 0x400000

section .bss
	aux resq 1
    regs_backup resq 18