Threefizer
==========

A simple unix command line utitily to encrypt and decrypt files with threefish

Specifications
--------------

* Uses Skein/Threefish from the NIST submission 
* Operates in cbc mode with a Skein MAC (note this is not the notorious CBC-MAC)
* Accepts standard unix/linux args
* Uses queued buffers to Encrypt/Decrypt with minimal memory overhead
* Supports 256, 512, 1024 bit state sizes

Features
--------

coming soon


Intended Use
------------

Encrypting back ups locally to be stored on cheap (but insecure) cloud services
