	.file	"test.c"
	.section	.rodata
.LC0:
	.string	"Arithmetic Operations:\n"
	.section	.text.testSimple,"ax",@progbits
	.balign 2
	.type	testSimple, @function
testSimple:
; start of function
; framesize_regs:     0
; framesize_locals:   8
; framesize_outgoing: 2
; framesize:          10
; elim ap -> fp       2
; elim fp -> sp       10
; saved regs:(none)
	; start of prologue
	SUB.W	#10, R1
	; end of prologue
	MOV.W	#1, 6(R1)
	MOV.W	#2, 4(R1)
	MOV.W	#4, 2(R1)
	MOV.W	#0, 8(R1)
	MOV.W	#.LC0, @R1
	CALL	#printf
	MOV.W	6(R1), R12
	MOV.W	4(R1), R13
	CALL	#__mspabi_slli
	CMP.W	#4, R12 { JNE	.L2
	ADD.W	#1, 8(R1)
.L2:
	MOV.W	2(R1), R12
	MOV.W	6(R1), R13
	CALL	#__mspabi_srai
	CMP.W	#2, R12 { JNE	.L5
	ADD.W	#1, 8(R1)
.L5:
	ADD.W	#1, 8(R1)
	CMP.W	6(R1), 4(R1) { JL	.L6
	ADD.W	#1, 8(R1)
.L6:
	NOP
	; start of epilogue
	ADD.W	#10, R1
	RET
	.size	testSimple, .-testSimple
	.section	.rodata
.LC1:
	.string	"EXIT\n"
	.section	.text.main,"ax",@progbits
	.balign 2
	.global	main
	.type	main, @function
main:
; start of function
; framesize_regs:     0
; framesize_locals:   0
; framesize_outgoing: 2
; framesize:          2
; elim ap -> fp       2
; elim fp -> sp       2
; saved regs:(none)
	; start of prologue
	SUB.W	#2, R1
	; end of prologue
	CALL	#testSimple
	MOV.W	#.LC1, @R1
	CALL	#printf
	MOV.B	#0, R12
	; start of epilogue
	.refsym	__crt0_call_exit
	ADD.W	#2, R1
	RET
	.size	main, .-main
	.ident	"GCC: (SOMNIUM Technologies Limited - msp430-gcc 6.4.0.32) 6.4.0"
