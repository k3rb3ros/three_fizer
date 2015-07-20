#include "print.h"

void showBuff(uint64_t cnt, const uint8_t* b)
{
    for (uint64_t i=0;i < cnt;i++)
    {
        if (i %16 ==  0) printf(INDENT);
        else if (i % 4 == 0) printf(" ");
        printf(" %02X",b[i]);
        if (i %16 == 15 || i==cnt-1) printf("\n");
    }
}
