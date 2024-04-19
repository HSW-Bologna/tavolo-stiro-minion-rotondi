#include "hardwareprofile.h"
#include "system.h"
#include "heartbit.h"
#include "services/system_time.h"


void heartbit_init(void) {
    HAP_RUN_TRIS = SYSTEM_TRIS_OUTPUT;
    HAP_RUN_LAT = 0;
}


void heartbit_manage(unsigned long timestamp) {
    static uint8_t led_on = 0;
    static unsigned long led_ts = 0;
    
    if (led_on && is_expired(led_ts, timestamp, 50UL)) {
        HAP_RUN_LAT = 0;
        led_on = 0;
        led_ts = timestamp;
    } else if (!led_on && is_expired(led_ts, timestamp, 1950UL)) {
        HAP_RUN_LAT = 1;
        led_on = 1;
        led_ts = timestamp;
    }
}