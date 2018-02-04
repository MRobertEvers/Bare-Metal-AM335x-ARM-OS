// The start of the UART0 register bank.
#include "system.h"
#include "utils.h"

// UART0 is on the wakeup power domain
// Thus look to the CM_WKUP register bank for the UART0 clock
#define CM_WKUP_BANK 0x44E00400

// Section 8.1.12.2.46 for register description
// This register manages the UART0 clocks.
#define CM_WKUP_UART0_CLKCTRL 0xB4 // UART0 Clocks

// 9.3.1 Control module registers
// This register controls the mode of the muxed pins.
// Use these to make sure that the pin we are looking
// at is in the mode that we want.
#define MODULE_CONTROL_BANK 0x44E10000
#define CONF_UART0_RXD 0x970
#define CONF_UART0_TXD 0x974

#define UART0_BANK 0x44E09000

// DLL is accessible in both config mode A, B. It contains the LSB of the
// LSB of the Baud Clock Divisor.
#define UART_DLL  0x00 // Divisor latches low register
#define UART_THR  0x00 // Transmit holding register
#define UART_RHR  0x00 // Receiver holding register

// DLL is accessible in both config mode A, B. It contains the MSB of the
// MSB of the Baud Clock Divisor.
#define UART_DLH  0x04 // Divisor latches high register
#define UART_LCR  0x0C // Line control register
#define UART_MCR  0x10 // Modem control register (MCR) is selected with a register bit setting of LCR[7] = 0
#define UART_FCR  0x08 // Controls the FIFO Queue.
#define UART_LSR  0x14 // Status register
#define UART_MDR1 0x20 // Mode definition register 
#define UART_SYSC 0x54 // System configuration register
#define UART_SYSS 0x58 // System status register

#define MAX_BUFFER_SIZE 120

static unsigned char serial_RX_Buffer[MAX_BUFFER_SIZE];
static unsigned char serial_RX_Buffer_Index = 0;

static unsigned int serial_recv( void )
{
    // 0x1 indicates At least one data character in the RX FIFO. Sect 19.5.1.19
    while(1)
    {
        if (Register_Read(UART0_BANK, UART_LSR) & 0x01) break;
    }
    
    return (Register_Read(UART0_BANK, UART_RHR) & 0xFF);
}

static void serial_send( unsigned int c )
{
    // 0x20 indicates that the transmit hold register is empty.
    while(1)
    {
        if (Register_Read(UART0_BANK, UART_LSR) & 0x20) break;
    }
    
    Register_Write(UART0_BANK, UART_THR, c);
}

static void serial_flush( void )
{
    // Just busy wait for the TX FIFO to empty.
    // 0x40 indicates that the transmit hold and shift registers are empty.
    while(1)
    {
        if (Register_Read(UART0_BANK, UART_LSR) & 0x40) break;
    }
}

static void serial_hex_to_ascii_hex ( unsigned int aiHex )
{
    // Since each character when typing a hex number
    // corresponds to 4 bits. We must grab each chunk of
    // 4 bits and calculate which character was used to
    // write that value.
    // e.g. 0x41. Or 0100 0001.
    // So look at 0100, which is 0x4. To convert that
    // to ascii '4', add 0x30. ASCII numbers start at 0x30 with 0.
    // Since we read the MSB of numbers first, that must be sent first.
    unsigned char iCharacter = 0;
    unsigned int iBitShift = 32;
    while(iBitShift > 0)
    {
        iBitShift -= 4;
        iCharacter = (aiHex >> iBitShift) & 0xF;
        if(iCharacter > 0x9)
        {
            
            // If the value of the character cannot be represented by
            // numerals. We need to use A,B,C,D,E, and F from Hex Representation.
            // ASCII capital letters start at 0x41 (65) with A.
            // Since A is equal to Decimal 10, we want to add a number to
            // the character so that it will be ascii 'A'... so 65 - 10 = 55
            // which is 0x37.
            iCharacter += 0x37;
        }
        else
        {
            // This is a numerals
            iCharacter += 0x30;
        }
        serial_send(iCharacter);
    }
    serial_send(0x0D);
    serial_send(0x0A);
}

static void serial_software_reset(void)
{
    // Perform reset - Set reset bit in SYSC
    Register_Write(UART0_BANK, UART_SYSC,0x2);
    
    // 0x1 indicates reset is complete.
    while(Register_Read(UART0_BANK, UART_SYSS) & 0x1 != 1);
}

