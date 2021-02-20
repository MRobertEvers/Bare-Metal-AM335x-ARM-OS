# Bare-Metal-AM335x-ARM-OS

Bare Metal OS for the PocketBeagle. In-Progress.

## Installation

Download and install the `gcc-arm-none-eabi` toolchain for your platform - ideally include the installation path in your environment variables. This can be found on the arm developer website. https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads

# Notes:

## PRU

The PRU has special instructions that GCC doesn't support - you have to use TI's compiler to generate the code for the PRU.

```
UART 0 PIN MUX
SOC_CONTROL_REGS (0x44E10000)

(Pin names from AM3358 datasheet)            - PRU Signal                                  - Control Register (offset from SOC CONTROL REGS)
SPI0_CS0 (SPI0_CS0 on beagleboard schematic) - pr1_uart0_txd (mux 4 from am3358 datasheet) - (0x958 from TRM)
SPI0_D1 (SPI0_MOSI on beagleboard schematic) - pr1_uart0_rxd (mux 4 from am3358 datasheet) - (0x95C from TRM)
```

## Pin Mux

See 9.3.1.50 for register. See datasheet Pin Attributes section for mux pin modes.

```
// ALL OF THESE COME FROM THE TI AM335X STARTERWARE

// https://github.com/dawbarton/starterware/blob/master/include/hw/soc_AM335x.h
#define SOC_CONTROL_REGS                     (0x44E10000)
// https://e2e.ti.com/support/legacy_forums/embedded/starterware/f/790/t/408080?BeagleBone-Bare-Metal-UART-Driver
#define CONTROL_CONF_UART_TXD(n) (0x974 + ((n) * 0x10))

// https://e2e.ti.com/support/processors/f/791/t/652913?CCS-AM3354-eHRPWM0B-output-on-GPIO3-15
#define CONTROL_CONF_MUXMODE(n) (n)

	//*********************************************
	// UART TXD  = PR1_UART0_TXD
	//*********************************************
	HWREG(SOC_CONTROL_REGS + CONTROL_CONF_UART_TXD(1)) = AM335X_PIN_OUTPUT | CONTROL_CONF_MUXMODE(5);

```

TI Starterware has register definitions in `TI/include`. For this chip `soc_AM335x.h`

## Power Supply

P1 is a VIN. It takes 5V. The power LED should light up if the board is powered regardless of source.

## Boot Pins

The pocketbeagle board has the boot pins SYSBOOT3, SYSBOOT4 and SYSBOOT14 tied high. See /docs/PocketBeagle_sch.pdf.

This means the boot media check order is (from table 26-7 of the reference manual)

```
1. SPI
2. MMC
3. USB (Doesn't work - see errata)
4. UART0
```

## Other

Readme formalization to come.

1. Use <Toolchain>-objcopy -O binary $(program_NAME).elf $(program_NAME) to extract the raw binary from the compiled file. This is necessary to remove all the OS required data.
2. Remeber that GPIO_OE = 0x0 sets all the pins to OUTPUT. (I've frequently tried to put them to 0xFFFFFFFF)
3. The raw binary starts at 0x208 after the GP header on the SD card in raw memory.
4. The first 0x208 bytes are specified by the TOC and GP Header in the AM3358 Manual. Use HxD to write raw binary to the SD card.
5. Use the int main(void) **attribute** ((section (".text.main"))); declaration of main to assign a unique section to main, then in the linker script, SECTIONS
   {
   .text :
   {
   _(.text.main);
   _(.text\*)
   } > ram
   },
   will put the main function at the first address.
6. The C compiler calls functions with bl which stores the PC in the lr register. Use bx lr to return from such a function.
7. The GPIO clock must also be enabled in order for the OE and the DATAOUT registers to work properly.
8. btoi.exe is a binary used to help me create a bootable image. See [its repository](https://github.com/MRobertEvers/AM335x-Binary-To-Image-Tool).
