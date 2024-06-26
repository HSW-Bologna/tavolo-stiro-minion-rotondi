#include "hardwareprofile.h"
#include "system.h"
#include "relay.h"



#define POMPA_H2O_VAPORE            HAP_POMPA_LAT   // 01 - TRIAC

#define RISCALDAMENTO_PIANO         HAP_REL6_LAT    // 02
#define RISCALDAMENTO_H2O_VAPORE    HAP_REL5_LAT    // 03
#define RISCALDAMENTO_BRACCIOLO     HAP_REL2_LAT    // 04

#define FERRO_1                     HAP_REL1_LAT    // 05
#define FERRO_2                     HAP_REL4_LAT    // 06
#define LUCE                        HAP_REL3_LAT    // 07

#define RECUPERATORE                HAP_REL7_LAT    // XX



void relay_init(void) {
    HAP_REL1_TRIS = SYSTEM_TRIS_OUTPUT;
    HAP_REL2_TRIS = SYSTEM_TRIS_OUTPUT;
    HAP_REL3_TRIS = SYSTEM_TRIS_OUTPUT;
    HAP_REL4_TRIS = SYSTEM_TRIS_OUTPUT;
    HAP_REL5_TRIS = SYSTEM_TRIS_OUTPUT;
    HAP_REL6_TRIS = SYSTEM_TRIS_OUTPUT;
    HAP_REL7_TRIS = SYSTEM_TRIS_OUTPUT;
    
    HAP_TAGLIOLA_A_TRIS = SYSTEM_TRIS_OUTPUT;
    HAP_TAGLIOLA_B_TRIS = SYSTEM_TRIS_OUTPUT;
    HAP_AUX_M_TRIS = SYSTEM_TRIS_OUTPUT;
    
    HAP_POMPA_TRIS = SYSTEM_TRIS_OUTPUT;    // TRIAC POMPA ON/OFF

    
    relay_t relay = RELAY_1;
    for(relay = RELAY_1; relay < RELAY_NUM; relay++) {
        RELAY_CLEAR(relay);
    }
}


void relay_update(relay_t relay, uint8_t value) {
    switch (relay) {
        case RELAY_1:
            POMPA_H2O_VAPORE = value;
        case RELAY_2:
            RISCALDAMENTO_PIANO = value;
        case RELAY_3:
            RISCALDAMENTO_H2O_VAPORE = value;
        case RELAY_4:
            RISCALDAMENTO_BRACCIOLO = value;
        case RELAY_5:
            FERRO_1 = value;
        case RELAY_6:
            FERRO_2 = value;
        case RELAY_7:
            LUCE = value;
        case RELAY_8:
            RECUPERATORE = value;
        case RELAY_AUX:
            HAP_AUX_M_LAT = value;
        case RELAY_TAGLIOLA_A:
            HAP_TAGLIOLA_A_LAT = value;
        case RELAY_TAGLIOLA_B:
            HAP_TAGLIOLA_B_LAT = value;
        default:
            break;
    }
}


uint8_t relay_read(relay_t relay) {
    switch (relay) {
        case RELAY_1:
            return POMPA_H2O_VAPORE;
        case RELAY_2:
            return RISCALDAMENTO_PIANO;
        case RELAY_3:
            return RISCALDAMENTO_H2O_VAPORE;
        case RELAY_4:
            return RISCALDAMENTO_BRACCIOLO;
        case RELAY_5:
            return FERRO_1;
        case RELAY_6:
            return FERRO_2;
        case RELAY_7:
            return LUCE;
        case RELAY_8:
            return RECUPERATORE;
        case RELAY_AUX:
            return HAP_AUX_M_LAT;
        case RELAY_TAGLIOLA_A:
            return HAP_TAGLIOLA_A_LAT;
        case RELAY_TAGLIOLA_B:
            return HAP_TAGLIOLA_B_LAT;
        default:
            return 0;
    }
}


uint8_t relay_read_map(void) {
    return POMPA_H2O_VAPORE | (RISCALDAMENTO_PIANO << 1) | (RISCALDAMENTO_H2O_VAPORE << 2) | (RISCALDAMENTO_BRACCIOLO << 3) | (FERRO_1 << 4) | (FERRO_2 << 5) | (LUCE << 6) | (RECUPERATORE << 7);
}
