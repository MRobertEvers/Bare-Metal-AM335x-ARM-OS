# PRU Code for project

This source it to test the UART0 output of the PRU.

You must build with TI's special compiler - it supports the unique instructions required for the PRU.

Code Composer Studio from TI is easiest to use. Make sure to install the special PRU compiler. GCC does not support the R30 and R31 registers.

## Compilation

Open the project in Code Composer Studio and compile. Ensure that the Hex Utility is set to output '--array'. This will produce a `.hex` file which contains a C-Style array like source

```
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
```

This array contains the instruction code for the PRU. You can simply write it to the PRU's IRAM (which is memory mapped) and then enable the PRU. It will print "Jello!" in a loop.

## Uart

The uart register docs are located at 4.5.5 of the TRM.
