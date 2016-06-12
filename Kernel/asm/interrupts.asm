GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL _lidt
GLOBAL haltcpu

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _irq80Handler

EXTERN irqDispatcher
EXTERN syscallDispatcher

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

	mov rdi, %1
	call irqDispatcher
	
	;signal pic EOI
	mov al, 20h
	out 20h, al

	popState		
	iretq
%endmacro

SECTION .text

_cli:
	cli
	ret

_sti:
	sti
	ret

;picMasterMask(mask)
picMasterMask:
	push rbp
    mov rbp, rsp

    mov ax, di
    out	21h,al

    mov rsp, rbp
    pop rbp
    retn


;picSlaveMask(mask)
picSlaveMask:
	push    rbp
    mov     rbp, rsp

    mov     ax, di  	; ax = mascara de 16 bits
    out		0A1h,al

    mov 	rsp, rbp
    pop     rbp
    retn

; Carga el IDTR (unused)
;_lidt(idtr)
_lidt:				
    push    rbp
    mov     rbp, rsp

    push    rbx	
    mov     rbx, rdi	; puntero a IDTR
	lidt    [rbx]		; carga IDTR
    pop     rbx

    mov 	rsp, rbp
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

_irq80Handler:
	push rbp
	mov rbp, rsp

	mov rdi, rax
	mov rsi, rbx

	call syscallDispatcher

	mov rsp, rbp
	pop rbp
	iretq

haltcpu:
	cli
	hlt
	ret