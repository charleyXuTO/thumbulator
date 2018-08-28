	.file	"aes.c"
	.section	.bss.state,"aw",@nobits
	.balign 2
	.type	state, @object
	.size	state, 2
state:
	.zero	2
	.section	.bss.RoundKey,"aw",@nobits
	.type	RoundKey, @object
	.size	RoundKey, 176
RoundKey:
	.zero	176
	.section	.bss.Key,"aw",@nobits
	.balign 2
	.type	Key, @object
	.size	Key, 2
Key:
	.zero	2
	.section	.bss.Iv,"aw",@nobits
	.balign 2
	.type	Iv, @object
	.size	Iv, 2
Iv:
	.zero	2
	.section	.rodata.sbox,"a",@progbits
	.type	sbox, @object
	.size	sbox, 256
sbox:
	.byte	99
	.byte	124
	.byte	119
	.byte	123
	.byte	-14
	.byte	107
	.byte	111
	.byte	-59
	.byte	48
	.byte	1
	.byte	103
	.byte	43
	.byte	-2
	.byte	-41
	.byte	-85
	.byte	118
	.byte	-54
	.byte	-126
	.byte	-55
	.byte	125
	.byte	-6
	.byte	89
	.byte	71
	.byte	-16
	.byte	-83
	.byte	-44
	.byte	-94
	.byte	-81
	.byte	-100
	.byte	-92
	.byte	114
	.byte	-64
	.byte	-73
	.byte	-3
	.byte	-109
	.byte	38
	.byte	54
	.byte	63
	.byte	-9
	.byte	-52
	.byte	52
	.byte	-91
	.byte	-27
	.byte	-15
	.byte	113
	.byte	-40
	.byte	49
	.byte	21
	.byte	4
	.byte	-57
	.byte	35
	.byte	-61
	.byte	24
	.byte	-106
	.byte	5
	.byte	-102
	.byte	7
	.byte	18
	.byte	-128
	.byte	-30
	.byte	-21
	.byte	39
	.byte	-78
	.byte	117
	.byte	9
	.byte	-125
	.byte	44
	.byte	26
	.byte	27
	.byte	110
	.byte	90
	.byte	-96
	.byte	82
	.byte	59
	.byte	-42
	.byte	-77
	.byte	41
	.byte	-29
	.byte	47
	.byte	-124
	.byte	83
	.byte	-47
	.byte	0
	.byte	-19
	.byte	32
	.byte	-4
	.byte	-79
	.byte	91
	.byte	106
	.byte	-53
	.byte	-66
	.byte	57
	.byte	74
	.byte	76
	.byte	88
	.byte	-49
	.byte	-48
	.byte	-17
	.byte	-86
	.byte	-5
	.byte	67
	.byte	77
	.byte	51
	.byte	-123
	.byte	69
	.byte	-7
	.byte	2
	.byte	127
	.byte	80
	.byte	60
	.byte	-97
	.byte	-88
	.byte	81
	.byte	-93
	.byte	64
	.byte	-113
	.byte	-110
	.byte	-99
	.byte	56
	.byte	-11
	.byte	-68
	.byte	-74
	.byte	-38
	.byte	33
	.byte	16
	.byte	-1
	.byte	-13
	.byte	-46
	.byte	-51
	.byte	12
	.byte	19
	.byte	-20
	.byte	95
	.byte	-105
	.byte	68
	.byte	23
	.byte	-60
	.byte	-89
	.byte	126
	.byte	61
	.byte	100
	.byte	93
	.byte	25
	.byte	115
	.byte	96
	.byte	-127
	.byte	79
	.byte	-36
	.byte	34
	.byte	42
	.byte	-112
	.byte	-120
	.byte	70
	.byte	-18
	.byte	-72
	.byte	20
	.byte	-34
	.byte	94
	.byte	11
	.byte	-37
	.byte	-32
	.byte	50
	.byte	58
	.byte	10
	.byte	73
	.byte	6
	.byte	36
	.byte	92
	.byte	-62
	.byte	-45
	.byte	-84
	.byte	98
	.byte	-111
	.byte	-107
	.byte	-28
	.byte	121
	.byte	-25
	.byte	-56
	.byte	55
	.byte	109
	.byte	-115
	.byte	-43
	.byte	78
	.byte	-87
	.byte	108
	.byte	86
	.byte	-12
	.byte	-22
	.byte	101
	.byte	122
	.byte	-82
	.byte	8
	.byte	-70
	.byte	120
	.byte	37
	.byte	46
	.byte	28
	.byte	-90
	.byte	-76
	.byte	-58
	.byte	-24
	.byte	-35
	.byte	116
	.byte	31
	.byte	75
	.byte	-67
	.byte	-117
	.byte	-118
	.byte	112
	.byte	62
	.byte	-75
	.byte	102
	.byte	72
	.byte	3
	.byte	-10
	.byte	14
	.byte	97
	.byte	53
	.byte	87
	.byte	-71
	.byte	-122
	.byte	-63
	.byte	29
	.byte	-98
	.byte	-31
	.byte	-8
	.byte	-104
	.byte	17
	.byte	105
	.byte	-39
	.byte	-114
	.byte	-108
	.byte	-101
	.byte	30
	.byte	-121
	.byte	-23
	.byte	-50
	.byte	85
	.byte	40
	.byte	-33
	.byte	-116
	.byte	-95
	.byte	-119
	.byte	13
	.byte	-65
	.byte	-26
	.byte	66
	.byte	104
	.byte	65
	.byte	-103
	.byte	45
	.byte	15
	.byte	-80
	.byte	84
	.byte	-69
	.byte	22
	.section	.rodata.rsbox,"a",@progbits
	.type	rsbox, @object
	.size	rsbox, 256
