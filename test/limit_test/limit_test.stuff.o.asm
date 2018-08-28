	.file	"limit_test.stuff.c"
	.section	.text.ult,"ax",@progbits
	.balign 2
	.global	ult
	.type	ult, @function
ult:
; start of function
; framesize_regs:     0
; framesize_locals:   4
; framesize_outgoing: 0
; framesize:          4
; elim ap -> fp       2
; elim fp -> sp       4
; saved regs:(none)
	; start of prologue
	SUB.W	#4, R1
	; end of prologue
	MOV.W	R12, 2(R1)
	MOV.W	R13, @R1
	CMP.W	@R1, 2(R1) { JHS	.L2
	MOV.B	#1, R12
	BR	#.L3
.L2:
	MOV.B	#0, R12
.L3:
	; start of epilogue
	ADD.W	#4, R1
	RET
	.size	ult, .-ult
	.section	.text.slt,"ax",@progbits
	.balign 2
	.global	slt
	.type	slt, @function
slt:
; start of function
; framesize_regs:     0
; framesize_locals:   4
; framesize_outgoing: 0
; framesize:          4
; elim ap -> fp       2
; elim fp -> sp       4
; saved regs:(none)
	; start of prologue
	SUB.W	#4, R1
	; end of prologue
	MOV.W	R12, 2(R1)
	MOV.W	R13, @R1
	CMP.W	@R1, 2(R1) { JGE	.L5
	MOV.B	#1, R12
	BR	#.L6
.L5:
	MOV.B	#0, R12
.L6:
	; start of epilogue
	ADD.W	#4, R1
	RET
	.size	slt, .-slt
	.section	.text.ulte,"ax",@progbits
	.balign 2
	.global	ulte
	.type	ulte, @function
ulte:
; start of function
; framesize_regs:     0
; framesize_locals:   4
; framesize_outgoing: 0
; framesize:          4
; elim ap -> fp       2
; elim fp -> sp       4
; saved regs:(none)
	; start of prologue
	SUB.W	#4, R1
	; end of prologue
	MOV.W	R12, 2(R1)
	MOV.W	R13, @R1
	CMP.W	2(R1), @R1 { JLO	.L8
	MOV.B	#1, R12
	BR	#.L9
.L8:
	MOV.B	#0, R12
.L9:
	; start of epilogue
	ADD.W	#4, R1
	RET
	.size	ulte, .-ulte
	.section	.text.slte,"ax",@progbits
	.balign 2
	.global	slte
	.type	slte, @function
slte:
; start of function
; framesize_regs:     0
; framesize_locals:   4
; framesize_outgoing: 0
; framesize:          4
; elim ap -> fp       2
; elim fp -> sp       4
; saved regs:(none)
	; start of prologue
	SUB.W	#4, R1
	; end of prologue
	MOV.W	R12, 2(R1)
	MOV.W	R13, @R1
	CMP.W	2(R1), @R1 { JL	.L11
	MOV.B	#1, R12
	BR	#.L12
.L11:
	MOV.B	#0, R12
.L12:
	; start of epilogue
	ADD.W	#4, R1
	RET
	.size	slte, .-slte
	.section	.text.ugt,"ax",@progbits
	.balign 2
	.global	ugt
	.type	ugt, @function
ugt:
; start of function
; framesize_regs:     0
; framesize_locals:   4
; framesize_outgoing: 0
; framesize:          4
; elim ap -> fp       2
; elim fp -> sp       4
; saved regs:(none)
	; start of prologue
	SUB.W	#4, R1
	; end of prologue
	MOV.W	R12, 2(R1)
	MOV.W	R13, @R1
	CMP.W	2(R1), @R1 { JHS	.L14
	MOV.B	#1, R12
	BR	#.L15
.L14:
	MOV.B	#0, R12
.L15:
	; start of epilogue
	ADD.W	#4, R1
	RET
	.size	ugt, .-ugt
	.section	.text.sgt,"ax",@progbits
	.balign 2
	.global	sgt
	.type	sgt, @function
sgt:
; start of function
; framesize_regs:     0
; framesize_locals:   4
; framesize_outgoing: 0
; framesize:          4
; elim ap -> fp       2
; elim fp -> sp       4
; saved regs:(none)
	; start of prologue
	SUB.W	#4, R1
	; end of prologue
	MOV.W	R12, 2(R1)
	MOV.W	R13, @R1
	CMP.W	2(R1), @R1 { JGE	.L17
	MOV.B	#1, R12
	BR	#.L18
.L17:
	MOV.B	#0, R12
.L18:
	; start of epilogue
	ADD.W	#4, R1
	RET
	.size	sgt, .-sgt
	.section	.text.ugte,"ax",@progbits
	.balign 2
	.global	ugte
	.type	ugte, @function
ugte:
; start of function
; framesize_regs:     0
; framesize_locals:   4
; framesize_outgoing: 0
; framesize:          4
; elim ap -> fp       2
; elim fp -> sp       4
; saved regs:(none)
	; start of prologue
	SUB.W	#4, R1
	; end of prologue
	MOV.W	R12, 2(R1)
	MOV.W	R13, @R1
	CMP.W	@R1, 2(R1) { JLO	.L20
	MOV.B	#1, R12
	BR	#.L21
.L20:
	MOV.B	#0, R12
.L21:
	; start of epilogue
	ADD.W	#4, R1
	RET
	.size	ugte, .-ugte
	.section	.text.sgte,"ax",@progbits
	.balign 2
	.global	sgte
	.type	sgte, @function
sgte:
; start of function
; framesize_regs:     0
; framesize_locals:   4
; framesize_outgoing: 0
; framesize:          4
; elim ap -> fp       2
; elim fp -> sp       4
; saved regs:(none)
	; start of prologue
	SUB.W	#4, R1
	; end of prologue
	MOV.W	R12, 2(R1)
	MOV.W	R13, @R1
	CMP.W	@R1, 2(R1) { JL	.L23
	MOV.B	#1, R12
	BR	#.L24
.L23:
	MOV.B	#0, R12
.L24:
	; start of epilogue
	ADD.W	#4, R1
	RET
	.size	sgte, .-sgte
	.ident	"GCC: (SOMNIUM Technologies Limited - msp430-gcc 6.4.0.32) 6.4.0"
