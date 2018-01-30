.global Register_Write

@ Register_Write
@ Arg 1 r0: Register Bank
@ Arg 2 r1: Register Offset
@ Arg 3 r2: Word Value to Write
Register_Write:
	add r0, r1
	mov r1, r2
	str r1, [r0]
	bx  lr
	