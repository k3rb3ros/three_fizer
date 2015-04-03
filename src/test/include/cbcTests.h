#ifndef CBCTESTS_H
#define CBCTESTS_H

#include <assert.h>                       //assert()
#include <stdint.h>                       //uintxx_t types
#include <string.h>                       //strcpy(), memcpy()
#include "print.h"                        //showBuff()
#include "../../include/cbc.h"            //getChainInBuffer()
#include "../../include/cbcDecrypt.h"
#include "../../include/cbcEncrypt.h"
#include "../../include/encrypt.h"
#include "../../include/decrypt.h"
#include "../../include/threefishApi.h"   //ThreefishKey_t, threefishSetKey()
#include "../../include/tweak.h"          //threefizer_tweak the same tweak constant used in program operation

void runCBCTests();

#endif