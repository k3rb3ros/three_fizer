Threefizer 0.6.1
================
A simple *nix command line utility to encrypt and decrypt files with threefish

Specifications
--------------
**Key stretching** Scrypt with with SKEIN512

**Block Cipher** Threefish

**Mode of operation** Cipher Block Chaining

**File authentication** Header check + SkeinMAC

**Random number generation** PCG32_basic

**Supported cipher sizes** 256, 512, 1024 bits

Limitations
--------------
* Only works on full files not stdin as the file size must be known before encryption/decryption
* Requires ≈ 2GB of RAM for large files
* Due to limitations of Cipher Block Chaining encryption cannot be parallelized

Features
--------
* Multi threaded
* Fast encryption/decryption
* Header and MAC checking on decrypt
* Only overwrites files on success
* Progress bar for large files

Licensing
---------
This program uses several third party open source components covered under different licenses and copyrights. All source files covered under a different license are isolated from the main project source by directory and have their respective licenses in their header files. Anything not explicitly marked otherwise falls under the MIT license at the top level of the project.

Building
--------
* mkdir build
* cd build
* cmake ../
* make
* cd build/src
* ensure threefizer_tests pass
* use the program

Notes
-----
The program was designed on an x86_64 cpu. With 4 cores and access to 2GB of ram or more. While it will run on a single core system it will perform poorly and may fail to encrypt and decrypt large files on systems with less than 2GB or ram. If you want to use this on a system with limited resources you will need to adjust the QUE_SIZE and MAX_CHUNK_SIZE macros in chunkQueue.h and recompile.

This program uses Cipher Block Chaining mode to prevent blocks containing the same text from producing the same output. To prevent an attacker from potentially modifying blocks of the cipher text in a malicious manner. A header containing the size of plain text and state size of the cipher is prepended to the plain text and encrypted as the first block. The cipher text is then run through SkeinMAC and the MAC is then appended to the file. Any changes to the initialization vector or header will cause the header check to fail and decryption to be aborted. Any changes or additions to the cipher text or header will cause the MAC check to fail and decryption to be aborted. At the time of writing this (April 2017) there are no published attacks that break Threefish or Skein and these algorithms are beleived to be secure. This is program as a work in progress and has not undergone professional security analysis. I have done what I know how to do to ensure this implementation and the cryptographic primatives used are secure, but I make no guarantees. USE AT YOUR OWR RISK.

Intended Use
------------
Encrypting files locally to be stored for backup in a potentially insecure locations e.g. Dropbox or Google Drive.

Referenced Code
-------------------
Components of this project were taken from the following open source projects

https://github.com/wernerd/Skein3Fish

http://www.pcg-random.org/download.html

https://www.tarsnap.com/scrypt.html
