#include "hardwareprofile.h"
#include "debounce.h"
#include "system.h"
#include "input.h"


static debounce_filter_t filter = {0};


void input_init(void) {
    HAP_IN1_TRIS = SYSTEM_TRIS_INPUT;
    HAP_IN2_TRIS = SYSTEM_TRIS_INPUT;
    HAP_IN3_TRIS = SYSTEM_TRIS_INPUT;
    HAP_IN4_TRIS = SYSTEM_TRIS_INPUT;
    
    debounce_filter_init(&filter);
}


uint8_t input_take_reading(void) {
    uint32_t value = HAP_IN1_PORT | (HAP_IN2_PORT << 1) | (HAP_IN3_PORT << 2) | (HAP_IN4_PORT << 3) | (HAP_VAP_PORT << 4);
    value = (~value) & 0x1F;
    return debounce_filter(&filter, value, 10);
}


uint8_t input_get(input_t input) {
    return debounce_read(&filter, input);
}


uint16_t input_get_map(void) {
    return debounce_value(&filter);
}