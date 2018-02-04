#include "utils.h"

// Busy Wait in cycles
// Max wait is 2^32 / 6 = cycles. Or ~2 seconds.
void Busy_Wait(unsigned long wait)
{
    volatile unsigned long i = wait / 6;
    
    // This loop takes 5 instructions to perform.
    // So to calculate the busy wait time in seconds,
    // take (wait*5)/InstructionsPerSecond
    // The AM3358 runs at about 2,000 MIPS
    // So (wait / 5) * 2,000,000,000 gives delay time
    while(i > 0)
    {
        i--;
    }
}
