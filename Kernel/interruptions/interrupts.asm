GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler	
GLOBAL _irq03Handler	
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _exception0Handler
GLOBAL _exception6Handler
GLOBAL _sysCallHandler

EXTERN irqDispatcher
EXTERN sysCallDispatcher
EXTERN print_registers
EXTERN exceptionDispatcher
SECTION .text

%macro pushState 0
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
	push rax
%endmacro

%macro popState 0
	pop rax
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
%endmacro

%macro irqHandlerMaster 1
	push rsp
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	pop rsp
	iretq
%endmacro

%macro exceptionHandler 1
	pushState

	call print_registers

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	popState
	iretq

%endmacro
_hlt:
	sti
	hlt
	ret
_cli:
	cli
	ret
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

; 8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

; Keyboard
_irq01Handler:
	irqHandlerMaster 1

; Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

; Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

; Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

; USBsysCallDispatcher
_irq05Handler:
	irqHandlerMaster 5


; Zero Division Exception
_exception0Handler:
	exceptionHandler 0


; Invalid Operand Exception
_exception6Handler:
	exceptionHandler 6

_sysCallHandler:
	pushState
	pop rax

	call sysCallDispatcher

	push rax
	popState
	iretq

haltcpu:
	cli
	hlt
	ret

SECTION .bss
	aux resq 1
