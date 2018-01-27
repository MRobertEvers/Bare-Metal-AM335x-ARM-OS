#define CM_PER 0x44E00000
#define GPIO1 0x4804C000

#define CM_PER_GPIO1_CLKCTRL 0xAC
#define GPIO_OE 0x134
#define GPIO_DATAOUT 0x13C

void Register_Write(unsigned int, unsigned int, unsigned int);

static void system_enable_GPIO1(void);
static void system_enable_GPIO1(void)
{
	Register_Write(CM_PER, CM_PER_GPIO1_CLKCTRL, 2);
}

int main(void) __attribute__ ((section (".text.main")));
int main(void)
{
	Register_Write(CM_PER, CM_PER_GPIO1_CLKCTRL, 2);
	Register_Write(GPIO1, GPIO_OE, 0);
	Register_Write(GPIO1, GPIO_DATAOUT, 0xFFFFFFFF);
	while(1);
}