#ifndef PARAM_H_INCLUDED
#define PARAM_H_INCLUDED

/**
* La version de l'algo Present
*/
#define PRESENT (80)
/**
* Le nombre de ronde (u unsigned)
*/
#define PRESENT_ROUND_COUNT (31u)
/**
* le nombre de bit du state(message) et cle de ronde
*/
#define PRESENT_CRYPT_BIT_SIZE (64u)

/**
* le nbre de bytes pour stocker un state (exemple 64 bits correspond a 16 bytes)
*/
#define PRESENT_CRYPT_SIZE (PRESENT_CRYPT_BIT_SIZE / 8u)

/**
* PRESENT_KEY_BIT_SIZE : La taille de la cle mere en bits
*/
#if PRESENT == 80
    #define PRESENT_KEY_BIT_SIZE (80u)
#else
    #define PRESENT_KEY_BIT_SIZE (128u)
#endif
/**
* PRESENT_KEY_SIZE : La taille de la cle mere en bytes
*/
#define PRESENT_KEY_SIZE (PRESENT_KEY_BIT_SIZE/8u)


#endif // PARAM_H_INCLUDED
