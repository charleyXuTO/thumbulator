	.file	"uart.c"
	.section	.text.uart_send,"ax",@progbits
	.balign 2
	.global	uart_send
	.type	uart_send, @function
uart_send:
; start of function
; framesize_regs:     0
; framesize_locals:   2
; framesize_outgoing: 0
; framesize:          2
; elim ap -> fp       2
; elim fp -> sp       2
; saved regs:(none)
	; start of prologue
	SUB.W	#2, R1
	; end of prologue
	MOV.W	R12, @R1
	NOP
	; start of epilogue
	ADD.W	#2, R1
	RET
	.size	uart_send, .-uart_send
	.section	.text.uart_recv,"ax",@progbits
	.balign 2
	.global	uart_recv
	.type	uart_recv, @function
uart_recv:
; start of function
; framesize_regs:     0
; framesize_locals:   0
; framesize_outgoing: 0
; framesize:          0
; elim ap -> fp       2
; elim fp -> sp       0
; saved regs:(none)
	; start of prologue
	; end of prologue
	MOV.B	#32, R12
	; start of epilogue
	RET
	.size	uart_recv, .-uart_recv
	.section	.text.uart_init,"ax",@progbits
	.balign 2
	.global	uart_init
	.type	uart_init, @function
uart_init:
; start of function
; framesize_regs:     0
; framesize_locals:   0
; framesize_outgoing: 0
; framesize:          0
; elim ap -> fp       2
; elim fp -> sp       0
; saved regs:(none)
	; start of prologue
	; end of prologue
	NOP
	; start of epilogue
	RET
	.size	uart_init, .-uart_init
	.section	.text.uart_fifo_flush,"ax",@progbits
	.balign 2
	.global	uart_fifo_flush
	.type	uart_fifo_flush, @function
uart_fifo_flush:
; start of function
; framesize_regs:     0
; framesize_locals:   0
; framesize_outgoing: 0
; framesize:          0
; elim ap -> fp       2
; elim fp -> sp       0
; saved regs:(none)
	; start of prologue
	; end of prologue
	NOP
	; start of epilogue
	RET
	.size	uart_fifo_flush, .-uart_fifo_flush
	.ident	"GCC: (SOMNIUM Technologies Limited - msp430-gcc 6.4.0.32) 6.4.0"
