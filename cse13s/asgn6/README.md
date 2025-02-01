### Assignment 6 - LZ78

## Description

Implements LZ78 data compression. The executable encode accepts an input file and uses a trie to compress it. This compressed file can be decompressed with decode, which uses a wordtable to obtain the original message.

## Building

Run $make, $make decode, or $make encode to compile the executables needed, or simply run $make all. This will create the executables decode and encode as well as the necessary object files.

## Running 

To run the executables, type ./decode or ./encode. However, decode will not accept an input that does not include the correct magic number in the header. Decode and encode both accept the same flags. -v prints verbose statistics (Compressed file size, uncompressed file size, space saving as a percent), -i allows the user to specify the input file, -o allows the user to specify the output file, and -h prints the help message. 

## Known errors

Make scan-build does not generate errors or warnings. Valgrind and gdb likewise do not generate errors.
