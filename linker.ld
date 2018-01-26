/* rammap */
MEMORY
{
    ram : ORIGIN = 0x402F0400, LENGTH = 0x10000
}
 
SECTIONS
{
    .text : { *(.text*) } > ram
}