#include "word.h"
#include "io.h"
#include "endian.h"
#include "code.h"

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>

static uint8_t buf_pair[BLOCK];
static uint8_t buf_char[BLOCK];
static uint32_t buf_index = 0;
static uint32_t pair_index = 0;
static uint32_t bits_left = 0;
static uint32_t byte_left = 0;
uint64_t total_syms;
uint64_t total_bits;

//resets buffer for read pair
uint32_t reset_buffer(int infile);

//loops calls to read()
int read_bytes(int infile, uint8_t *buf, int to_read) {
    int bytes_read = 0;
    int x = 0;
    while (bytes_read != to_read) {
        x = read(infile, buf, (to_read - bytes_read));
        if (x < 1) {
            break;
        }
        bytes_read += x;
    }
    return bytes_read;
}

//loops calls to write()
int write_bytes(int outfile, uint8_t *buf, int to_write) {
    int bytes_write = 0;
    int x = 0;
    while (bytes_write != to_write) {
        x = write(outfile, buf, (to_write - bytes_write));
        if (x < 1) {
            break;
        }
        bytes_write += x;
    }
    return bytes_write;
}

//reads a header into header, swaps endianness if necessary
void read_header(int infile, FileHeader *header) {
    read(infile, header, sizeof(FileHeader));
    if (big_endian()) {
        header->magic = swap32(header->magic);
        header->protection = swap32(header->protection);
    }
    assert((header->magic == 0xBAADBAAC));
    total_bits += 64;
}

//writes a header into header, swaps endianness if necessary
void write_header(int outfile, FileHeader *header) {
    if (big_endian()) {
        header->magic = swap32(header->magic);
        header->protection = swap32(header->protection);
    }
    write(outfile, header, sizeof(FileHeader));
    total_bits += 64;
}

bool read_sym(int infile, uint8_t *sym) {
    int x = 0;
    //if buffer is not full or empty
    if (byte_left == 0) {
        //reset buffer to all 0s
        for (int i = 0; i < BLOCK; i++) {
            buf_char[i] = 0;
        }
        //reset buffer index
        buf_index = 0;
        //read from infile into character buffer
        x = read_bytes(infile, buf_char, sizeof(buf_char));
        //if read returns error or EOF, no more to read
        if (x < 1) {
            return false;
        }
        //otherwise, add bytes read to bytes left
        byte_left += x;
    }
    if (byte_left > 0) {
        //check syntax
        *sym = buf_char[buf_index];
        //increment total syms
        total_syms++;
        //increment index of buffer
        buf_index++;
        //decrement how many bytes are left
        byte_left--;
    }
    return (byte_left > 0);
}

void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen) {
    for (int i = 0; i < bitlen; i++) {
        //index / 8 is the byte
        //shift left by index mod 8 to set specific bit
        //right shift code to choose the right bit
        buf_pair[pair_index / 8] |= ((code >> i) & 1) << (pair_index % 8);
        pair_index++;
        //if buffer is full (index is 4096 * 8)
        //reset buffer to 0s and flush pairs
        if (pair_index == (4096 * 8)) {
            flush_pairs(outfile);
        }
    }
    for (int i = 0; i < 8; i++) {
        buf_pair[pair_index / 8] |= ((sym >> i) & 1) << (pair_index % 8);
        pair_index++;
        //if buffer is full (index is 4096 * 8)
        //reset buffer to 0s and flush pairs
        if (pair_index == (4096 * 8)) {
            flush_pairs(outfile);
        }
    }
    //remember to call flush pairs another time in decode/encode
}

void flush_pairs(int outfile) {
    total_bits += (pair_index);
    write_bytes(outfile, buf_pair, (pair_index / 8));
    for (int i = 0; i < 4096; i++) {
        buf_pair[i] = 0;
    }
    pair_index = 0;
}

bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen) {
    *code = 0;
    *sym = 0;
    //printf("%" PRIu32 "\n",bits_left);
    uint32_t x = 0;
    //no bits
    if (bits_left == 0) {
        x = reset_buffer(infile);
        if (x < 1) {
            //returns EOF or error, return false
            return false;
        }
    }

    //if there are bits left in the buffer and buffer is not empty
    if (bits_left > 0) {
        //read bitlen bits from buffer for code
        //based on pseudocode from Jessie Srinivas' section (3/6/23)
        //printf("code");
        for (int i = 0; i < bitlen; i++) {
            //printf("\ncurrent byte: %d\n", buf_pair[pair_index / 8]);
            //printf("%x", ((buf_pair[pair_index / 8] >> (pair_index % 8)) & 1));
            //or bit i of code with the current bit in buf_pair
            *code |= ((buf_pair[pair_index / 8] >> (pair_index % 8)) & 1) << i;
            //increment pair_index
            pair_index++;
            total_bits++;
            bits_left--;
            if (pair_index == (4096 * 8)) {
                reset_buffer(infile);
            }
        }
        //printf("\nsym");
        //read sym
        for (int i = 0; i < 8; i++) {
            //printf("\ncurrent byte: %d\n", buf_pair[pair_index / 8]);
            //printf("%x", ((buf_pair[pair_index / 8] >> (pair_index % 8)) & 1));
            //or bit i of sym with the current bit in buf_pair
            *sym |= ((buf_pair[pair_index / 8] >> (pair_index % 8)) & 1) << i;
            //increment pair_index
            pair_index++;
            total_bits++;
            bits_left--;
            if (pair_index == (4096 * 8)) {
                reset_buffer(infile);
            }
        }
        //printf("\n");
    }
    if (*code == STOP_CODE) {
        return false;
    }
    return (bits_left > 0);
}

void write_word(int outfile, Word *w) {
    for (uint32_t i = 0; i < w->len; i++) {
        buf_char[buf_index] = w->syms[i];
        buf_index++;
        if (buf_index == (4096)) {
            flush_words(outfile);
        }
    }
}

void flush_words(int outfile) {
    total_syms += buf_index;
    write_bytes(outfile, buf_char, buf_index);
    for (int i = 0; i < 4096; i++) {
        buf_char[i] = 0;
    }
    buf_index = 0;
}

//resets the buffer for read_pairs
//because it's completely possible for the buffer to fill up while code or sym is not finished
uint32_t reset_buffer(int infile) {
    uint32_t x = 0;
    for (int i = 0; i < 4096; i++) {
        buf_pair[i] = 0;
    }
    //reset pair index
    pair_index = 0;
    //read bytes into buffer
    x = read_bytes(infile, buf_pair, sizeof(buf_pair));
    if (x < 1) {
        //returns EOF or error, return false
        return x;
    }
    //add bytes read * 8 to bits_left
    bits_left += (x * 8);
    return x;
}
