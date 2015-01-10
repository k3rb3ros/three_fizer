#include "include/debug.h"

void showBuff(uint64_t size, const void* buffer)
{
    const uint8_t* b = buffer;
    for (uint64_t i=0;i < size;i++)
    {
        if (i %16 ==  0) { printf(INDENT); }
	else if (i % 4 == 0) { printf(" "); }
	printf(" %02X",b[i]);
	if (i %16 == 15 || i==size-1) { printf("\n"); }								    }
}

