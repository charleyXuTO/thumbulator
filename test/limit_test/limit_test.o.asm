	.file	"limit_test.c"
	.section	.text.uart_putc,"ax",@progbits
	.balign 2
	.global	uart_putc
	.type	uart_putc, @function
uart_putc:
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
	.size	uart_putc, .-uart_putc
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
.L7:
	ADD.W	#-4, 4(R1)
	MOV.W	@R1, R12
	MOV.W	4(R1), R13
	CALL	#__mspabi_srli
	MOV.W	R12, R13
	AND.B	#15, R13
	MOV.W	R13, 2(R1)
	MOV.B	#9, R12
	CMP.W	2(R1), R12 { JHS	.L3
	ADD.W	#55, 2(R1)
	BR	#.L4
.L3:
	ADD.W	#48, 2(R1)
.L4:
	MOV.W	2(R1), R12
	CALL	#uart_putc
	CMP.W	#0, 4(R1) { JEQ	.L9
	BR	#.L7
.L9:
	NOP
	MOV.B	#13, R12
	CALL	#uart_putc
	MOV.B	#10, R12
	CALL	#uart_putc
	NOP
	; start of epilogue
	ADD.W	#6, R1
	RET
	.size	hexstring, .-hexstring
	.section	.text.limit_test,"ax",@progbits
	.balign 2
	.global	limit_test
	.type	limit_test, @function
limit_test:
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
	MOV.W	#0, 2(R1)
.L13:
	MOV.B	#7, R13
	MOV.W	2(R1), R12
	CALL	#ult
	CMP.W	#0, R12 { JEQ	.L77
	MOV.W	2(R1), R12
	MOV.W	R12, R13
	ADD.W	#1, R13
	MOV.W	R13, 2(R1)
	BR	#.L13
.L77:
	NOP
	MOV.W	2(R1), R12
	CALL	#hexstring
	CMP.W	#7, 2(R1) { JEQ	.L14
	MOV.B	#1, R12
	BR	#.L15
.L14:
	MOV.W	#-4096, 2(R1)
.L18:
	MOV.W	#-4089, R13
	MOV.W	2(R1), R12
	CALL	#ult
	CMP.W	#0, R12 { JEQ	.L78
	MOV.W	2(R1), R12
	MOV.W	R12, R13
	ADD.W	#1, R13
	MOV.W	R13, 2(R1)
	BR	#.L18
.L78:
	NOP
	MOV.W	2(R1), R12
	CALL	#hexstring
	CMP.W	#-4089, 2(R1) { JEQ	.L19
	MOV.B	#1, R12
	BR	#.L15
.L19:
	MOV.W	#-7, @R1
.L22:
	MOV.B	#7, R13
	MOV.W	@R1, R12
	CALL	#slt
	CMP.W	#0, R12 { JEQ	.L79
	MOV.W	@R1, R12
	ADD.W	#1, R12
	MOV.W	R12, @R1
	BR	#.L22
.L79:
	NOP
	MOV.W	@R1, R12
	CALL	#hexstring
	CMP.W	#7, @R1 { JEQ	.L23
	MOV.B	#1, R12
	BR	#.L15
.L23:
	MOV.W	#-17, @R1
.L26:
	MOV.W	#-7, R13
	MOV.W	@R1, R12
	CALL	#slt
	CMP.W	#0, R12 { JEQ	.L80
	MOV.W	@R1, R12
	ADD.W	#1, R12
	MOV.W	R12, @R1
	BR	#.L26
.L80:
	NOP
	MOV.W	@R1, R12
	CALL	#hexstring
	CMP.W	#-7, @R1 { JEQ	.L27
	MOV.B	#1, R12
	BR	#.L15
.L27:
	MOV.W	#0, 2(R1)
.L30:
	MOV.B	#7, R13
	MOV.W	2(R1), R12
	CALL	#ulte
	CMP.W	#0, R12 { JEQ	.L81
	MOV.W	2(R1), R12
	MOV.W	R12, R13
	ADD.W	#1, R13
	MOV.W	R13, 2(R1)
	BR	#.L30
.L81:
	NOP
	MOV.W	2(R1), R12
	CALL	#hexstring
	CMP.W	#8, 2(R1) { JEQ	.L31
	MOV.B	#1, R12
	BR	#.L15
.L31:
	MOV.W	#-4096, 2(R1)
.L34:
	MOV.W	#-4089, R13
	MOV.W	2(R1), R12
	CALL	#ulte
	CMP.W	#0, R12 { JEQ	.L82
	MOV.W	2(R1), R12
	MOV.W	R12, R13
	ADD.W	#1, R13
	MOV.W	R13, 2(R1)
	BR	#.L34
.L82:
	NOP
	MOV.W	2(R1), R12
	CALL	#hexstring
	CMP.W	#-4088, 2(R1) { JEQ	.L35
	MOV.B	#1, R12
	BR	#.L15
.L35:
	MOV.W	#-4096, 2(R1)
.L38:
	MOV.B	#7, R13
	MOV.W	2(R1), R12
	CALL	#ulte
	CMP.W	#0, R12 { JEQ	.L83
	MOV.W	2(R1), R12
	MOV.W	R12, R13
	ADD.W	#1, R13
	MOV.W	R13, 2(R1)
	BR	#.L38
.L83:
	NOP
	MOV.W	2(R1), R12
	CALL	#hexstring
	CMP.W	#-4096, 2(R1) { JEQ	.L39
	MOV.B	#1, R12
	BR	#.L15
.L39:
	MOV.W	#-7, @R1
.L42:
	MOV.B	#7, R13
	MOV.W	@R1, R12
	CALL	#slte
	CMP.W	#0, R12 { JEQ	.L84
	MOV.W	@R1, R12
	ADD.W	#1, R12
	MOV.W	R12, @R1
	BR	#.L42
.L84:
	NOP
	MOV.W	@R1, R12
	CALL	#hexstring
	CMP.W	#8, @R1 { JEQ	.L43
	MOV.B	#1, R12
	BR	#.L15
.L43:
	MOV.W	#-17, @R1
.L46:
	MOV.W	#-7, R13
	MOV.W	@R1, R12
	CALL	#slte
	CMP.W	#0, R12 { JEQ	.L85
	MOV.W	@R1, R12
	ADD.W	#1, R12
	MOV.W	R12, @R1
	BR	#.L46
.L85:
	NOP
	MOV.W	@R1, R12
	CALL	#hexstring
	CMP.W	#-6, @R1 { JEQ	.L47
	MOV.B	#1, R12
	BR	#.L15
.L47:
	MOV.W	#0, 2(R1)
.L50:
	MOV.B	#7, R13
	MOV.W	2(R1), R12
	CALL	#ugt
	CMP.W	#0, R12 { JNE	.L86
	MOV.W	2(R1), R12
	MOV.W	R12, R13
	ADD.W	#1, R13
	MOV.W	R13, 2(R1)
	BR	#.L50
.L86:
	NOP
	MOV.W	2(R1), R12
	CALL	#hexstring
	CMP.W	#8, 2(R1) { JEQ	.L51
	MOV.B	#1, R12
	BR	#.L15
.L51:
	MOV.W	#-4096, 2(R1)
.L54:
	MOV.W	#-4089, R13
	MOV.W	2(R1), R12
	CALL	#ugt
	CMP.W	#0, R12 { JNE	.L87
	MOV.W	2(R1), R12
	MOV.W	R12, R13
	ADD.W	#1, R13
	MOV.W	R13, 2(R1)
	BR	#.L54
.L87:
	NOP
	MOV.W	2(R1), R12
	CALL	#hexstring
	CMP.W	#-4088, 2(R1) { JEQ	.L55
	MOV.B	#1, R12
	BR	#.L15
.L55:
	MOV.W	#0, 2(R1)
.L58:
	MOV.B	#7, R13
	MOV.W	2(R1), R12
	CALL	#ugte
	CMP.W	#0, R12 { JNE	.L88
	MOV.W	2(R1), R12
	MOV.W	R12, R13
	ADD.W	#1, R13
	MOV.W	R13, 2(R1)
	BR	#.L58
.L88:
	NOP
	MOV.W	2(R1), R12
	CALL	#hexstring
	CMP.W	#7, 2(R1) { JEQ	.L59
	MOV.B	#1, R12
	BR	#.L15
.L59:
	MOV.W	#-4096, 2(R1)
.L62:
	MOV.W	#-4089, R13
	MOV.W	2(R1), R12
	CALL	#ugte
	CMP.W	#0, R12 { JNE	.L89
	MOV.W	2(R1), R12
	MOV.W	R12, R13
	ADD.W	#1, R13
	MOV.W	R13, 2(R1)
	BR	#.L62
.L89:
	NOP
	MOV.W	2(R1), R12
	CALL	#hexstring
	CMP.W	#-4089, 2(R1) { JEQ	.L63
	MOV.B	#1, R12
	BR	#.L15
.L63:
	MOV.B	#0, R12
.L15:
	; start of epilogue
	ADD.W	#4, R1
	RET
	.size	limit_test, .-limit_test
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
	MOV.W	#4660, R12
	CALL	#hexstring
	CALL	#limit_test
	CMP.W	#0, R12 { JEQ	.L91
	MOV.W	#2989, R12
	CALL	#hexstring
.L91:
	MOV.W	#4660, R12
	CALL	#hexstring
	MOV.B	#0, R12
	; start of epilogue
	.refsym	__crt0_call_exit
	RET
	.size	main, .-main
	.ident	"GCC: (SOMNIUM Technologies Limited - msp430-gcc 6.4.0.32) 6.4.0"
