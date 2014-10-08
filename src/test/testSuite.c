#include "include/testSuite.h"

int main()
{
    runThreefishTests();
    #ifdef NDEBUG
        printf("NDEBUG is defined retest with it off");
    #else
        printf("All tests passed :)\n");
    #endif
    return 0;
}
