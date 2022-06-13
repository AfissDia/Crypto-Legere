#ifndef PRESENT_H_INCLUDED
#define PRESENT_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef enum {
    ENCRYPT_MODE,
    DECRYPT_MODE,
} op_mode;

extern  uint8_t * state;
extern  uint8_t * register_key;
extern  uint8_t *round_key;

/**
 * Lookup table for PRESENT substitution process.
 */
static uint8_t const sbox[] = {0x0Cu, 0x05u, 0x06u, 0x0Bu, \
                                 0x09u, 0x00u, 0x0Au, 0x0Du, \
                                 0x03u, 0x0Eu, 0x0Fu, 0x08u, \
                                 0x04u, 0x07u, 0x01u, 0x02u};

/**
 * Lookup table for PRESENT inverse substitution process.
 */
static uint8_t const sbox_inv[] = {0x05u, 0x0Eu, 0x0Fu, 0x08u, \
                                     0x0Cu, 0x01u, 0x02u, 0x0Du, \
                                     0x0Bu, 0x04u, 0x06u, 0x03u, \
                                     0x00u, 0x07u, 0x09u, 0x0Au};


void init();
void keygen();

void r_circle_shift();
void update(uint8_t round_counter, op_mode mode);
void sub_key();

void add_round_key();
uint8_t two_sbox(uint8_t input);
void simple_sbox(uint8_t input);
void sBoxLayer();
void inv_sBoxLayer();
void pLayer(op_mode mode);


void inv_pLayer(op_mode mode);
void encrypt(uint8_t * msg, uint8_t const * key);
void print_byte(uint8_t * bytes, int size);
void decrypt(uint8_t * msg, uint8_t const * register_key);
void printHex(uint8_t * bytes, int size);
#endif 

