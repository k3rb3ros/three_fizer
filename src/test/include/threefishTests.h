#ifndef THREEFISHTESTS_H
#define THREEFISHTESTS_H

#include <assert.h>

void testSetupKey();

void testThreefish256Encrypt();

void testThreefish512Encrypt();

void testThreefish1024Encrypt();

void testThreefish256Decrypt();

void testThreefish512Decrypt();

void testThreefish1024Decrypt();

void runThreefishTests();

#endif
