	.file	"main.c"
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
	CALL	#test_encrypt_cbc
	CALL	#test_decrypt_cbc
	CALL	#test_decrypt_ecb
	CALL	#test_encrypt_ecb
	CALL	#test_encrypt_ecb_verbose
	MOV.B	#0, R12
	; start of epilogue
	.refsym	__crt0_call_exit
	RET
	.size	main, .-main
	.section	.text.phex,"ax",@progbits
	.balign 2
	.type	phex, @function
phex:
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
	.size	phex, .-phex
.data
.LC0:
	.byte	43
	.byte	126
	.byte	21
	.byte	22
	.byte	40
	.byte	-82
	.byte	-46
	.byte	-90
	.byte	-85
	.byte	-9
	.byte	21
	.byte	-120
	.byte	9
	.byte	-49
	.byte	79
	.byte	60
.LC1:
	.byte	107
	.byte	-63
	.byte	-66
	.byte	-30
	.byte	46
	.byte	64
	.byte	-97
	.byte	-106
	.byte	-23
	.byte	61
	.byte	126
	.byte	17
	.byte	115
	.byte	-109
	.byte	23
	.byte	42
	.byte	-82
	.byte	45
	.byte	-118
	.byte	87
	.byte	30
	.byte	3
	.byte	-84
	.byte	-100
	.byte	-98
	.byte	-73
	.byte	111
	.byte	-84
	.byte	69
	.byte	-81
	.byte	-114
	.byte	81
	.byte	48
	.byte	-56
	.byte	28
	.byte	70
	.byte	-93
	.byte	92
	.byte	-28
	.byte	17
	.byte	-27
	.byte	-5
	.byte	-63
	.byte	25
	.byte	26
	.byte	10
	.byte	82
	.byte	-17
	.byte	-10
	.byte	-97
	.byte	36
	.byte	69
	.byte	-33
	.byte	79
	.byte	-101
	.byte	23
	.byte	-83
	.byte	43
	.byte	65
	.byte	123
	.byte	-26
	.byte	108
	.byte	55
	.byte	16
	.section	.text.test_encrypt_ecb_verbose,"ax",@progbits
	.balign 2
	.type	test_encrypt_ecb_verbose, @function
test_encrypt_ecb_verbose:
; start of function
; framesize_regs:     0
; framesize_locals:   210
; framesize_outgoing: 0
; framesize:          210
; elim ap -> fp       2
; elim fp -> sp       210
; saved regs:(none)
	; start of prologue
	SUB.W	#210, R1
	; end of prologue
	MOV.W	R1, R12
	ADD.W	#65, R12
	MOV.W	#.LC0, R13
	MOV.B	#16, R14
	CALL	#memcpy
	MOV.W	R1, R12
	ADD.W	#1, R12
	MOV.W	#.LC1, R13
	MOV.B	#64, R14
	CALL	#memcpy
	MOV.W	R1, R12
	ADD.W	#145, R12
	MOV.B	#64, R14
	MOV.B	#0, R13
	CALL	#memset
	MOV.W	R1, R12
	ADD.W	#81, R12
	MOV.B	#64, R14
	MOV.B	#0, R13
	CALL	#memset
	MOV.B	#0, 209(R1)
	BR	#.L5
.L6:
	MOV.B	209(R1), R12
	rpt	#4 { rlax.w	R12
	MOV.W	R1, R13
	ADD.W	#1, R13
	ADD.W	R13, R12
	CALL	#phex
	ADD.B	#1, 209(R1)
