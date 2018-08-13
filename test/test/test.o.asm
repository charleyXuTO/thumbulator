	.file	"test.c"
	.global	__mulhi2_f5
	.section	.text.testSimple,"ax",@progbits
	.balign 2
	.type	testSimple, @function
testSimple:
; start of function
; framesize_regs:     0
; framesize_locals:   30
; framesize_outgoing: 0
; framesize:          30
; elim ap -> fp       2
; elim fp -> sp       30
; saved regs:(none)
	; start of prologue
	SUB.W	#30, R1
	; end of prologue
	MOV.W	#0, @R1
	MOV.W	#0, 2(R1)
	MOV.W	#0, 4(R1)
	MOV.W	#0, 6(R1)
	MOV.W	#0, 8(R1)
	MOV.W	#0, 10(R1)
	MOV.W	#0, 12(R1)
	MOV.W	#0, 14(R1)
	MOV.W	#0, 16(R1)
	MOV.W	#0, 18(R1)
	MOV.W	#1, @R1
	MOV.W	#2, 2(R1)
	MOV.W	#3, 4(R1)
	MOV.W	#5, 6(R1)
	MOV.W	#6, 8(R1)
	MOV.W	#7, 10(R1)
	MOV.W	#8, 12(R1)
	MOV.W	#9, 14(R1)
	MOV.W	#1, 26(R1)
	MOV.W	#2, 24(R1)
	MOV.W	#4, 22(R1)
	MOV.W	#0, 28(R1)
	MOV.W	26(R1), R12
	MOV.W	24(R1), R13
	CALL	#__mspabi_slli
	CMP.W	#4, R12 { JNE	.L2
	ADD.W	#1, 28(R1)
.L2:
	MOV.W	22(R1), R12
	MOV.W	26(R1), R13
	CALL	#__mspabi_srai
	CMP.W	#2, R12 { JNE	.L8
	ADD.W	#1, 28(R1)
.L8:
	ADD.W	#1, 28(R1)
	CMP.W	26(R1), 24(R1) { JL	.L4
	ADD.W	#1, 28(R1)
.L4:
	MOV.W	#1, 26(R1)
	MOV.W	#2, 24(R1)
	MOV.W	#4, 22(R1)
	MOV.W	22(R1), R13
	MOV.W	24(R1), R12
	CALL	#__mulhi2_f5
	ADD.W	R12, 26(R1)
	CMP.W	#4, 28(R1) { JNE	.L5
	CMP.W	#9, 26(R1) { JEQ	.L9
.L5:
	MOV.W	#5, 20(R1)
	BR	#.L5
.L9:
	MOV.W	2(R1), R12
	CMP.W	R12, 24(R1) { JGE	.L10
	ADD.W	#1, 28(R1)
.L10:
	NOP
	; start of epilogue
	ADD.W	#30, R1
	RET
	.size	testSimple, .-testSimple
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
	CALL	#testSimple
	MOV.B	#0, R12
	; start of epilogue
	.refsym	__crt0_call_exit
	RET
	.size	main, .-main
	.ident	"GCC: (SOMNIUM Technologies Limited - msp430-gcc 6.4.0.32) 6.4.0"
