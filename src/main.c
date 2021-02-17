#include "main.h"

#include "megos_uart.h"
#include "pru.h"
#include "system.h"
#include "utils.h"
const unsigned int  PRU_Hardware_UART_image_0[] = {
0x240000c0,
0x24010080,
0x0504e0e2,
0x2eff818e,
0x230007c3,
0x240001ee,
0x230046c3,
0x0508e2e2,
0x240068e0,
0x2eff8181,
0x81202780,
0x240100ee,
0x2400480f,
0x81242781,
0x240007e0,
0x24000010,
0x81342781,
0x81042780,
0x24000fe0,
0x81082780,
0x240003e0,
0x810c2780,
0x246001e0,
0x81102781,
0x81302780,
0x0101eee0,
0xe100000f,
0x2400650f,
0xe1020e0f,
0x24006c0f,
0xe1030e0f,
0xe1040e0f,
0x24006f0f,
0xe1050e0f,
0x2400210f,
0xe1060e0f,
0xe1070e01,
0x9104248e,
0x1d04eeee,
0x8104248e,
0x2eff018e,
0xe104228e,
0xf104228e,
0xf100228f,
0x14eeefee,
0xe100228e,
0xf100000e,
0x51000e13,
0x100e0eee,
0x8100278e,
0x240111ee,
0x00ee10ef,
0x9114278e,
0xcf00eeff,
0x9100278e,
0xe1000f0e,
0x9108278e,
0xcf01eeff,
0x248000ce,
0x2400008e,
0x01011010,
0x0101e0e0,
0x14ee10ef,
0x248000ce,
0x2400088e,
0x4eefeeed,
0x81302781,
0x2a000000,
0x0108e2e2,
0x20c30000,
0x230048c3,
0x21004700,
0x10000000,
0x20c30000};


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

		serial_hex_to_ascii_hex(*((unsigned int*)(PRU0_STS_BASE)));
		j = !j;

		while( wow-- )
		{
		}
	}
	
}