.L5:
	MOV.B	#3, R12
	CMP.B	209(R1), R12 { JHS	.L6
	MOV.W	R1, R12
	ADD.W	#65, R12
	CALL	#phex
	MOV.B	#0, 209(R1)
	BR	#.L7
.L8:
	MOV.B	209(R1), R12
	rpt	#4 { rlax.w	R12
	MOV.W	R1, R13
	ADD.W	#1, R13
	MOV.W	R13, R15
	ADD.W	R12, R15
	MOV.B	209(R1), R12
	rpt	#4 { rlax.w	R12
	MOV.W	R1, R13
	ADD.W	#145, R13
	ADD.W	R12, R13
	MOV.W	R1, R12
	ADD.W	#65, R12
	MOV.W	R13, R14
	MOV.W	R12, R13
	MOV.W	R15, R12
	CALL	#AES128_ECB_encrypt
	MOV.B	209(R1), R12
	rpt	#4 { rlax.w	R12
	MOV.W	R1, R13
	ADD.W	#145, R13
	ADD.W	R13, R12
	CALL	#phex
	ADD.B	#1, 209(R1)
.L7:
	MOV.B	#3, R12
	CMP.B	209(R1), R12 { JHS	.L8
	NOP
	; start of epilogue
	ADD.W	#210, R1
	RET
	.size	test_encrypt_ecb_verbose, .-test_encrypt_ecb_verbose
.data
.LC2:
	.byte	107
	.byte	-63
	.byte	-66
	.byte	-30
	.byte	46
	.byte	64
	.byte	-97
	.byte	-106
	.byte	-23
	.byte	61
	.byte	126
	.byte	17
	.byte	115
	.byte	-109
	.byte	23
	.byte	42
.LC3:
	.byte	58
	.byte	-41
	.byte	123
	.byte	-76
	.byte	13
	.byte	122
	.byte	54
	.byte	96
	.byte	-88
	.byte	-98
	.byte	-54
	.byte	-13
	.byte	36
	.byte	102
	.byte	-17
	.byte	-105
	.section	.text.test_encrypt_ecb,"ax",@progbits
	.balign 2
	.type	test_encrypt_ecb, @function
test_encrypt_ecb:
; start of function
; framesize_regs:     0
; framesize_locals:   66
; framesize_outgoing: 0
; framesize:          66
; elim ap -> fp       2
; elim fp -> sp       66
; saved regs:(none)
	; start of prologue
	SUB.W	#66, R1
	; end of prologue
	MOV.W	R1, R12
	ADD.W	#48, R12
	MOV.W	#.LC0, R13
	MOV.B	#16, R14
	CALL	#memcpy
	MOV.W	R1, R12
	ADD.W	#32, R12
	MOV.W	#.LC2, R13
	MOV.B	#16, R14
	CALL	#memcpy
	MOV.W	R1, R12
	ADD.W	#16, R12
	MOV.W	#.LC3, R13
	MOV.B	#16, R14
	CALL	#memcpy
	MOV.W	R1, R14
	MOV.W	R1, R13
	ADD.W	#48, R13
	MOV.W	R1, R12
	ADD.W	#32, R12
	CALL	#AES128_ECB_encrypt
	MOV.W	R1, R13
	MOV.W	R1, R12
	ADD.W	#16, R12
	MOV.B	#16, R14
	CALL	#strncmp
	CMP.W	#0, R12 { JEQ	.L12
.L11:
	MOV.W	#0, 64(R1)
	BR	#.L11
.L12:
	NOP
	; start of epilogue
	ADD.W	#66, R1
	RET
	.size	test_encrypt_ecb, .-test_encrypt_ecb
.data
.LC4:
	.byte	0
	.byte	1
	.byte	2
	.byte	3
	.byte	4
	.byte	5
	.byte	6
	.byte	7
	.byte	8
	.byte	9
	.byte	10
	.byte	11
	.byte	12
	.byte	13
	.byte	14
	.byte	15
.LC5:
	.byte	118
	.byte	73
	.byte	-85
	.byte	-84
	.byte	-127
	.byte	25
	.byte	-78
	.byte	70
	.byte	-50
	.byte	-23
	.byte	-114
	.byte	-101
	.byte	18
	.byte	-23
	.byte	25
	.byte	125
	.byte	80
	.byte	-122
	.byte	-53
	.byte	-101
	.byte	80
	.byte	114
	.byte	25
	.byte	-18
	.byte	-107
	.byte	-37
	.byte	17
	.byte	58
	.byte	-111
	.byte	118
	.byte	120
	.byte	-78
	.byte	115
	.byte	-66
	.byte	-42
	.byte	-72
	.byte	-29
	.byte	-63
	.byte	116
	.byte	59
	.byte	113
	.byte	22
	.byte	-26
	.byte	-98
	.byte	34
	.byte	34
	.byte	-107
	.byte	22
	.byte	63
	.byte	-15
	.byte	-54
	.byte	-95
	.byte	104
	.byte	31
	.byte	-84
	.byte	9
	.byte	18
	.byte	14
	.byte	-54
	.byte	48
	.byte	117
	.byte	-122
	.byte	-31
	.byte	-89
	.section	.text.test_decrypt_cbc,"ax",@progbits
	.balign 2
	.type	test_decrypt_cbc, @function
test_decrypt_cbc:
; start of function
; framesize_regs:     0
; framesize_locals:   226
; framesize_outgoing: 4
; framesize:          230
; elim ap -> fp       2
; elim fp -> sp       230
; saved regs:(none)
	; start of prologue
	SUB.W	#230, R1
	; end of prologue
	MOV.W	R1, R12
	ADD.W	#212, R12
	MOV.W	#.LC0, R13
	MOV.B	#16, R14
	CALL	#memcpy
	MOV.W	R1, R12
	ADD.W	#196, R12
	MOV.W	#.LC4, R13
	MOV.B	#16, R14
	CALL	#memcpy
	MOV.W	R1, R12
	ADD.W	#132, R12
	MOV.W	#.LC5, R13
	MOV.B	#64, R14
	CALL	#memcpy
	MOV.W	R1, R12
	ADD.W	#68, R12
	MOV.W	#.LC1, R13
	MOV.B	#64, R14
	CALL	#memcpy
	MOV.W	R1, R13
	ADD.W	#132, R13
	MOV.W	R1, R12
	ADD.W	#4, R12
	MOV.B	#230, R14
	ADD.W	R1, R14
	ADD.W	#-34, R14
	MOV.W	R14, 2(R1)
	MOV.B	#230, R14
	ADD.W	R1, R14
	ADD.W	#-18, R14
	MOV.W	R14, @R1
	MOV.B	#16, R14
	MOV.B	#0, R15
	CALL	#AES128_CBC_decrypt_buffer
	MOV.W	R1, R12
	ADD.W	#4, R12
	ADD.W	#16, R12
	MOV.W	R1, R13
	ADD.W	#132, R13
	ADD.W	#16, R13
	MOV.W	#0, 2(R1)
	MOV.W	#0, @R1
	MOV.B	#16, R14
	MOV.B	#0, R15
	CALL	#AES128_CBC_decrypt_buffer
	MOV.W	R1, R12
	ADD.W	#4, R12
	ADD.W	#32, R12
	MOV.W	R1, R13
	ADD.W	#132, R13
	ADD.W	#32, R13
	MOV.W	#0, 2(R1)
	MOV.W	#0, @R1
	MOV.B	#16, R14
	MOV.B	#0, R15
	CALL	#AES128_CBC_decrypt_buffer
	MOV.W	R1, R12
	ADD.W	#4, R12
	ADD.W	#48, R12
	MOV.W	R1, R13
	ADD.W	#132, R13
	ADD.W	#48, R13
	MOV.W	#0, 2(R1)
	MOV.W	#0, @R1
	MOV.B	#16, R14
	MOV.B	#0, R15
	CALL	#AES128_CBC_decrypt_buffer
	MOV.W	R1, R13
	ADD.W	#4, R13
	MOV.W	R1, R12
	ADD.W	#68, R12
	MOV.B	#64, R14
	CALL	#strncmp
	CMP.W	#0, R12 { JEQ	.L16
.L15:
	MOV.W	#0, 228(R1)
	BR	#.L15
.L16:
	NOP
	; start of epilogue
	ADD.W	#230, R1
	RET
	.size	test_decrypt_cbc, .-test_decrypt_cbc
	.section	.text.test_encrypt_cbc,"ax",@progbits
	.balign 2
	.type	test_encrypt_cbc, @function
test_encrypt_cbc:
; start of function
; framesize_regs:     0
; framesize_locals:   226
; framesize_outgoing: 4
; framesize:          230
; elim ap -> fp       2
; elim fp -> sp       230
; saved regs:(none)
	; start of prologue
	SUB.W	#230, R1
	; end of prologue
	MOV.W	R1, R12
	ADD.W	#212, R12
	MOV.W	#.LC0, R13
	MOV.B	#16, R14
	CALL	#memcpy
	MOV.W	R1, R12
	ADD.W	#196, R12
	MOV.W	#.LC4, R13
	MOV.B	#16, R14
	CALL	#memcpy
	MOV.W	R1, R12
	ADD.W	#132, R12
	MOV.W	#.LC1, R13
	MOV.B	#64, R14
	CALL	#memcpy
	MOV.W	R1, R12
	ADD.W	#68, R12
	MOV.W	#.LC5, R13
	MOV.B	#64, R14
	CALL	#memcpy
	MOV.W	R1, R13
	ADD.W	#132, R13
	MOV.W	R1, R12
	ADD.W	#4, R12
	MOV.B	#230, R14
	ADD.W	R1, R14
	ADD.W	#-34, R14
	MOV.W	R14, 2(R1)
	MOV.B	#230, R14
	ADD.W	R1, R14
	ADD.W	#-18, R14
	MOV.W	R14, @R1
	MOV.B	#64, R14
	MOV.B	#0, R15
	CALL	#AES128_CBC_encrypt_buffer
	MOV.W	R1, R13
	ADD.W	#4, R13
	MOV.W	R1, R12
	ADD.W	#68, R12
	MOV.B	#64, R14
	CALL	#strncmp
	CMP.W	#0, R12 { JEQ	.L20
.L19:
	MOV.W	#0, 228(R1)
	BR	#.L19
.L20:
	NOP
	; start of epilogue
	ADD.W	#230, R1
	RET
	.size	test_encrypt_cbc, .-test_encrypt_cbc
	.section	.text.test_decrypt_ecb,"ax",@progbits
	.balign 2
	.type	test_decrypt_ecb, @function
test_decrypt_ecb:
; start of function
; framesize_regs:     0
; framesize_locals:   66
; framesize_outgoing: 0
; framesize:          66
; elim ap -> fp       2
; elim fp -> sp       66
; saved regs:(none)
	; start of prologue
	SUB.W	#66, R1
	; end of prologue
	MOV.W	R1, R12
	ADD.W	#48, R12
	MOV.W	#.LC0, R13
	MOV.B	#16, R14
	CALL	#memcpy
	MOV.W	R1, R12
	ADD.W	#32, R12
	MOV.W	#.LC3, R13
	MOV.B	#16, R14
	CALL	#memcpy
	MOV.W	R1, R12
	ADD.W	#16, R12
	MOV.W	#.LC2, R13
	MOV.B	#16, R14
	CALL	#memcpy
	MOV.W	R1, R14
	MOV.W	R1, R13
	ADD.W	#48, R13
	MOV.W	R1, R12
	ADD.W	#32, R12
	CALL	#AES128_ECB_decrypt
	MOV.W	R1, R13
	MOV.W	R1, R12
	ADD.W	#16, R12
	MOV.B	#16, R14
	CALL	#strncmp
	CMP.W	#0, R12 { JEQ	.L24
.L23:
	MOV.W	#0, 64(R1)
	BR	#.L23
.L24:
	NOP
	; start of epilogue
	ADD.W	#66, R1
	RET
	.size	test_decrypt_ecb, .-test_decrypt_ecb
	.ident	"GCC: (SOMNIUM Technologies Limited - msp430-gcc 6.4.0.32) 6.4.0"
