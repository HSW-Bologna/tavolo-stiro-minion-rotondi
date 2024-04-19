#ifndef RELAY_H_INCLUDED
#define RELAY_H_INCLUDED


#include <stdint.h>
#include "hardwareprofile.h"




/*
#define POMPA_H2O_VAPORE            HAP_POMPA_LAT   // 01 - TRIAC

#define RISCALDAMENTO_PIANO         HAP_REL6_LAT    // 02
#define RISCALDAMENTO_H2O_VAPORE    HAP_REL5_LAT    // 03
#define RISCALDAMENTO_BRACCIOLO     HAP_REL2_LAT    // 04

#define FERRO_1                     HAP_REL1_LAT    // 05
#define FERRO_2                     HAP_REL4_LAT    // 06
#define LUCE                        HAP_REL3_LAT    // 07

#define RECUPERATORE                HAP_REL5_LAT    // XX
 */



#define RELAY_SET(r)   relay_update(r, 1);
#define RELAY_CLEAR(r) relay_update(r, 0);

#define RELAY_AUX RELAY_9
#define RELAY_TAGLIOLA_A RELAY_10
#define RELAY_TAGLIOLA_B RELAY_11


typedef enum {
    RELAY_1 = 0,
    RELAY_2,
    RELAY_3,
    RELAY_4,
    RELAY_5,
    RELAY_6,
    RELAY_7,
    RELAY_8,
    RELAY_9,
    RELAY_10,
    RELAY_11,
#define RELAY_NUM RELAY_8+1
} relay_t;


void    relay_init(void);
void    relay_update(relay_t relay, uint8_t value);
uint8_t relay_read(relay_t relay);
uint8_t relay_read_map(void);

#endif
