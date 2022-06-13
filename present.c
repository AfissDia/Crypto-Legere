#include <stdio.h>
#include <stdlib.h>  
#include <time.h>    
#include <math.h>
#include <string.h>
#include "param.h"
#include "present.h"


uint8_t * state;
uint8_t * register_key;
uint8_t *round_key;

/**
* Ici on fait l'allocation dynamique de la memoire avec calloc pour mettre les valeur � z�ro
*/
void init()
{
    state = (uint8_t *) calloc( PRESENT_CRYPT_SIZE, sizeof(uint8_t) );
    register_key = (uint8_t *) calloc( PRESENT_KEY_SIZE, sizeof(uint8_t) );
    round_key = (uint8_t *) calloc( PRESENT_CRYPT_SIZE, sizeof(uint8_t) );
}

void keygen()
{
    time_t t;
    register_key = (uint8_t *) calloc( PRESENT_KEY_SIZE, sizeof(uint8_t) );
    srand((unsigned) time(&t));
    int i=0;
    for( i = 0 ; i < PRESENT_KEY_SIZE ; i++ )
    {
        register_key[i]=(uint8_t)rand();
    }
}

void sub_key()
{
    round_key = (uint8_t *) calloc( PRESENT_CRYPT_SIZE, sizeof(uint8_t) );
    int i=0;
    for( i = 2 ; i < PRESENT_KEY_SIZE ; i++ )
    {
        round_key[i-2]=register_key[i];
    }
}

void add_round_key()
{

    int i=0;
    for(i=0; i<PRESENT_CRYPT_SIZE ; i++ )
    {
        state[i]=state[i]^round_key[i];
    }
}


uint8_t two_sbox(uint8_t input)
{
    uint8_t high=(input>>4) & 0x0F;
    high=sbox[high];
    uint8_t low=(input) & 0x0F;
    low=sbox[low];
    return ((high<<4) | low)& 0xFF;
}

void sBoxLayer()
{
    int i=0;
    for(i=0; i<PRESENT_CRYPT_SIZE ; i++ )
    {
        state[i]=two_sbox(state[i]);
    }
}

//Sbox_inverse 

uint8_t inv_two_sbox(uint8_t sBoxLayer)
{
    uint8_t highs=(sBoxLayer>>4) & 0x0F;
    highs=sbox_inv[highs];
    uint8_t lows=(sBoxLayer) & 0x0F;
    lows=sbox_inv[lows];
    return ((highs)<<4 | (lows))& 0xFF;
}

void inv_sBoxLayer()
{
    int i=0;
    for(i=0; i<PRESENT_CRYPT_SIZE ; i++ )
    {
        state[i]=inv_two_sbox(state[i]);
    }
}

//fin inv_sbox

void pLayer(op_mode mode)
{
    int i, poz=0;
    uint8_t * int_state;
    uint8_t UN=1u;
    uint8_t bit=0;
    uint8_t offset=(mode==ENCRYPT_MODE)?16:4;
    int_state = (uint8_t *) malloc( PRESENT_CRYPT_SIZE);
    memcpy( int_state, state, PRESENT_CRYPT_SIZE);

    for(i=0; i<PRESENT_CRYPT_BIT_SIZE-1 ; i++ )
    {
        
        poz=(offset*i) % 63;
        bit=(int_state[i/8]>>(i%8)) & UN;
        if(bit)state[poz/8] |= UN<<(poz%8);
        else state[poz/8] &= (UN<<(poz%8))^0xFF;
    }
}

//Inv_PLAYER
void inv_pLayer(op_mode mode)
{
    int i, poz=0;
    uint8_t * int_state;
    uint8_t UN=1u;
    uint8_t bit=0;
    //uint8_t offset=4;
    uint8_t offset=(mode==DECRYPT_MODE)?16:4;
    int_state = (uint8_t *) malloc( PRESENT_CRYPT_SIZE);
    memcpy( int_state, state, PRESENT_CRYPT_SIZE);

    for(i=0; i<PRESENT_CRYPT_BIT_SIZE-1 ; i++ )
    {
        poz=(offset*i) % 63;
        bit=(int_state[i/8]>>(i%8)) & UN;
        if(bit)state[poz/8] |= UN<<(poz%8);
        else state[poz/8] &= (UN<<(poz%8))^0xFF;
    }
}

//rigth_circle_shift
void r_circle_shift()
{
    if(PRESENT==80)
    {
        //permute les deux premier bloc
        int i=0;
        uint8_t temp1=register_key[0];
        uint8_t temp2=register_key[1];
        for(i=0; i<PRESENT_KEY_SIZE-2 ; i++ )
        {
            register_key[i]=register_key[i+2];
        }
        register_key[PRESENT_KEY_SIZE-1]=temp2;
        register_key[PRESENT_KEY_SIZE-2]=temp1;

        //Permuter les 3 bit restant
        temp1=register_key[0];
        for(i=0; i<PRESENT_KEY_SIZE-1 ; i++ )
        {
            register_key[i]=(register_key[i]>>3)|(register_key[i+1]<< 5);
        }
        register_key[PRESENT_KEY_SIZE-1]=(register_key[PRESENT_KEY_SIZE-1]>>3)|(temp1<< 5);
    }
}