rsbox:
	.byte	82
	.byte	9
	.byte	106
	.byte	-43
	.byte	48
	.byte	54
	.byte	-91
	.byte	56
	.byte	-65
	.byte	64
	.byte	-93
	.byte	-98
	.byte	-127
	.byte	-13
	.byte	-41
	.byte	-5
	.byte	124
	.byte	-29
	.byte	57
	.byte	-126
	.byte	-101
	.byte	47
	.byte	-1
	.byte	-121
	.byte	52
	.byte	-114
	.byte	67
	.byte	68
	.byte	-60
	.byte	-34
	.byte	-23
	.byte	-53
	.byte	84
	.byte	123
	.byte	-108
	.byte	50
	.byte	-90
	.byte	-62
	.byte	35
	.byte	61
	.byte	-18
	.byte	76
	.byte	-107
	.byte	11
	.byte	66
	.byte	-6
	.byte	-61
	.byte	78
	.byte	8
	.byte	46
	.byte	-95
	.byte	102
	.byte	40
	.byte	-39
	.byte	36
	.byte	-78
	.byte	118
	.byte	91
	.byte	-94
	.byte	73
	.byte	109
	.byte	-117
	.byte	-47
	.byte	37
	.byte	114
	.byte	-8
	.byte	-10
	.byte	100
	.byte	-122
	.byte	104
	.byte	-104
	.byte	22
	.byte	-44
	.byte	-92
	.byte	92
	.byte	-52
	.byte	93
	.byte	101
	.byte	-74
	.byte	-110
	.byte	108
	.byte	112
	.byte	72
	.byte	80
	.byte	-3
	.byte	-19
	.byte	-71
	.byte	-38
	.byte	94
	.byte	21
	.byte	70
	.byte	87
	.byte	-89
	.byte	-115
	.byte	-99
	.byte	-124
	.byte	-112
	.byte	-40
	.byte	-85
	.byte	0
	.byte	-116
	.byte	-68
	.byte	-45
	.byte	10
	.byte	-9
	.byte	-28
	.byte	88
	.byte	5
	.byte	-72
	.byte	-77
	.byte	69
	.byte	6
	.byte	-48
	.byte	44
	.byte	30
	.byte	-113
	.byte	-54
	.byte	63
	.byte	15
	.byte	2
	.byte	-63
	.byte	-81
	.byte	-67
	.byte	3
	.byte	1
	.byte	19
	.byte	-118
	.byte	107
	.byte	58
	.byte	-111
	.byte	17
	.byte	65
	.byte	79
	.byte	103
	.byte	-36
	.byte	-22
	.byte	-105
	.byte	-14
	.byte	-49
	.byte	-50
	.byte	-16
	.byte	-76
	.byte	-26
	.byte	115
	.byte	-106
	.byte	-84
	.byte	116
	.byte	34
	.byte	-25
	.byte	-83
	.byte	53
	.byte	-123
	.byte	-30
	.byte	-7
	.byte	55
	.byte	-24
	.byte	28
	.byte	117
	.byte	-33
	.byte	110
	.byte	71
	.byte	-15
	.byte	26
	.byte	113
	.byte	29
	.byte	41
	.byte	-59
	.byte	-119
	.byte	111
	.byte	-73
	.byte	98
	.byte	14
	.byte	-86
	.byte	24
	.byte	-66
	.byte	27
	.byte	-4
	.byte	86
	.byte	62
	.byte	75
	.byte	-58
	.byte	-46
	.byte	121
	.byte	32
	.byte	-102
	.byte	-37
	.byte	-64
	.byte	-2
	.byte	120
	.byte	-51
	.byte	90
	.byte	-12
	.byte	31
	.byte	-35
	.byte	-88
	.byte	51
	.byte	-120
	.byte	7
	.byte	-57
	.byte	49
	.byte	-79
	.byte	18
	.byte	16
	.byte	89
	.byte	39
	.byte	-128
	.byte	-20
	.byte	95
	.byte	96
	.byte	81
	.byte	127
	.byte	-87
	.byte	25
	.byte	-75
	.byte	74
	.byte	13
	.byte	45
	.byte	-27
	.byte	122
	.byte	-97
	.byte	-109
	.byte	-55
	.byte	-100
	.byte	-17
	.byte	-96
	.byte	-32
	.byte	59
	.byte	77
	.byte	-82
	.byte	42
	.byte	-11
	.byte	-80
	.byte	-56
	.byte	-21
	.byte	-69
	.byte	60
	.byte	-125
	.byte	83
	.byte	-103
	.byte	97
	.byte	23
	.byte	43
	.byte	4
	.byte	126
	.byte	-70
	.byte	119
	.byte	-42
	.byte	38
	.byte	-31
	.byte	105
	.byte	20
	.byte	99
	.byte	85
	.byte	33
	.byte	12
	.byte	125
	.section	.rodata.Rcon,"a",@progbits
	.type	Rcon, @object
	.size	Rcon, 255
Rcon:
	.byte	-115
	.byte	1
	.byte	2
	.byte	4
	.byte	8
	.byte	16
	.byte	32
	.byte	64
	.byte	-128
	.byte	27
	.byte	54
	.byte	108
	.byte	-40
	.byte	-85
	.byte	77
	.byte	-102
	.byte	47
	.byte	94
	.byte	-68
	.byte	99
	.byte	-58
	.byte	-105
	.byte	53
	.byte	106
	.byte	-44
	.byte	-77
	.byte	125
	.byte	-6
	.byte	-17
	.byte	-59
	.byte	-111
	.byte	57
	.byte	114
	.byte	-28
	.byte	-45
	.byte	-67
	.byte	97
	.byte	-62
	.byte	-97
	.byte	37
	.byte	74
	.byte	-108
	.byte	51
	.byte	102
	.byte	-52
	.byte	-125
	.byte	29
	.byte	58
	.byte	116
	.byte	-24
	.byte	-53
	.byte	-115
	.byte	1
	.byte	2
	.byte	4
	.byte	8
	.byte	16
	.byte	32
	.byte	64
	.byte	-128
	.byte	27
	.byte	54
	.byte	108
	.byte	-40
	.byte	-85
	.byte	77
	.byte	-102
	.byte	47
	.byte	94
	.byte	-68
	.byte	99
	.byte	-58
	.byte	-105
	.byte	53
	.byte	106
	.byte	-44
	.byte	-77
	.byte	125
	.byte	-6
	.byte	-17
	.byte	-59
	.byte	-111
	.byte	57
	.byte	114
	.byte	-28
	.byte	-45
	.byte	-67
	.byte	97
	.byte	-62
	.byte	-97
	.byte	37
	.byte	74
	.byte	-108
	.byte	51
	.byte	102
	.byte	-52
	.byte	-125
	.byte	29
	.byte	58
	.byte	116
	.byte	-24
	.byte	-53
	.byte	-115
	.byte	1
	.byte	2
	.byte	4
	.byte	8
	.byte	16
	.byte	32
	.byte	64
	.byte	-128
	.byte	27
	.byte	54
	.byte	108
	.byte	-40
	.byte	-85
	.byte	77
	.byte	-102
	.byte	47
	.byte	94
	.byte	-68
	.byte	99
	.byte	-58
	.byte	-105
	.byte	53
	.byte	106
	.byte	-44
	.byte	-77
	.byte	125
	.byte	-6
	.byte	-17
	.byte	-59
	.byte	-111
	.byte	57
	.byte	114
	.byte	-28
	.byte	-45
	.byte	-67
	.byte	97
	.byte	-62
	.byte	-97
	.byte	37
	.byte	74
	.byte	-108
	.byte	51
	.byte	102
	.byte	-52
	.byte	-125
	.byte	29
	.byte	58
	.byte	116
	.byte	-24
	.byte	-53
	.byte	-115
	.byte	1
	.byte	2
	.byte	4
	.byte	8
	.byte	16
	.byte	32
	.byte	64
	.byte	-128
	.byte	27
	.byte	54
	.byte	108
	.byte	-40
	.byte	-85
	.byte	77
	.byte	-102
	.byte	47
	.byte	94
	.byte	-68
	.byte	99
	.byte	-58
	.byte	-105
	.byte	53
	.byte	106
	.byte	-44
	.byte	-77
	.byte	125
	.byte	-6
	.byte	-17
	.byte	-59
	.byte	-111
	.byte	57
	.byte	114
	.byte	-28
	.byte	-45
	.byte	-67
	.byte	97
	.byte	-62
	.byte	-97
	.byte	37
	.byte	74
	.byte	-108
	.byte	51
	.byte	102
	.byte	-52
	.byte	-125
	.byte	29
	.byte	58
	.byte	116
	.byte	-24
	.byte	-53
	.byte	-115
	.byte	1
	.byte	2
	.byte	4
	.byte	8
	.byte	16
	.byte	32
	.byte	64
	.byte	-128
	.byte	27
	.byte	54
	.byte	108
	.byte	-40
	.byte	-85
	.byte	77
	.byte	-102
	.byte	47
	.byte	94
	.byte	-68
	.byte	99
	.byte	-58
	.byte	-105
	.byte	53
	.byte	106
	.byte	-44
	.byte	-77
	.byte	125
	.byte	-6
	.byte	-17
	.byte	-59
	.byte	-111
	.byte	57
	.byte	114
	.byte	-28
	.byte	-45
	.byte	-67
	.byte	97
	.byte	-62
	.byte	-97
	.byte	37
	.byte	74
	.byte	-108
	.byte	51
	.byte	102
	.byte	-52
	.byte	-125
	.byte	29
	.byte	58
	.byte	116
	.byte	-24
	.byte	-53
	.section	.text.getSBoxValue,"ax",@progbits
	.balign 2
	.type	getSBoxValue, @function
