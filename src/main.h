#ifndef MAIN_H
#define MAIN_H

#define CM_PER 0x44E00000
#define GPIO1 0x4804C000

#define CM_PER_GPIO1_CLKCTRL 0xAC
#define GPIO_OE 0x134
#define GPIO_DATAOUT 0x13C

#define USR0 21

#define HWREG(x) (*((volatile unsigned int*)(x)))
#define SOC_CONTROL_REGS (0x44E10000)
#define CONTROL_CONF_UART_RXD (0x958)
#define CONTROL_CONF_UART_TXD (0x95C)
#endif