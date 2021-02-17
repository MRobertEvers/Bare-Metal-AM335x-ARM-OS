#include "pru.h"

#include "main.h"

void
pru_load_program(unsigned int const* data, unsigned int data_len)
{
	unsigned int* rambase = (unsigned int*)PRU0_IRAM_BASE;

	data_len = (data_len + 3) >> 2;

	for( int i = 0; i < data_len; ++i )
	{
		*(rambase + i) = data[i];
	}
}

void
pru_enable(void)
{
	*((unsigned int*)(PRU0_CTRL_BASE)) = 0x0000000B;
}

void
pru_test(void)
{
	// int wow = 10000;
	// int j = 0;

	while( 1 )
	{
		// wow = 10000;

		// j = !j;

		// while( wow-- )
		// {
		// }
	}
}