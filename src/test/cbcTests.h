#pragma once

#include <assert.h> //assert()
#include <stdint.h> //uintxx_t types
#include <string.h> //strcpy(), memcpy()
#include "cbc.h" //getChainInBuffer()
#include "cbcDecrypt.h" //encryptInPlace()
#include "cbcEncrypt.h" //decryptInPlace()
#include "encrypt.h" //encryptHeader()
#include "decrypt.h" //decryptHeader()
#include "print.h" //showBuff()
#include "tweak.h" //threefizer_tweak the same tweak constant used in program operation
#include "skein3fish/threefishApi.h" //ThreefishKey_t, threefishSetKey()

void runCBCTests();
