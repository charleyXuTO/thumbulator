	.file	"main.c"
	.section	.text.main,"ax",@progbits
	.balign 2
	.global	main
	.type	main, @function
main:
; start of function
; framesize_regs:     0
; framesize_locals:   10
; framesize_outgoing: 0
; framesize:          10
; elim ap -> fp       2
; elim fp -> sp       10
; saved regs:(none)
	; start of prologue
	SUB.W	#10, R1
	; end of prologue
	MOV.W	#12849, @R1
	MOV.W	#13363, 2(R1)
	MOV.W	#13877, 4(R1)
	MOV.W	#14391, 6(R1)
	MOV.W	#57, 8(R1)
	MOV.W	R1, R12
	MOV.B	#9, R13
	CALL	#crcSlow
	CALL	#crcInit
	MOV.W	R1, R12
	MOV.B	#9, R13
	CALL	#crcFast
	MOV.B	#0, R12
	; start of epilogue
	.refsym	__crt0_call_exit
	ADD.W	#10, R1
	RET
	.size	main, .-main
	.ident	"GCC: (SOMNIUM Technologies Limited - msp430-gcc 6.4.0.32) 6.4.0"
