	.file	"crc.c"
	.section	.text.reflect,"ax",@progbits
	.balign 2
	.type	reflect, @function
reflect:
; start of function
; framesize_regs:     4
; framesize_locals:   12
; framesize_outgoing: 0
; framesize:          16
; elim ap -> fp       6
; elim fp -> sp       12
; saved regs: R9 R10
	; start of prologue
	PUSHM.W	#2, R10
	SUB.W	#12, R1
	; end of prologue
	MOV.W	R12, 2(R1)
	MOV.W	R13, 4(R1)
	MOV.B	R14, 1(R1)
	MOV.W	#0, 8(R1)
	MOV.W	#0, 10(R1)
	MOV.B	#0, 7(R1)
	BR	#.L2
.L5:
	MOV.W	2(R1), R9
	AND.B	#1, R9
	MOV.W	4(R1), R10
	BIC.W	#-1, R10
	MOV.W	R9, R12
	BIS.W	R10, R12
	CMP.W	#0, R12 { JEQ	.L3
	MOV.B	1(R1), R12
	MOV.W	R12, R13
	ADD.W	#-1, R13
	MOV.B	7(R1), R12
	SUB.W	R12, R13
	MOV.B	#1, R12
	CALL	#__mspabi_slli
	BIT.W	#0x8000, R12 { SUBC.W	R13, R13 { INV.W	R13, R13
	BIS.W	R12, 8(R1)
	BIS.W	R13, 10(R1)
.L3:
	CLRC { RRC.W	4(R1) { RRC.W	2(R1)
	ADD.B	#1, 7(R1)
.L2:
	CMP.B	1(R1), 7(R1) { JLO	.L5
	MOV.W	8(R1), R12
	MOV.W	10(R1), R13
	; start of epilogue
	ADD.W	#12, R1
	POPM.W	#2, r10
	RET
	.size	reflect, .-reflect
	.section	.text.crcSlow,"ax",@progbits
	.balign 2
	.global	crcSlow
	.type	crcSlow, @function
crcSlow:
; start of function
; framesize_regs:     0
; framesize_locals:   12
; framesize_outgoing: 0
; framesize:          12
; elim ap -> fp       2
; elim fp -> sp       12
; saved regs:(none)
	; start of prologue
	SUB.W	#12, R1
	; end of prologue
	MOV.W	R12, 2(R1)
	MOV.W	R13, @R1
	MOV.W	#-1, 10(R1)
	MOV.W	#0, 8(R1)
	BR	#.L8
.L13:
	MOV.W	8(R1), R12
	ADD.W	2(R1), R12
	MOV.B	@R12, R12
	rpt	#8 { rlax.w	R12
	XOR.W	R12, 10(R1)
	MOV.B	#8, 7(R1)
	BR	#.L9
.L12:
	MOV.W	10(R1), R12
	CMP.W	#0, R12 { JGE	.L10
	MOV.W	10(R1), R12
	ADD.W	R12, R12
	MOV.W	R12, R13
	XOR.W	#4129, R13
	MOV.W	R13, 10(R1)
	BR	#.L11
.L10:
	MOV.W	10(R1), R12
	ADD.W	R12, R12
	MOV.W	R12, 10(R1)
.L11:
	ADD.B	#-1, 7(R1)
.L9:
	CMP.B	#0, 7(R1) { JNE	.L12
	ADD.W	#1, 8(R1)
.L8:
	CMP.W	@R1, 8(R1) { JL	.L13
	CMP.W	#10673, 10(R1) { JEQ	.L14
.L15:
	MOV.W	#0, 4(R1)
	BR	#.L15
.L14:
	MOV.W	10(R1), R12
	; start of epilogue
	ADD.W	#12, R1
	RET
	.size	crcSlow, .-crcSlow
	.comm	crcTable,512,2
	.section	.text.crcInit,"ax",@progbits
	.balign 2
	.global	crcInit
	.type	crcInit, @function
crcInit:
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
	MOV.W	#0, 2(R1)
	BR	#.L18
.L23:
	MOV.W	2(R1), R12
	CALL	#__mspabi_slli_8
	MOV.W	R12, 4(R1)
	MOV.B	#8, 1(R1)
	BR	#.L19
.L22:
	MOV.W	4(R1), R12
	CMP.W	#0, R12 { JGE	.L20
	MOV.W	4(R1), R12
	ADD.W	R12, R12
	MOV.W	R12, R13
	XOR.W	#4129, R13
	MOV.W	R13, 4(R1)
	BR	#.L21
.L20:
	MOV.W	4(R1), R12
	ADD.W	R12, R12
	MOV.W	R12, 4(R1)
.L21:
	ADD.B	#-1, 1(R1)
.L19:
	CMP.B	#0, 1(R1) { JNE	.L22
	MOV.W	2(R1), R12
	ADD.W	R12, R12
	ADD.W	#crcTable, R12
	MOV.W	4(R1), @R12
	ADD.W	#1, 2(R1)
.L18:
	MOV.B	#255, R12
	CMP.W	2(R1), R12 { JGE	.L23
	NOP
	; start of epilogue
	ADD.W	#6, R1
	RET
	.size	crcInit, .-crcInit
	.section	.text.crcFast,"ax",@progbits
	.balign 2
	.global	crcFast
	.type	crcFast, @function
crcFast:
; start of function
; framesize_regs:     2
; framesize_locals:   10
; framesize_outgoing: 0
; framesize:          12
; elim ap -> fp       4
; elim fp -> sp       10
; saved regs: R10
	; start of prologue
	PUSHM.W	#1, R10
	SUB.W	#10, R1
	; end of prologue
	MOV.W	R12, 2(R1)
	MOV.W	R13, @R1
	MOV.W	#-1, 8(R1)
	MOV.W	#0, 6(R1)
	BR	#.L25
.L26:
	MOV.W	6(R1), R12
	ADD.W	2(R1), R12
	MOV.B	@R12, R10
	MOV.W	8(R1), R12
	CALL	#__mspabi_srli_8
	AND	#0xff, R12
	MOV.B	R10, R13
	XOR.B	R12, R13
	MOV.B	R13, 5(R1)
	MOV.B	5(R1), R12
	ADD.W	R12, R12
	ADD.W	#crcTable, R12
	MOV.W	@R12, R10
	MOV.W	8(R1), R12
	CALL	#__mspabi_slli_8
	MOV.W	R10, R13
	XOR.W	R12, R13
	MOV.W	R13, 8(R1)
	ADD.W	#1, 6(R1)
.L25:
	CMP.W	@R1, 6(R1) { JL	.L26
	MOV.W	8(R1), R12
	; start of epilogue
	ADD.W	#10, R1
	POPM.W	#1, r10
	RET
	.size	crcFast, .-crcFast
	.ident	"GCC: (SOMNIUM Technologies Limited - msp430-gcc 6.4.0.32) 6.4.0"