getSBoxValue:
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
	MOV.B	R12, 1(R1)
	MOV.B	1(R1), R12
	MOV.B	sbox(R12), R12
	; start of epilogue
	ADD.W	#2, R1
	RET
	.size	getSBoxValue, .-getSBoxValue
	.section	.text.getSBoxInvert,"ax",@progbits
	.balign 2
	.type	getSBoxInvert, @function
getSBoxInvert:
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
	MOV.B	R12, 1(R1)
	MOV.B	1(R1), R12
	MOV.B	rsbox(R12), R12
	; start of epilogue
	ADD.W	#2, R1
	RET
	.size	getSBoxInvert, .-getSBoxInvert
	.section	.text.KeyExpansion,"ax",@progbits
	.balign 2
	.type	KeyExpansion, @function
KeyExpansion:
; start of function
; framesize_regs:     14
; framesize_locals:   64
; framesize_outgoing: 0
; framesize:          78
; elim ap -> fp       16
; elim fp -> sp       64
; saved regs: R4 R5 R6 R7 R8 R9 R10
	; start of prologue
	PUSHM.W	#7, R10
	SUB.W	#64, R1
	; end of prologue
	MOV.W	#0, 60(R1)
	MOV.W	#0, 62(R1)
	BR	#.L6
.L7:
	MOV.W	60(R1), R13
	MOV.W	62(R1), R14
	RLA.W	R13 { RLC.W	R14 { RLA.W	R13 { RLC.W	R14
	MOV.W	&Key, R15
	MOV.W	60(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	R15, R12
	MOV.B	@R12, R15
	MOV.W	R13, R12
	ADD.W	#RoundKey, R12
	MOV.B	R15, @R12
	MOV.W	60(R1), R12
	MOV.W	62(R1), R13
	RLA.W	R12 { RLC.W	R13 { RLA.W	R12 { RLC.W	R13
	MOV.W	R12, R6
	ADD	#1, R6 ; cy
	MOV.W	R13, R7
	ADDC	#0, R7
	MOV.W	&Key, R13
	MOV.W	60(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	#1, R12
	ADD.W	R13, R12
	MOV.B	@R12, R13
	MOV.W	R6, R12
	ADD.W	#RoundKey, R12
	MOV.B	R13, @R12
	MOV.W	60(R1), R12
	MOV.W	62(R1), R13
	RLA.W	R12 { RLC.W	R13 { RLA.W	R12 { RLC.W	R13
	MOV.W	R12, R8
	ADD	#2, R8 ; cy
	MOV.W	R13, R9
	ADDC	#0, R9
	MOV.W	&Key, R13
	MOV.W	60(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	#2, R12
	ADD.W	R13, R12
	MOV.B	@R12, R13
	MOV.W	R8, R12
	ADD.W	#RoundKey, R12
	MOV.B	R13, @R12
	MOV.W	60(R1), R12
	MOV.W	62(R1), R13
	RLA.W	R12 { RLC.W	R13 { RLA.W	R12 { RLC.W	R13
	MOV.W	R12, R10
	ADD	#3, R10 ; cy
	MOV.W	R13, R11
	ADDC	#0, R11
	MOV.W	&Key, R13
	MOV.W	60(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	#3, R12
	ADD.W	R13, R12
	MOV.B	@R12, R13
	MOV.W	R10, R12
	ADD.W	#RoundKey, R12
	MOV.B	R13, @R12
	ADD	#1, 60(R1) { ADDC	#0, 62(R1)
.L6:
	CMP.W	#0, 62(R1) { JNE	.L9
	CMP.W	#0, 62(R1) { JNE	.L7
	MOV.B	#3, R12
	CMP.W	60(R1), R12 { JHS	.L7
	BR	#.L9
.L14:
	MOV.W	#0, 56(R1)
	MOV.W	#0, 58(R1)
	BR	#.L10
.L11:
	MOV.W	60(R1), R12
	ADD	#-1, R12 ; cy
	MOV.W	R12, 44(R1)
	MOV.W	62(R1), R13
	ADDC	#16383, R13
	MOV.W	R13, 46(R1)
	MOV.W	44(R1), R12
	MOV.W	46(R1), R13
	RLA.W	R12 { RLC.W	R13 { RLA.W	R12 { RLC.W	R13
	MOV.W	R12, R14
	ADD	56(R1), R14 ; cy
	MOV.W	R14, 40(R1)
	MOV.W	R13, R14
	ADDC	58(R1), R14
	MOV.W	R14, 42(R1)
	MOV.W	40(R1), R12
	ADD.W	#RoundKey, R12
	MOV.B	@R12, R13
	MOV.W	R1, R12
	ADD.W	#48, R12
	ADD.W	56(R1), R12
	MOV.B	R13, @R12
	ADD	#1, 56(R1) { ADDC	#0, 58(R1)
.L10:
	CMP.W	#0, 58(R1) { JNE	.L17
	CMP.W	#0, 58(R1) { JNE	.L11
	MOV.B	#3, R12
	CMP.W	56(R1), R12 { JHS	.L11
.L17:
	MOV.W	60(R1), R4
	AND.B	#3, R4
	MOV.W	62(R1), R5
	BIC.W	#-1, R5
	MOV.W	R4, R12
	MOV.W	R5, R13
	BIS.W	R13, R12
	CMP.W	#0, R12 { JNE	.L13
	MOV.B	48(R1), R12
	MOV.B	R12,R13 { CLR	R14
	MOV.W	R13, 52(R1)
	MOV.W	R14, 54(R1)
	MOV.B	49(R1), R12
	MOV.B	R12, 48(R1)
	MOV.B	50(R1), R12
	MOV.B	R12, 49(R1)
	MOV.B	51(R1), R12
	MOV.B	R12, 50(R1)
	MOV.B	52(R1), R12
	MOV.B	R12, 51(R1)
	MOV.B	48(R1), R12
	CALL	#getSBoxValue
	AND	#0xff, R12
	MOV.B	R12, 48(R1)
	MOV.B	49(R1), R12
	CALL	#getSBoxValue
	AND	#0xff, R12
	MOV.B	R12, 49(R1)
	MOV.B	50(R1), R12
	CALL	#getSBoxValue
	AND	#0xff, R12
	MOV.B	R12, 50(R1)
	MOV.B	51(R1), R12
	CALL	#getSBoxValue
	AND	#0xff, R12
	MOV.B	R12, 51(R1)
	MOV.B	48(R1), R14
	MOV.W	60(R1), R12
	MOV.W	62(R1), R13
	RRUX.W	R13 { RRC.W	R12 { RRUX.W	R13 { RRC.W	R12
	ADD.W	#Rcon, R12
	MOV.B	@R12, R12
	XOR.B	R14, R12
	AND	#0xff, R12
	MOV.B	R12, 48(R1)
.L13:
	MOV.W	60(R1), R14
	MOV.W	62(R1), R15
	RLA.W	R14 { RLC.W	R15 { RLA.W	R14 { RLC.W	R15
	MOV.W	60(R1), R12
	ADD	#-4, R12 ; cy
	MOV.W	R12, 36(R1)
	MOV.W	62(R1), R13
	ADDC	#16383, R13
	MOV.W	R13, 38(R1)
	MOV.W	36(R1), R12
	MOV.W	38(R1), R13
	RLA.W	R12 { RLC.W	R13 { RLA.W	R12 { RLC.W	R13
	ADD.W	#RoundKey, R12
	MOV.B	@R12, R13
	MOV.B	48(R1), R12
	XOR.B	R13, R12
	MOV.B	R12, R13
	MOV.W	R14, R12
	ADD.W	#RoundKey, R12
	MOV.B	R13, @R12
	MOV.W	60(R1), R12
	MOV.W	62(R1), R13
	RLA.W	R12 { RLC.W	R13 { RLA.W	R12 { RLC.W	R13
	MOV.W	R12, R14
	ADD	#1, R14 ; cy
	MOV.W	R14, 32(R1)
	MOV.W	R13, R14
	ADDC	#0, R14
	MOV.W	R14, 34(R1)
	MOV.W	60(R1), R12
	ADD	#-4, R12 ; cy
	MOV.W	R12, 28(R1)
	MOV.W	62(R1), R13
	ADDC	#16383, R13
	MOV.W	R13, 30(R1)
	MOV.W	28(R1), R12
	MOV.W	30(R1), R13
	RLA.W	R12 { RLC.W	R13 { RLA.W	R12 { RLC.W	R13
	MOV.W	R12, R14
	ADD	#1, R14 ; cy
	MOV.W	R14, 24(R1)
	MOV.W	R13, R14
	ADDC	#0, R14
	MOV.W	R14, 26(R1)
	MOV.W	24(R1), R12
	ADD.W	#RoundKey, R12
	MOV.B	@R12, R13
	MOV.B	49(R1), R12
	XOR.B	R13, R12
	MOV.B	R12, R13
	MOV.W	32(R1), R12
	ADD.W	#RoundKey, R12
	MOV.B	R13, @R12
	MOV.W	60(R1), R12
	MOV.W	62(R1), R13
	RLA.W	R12 { RLC.W	R13 { RLA.W	R12 { RLC.W	R13
	MOV.W	R12, R14
	ADD	#2, R14 ; cy
	MOV.W	R14, 20(R1)
	MOV.W	R13, R14
	ADDC	#0, R14
	MOV.W	R14, 22(R1)
	MOV.W	60(R1), R12
	ADD	#-4, R12 ; cy
	MOV.W	R12, 16(R1)
	MOV.W	62(R1), R13
	ADDC	#16383, R13
	MOV.W	R13, 18(R1)
	MOV.W	16(R1), R12
	MOV.W	18(R1), R13
	RLA.W	R12 { RLC.W	R13 { RLA.W	R12 { RLC.W	R13
	MOV.W	R12, R14
	ADD	#2, R14 ; cy
	MOV.W	R14, 12(R1)
	MOV.W	R13, R14
	ADDC	#0, R14
	MOV.W	R14, 14(R1)
	MOV.W	12(R1), R12
	ADD.W	#RoundKey, R12
	MOV.B	@R12, R13
	MOV.B	50(R1), R12
	XOR.B	R13, R12
	MOV.B	R12, R13
	MOV.W	20(R1), R12
	ADD.W	#RoundKey, R12
	MOV.B	R13, @R12
	MOV.W	60(R1), R12
	MOV.W	62(R1), R13
	RLA.W	R12 { RLC.W	R13 { RLA.W	R12 { RLC.W	R13
	MOV.W	R12, R14
	ADD	#3, R14 ; cy
	MOV.W	R14, 8(R1)
	MOV.W	R13, R14
	ADDC	#0, R14
	MOV.W	R14, 10(R1)
	MOV.W	60(R1), R12
	ADD	#-4, R12 ; cy
	MOV.W	R12, 4(R1)
	MOV.W	62(R1), R13
	ADDC	#16383, R13
	MOV.W	R13, 6(R1)
	MOV.W	4(R1), R12
	MOV.W	6(R1), R13
	RLA.W	R12 { RLC.W	R13 { RLA.W	R12 { RLC.W	R13
	MOV.W	R12, R14
	ADD	#3, R14 ; cy
	MOV.W	R14, @R1
	MOV.W	R13, R14
	ADDC	#0, R14
	MOV.W	R14, 2(R1)
	MOV.W	@R1, R12
	ADD.W	#RoundKey, R12
	MOV.B	@R12, R13
	MOV.B	51(R1), R12
	XOR.B	R13, R12
	MOV.B	R12, R13
	MOV.W	8(R1), R12
	ADD.W	#RoundKey, R12
	MOV.B	R13, @R12
	ADD	#1, 60(R1) { ADDC	#0, 62(R1)
.L9:
	CMP.W	#0, 62(R1) { JNE	.L18
	CMP.W	#0, 62(R1) { JNE	.L14
	MOV.B	#43, R12
	CMP.W	60(R1), R12 { JHS	.L14
.L18:
	NOP
	; start of epilogue
	ADD.W	#64, R1
	POPM.W	#7, r10
	RET
	.size	KeyExpansion, .-KeyExpansion
	.section	.text.AddRoundKey,"ax",@progbits
	.balign 2
	.type	AddRoundKey, @function
AddRoundKey:
; start of function
; framesize_regs:     4
; framesize_locals:   4
; framesize_outgoing: 0
; framesize:          8
; elim ap -> fp       6
; elim fp -> sp       4
; saved regs: R9 R10
	; start of prologue
	PUSHM.W	#2, R10
	SUB.W	#4, R1
	; end of prologue
	MOV.B	R12, 1(R1)
	MOV.B	#0, 3(R1)
	BR	#.L20
.L23:
	MOV.B	#0, 2(R1)
	BR	#.L21
.L22:
	MOV.W	&state, R9
	MOV.B	3(R1), R12
	MOV.B	2(R1), R14
	MOV.W	&state, R10
	MOV.B	3(R1), R13
	MOV.B	2(R1), R15
	rpt	#2 { rlax.w	R13
	ADD.W	R10, R13
	ADD.W	R15, R13
	MOV.B	@R13, R15
	MOV.B	1(R1), R13
	MOV.W	R13, R10
	rpt	#2 { rlax.w	R10
	MOV.B	3(R1), R13
	ADD.W	R10, R13
	MOV.W	R13, R10
	rpt	#2 { rlax.w	R10
	MOV.B	2(R1), R13
	ADD.W	R10, R13
	MOV.B	RoundKey(R13), R13
	XOR.B	R15, R13
	AND	#0xff, R13
	rpt	#2 { rlax.w	R12
	ADD.W	R9, R12
	ADD.W	R14, R12
	MOV.B	R13, @R12
	ADD.B	#1, 2(R1)
.L21:
	MOV.B	#3, R12
	CMP.B	2(R1), R12 { JHS	.L22
	ADD.B	#1, 3(R1)
.L20:
	MOV.B	#3, R12
	CMP.B	3(R1), R12 { JHS	.L23
	NOP
	; start of epilogue
	ADD.W	#4, R1
	POPM.W	#2, r10
	RET
	.size	AddRoundKey, .-AddRoundKey
	.section	.text.SubBytes,"ax",@progbits
	.balign 2
	.type	SubBytes, @function
SubBytes:
; start of function
; framesize_regs:     6
; framesize_locals:   2
; framesize_outgoing: 0
; framesize:          8
; elim ap -> fp       8
; elim fp -> sp       2
; saved regs: R8 R9 R10
	; start of prologue
	PUSHM.W	#3, R10
	SUB.W	#2, R1
	; end of prologue
	MOV.B	#0, 1(R1)
	BR	#.L25
.L28:
	MOV.B	#0, @R1
	BR	#.L26
.L27:
	MOV.W	&state, R8
	MOV.B	@R1, R10
	MOV.B	1(R1), R9
	MOV.W	&state, R14
	MOV.B	@R1, R12
	MOV.B	1(R1), R13
	rpt	#2 { rlax.w	R12
	ADD.W	R14, R12
	ADD.W	R13, R12
	MOV.B	@R12, R12
	CALL	#getSBoxValue
	MOV.B	R12, R13
	MOV.W	R10, R12
	rpt	#2 { rlax.w	R12
	ADD.W	R8, R12
	ADD.W	R9, R12
	MOV.B	R13, @R12
	ADD.B	#1, @R1
.L26:
	MOV.B	#3, R12
	CMP.B	@R1, R12 { JHS	.L27
	ADD.B	#1, 1(R1)
.L25:
	MOV.B	#3, R12
	CMP.B	1(R1), R12 { JHS	.L28
	NOP
	; start of epilogue
	ADD.W	#2, R1
	POPM.W	#3, r10
	RET
	.size	SubBytes, .-SubBytes
	.section	.text.ShiftRows,"ax",@progbits
	.balign 2
	.type	ShiftRows, @function
ShiftRows:
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
	MOV.W	&state, R12
	MOV.B	1(R12), 1(R1)
	MOV.W	&state, R12
	MOV.W	&state, R13
	MOV.B	5(R13), R13
	MOV.B	R13, 1(R12)
	MOV.W	&state, R12
	MOV.W	&state, R13
	MOV.B	9(R13), R13
	MOV.B	R13, 5(R12)
	MOV.W	&state, R12
	MOV.W	&state, R13
	MOV.B	13(R13), R13
	MOV.B	R13, 9(R12)
	MOV.W	&state, R12
	MOV.B	1(R1), 13(R12)
	MOV.W	&state, R12
	MOV.B	2(R12), 1(R1)
	MOV.W	&state, R12
	MOV.W	&state, R13
	MOV.B	10(R13), R13
	MOV.B	R13, 2(R12)
	MOV.W	&state, R12
	MOV.B	1(R1), 10(R12)
	MOV.W	&state, R12
	MOV.B	6(R12), 1(R1)
	MOV.W	&state, R12
	MOV.W	&state, R13
	MOV.B	14(R13), R13
	MOV.B	R13, 6(R12)
	MOV.W	&state, R12
	MOV.B	1(R1), 14(R12)
	MOV.W	&state, R12
	MOV.B	3(R12), 1(R1)
	MOV.W	&state, R12
	MOV.W	&state, R13
	MOV.B	15(R13), R13
	MOV.B	R13, 3(R12)
	MOV.W	&state, R12
	MOV.W	&state, R13
	MOV.B	11(R13), R13
	MOV.B	R13, 15(R12)
	MOV.W	&state, R12
	MOV.W	&state, R13
	MOV.B	7(R13), R13
	MOV.B	R13, 11(R12)
	MOV.W	&state, R12
	MOV.B	1(R1), 7(R12)
	NOP
	; start of epilogue
	ADD.W	#2, R1
	RET
	.size	ShiftRows, .-ShiftRows
	.section	.text.xtime,"ax",@progbits
	.balign 2
	.type	xtime, @function
xtime:
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
	MOV.B	R12, 1(R1)
	MOV.B	1(R1), R12
	ADD.W	R12, R12
	MOV.B	R12, R14
	SXT	R14
	MOV.B	1(R1), R12
	rrum.w	#4, R12 { rrum.w	#3, R12 
	AND	#0xff, R12
	MOV.B	R12, R13
	MOV.B	R13, R12
	ADD.B	R12, R12
	ADD.B	R13, R12
	MOV.W	R12, R13
	rpt	#3 { rlax.w	R13
	ADD.B	R13, R12
	AND	#0xff, R12
	SXT	R12
	XOR.B	R14, R12
	SXT	R12
	AND	#0xff, R12
	; start of epilogue
	ADD.W	#2, R1
	RET
	.size	xtime, .-xtime
	.section	.text.MixColumns,"ax",@progbits
	.balign 2
	.type	MixColumns, @function
MixColumns:
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
	MOV.B	#0, 3(R1)
	BR	#.L33
.L34:
	MOV.W	&state, R13
	MOV.B	3(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	R13, R12
	MOV.B	@R12, 2(R1)
	MOV.W	&state, R13
	MOV.B	3(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	R13, R12
	MOV.B	@R12, R13
	MOV.W	&state, R14
	MOV.B	3(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	R14, R12
	ADD.W	#1, R12
	MOV.B	@R12, R12
	XOR.B	R13, R12
	MOV.B	R12, R14
	MOV.W	&state, R13
	MOV.B	3(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	R13, R12
	ADD.W	#2, R12
	MOV.B	@R12, R12
	XOR.B	R14, R12
	MOV.B	R12, R14
	MOV.W	&state, R13
	MOV.B	3(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	R13, R12
	ADD.W	#3, R12
	MOV.B	@R12, R12
	MOV.B	R14, R13
	XOR.B	R12, R13
	MOV.B	R13, 1(R1)
	MOV.W	&state, R13
	MOV.B	3(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	R13, R12
	MOV.B	@R12, R13
	MOV.W	&state, R14
	MOV.B	3(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	R14, R12
	ADD.W	#1, R12
	MOV.B	@R12, R12
	MOV.B	R13, R14
	XOR.B	R12, R14
	MOV.B	R14, @R1
	MOV.B	@R1, R12
	CALL	#xtime
	MOV.B	R12, @R1
	MOV.W	&state, R14
	MOV.B	3(R1), R12
	MOV.W	&state, R15
	MOV.B	3(R1), R13
	rpt	#2 { rlax.w	R13
	ADD.W	R15, R13
	MOV.B	@R13, R13
	MOV.B	@R1, R15
	XOR.B	1(R1), R15
	AND	#0xff, R15
	XOR.B	R15, R13
	AND	#0xff, R13
	rpt	#2 { rlax.w	R12
	ADD.W	R14, R12
	MOV.B	R13, @R12
	MOV.W	&state, R13
	MOV.B	3(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	R13, R12
	ADD.W	#1, R12
	MOV.B	@R12, R13
	MOV.W	&state, R14
	MOV.B	3(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	R14, R12
	ADD.W	#2, R12
	MOV.B	@R12, R12
	MOV.B	R13, R14
	XOR.B	R12, R14
	MOV.B	R14, @R1
	MOV.B	@R1, R12
	CALL	#xtime
	MOV.B	R12, @R1
	MOV.W	&state, R14
	MOV.B	3(R1), R12
	MOV.W	&state, R15
	MOV.B	3(R1), R13
	rpt	#2 { rlax.w	R13
	ADD.W	R15, R13
	ADD.W	#1, R13
	MOV.B	@R13, R13
	MOV.B	@R1, R15
	XOR.B	1(R1), R15
	AND	#0xff, R15
	XOR.B	R15, R13
	AND	#0xff, R13
	rpt	#2 { rlax.w	R12
	ADD.W	R14, R12
	ADD.W	#1, R12
	MOV.B	R13, @R12
	MOV.W	&state, R13
	MOV.B	3(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	R13, R12
	ADD.W	#2, R12
	MOV.B	@R12, R13
	MOV.W	&state, R14
	MOV.B	3(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	R14, R12
	ADD.W	#3, R12
	MOV.B	@R12, R12
	MOV.B	R13, R14
	XOR.B	R12, R14
	MOV.B	R14, @R1
	MOV.B	@R1, R12
	CALL	#xtime
	MOV.B	R12, @R1
	MOV.W	&state, R14
	MOV.B	3(R1), R12
	MOV.W	&state, R15
	MOV.B	3(R1), R13
	rpt	#2 { rlax.w	R13
	ADD.W	R15, R13
	ADD.W	#2, R13
	MOV.B	@R13, R13
	MOV.B	@R1, R15
	XOR.B	1(R1), R15
	AND	#0xff, R15
	XOR.B	R15, R13
	AND	#0xff, R13
	rpt	#2 { rlax.w	R12
	ADD.W	R14, R12
	ADD.W	#2, R12
	MOV.B	R13, @R12
	MOV.W	&state, R13
	MOV.B	3(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	R13, R12
	ADD.W	#3, R12
	MOV.B	@R12, R12
	MOV.B	2(R1), R13
	XOR.B	R12, R13
	MOV.B	R13, @R1
	MOV.B	@R1, R12
	CALL	#xtime
	MOV.B	R12, @R1
	MOV.W	&state, R14
	MOV.B	3(R1), R12
	MOV.W	&state, R15
	MOV.B	3(R1), R13
	rpt	#2 { rlax.w	R13
	ADD.W	R15, R13
	ADD.W	#3, R13
	MOV.B	@R13, R13
	MOV.B	@R1, R15
	XOR.B	1(R1), R15
	AND	#0xff, R15
	XOR.B	R15, R13
	AND	#0xff, R13
	rpt	#2 { rlax.w	R12
	ADD.W	R14, R12
	ADD.W	#3, R12
	MOV.B	R13, @R12
	ADD.B	#1, 3(R1)
.L33:
	MOV.B	#3, R12
	CMP.B	3(R1), R12 { JHS	.L34
	NOP
	; start of epilogue
	ADD.W	#4, R1
	RET
	.size	MixColumns, .-MixColumns
	.section	.text.InvMixColumns,"ax",@progbits
	.balign 2
	.type	InvMixColumns, @function
InvMixColumns:
; start of function
; framesize_regs:     6
; framesize_locals:   6
; framesize_outgoing: 0
; framesize:          12
; elim ap -> fp       8
; elim fp -> sp       6
; saved regs: R8 R9 R10
	; start of prologue
	PUSHM.W	#3, R10
	SUB.W	#6, R1
	; end of prologue
	MOV.W	#0, 4(R1)
	BR	#.L36
.L37:
	MOV.W	&state, R13
	MOV.W	4(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	R13, R12
	MOV.B	@R12, 3(R1)
	MOV.W	&state, R13
	MOV.W	4(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	R13, R12
	ADD.W	#1, R12
	MOV.B	@R12, 2(R1)
	MOV.W	&state, R13
	MOV.W	4(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	R13, R12
	ADD.W	#2, R12
	MOV.B	@R12, 1(R1)
	MOV.W	&state, R13
	MOV.W	4(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	R13, R12
	ADD.W	#3, R12
	MOV.B	@R12, @R1
	MOV.B	3(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.B	2(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.B	2(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.B	1(R1), R12
	CALL	#xtime
	MOV.B	1(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.B	@R1, R12
	CALL	#xtime
	MOV.B	@R1, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.B	@R1, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.W	&state, R10
	MOV.B	3(R1), R12
	CALL	#xtime
	MOV.B	R12, R9
	MOV.B	3(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	R9, R12
	MOV.B	R12, R9
	MOV.B	3(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	R9, R12
	MOV.B	R12, R9
	MOV.B	2(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	2(R1), R12
	MOV.B	R12, R8
	MOV.B	2(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	R8, R12
	AND	#0xff, R12
	XOR.B	R9, R12
	MOV.B	R12, R9
	MOV.B	1(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	1(R1), R12
	MOV.B	R12, R8
	MOV.B	1(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	R8, R12
	AND	#0xff, R12
	XOR.B	R9, R12
	MOV.B	R12, R9
	MOV.B	@R1, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	@R1, R12
	AND	#0xff, R12
	XOR.B	R9, R12
	MOV.B	R12, R13
	MOV.W	4(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	R10, R12
	MOV.B	R13, @R12
	MOV.B	3(R1), R12
	CALL	#xtime
	MOV.B	3(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.B	3(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.B	2(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.B	1(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.B	1(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.B	@R1, R12
	CALL	#xtime
	MOV.B	@R1, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.W	&state, R10
	MOV.B	3(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	3(R1), R12
	MOV.B	R12, R9
	MOV.B	2(R1), R12
	CALL	#xtime
	MOV.B	R12, R8
	MOV.B	2(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	R8, R12
	MOV.B	R12, R8
	MOV.B	2(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	R8, R12
	AND	#0xff, R12
	XOR.B	R9, R12
	MOV.B	R12, R9
	MOV.B	1(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	1(R1), R12
	MOV.B	R12, R8
	MOV.B	1(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	R8, R12
	AND	#0xff, R12
	XOR.B	R9, R12
	MOV.B	R12, R9
	MOV.B	@R1, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	@R1, R12
	MOV.B	R12, R8
	MOV.B	@R1, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	R8, R12
	AND	#0xff, R12
	XOR.B	R9, R12
	MOV.B	R12, R13
	MOV.W	4(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	R10, R12
	ADD.W	#1, R12
	MOV.B	R13, @R12
	MOV.B	3(R1), R12
	CALL	#xtime
	MOV.B	3(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.B	2(R1), R12
	CALL	#xtime
	MOV.B	2(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.B	2(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.B	1(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.B	@R1, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.B	@R1, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.W	&state, R10
	MOV.B	3(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	3(R1), R12
	MOV.B	R12, R9
	MOV.B	3(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	R9, R12
	MOV.B	R12, R9
	MOV.B	2(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	2(R1), R12
	AND	#0xff, R12
	XOR.B	R9, R12
	MOV.B	R12, R9
	MOV.B	1(R1), R12
	CALL	#xtime
	MOV.B	R12, R8
	MOV.B	1(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	R8, R12
	MOV.B	R12, R8
	MOV.B	1(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	R8, R12
	AND	#0xff, R12
	XOR.B	R9, R12
	MOV.B	R12, R9
	MOV.B	@R1, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	@R1, R12
	MOV.B	R12, R8
	MOV.B	@R1, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	R8, R12
	AND	#0xff, R12
	XOR.B	R9, R12
	MOV.B	R12, R13
	MOV.W	4(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	R10, R12
	ADD.W	#2, R12
	MOV.B	R13, @R12
	MOV.B	3(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.B	3(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.B	2(R1), R12
	CALL	#xtime
	MOV.B	2(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.B	1(R1), R12
	CALL	#xtime
	MOV.B	1(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.B	1(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.B	@R1, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	MOV.W	&state, R10
	MOV.B	3(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	3(R1), R12
	MOV.B	R12, R9
	MOV.B	3(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	R9, R12
	MOV.B	R12, R9
	MOV.B	2(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	2(R1), R12
	MOV.B	R12, R8
	MOV.B	2(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	R8, R12
	AND	#0xff, R12
	XOR.B	R9, R12
	MOV.B	R12, R9
	MOV.B	1(R1), R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	1(R1), R12
	AND	#0xff, R12
	XOR.B	R9, R12
	MOV.B	R12, R9
	MOV.B	@R1, R12
	CALL	#xtime
	MOV.B	R12, R8
	MOV.B	@R1, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	R8, R12
	MOV.B	R12, R8
	MOV.B	@R1, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	CALL	#xtime
	AND	#0xff, R12
	XOR.B	R8, R12
	AND	#0xff, R12
	XOR.B	R9, R12
	MOV.B	R12, R13
	MOV.W	4(R1), R12
	rpt	#2 { rlax.w	R12
	ADD.W	R10, R12
	ADD.W	#3, R12
	MOV.B	R13, @R12
	ADD.W	#1, 4(R1)
.L36:
	MOV.B	#3, R12
	CMP.W	4(R1), R12 { JGE	.L37
	NOP
	; start of epilogue
	ADD.W	#6, R1
	POPM.W	#3, r10
	RET
	.size	InvMixColumns, .-InvMixColumns
	.section	.text.InvSubBytes,"ax",@progbits
	.balign 2
	.type	InvSubBytes, @function
InvSubBytes:
; start of function
; framesize_regs:     6
; framesize_locals:   2
; framesize_outgoing: 0
; framesize:          8
; elim ap -> fp       8
; elim fp -> sp       2
; saved regs: R8 R9 R10
	; start of prologue
	PUSHM.W	#3, R10
	SUB.W	#2, R1
	; end of prologue
	MOV.B	#0, 1(R1)
	BR	#.L39
.L42:
	MOV.B	#0, @R1
	BR	#.L40
.L41:
	MOV.W	&state, R8
	MOV.B	@R1, R10
	MOV.B	1(R1), R9
	MOV.W	&state, R14
	MOV.B	@R1, R12
	MOV.B	1(R1), R13
	rpt	#2 { rlax.w	R12
	ADD.W	R14, R12
	ADD.W	R13, R12
	MOV.B	@R12, R12
	CALL	#getSBoxInvert
	MOV.B	R12, R13
	MOV.W	R10, R12
	rpt	#2 { rlax.w	R12
	ADD.W	R8, R12
	ADD.W	R9, R12
	MOV.B	R13, @R12
	ADD.B	#1, @R1
.L40:
	MOV.B	#3, R12
	CMP.B	@R1, R12 { JHS	.L41
	ADD.B	#1, 1(R1)
.L39:
	MOV.B	#3, R12
	CMP.B	1(R1), R12 { JHS	.L42
	NOP
	; start of epilogue
	ADD.W	#2, R1
	POPM.W	#3, r10
	RET
	.size	InvSubBytes, .-InvSubBytes
	.section	.text.InvShiftRows,"ax",@progbits
	.balign 2
	.type	InvShiftRows, @function
InvShiftRows:
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
	MOV.W	&state, R12
	MOV.B	13(R12), 1(R1)
	MOV.W	&state, R12
	MOV.W	&state, R13
	MOV.B	9(R13), R13
	MOV.B	R13, 13(R12)
	MOV.W	&state, R12
	MOV.W	&state, R13
	MOV.B	5(R13), R13
	MOV.B	R13, 9(R12)
	MOV.W	&state, R12
	MOV.W	&state, R13
	MOV.B	1(R13), R13
	MOV.B	R13, 5(R12)
	MOV.W	&state, R12
	MOV.B	1(R1), 1(R12)
	MOV.W	&state, R12
	MOV.B	2(R12), 1(R1)
	MOV.W	&state, R12
	MOV.W	&state, R13
	MOV.B	10(R13), R13
	MOV.B	R13, 2(R12)
	MOV.W	&state, R12
	MOV.B	1(R1), 10(R12)
	MOV.W	&state, R12
	MOV.B	6(R12), 1(R1)
	MOV.W	&state, R12
	MOV.W	&state, R13
	MOV.B	14(R13), R13
	MOV.B	R13, 6(R12)
	MOV.W	&state, R12
	MOV.B	1(R1), 14(R12)
	MOV.W	&state, R12
	MOV.B	3(R12), 1(R1)
	MOV.W	&state, R12
	MOV.W	&state, R13
	MOV.B	7(R13), R13
	MOV.B	R13, 3(R12)
	MOV.W	&state, R12
	MOV.W	&state, R13
	MOV.B	11(R13), R13
	MOV.B	R13, 7(R12)
	MOV.W	&state, R12
	MOV.W	&state, R13
	MOV.B	15(R13), R13
	MOV.B	R13, 11(R12)
	MOV.W	&state, R12
	MOV.B	1(R1), 15(R12)
	NOP
	; start of epilogue
	ADD.W	#2, R1
	RET
	.size	InvShiftRows, .-InvShiftRows
	.section	.text.Cipher,"ax",@progbits
	.balign 2
	.type	Cipher, @function
Cipher:
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
	MOV.B	#0, 1(R1)
	MOV.B	#0, R12
	CALL	#AddRoundKey
	MOV.B	#1, 1(R1)
	BR	#.L45
.L46:
	CALL	#SubBytes
	CALL	#ShiftRows
	CALL	#MixColumns
	MOV.B	1(R1), R12
	CALL	#AddRoundKey
	ADD.B	#1, 1(R1)
.L45:
	MOV.B	#9, R12
	CMP.B	1(R1), R12 { JHS	.L46
	CALL	#SubBytes
	CALL	#ShiftRows
	MOV.B	#10, R12
	CALL	#AddRoundKey
	NOP
	; start of epilogue
	ADD.W	#2, R1
	RET
	.size	Cipher, .-Cipher
	.section	.text.InvCipher,"ax",@progbits
	.balign 2
	.type	InvCipher, @function
InvCipher:
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
	MOV.B	#0, 1(R1)
	MOV.B	#10, R12
	CALL	#AddRoundKey
	MOV.B	#9, 1(R1)
	BR	#.L48
.L49:
	CALL	#InvShiftRows
	CALL	#InvSubBytes
	MOV.B	1(R1), R12
	CALL	#AddRoundKey
	CALL	#InvMixColumns
	MOV.B	1(R1), R12
	MOV.B	R12, R13
	ADD.B	#-1, R13
	MOV.B	R13, 1(R1)
.L48:
	CMP.B	#0, 1(R1) { JNE	.L49
	CALL	#InvShiftRows
	CALL	#InvSubBytes
	MOV.B	#0, R12
	CALL	#AddRoundKey
	NOP
	; start of epilogue
	ADD.W	#2, R1
	RET
	.size	InvCipher, .-InvCipher
	.section	.text.BlockCopy,"ax",@progbits
	.balign 2
	.type	BlockCopy, @function
BlockCopy:
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
	MOV.W	R12, 2(R1)
	MOV.W	R13, @R1
	MOV.B	#0, 5(R1)
	BR	#.L51
.L52:
	MOV.B	5(R1), R12
	ADD.W	2(R1), R12
	MOV.B	5(R1), R13
	ADD.W	@R1, R13
	MOV.B	@R13, R13
	MOV.B	R13, @R12
	ADD.B	#1, 5(R1)
.L51:
	MOV.B	#15, R12
	CMP.B	5(R1), R12 { JHS	.L52
	NOP
	; start of epilogue
	ADD.W	#6, R1
	RET
	.size	BlockCopy, .-BlockCopy
	.section	.text.AES128_ECB_encrypt,"ax",@progbits
	.balign 2
	.global	AES128_ECB_encrypt
	.type	AES128_ECB_encrypt, @function
AES128_ECB_encrypt:
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
	MOV.W	R12, 4(R1)
	MOV.W	R13, 2(R1)
	MOV.W	R14, @R1
	MOV.W	4(R1), R13
	MOV.W	@R1, R12
	CALL	#BlockCopy
	MOV.W	@R1, &state
	MOV.W	2(R1), &Key
	CALL	#KeyExpansion
	CALL	#Cipher
	NOP
	; start of epilogue
	ADD.W	#6, R1
	RET
	.size	AES128_ECB_encrypt, .-AES128_ECB_encrypt
	.section	.text.AES128_ECB_decrypt,"ax",@progbits
	.balign 2
	.global	AES128_ECB_decrypt
	.type	AES128_ECB_decrypt, @function
AES128_ECB_decrypt:
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
	MOV.W	R12, 4(R1)
	MOV.W	R13, 2(R1)
	MOV.W	R14, @R1
	MOV.W	4(R1), R13
	MOV.W	@R1, R12
	CALL	#BlockCopy
	MOV.W	@R1, &state
	MOV.W	2(R1), &Key
	CALL	#KeyExpansion
	CALL	#InvCipher
	NOP
	; start of epilogue
	ADD.W	#6, R1
	RET
	.size	AES128_ECB_decrypt, .-AES128_ECB_decrypt
	.section	.text.XorWithIv,"ax",@progbits
	.balign 2
	.type	XorWithIv, @function
XorWithIv:
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
	MOV.W	R12, @R1
	MOV.B	#0, 3(R1)
	BR	#.L56
.L57:
	MOV.B	3(R1), R12
	ADD.W	@R1, R12
	MOV.B	3(R1), R13
	ADD.W	@R1, R13
	MOV.B	@R13, R14
	MOV.W	&Iv, R15
	MOV.B	3(R1), R13
	ADD.W	R15, R13
	MOV.B	@R13, R13
	XOR.B	R14, R13
	AND	#0xff, R13
	MOV.B	R13, @R12
	ADD.B	#1, 3(R1)
.L56:
	MOV.B	#15, R12
	CMP.B	3(R1), R12 { JHS	.L57
	NOP
	; start of epilogue
	ADD.W	#4, R1
	RET
	.size	XorWithIv, .-XorWithIv
	.section	.text.AES128_CBC_encrypt_buffer,"ax",@progbits
	.balign 2
	.global	AES128_CBC_encrypt_buffer
	.type	AES128_CBC_encrypt_buffer, @function
AES128_CBC_encrypt_buffer:
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
	MOV.W	R12, 6(R1)
	MOV.W	R13, 4(R1)
	MOV.W	R14, @R1
	MOV.W	R15, 2(R1)
	MOV.B	@R1, R12
	MOV.B	R12, R13
	AND.B	#15, R13
	MOV.B	R13, 9(R1)
	MOV.W	4(R1), R13
	MOV.W	6(R1), R12
	CALL	#BlockCopy
	MOV.W	6(R1), &state
	CMP.W	#0, 14(R1) { JEQ	.L59
	MOV.W	14(R1), &Key
	CALL	#KeyExpansion
.L59:
	CMP.W	#0, 16(R1) { JEQ	.L60
	MOV.W	16(R1), &Iv
.L60:
	MOV.W	#0, 10(R1)
	BR	#.L61
.L62:
	MOV.W	4(R1), R12
	CALL	#XorWithIv
	MOV.W	4(R1), R13
	MOV.W	6(R1), R12
	CALL	#BlockCopy
	MOV.W	6(R1), &state
	CALL	#Cipher
	MOV.W	6(R1), &Iv
	ADD.W	#16, 4(R1)
	ADD.W	#16, 6(R1)
	ADD.W	#16, 10(R1)
.L61:
	MOV.W	10(R1), R14
	MOV.W	R14, R12 { MOV.W	R14, R13 { RPT	#15 { RRAX.W	R13
	MOV.W	R13, R14
	CMP.W	2(R1), R14 { JLO	.L62
	CMP.W	R13, 2(R1) { JNE	.L65
	CMP.W	@R1, R12 { JLO	.L62
.L65:
	CMP.B	#0, 9(R1) { JEQ	.L66
	MOV.W	4(R1), R13
	MOV.W	6(R1), R12
	CALL	#BlockCopy
	MOV.B	9(R1), R12
	MOV.W	6(R1), R15
	ADD.W	R12, R15
	MOV.B	9(R1), R12
	MOV.W	#16, R13 { SUB.W	R12, R13
	MOV.W	R13, R12
	MOV.W	R12, R14
	MOV.B	#0, R13
	MOV.W	R15, R12
	CALL	#memset
	MOV.W	6(R1), &state
	CALL	#Cipher
.L66:
	NOP
	; start of epilogue
	ADD.W	#12, R1
	RET
	.size	AES128_CBC_encrypt_buffer, .-AES128_CBC_encrypt_buffer
	.section	.text.AES128_CBC_decrypt_buffer,"ax",@progbits
	.balign 2
	.global	AES128_CBC_decrypt_buffer
	.type	AES128_CBC_decrypt_buffer, @function
AES128_CBC_decrypt_buffer:
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
	MOV.W	R12, 6(R1)
	MOV.W	R13, 4(R1)
	MOV.W	R14, @R1
	MOV.W	R15, 2(R1)
	MOV.B	@R1, R12
	MOV.B	R12, R13
	AND.B	#15, R13
	MOV.B	R13, 9(R1)
	MOV.W	4(R1), R13
	MOV.W	6(R1), R12
	CALL	#BlockCopy
	MOV.W	6(R1), &state
	CMP.W	#0, 14(R1) { JEQ	.L68
	MOV.W	14(R1), &Key
	CALL	#KeyExpansion
.L68:
	CMP.W	#0, 16(R1) { JEQ	.L69
	MOV.W	16(R1), &Iv
.L69:
	MOV.W	#0, 10(R1)
	BR	#.L70
.L71:
	MOV.W	4(R1), R13
	MOV.W	6(R1), R12
	CALL	#BlockCopy
	MOV.W	6(R1), &state
	CALL	#InvCipher
	MOV.W	6(R1), R12
	CALL	#XorWithIv
	MOV.W	4(R1), &Iv
	ADD.W	#16, 4(R1)
	ADD.W	#16, 6(R1)
	ADD.W	#16, 10(R1)
.L70:
	MOV.W	10(R1), R14
	MOV.W	R14, R12 { MOV.W	R14, R13 { RPT	#15 { RRAX.W	R13
	MOV.W	R13, R14
	CMP.W	2(R1), R14 { JLO	.L71
	CMP.W	R13, 2(R1) { JNE	.L74
	CMP.W	@R1, R12 { JLO	.L71
.L74:
	CMP.B	#0, 9(R1) { JEQ	.L75
	MOV.W	4(R1), R13
	MOV.W	6(R1), R12
	CALL	#BlockCopy
	MOV.B	9(R1), R12
	MOV.W	6(R1), R15
	ADD.W	R12, R15
	MOV.B	9(R1), R12
	MOV.W	#16, R13 { SUB.W	R12, R13
	MOV.W	R13, R12
	MOV.W	R12, R14
	MOV.B	#0, R13
	MOV.W	R15, R12
	CALL	#memset
	MOV.W	6(R1), &state
	CALL	#InvCipher
.L75:
	NOP
	; start of epilogue
	ADD.W	#12, R1
	RET
	.size	AES128_CBC_decrypt_buffer, .-AES128_CBC_decrypt_buffer
	.ident	"GCC: (SOMNIUM Technologies Limited - msp430-gcc 6.4.0.32) 6.4.0"
