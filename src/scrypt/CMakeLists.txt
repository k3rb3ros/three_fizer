cmake_minimum_required (VERSION 2.6)

#C files for SCRYPT Key Derrivation Function
set (scrypt_c_src
     endianConvert.c
     pbkdf2Skein512.c
     scrypt.c
    )

add_library(${SCRYPT_LIB} STATIC ${scrypt_c_src})
