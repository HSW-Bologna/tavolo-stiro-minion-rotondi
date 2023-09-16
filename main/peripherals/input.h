#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED


typedef enum {
    INPUT_1 = 0,
    INPUT_2,
    INPUT_3,
    INPUT_4,
    INPUT_5,
#define INPUT_NUM 5
} input_t;


void input_init(void);
uint8_t input_take_reading(void);
uint8_t input_get(input_t input);
uint16_t input_get_map(void);


#endif