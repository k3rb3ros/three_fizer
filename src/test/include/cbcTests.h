#ifndef CBCTESTS_H
#define CBCTESTS_H

#include <assert.h> //assert()
#include <stdint.h> //uintxx_t types
#include <string.h> //strcpy(), memcpy()
#include "print.h" //showBuff()
#include "threefizer/cbc.h" //getChainInBuffer()
#include "threefizer/cbcDecrypt.h"
#include "threefizer/cbcEncrypt.h"
#include "threefizer/encrypt.h"
#include "threefizer/decrypt.h"
#include "threefizer/threefishApi.h" //ThreefishKey_t, threefishSetKey()
#include "threefizer/tweak.h" //threefizer_tweak the same tweak constant used in program operation

void runCBCTests();

#endif
