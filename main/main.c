#include "peripherals/system.h"
#include "peripherals/relay.h"
#include "peripherals/rs485_display.h"
#include "peripherals/ptc.h"
#include "peripherals/phase_cut.h"
#include "controller/controller.h"
#include "peripherals/input.h"
#include "peripherals/level.h"
#include "peripherals/timer.h"
#include "services/system_time.h"


int main(void) {
    timestamp_t input_ts = 0;
    timestamp_t adc_ts = 0;

    system_init();
    relay_init();
    input_init();
    timer_init();
    level_init();
    rs485_display_init();
    phase_cut_init();
    
    controller_init();
    
    for (;;) {
        if (is_expired(input_ts, get_millis(), 10)) {
            if (input_take_reading()) {
                //Inputs changed
            }
            
            input_ts = get_millis();
        }
        
        if (is_expired(adc_ts, get_millis(), 100)) {
            level_take_readings();
            ptc_take_readings();
           
            adc_ts = get_millis();
        }
        
        rs485_display_manage();
        controller_manage();
        Idle();
    }
    
    return 0;
}