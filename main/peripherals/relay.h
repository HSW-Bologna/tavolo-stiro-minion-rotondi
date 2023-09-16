#ifndef RELAY_H_INCLUDED
#define RELAY_H_INCLUDED


#include <stdint.h>


#define RELAY_SET(r)   relay_update(r, 1);
#define RELAY_CLEAR(r) relay_update(r, 0);



typedef enum {
    RELAY_1 = 0,
    RELAY_2,
    RELAY_3,
    RELAY_4,
    RELAY_5,
    RELAY_6,
    RELAY_7,
#define RELAY_NUM RELAY_7+1
} relay_t;


void    relay_init(void);
void    relay_update(relay_t relay, uint8_t value);
uint8_t relay_read(relay_t relay);
uint8_t relay_read_map(void);


#endif
