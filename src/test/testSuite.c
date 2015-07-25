#include "include/testSuite.h"

int main()
{
    #ifdef NDEBUG
        printf("NDEBUG is defined retest with it off");
    #else
        runThreefishTests();
        runSkeinTests();
	    runCBCTests();
        runChunkTests();
	    runFileIOTests();
        printf("All tests passed :)\n");
    #endif

    return 0;
}