void l_circle_shift()
{
    if(PRESENT==80)
    {
        //permute les deux premier bloc
        int i=0;
        uint8_t temp0=register_key[PRESENT_KEY_SIZE-2];
        uint8_t temp1=register_key[PRESENT_KEY_SIZE-1];
        for(i=PRESENT_KEY_SIZE-1 ;i>=2 ;i-- )
        {
            register_key[i]=register_key[i-2];
        }
        register_key[1]=temp0;
        register_key[0]=temp1;

        //Permuter les 3 bit restant
        temp1=(register_key[PRESENT_KEY_SIZE-1]>>5) & 0x07;
        for(i=PRESENT_KEY_SIZE-1 ; i>=1 ;i-- )
        {
            register_key[i]=(register_key[i]<<3)^(register_key[i-1]>>5 & 0x07);
        }
        register_key[0]=(register_key[0]<<3) ^ temp1;
    }
}

void update(uint8_t round_counter, op_mode mode)
{
    if(PRESENT==80 && mode==ENCRYPT_MODE)
    {
        r_circle_shift();

        uint8_t high=sbox[(register_key[PRESENT_KEY_SIZE-1]>>4) & 0x0F];
        uint8_t low=(register_key[PRESENT_KEY_SIZE-1]) & 0x0F;
        register_key[PRESENT_KEY_SIZE-1]=((high<<4) | low)& 0xFF;


        high=(register_key[2]>>4) & 0x0F;
        low=(register_key[2]& 0x0F);
        low^=round_counter>>1;
        register_key[2]=((high<<4) | low)& 0xFF;

        register_key[1]^=(round_counter& 0x01)<<7;

    }

}

void inv_update(uint8_t round_counter, op_mode mode)
{
    if(PRESENT==80 && mode==DECRYPT_MODE)
    {   
        uint8_t high=(register_key[2]>>4) & 0x0F;
        uint8_t low=(register_key[2]& 0x0F);
        low^=round_counter>>1;
        register_key[2]=((high<<4) | low)& 0xFF;

        register_key[1]^=(round_counter& 0x01)<<7;

        high=sbox_inv[(register_key[PRESENT_KEY_SIZE-1]>>4) & 0x0F];
        low=(register_key[PRESENT_KEY_SIZE-1]) & 0x0F;
        register_key[PRESENT_KEY_SIZE-1]=((high<<4) | low)& 0xFF;

        l_circle_shift();

    }

}

// le chiffrement 

void encrypt(uint8_t * msg, uint8_t const * key)
{
    init();
    memcpy(state, msg, PRESENT_CRYPT_SIZE);
    memcpy(register_key, key, PRESENT_KEY_SIZE);
    uint8_t round;
    round = 1u;
    
    printf("messages ::: ");
    printBinary(state, PRESENT_CRYPT_SIZE);
    printf("register ::: ");
    printBinary(register_key, PRESENT_KEY_SIZE);
    
    while (round <= PRESENT_ROUND_COUNT)
    {
        sub_key();
        add_round_key();
        sBoxLayer();
        pLayer(ENCRYPT_MODE);
        update(round,ENCRYPT_MODE);

        round++;
    };
    sub_key();
    add_round_key();


}

//dechiffrement
void decrypt(uint8_t * state, uint8_t const * register_key)
{
    init();
    uint8_t round;
    round = 1u;
    
    printf("state    ::: " );
    printBinary(state, PRESENT_CRYPT_SIZE);
    printf("register ::: ");
    printBinary(register_key, PRESENT_KEY_SIZE);
    
    sub_key();
    add_round_key();

    while (round >= PRESENT_ROUND_COUNT)
    {
        inv_pLayer(DECRYPT_MODE);
        inv_update(round,DECRYPT_MODE);
        inv_sBoxLayer(); 
        sub_key();
        add_round_key();

        round--;
    };

}

void print_byte(uint8_t * bytes, int size)
{
    int i=0;
    printf("\n");
    for(i=size-1; i >= 0; i--)
        printf("%d ", bytes[i]);
    printf("\n");
}
void printHex(uint8_t * bytes, int size)
{
    int i;
    for (i=size-1; i >= 0; i--)
    {
        if (i < size-1) printf(":");
        printf("%02X", bytes[i]);
    }
    printf("\n");
}
void toBinary(uint8_t a)
{
    uint8_t i;

    for(i=0x80;i!=0;i>>=1) {
        printf("%c",(a&i)?'1':'0');
        if (i==0x10)
            printf(" ");
    }
}

void printBinary(uint8_t * bytes, int size)
{
    int i;
    for (i=size-1; i >= 0; i--)
    {
        toBinary(bytes[i]);
        printf(" ");
    }
    printf("\n");
}
