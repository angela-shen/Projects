### Assignment 5 - Schmidt Samoa

## Description

Implements Schmidt Samoa encryption. The executable keygen generates the public and private keys needed for encrypt and decrypt. Encrypt accepts a message from the user or a file and outputs the encrypted version of the message. Decrypt accepts this encrypted message and decrypts it back to the original message.

## Building

Run $make, $make keygen, $make decrypt, or $make encrypt to compile the executables needed, or simply run $make all. This will create the executables keygen, decrypt, and encrypt as well as the necessary object files.

## Running 

To run the executables, type ./keygen, ./decrypt, or ./encrypt. However, ./decrypt and ./encrypt will not function without public or private keys, so make sure to have valid public and private keys (i.e. running ./keygen). 

## Known errors

Make scan-build does not generate errors or warnings. Valgrind and gdb likewise do not generate errors.
