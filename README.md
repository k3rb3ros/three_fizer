Threefizer 0.3.0
================

A simple unix command line utitily to encrypt and decrypt files with threefish

Specifications
--------------

* Uses Skein/Threefish from the NIST submission 
* Operates in cbc mode with a Skein MAC (note this is not the notorious CBC-MAC)
* Accepts standard unix/linux args
* Uses queued buffers to Read Encrypt/Decrypt and Write Continuously
* Supports 256, 512, 1024 bit state sizes

Features
--------

* Header and MAC checking on decrypt
* Multi threading
* Only Overites files on success

Intended Use
------------

Encrypting back ups locally to be stored on cheap (but insecure) cloud services
