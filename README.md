# Bare-Metal-AM335x-ARM-OS

Bare Metal OS for the PocketBeagle. In-Progress.

## Installation

Download and install the `gcc-arm-none-eabi` toolchain for your platform - ideally include the installation path in your environment variables. This can be found on the arm developer website. https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads

# Notes:

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
