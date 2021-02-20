#include "main.h"

#include "megos_uart.h"
#include "pru.h"
#include "system.h"
#include "utils.h"
const unsigned int PRU_Hardware_UART_image_0[] = {
	0x240000c0, 0x24010080, 0x0504e0e2, 0x2eff818e, 0x230007c3, 0x240001ee, 0x230032c3, 0x240068e0, 0x2eff8181,
	0x81202780, 0x240000e0, 0x81242780, 0x81342781, 0x240007e0, 0x81042780, 0x81082780, 0x240003e0, 0x810c2780,
	0x246001e0, 0x81102781, 0x24004a21, 0x81302780, 0x240114e0, 0xe1010021, 0x24006521, 0xe1020021, 0x24006c21,
	0xe1030021, 0xe1040021, 0x24006f21, 0xe1050021, 0x24002121, 0xe1060021, 0xe1070001, 0x240115e0, 0x2400000f,
	0xf1000001, 0x100101e1, 0x81002781, 0x91142781, 0xcf05e1ff, 0x248000c1, 0x24000081, 0x01010f0f, 0x0101e0e0,
	0x14e10fee, 0x248000c1, 0x24000781, 0x76eee1f2, 0x21002400, 0x230034c3, 0x21003300, 0x10000000, 0x20c30000};

const unsigned char PRU_Hardware_UART_image_1[] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
												   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
												   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

int main(void) __attribute__((section(".text.main")));
int
main(void)
{
	Register_Write(CM_PER, CM_PER_GPIO1_CLKCTRL, 2);
	Register_Write(GPIO1, GPIO_OE, 0);
	// Register_Write(GPIO1, GPIO_DATAOUT, 1 << USR0);

	HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_RXD) = 0x4;
	HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_TXD) = 0x4;

	megos_UART0_init();

	// *((unsigned int*)(GPIO1 + GPIO_DATAOUT)) = 0;
	*((unsigned int*)0x44E00C00) |= 0x2;
	*((unsigned int*)0x44E00C00) &= 0xFFFFFFFD;

	*((unsigned int*)0x44E000E8) |= 0x02;

	*((unsigned int*)0x44E00C00) |= 0x2;
	*((unsigned int*)0x44E00C00) &= 0xFFFFFFFD;

	serial_hex_to_ascii_hex((unsigned int)&pru_test);
	pru_load_program(PRU_Hardware_UART_image_0, sizeof(PRU_Hardware_UART_image_0));

	pru_enable();

	serial_hex_to_ascii_hex(*((unsigned int*)(PRU0_CTRL_BASE)));
	// megos_UART0_test();
	int wow = 10000;
	int j = 0;

	while( 1 )
	{
		wow = 100000;

		// serial_hex_to_ascii_hex(*((unsigned int*)(PRU0_STS_BASE)));
		j = !j;

		while( wow-- )
		{
		}
	}
}