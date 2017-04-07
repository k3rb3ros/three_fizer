#include "include/testSuite.h"

int main()
{
    int status = 0;
    #ifdef NDEBUG
        printf("NDEBUG is defined retest with it off");
        status = -1;
    #else
        runCBCTests();
        runChunkTests();
        runFileIOTests();
        runSkeinTests();
        runThreefishTests();
        runUtilTests();
        printf("All tests passed :)\n");
    #endif

    return status;
}
