#include "utils.h"

void Busy_Wait(unsigned int wait)
{
    volatile unsigned int i = wait;
    while(i > 0)
    {
        i--;
    }
}