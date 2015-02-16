#ifndef CBCTESTS_H
#define CBCTESTS_H

#include <assert.h> //assert()
#include <stdint.h> //uintxx_t types
#include <string.h> //strcpy(), memcpy()
#include "print.h" //showBuff()
#include "cbc.h" //getChainInBuffer()
#include "cbcDecrypt.h"
#include "cbcEncrypt.h"
#include "encrypt.h"
#include "decrypt.h"
#include "threefishApi.h" //ThreefishKey_t, threefishSetKey()
#include "tweak.h" //threefizer_tweak the same tweak constant used in program operation

void runCBCTests();

#endif