static void serial_set_pin_mux_mode(void)
{
    // Q: Why do we need to do this?
    // ANS: In this case, we dont. The Mux mode of these pins is the
    // correct by default. i.e. The RXD and TXD modes are mode 0 on
    // their designated pins.
    // 0x20 turns on receiver nenable explicitly, but that is not necessary,
    // because it is on by default anyway.
    Register_Write(MODULE_CONTROL_BANK, CONF_UART0_RXD, 0x20);
    Register_Write(MODULE_CONTROL_BANK, CONF_UART0_TXD, 0x00);
}

// See section 19.2.2 for BAUD values and divisor values
// This function only works in Config Mode (A/B)
static void serial_clock_set_baud(void)
{
    //From the table in 19.2.2, the divisor that gives baud 115,200
    // is 26 or 0x1A.
    
    // DLL is accessible in both A, B. It contains the LSB of the
    // LSB of the Baud Clock Divisor.
    Register_Write(UART0_BANK, UART_DLL, 0x1A);
    
    // DLH is accessible in both A, B. It contains the MSB of the
    // MSB of the Baud Clock Divisor.
    Register_Write(UART0_BANK, UART_DLH, 0x00);
}

static void serial_clock_enable(void)
{
    // UART0 is on the wakeup power domain
    // Thus look to the CM_WKUP register bank for the UART0 clock
    // 0x2 Sets the UART0 Clock to enabled mode
    Register_Write(CM_WKUP_BANK, CM_WKUP_UART0_CLKCTRL, 0x2);
}

static void serial_send_newline(void)
{
    serial_send(0x0D);
    serial_send(0x0A);
}

void megos_UART0_send_string(char* msg)
{
    unsigned int iChar = 0;
    if( msg == 0 )
    {
        return;
    }
    
    while(msg[iChar] != '\0')
    {
        serial_send((unsigned char)msg[iChar++]);
        Busy_Wait(0x10);
    }
}

// This procedure follows section 19.4 of the AM3358 manual.
void megos_UART0_init(void)
{   
    serial_clock_enable();
    
    serial_set_pin_mux_mode();

    serial_software_reset();
    
    // See section 19.4.1.1.3 (Protocol, Baud Rate,
    // and Interrupt Settings) for details. 
    // 1. Disable UART to access the UARTi.UART_DLL and UARTi.UART_DLH registers:
    //    Set the UARTi.UART_MDR1[2:0] MODE_SELECT bit field to 0x7.
    // This disables the UART.
    Register_Write(UART0_BANK, UART_MDR1, 0x07);
    
    // See 19.3.7.1 of the manual. The LCR controls which mode the module is in
    // The LCR[7]=1 indicates that we are in a configuration mode. 
    // Then if LCR=0xBF we are in configuration mode B.
    // If LCR !=0xBF, we are in configuration mode A.
    
    // Enter configuration mode A.
    Register_Write(UART0_BANK, UART_LCR, 0x80);
    
    serial_clock_set_baud();
    
    // This can be R/W to in config mode A or OP mode.
    // Disable modem control
    Register_Write(UART0_BANK, UART_MCR, 0x00);
    
    // This can be WRITTEN to in config mode A or OP mode.
    // 0x4 clears TX FIFO Queue, 0x2 clears RX FIFO Queue, 
    // 0x1 Enables 64 byte FIFO Queue (as opposed to 1 byte).
    Register_Write(UART0_BANK, UART_FCR, 0x07);
    
    // Enter configuration mode B.
    // Register_Write(UART0_BANK, UART_LCR, 0xBF);
    
    // Enter OP mode with 8 bit words, 1 stop bit and no parity.
    Register_Write(UART0_BANK, UART_LCR, 0x03);
    
    // Reenable UART 16x mode.
    Register_Write(UART0_BANK, UART_MDR1, 0x00); 
}

int megos_UART0_test(void)
{
    megos_UART0_init();
    serial_flush();
    
    megos_UART0_send_string("Serial Ready...\0");
    
    char c = '\0';
    while(1)
    {
        c = serial_recv();
        if( (serial_RX_Buffer_Index < MAX_BUFFER_SIZE) && (c != '\0') )
        {
            serial_RX_Buffer[serial_RX_Buffer_Index++] = c; 
            serial_send(c);
        }
        else
        {
            megos_UART0_send_string("Serial Buffer Full...\0");
        }

        // Perform command if new line is entered.
        if( c == '\r\n' )
        {
            megos_UART0_send_string("Message Recved...\0");
            for(int i = 0; i < serial_RX_Buffer_Index; ++i)
            {
                serial_send(serial_RX_Buffer[i]);
                Busy_Wait(0x10);
            }
            
            serial_RX_Buffer_Index = 0;
        }
        c = '\0';
    }

    return 0;
}