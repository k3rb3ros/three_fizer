#include "include/testSuite.h"

int main()
{
    runThreefishTests();
    #ifdef NDEBUG
        printf("NDEBUG is defined retest with it off");
    #else
        printf("Tests complete all tests passed :)\n");
    #endif
    return 0;
}
