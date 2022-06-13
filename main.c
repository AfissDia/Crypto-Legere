#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "param.h"
#include "present.h"

int main()
{
    init();
    uint8_t * msg;
    msg = (uint8_t *) calloc( PRESENT_CRYPT_SIZE, sizeof(uint8_t) );

    uint8_t * key;
    key = (uint8_t *) calloc( PRESENT_KEY_SIZE, sizeof(uint8_t) );
    printf("\n***************** Chiffrement *******************\n\n");
    encrypt(msg, key);
    printBinary(state, PRESENT_CRYPT_SIZE);
    printHex(state, PRESENT_CRYPT_SIZE);
 
    printf("\n\n***************** Déchiffrement *******************\n\n");
    decrypt(state, register_key);
    printBinary(state, PRESENT_CRYPT_SIZE);
    printHex(state, PRESENT_CRYPT_SIZE);

    return 0;

}
