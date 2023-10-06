#ifndef LEVEL_H_INCLUDED
#define LEVEL_H_INCLUDED


typedef enum {
    LEVEL_1 = 0,
    LEVEL_2,
#define LEVEL_NUM 2
} level_t;


void level_init(void);
uint16_t level_get_adc_value(level_t level);
void level_take_readings(void);

#endif
