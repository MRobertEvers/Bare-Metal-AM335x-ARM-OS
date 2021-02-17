#ifndef PRU_H
#define PRU_H

#define PRU0_CTRL_BASE 0x4a322000
#define PRU0_STS_BASE 0x4a322004
#define PRU0_IRAM_BASE 0x4a334000

void pru_load_program(unsigned int const* data, unsigned int data_len);
void pru_enable(void);

void pru_test(void);

#endif