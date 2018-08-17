	.file	"test.c"
	.global	__mspabi_subd
	.global	__eqdf2
	.section	.text.main,"ax",@progbits
	.balign 2
	.global	main
	.type	main, @function
main:
; start of function
; framesize_regs:     6
; framesize_locals:   8
; framesize_outgoing: 8
; framesize:          22
; elim ap -> fp       8
; elim fp -> sp       16
; saved regs: R8 R9 R10
	; start of prologue
	PUSHM.W	#3, R10
	SUB.W	#16, R1
	; end of prologue
	MOV.W	#-26214, 8(R1)
	MOV.W	#-26215, 10(R1)
	MOV.W	#-26215, 12(R1)
	MOV.W	#16369, 14(R1)
	MOV.W	8(R1), R12
	MOV.W	10(R1), R13
	MOV.W	12(R1), R14
	MOV.W	14(R1), R15
	MOV.W	#26214, R8
	MOV.W	#26214, R9
	MOV.W	#26214, R10
	MOV.W	#16394, R11
	CALL	#__mspabi_subd
	MOV.W	R12, R8
	MOV.W	R13, R9
	MOV.W	R14, R10
	MOV.W	R15, R11
	MOV.W	R8, 8(R1)
	MOV.W	R9, 10(R1)
	MOV.W	R10, 12(R1)
	MOV.W	R11, 14(R1)
	MOV.W	#-26214, @R1
	MOV.W	#-26215, 2(R1)
	MOV.W	#-26215, 4(R1)
	MOV.W	#16385, 6(R1)
	MOV.W	8(R1), R12
	MOV.W	10(R1), R13
	MOV.W	12(R1), R14
	MOV.W	14(R1), R15
	CALL	#__eqdf2
	CMP.W	#0, R12 { JNE	.L7
	MOV.W	#0, 8(R1)
	MOV.W	#0, 10(R1)
	MOV.W	#0, 12(R1)
	MOV.W	#16368, 14(R1)
	BR	#.L4
.L7:
	MOV.W	#0, 8(R1)
	MOV.W	#0, 10(R1)
	MOV.W	#0, 12(R1)
	MOV.W	#0, 14(R1)
.L4:
	MOV.B	#0, R12
	; start of epilogue
	.refsym	__crt0_call_exit
	ADD.W	#16, R1
	POPM.W	#3, r10
	RET
	.size	main, .-main
	.ident	"GCC: (SOMNIUM Technologies Limited - msp430-gcc 6.4.0.32) 6.4.0"
