.equ GPIO1, 0x4804C000
.equ GPIO_OE, 0x134
.equ CM_PER, 0x44E00000
.equ CM_PER_GPIO1_CLKCTRL, 0xAC
.equ GPIO_DATAOUT, 0x13C

.globl _start

_start:
	ldr r0, =CM_PER                @Clocks control register bus.
	ldr r1, =CM_PER_GPIO1_CLKCTRL  @Offset of the clock register for GPIO1
	add r0, r1
	mov r1, #2                     @Set the enable bit. Man 8.1.12.1.29
	str r1, [r0]
_led_enable:
	ldr r0, =GPIO1     @Register bank for GPIO1
	ldr r1, =GPIO_OE   @Register that controls output enable.
	add r0, r1
	mov r1, #0
	str r1, [r0]
_main:
	ldr r0, =GPIO1     
	ldr r1, =GPIO_DATAOUT  @Register than controls the output of GPIO1
	add r0, r1
	ldr r1, =0xFFFFFFFF
	str r1, [r0]
loop:
	ldr r2, =0x00010000     @The start of a loop that may or may not work.
	sub r2, #1              @I tried to make the USR LEDs blink, but I posted
	cmp r2, #0              @this before I tested it.
	beq _main
	mov r1, #0
	str r1, [r0]
_hang:
	b _main
	