	.file	"embedded.c"
	.section	.text.hexstring,"ax",@progbits
	.balign 2
	.global	hexstring
	.type	hexstring, @function
hexstring:
; start of function
; framesize_regs:     0
; framesize_locals:   6
; framesize_outgoing: 0
; framesize:          6
; elim ap -> fp       2
; elim fp -> sp       6
; saved regs:(none)
	; start of prologue
	SUB.W	#6, R1
	; end of prologue
	MOV.W	R12, @R1
	MOV.W	#32, 4(R1)
.L6:
	ADD.W	#-4, 4(R1)
	MOV.W	@R1, R12
	MOV.W	4(R1), R13
	CALL	#__mspabi_srli
	MOV.W	R12, R13
	AND.B	#15, R13
	MOV.W	R13, 2(R1)
	MOV.B	#9, R12
	CMP.W	2(R1), R12 { JHS	.L2
	ADD.W	#55, 2(R1)
	BR	#.L3
.L2:
	ADD.W	#48, 2(R1)
.L3:
	MOV.W	2(R1), R12
	AND	#0xff, R12
	CALL	#uart_send
	CMP.W	#0, 4(R1) { JEQ	.L8
	BR	#.L6
.L8:
	NOP
	MOV.B	#13, R12
	CALL	#uart_send
	MOV.B	#10, R12
	CALL	#uart_send
	NOP
	; start of epilogue
	ADD.W	#6, R1
	RET
	.size	hexstring, .-hexstring
	.section	.text.hexstrings,"ax",@progbits
	.balign 2
	.global	hexstrings
	.type	hexstrings, @function
hexstrings:
; start of function
; framesize_regs:     0
; framesize_locals:   6
; framesize_outgoing: 0
; framesize:          6
; elim ap -> fp       2
; elim fp -> sp       6
; saved regs:(none)
	; start of prologue
	SUB.W	#6, R1
	; end of prologue
	MOV.W	R12, @R1
	MOV.W	#32, 4(R1)
.L14:
	ADD.W	#-4, 4(R1)
	MOV.W	@R1, R12
	MOV.W	4(R1), R13
	CALL	#__mspabi_srli
	MOV.W	R12, R13
	AND.B	#15, R13
	MOV.W	R13, 2(R1)
	MOV.B	#9, R12
	CMP.W	2(R1), R12 { JHS	.L10
	ADD.W	#55, 2(R1)
	BR	#.L11
.L10:
	ADD.W	#48, 2(R1)
.L11:
	MOV.W	2(R1), R12
	AND	#0xff, R12
	CALL	#uart_send
	CMP.W	#0, 4(R1) { JEQ	.L16
	BR	#.L14
.L16:
	NOP
	MOV.B	#32, R12
	CALL	#uart_send
	NOP
	; start of epilogue
	ADD.W	#6, R1
	RET
	.size	hexstrings, .-hexstrings
	.section	.text.hexstringcr,"ax",@progbits
	.balign 2
	.global	hexstringcr
	.type	hexstringcr, @function
hexstringcr:
; start of function
; framesize_regs:     0
; framesize_locals:   6
; framesize_outgoing: 0
; framesize:          6
; elim ap -> fp       2
; elim fp -> sp       6
; saved regs:(none)
	; start of prologue
	SUB.W	#6, R1
	; end of prologue
	MOV.W	R12, @R1
	MOV.B	#13, R12
	CALL	#uart_send
	MOV.W	#32, 4(R1)
.L22:
	ADD.W	#-4, 4(R1)
	MOV.W	@R1, R12
	MOV.W	4(R1), R13
	CALL	#__mspabi_srli
	MOV.W	R12, R13
	AND.B	#15, R13
	MOV.W	R13, 2(R1)
	MOV.B	#9, R12
	CMP.W	2(R1), R12 { JHS	.L18
	ADD.W	#55, 2(R1)
	BR	#.L19
.L18:
	ADD.W	#48, 2(R1)
.L19:
	MOV.W	2(R1), R12
	AND	#0xff, R12
	CALL	#uart_send
	CMP.W	#0, 4(R1) { JEQ	.L24
	BR	#.L22
.L24:
	NOP
	MOV.B	#32, R12
	CALL	#uart_send
	NOP
	; start of epilogue
	ADD.W	#6, R1
	RET
	.size	hexstringcr, .-hexstringcr
	.section	.text.main,"ax",@progbits
	.balign 2
	.global	main
	.type	main, @function
main:
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
	CALL	#uart_init
	CALL	#run_tea_test
	MOV.B	#0, R12
	; start of epilogue
	.refsym	__crt0_call_exit
	RET
	.size	main, .-main
	.ident	"GCC: (SOMNIUM Technologies Limited - msp430-gcc 6.4.0.32) 6.4.0"
