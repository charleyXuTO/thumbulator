	.file	"test.c"
	.section	.text.testzero,"ax",@progbits
	.balign 2
	.type	testzero, @function
testzero:
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
	MOV.B	#1, R12
	MOV.B	#0, R13
	CMP.W	@R1, R13 { JL	.L2
	MOV.B	#0, R12
.L2:
	AND	#0xff, R12
	; start of epilogue
	ADD.W	#2, R1
	RET
	.size	testzero, .-testzero
	.section	.rodata
.LC0:
	.string	"Arithmetic Operations:\n"
.LC1:
	.string	"src/test.c"
.LC2:
	.string	"OK: !(testzero(0)) passed at %s:%d\n"
.LC3:
	.string	"FAIL: !(testzero(0)) failed at %s:%d\n"
	.section	.text.testSimple,"ax",@progbits
	.balign 2
	.type	testSimple, @function
testSimple:
; start of function
; framesize_regs:     0
; framesize_locals:   8
; framesize_outgoing: 6
; framesize:          14
; elim ap -> fp       2
; elim fp -> sp       14
; saved regs:(none)
	; start of prologue
	SUB.W	#14, R1
	; end of prologue
	MOV.W	#1, 12(R1)
	MOV.W	#2, 10(R1)
	MOV.W	#4, 8(R1)
	MOV.W	#0, 6(R1)
	MOV.W	#.LC0, @R1
	CALL	#printf
	MOV.B	#0, R12
	CALL	#testzero
	CMP.W	#0, R12 { JNE	.L5
	MOV.W	#51, 4(R1)
	MOV.W	#.LC1, 2(R1)
	MOV.W	#.LC2, @R1
	CALL	#printf
	BR	#.L7
.L5:
	MOV.W	#51, 4(R1)
	MOV.W	#.LC1, 2(R1)
	MOV.W	#.LC3, @R1
	CALL	#printf
.L7:
	NOP
	; start of epilogue
	ADD.W	#14, R1
	RET
	.size	testSimple, .-testSimple
	.section	.rodata
.LC4:
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
	MOV.W	#.LC4, @R1
	CALL	#printf
	MOV.B	#0, R12
	; start of epilogue
	.refsym	__crt0_call_exit
	ADD.W	#2, R1
	RET
	.size	main, .-main
	.ident	"GCC: (SOMNIUM Technologies Limited - msp430-gcc 6.4.0.32) 6.4.0"
