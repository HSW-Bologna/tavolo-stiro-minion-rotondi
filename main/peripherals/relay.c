#include "hardwareprofile.h"
#include "system.h"
#include "relay.h"


void relay_init(void) {
    HAP_REL1_TRIS = SYSTEM_TRIS_OUTPUT;
    HAP_REL2_TRIS = SYSTEM_TRIS_OUTPUT;
    HAP_REL3_TRIS = SYSTEM_TRIS_OUTPUT;
    HAP_REL4_TRIS = SYSTEM_TRIS_OUTPUT;
    HAP_REL5_TRIS = SYSTEM_TRIS_OUTPUT;
    HAP_REL6_TRIS = SYSTEM_TRIS_OUTPUT;
    HAP_REL7_TRIS = SYSTEM_TRIS_OUTPUT;
    
    relay_t relay = RELAY_1;
    for(relay = RELAY_1; relay < RELAY_NUM; relay++) {
        RELAY_CLEAR(relay);
    }
}


void relay_update(relay_t relay, uint8_t value) {
    switch (relay) {
        case RELAY_1:
            HAP_REL1_LAT = value;
        case RELAY_2:
            HAP_REL2_LAT = value;
        case RELAY_3:
            HAP_REL3_LAT = value;
        case RELAY_4:
            HAP_REL4_LAT = value;
        case RELAY_5:
            HAP_REL5_LAT = value;
        case RELAY_6:
            HAP_REL6_LAT = value;
        case RELAY_7:
            HAP_REL7_LAT = value;
        default:
            break;
    }
}


uint8_t relay_read(relay_t relay) {
    switch (relay) {
        case RELAY_1:
            return HAP_REL1_LAT;
        case RELAY_2:
            return HAP_REL2_LAT;
        case RELAY_3:
            return HAP_REL3_LAT;
        case RELAY_4:
            return HAP_REL4_LAT;
        case RELAY_5:
            return HAP_REL5_LAT;
        case RELAY_6:
            return HAP_REL6_LAT;
        case RELAY_7:
            return HAP_REL7_LAT;
        default:
            return 0;
    }
}


uint8_t relay_read_map(void) {
    return HAP_REL1_LAT | (HAP_REL2_LAT << 1) | (HAP_REL3_LAT << 2) | (HAP_REL4_LAT << 3) | (HAP_REL5_LAT << 4) | (HAP_REL6_LAT << 5) | (HAP_REL7_LAT << 6);
}
