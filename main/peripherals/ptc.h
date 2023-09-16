#ifndef PTC_H_INCLUDED
#define PTC_H_INCLUDED




typedef enum {
    PTC_1 = 0,
    PTC_2,
#define PTC_NUM 2
} ptc_t;


void ptc_take_readings(void);
int ptc_get_temperature(ptc_t ptc);
uint16_t ptc_get_adc_value(ptc_t ptc);

#endif

