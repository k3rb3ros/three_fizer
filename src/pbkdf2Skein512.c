#include "include/pbkdf2Skein512.h"

void PBKDF2_SKEIN512(const uint8_t* passwd, size_t pw_len,
                     const uint8_t* salt, size_t salt_len,
                     uint64_t c, uint8_t* buff, size_t dkLen)
{
    SkeinCtx_t Pctx, Hctx;
    uint8_t ivec[4];
    uint8_t U[32];
    uint8_t T[32];
    size_t clen;

    skeinCtxPrepare(&Pctx, SKEIN512);

    /* Compute MAC state after processing P and S. */
    skeinMacInit(&Pctx, passwd, pw_len, SKEIN512);
    skeinUpdate(&Pctx, salt, salt_len);

    /* Iterate through the blocks. */
    for(uint64_t i = 0; i * 32 < dkLen; ++i)
    {
        /* Generate INT(i + 1). */
        be32enc(ivec, (uint32_t)(i + 1));

        /* Compute U_1 = PRF(P, S || INT(i)). */
        memcpy(&Hctx, &Pctx, sizeof(SkeinCtx_t));
        skeinUpdate(&Hctx, ivec, 4);
        skeinFinal(&Hctx, U);

        /* T_i = U_1 ... */
        memcpy(T, U, 32);

        for(uint64_t j = 2; j <= c; ++j)
        {
            /* Compute U_j. */
            skeinMacInit(&Hctx, passwd, pw_len, SKEIN512);
            skeinUpdate(&Hctx, U, 32);
            skeinFinal(&Hctx, U);

            /* ... xor U_j ... */
            for(uint8_t k = 0; k < 32; ++k) { T[k] ^= U[k]; }
        }

         /* Copy as many bytes as necessary into buf. */
        clen = dkLen - i * 32;
        if (clen > 32) { clen = 32; }
        memcpy(&buff[i * 32], T, clen);
    }

    /* Clean PShctx, since we never called _Final on it. */
    memset(&Pctx, 0, sizeof(SkeinCtx_t));
}
