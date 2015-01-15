Threefizer 0.4.1
================

A simple unix command line utitily to encrypt and decrypt files with threefish

Specifications
--------------

* Uses Skein/Threefish from the NIST submission 
* Operates in CBC mode with a Skein MAC
* Accepts standard unix/linux args
* Uses queued buffers to Read Encrypt/Decrypt and Write Continuously on large files
* Supports 256, 512, 1024 bit state sizes

Features
--------

* Very fast encryption/decryption
* Header and MAC checking on decrypt
* Multi threading
* Only ovewrites files on success
* Progress bar for large files

Notes
-----

The code as is was designed for for x86_64 cpus with 4 or more cores and access to 2GB of ram or more. It will perform poorly on single core systems and will likely fail to encrypt and decrypt large files on systems with less then 2GB or ram. If you want to use this on a system with limited resources you will need to adjust the QUE_SIZE and MAX_CHUNK_SIZE macros in chunkQueue.h to values that fit the memory resources of your system.

This program uses Cipher Block Chaining to prevent blocks containing the same text from producing the same output. To prevent an attacker from potentially modifying blocks of the cipher text in a malicous manner. A header containing the size of plain text and state size of the cipher is prepended to the plain text and encrypted as the first block. The cipher text is then run through SkeinMAC and the MAC is then appended to the file. Any changes to the initialization vector or header will cause the header check to fail and decryption to be aborted. Any changes or additions to the cipher text or header will cause the MAC check to fail and decryption to be aborted. This system is only as secure the underlying cipher and hash functions. At the time of writing this (January 2015) there are no published attacks on all 72 rounds of Threefish and Skein and are beleived to be secure. It is the belief of the author that files encrypted with this program should be secure so long as the following conditions are met.

* The machine(s) threefizer is compiled and run on are not compromised (This is a risk to any network connected device).
* All tests pass on the machine that compiles threefizer.
* You don't use the -n --no-hash option
* Passwords used to encrypt data are kept secret (not written down or disclosed).
* Keyfiles used to encrypt data are not stored with the encrypted files and are kept in a secure and unobvious place (secondary encryption of key files is a not a bad idea).

This is program as a work in progress while I take all best faith measures to ensure this implementation is secure I make no guarantees. USE AT YOUR OWR RISK.

Intended Use
------------

Encrypting back ups locally to be stored for backup in a potentially insecure locations.
