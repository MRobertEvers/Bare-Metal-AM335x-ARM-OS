.global Register_Write
.global Register_Read

// Arguments are placed in registers based on 
// Application Binary Interface (ABI) for the ARM Architecture
// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.100748_0606_00_en/lmi1470147220260.html
@ Register_Write
@ Arg 1 r0: Register Bank
@ Arg 2 r1: Register Offset
@ Arg 3 r2: Word Value to Write
Register_Write:
    add r0, r1
    mov r1, r2
    str r1, [r0]
    bx  lr
    
@ Register_Read
@ Arg 1 r0: Register Bank
@ Arg 2 r1: Register Offset
@ Return r0: Register value
Register_Read:
    add r0, r1
    mov r1, r2
    ldr r0, [r0]
    bx  lr
