#pragma once

#include <assert.h> //assert()
#include <stdint.h> //uintxx_t types
#include <string.h> //strcpy(), memcpy()
#include "app/print.h" //showBuff()
#include "app/cbc.h" //getChainInBuffer()
#include "app/cbcDecrypt.h" //encryptInPlace()
#include "app/cbcEncrypt.h" //decryptInPlace()
#include "app/encrypt.h" //encryptHeader()
#include "app/decrypt.h" //decryptHeader()
#include "skein3fish/threefishApi.h" //ThreefishKey_t, threefishSetKey()
#include "app/tweak.h" //threefizer_tweak the same tweak constant used in program operation

void runCBCTests();
