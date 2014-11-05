#ifndef HMAC_H
#define HMAC_H

#include <stdbool.h> //bool type
#include <stdint.h> //stdint types
#include "threefishApi.h" //ThreefishKey_t

bool checkHMAC(ThreefishKey_t* tf_key);

static uint64_t* genHMAC(ThreefishKey_t* tf_key);

#endif
