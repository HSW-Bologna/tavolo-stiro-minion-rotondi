#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED

unsigned long get_millis(void);
void          timer_init(void);
int           timer_second_passed(void);
uint8_t timer_get_gettoni(uint16_t *gettoni, uint16_t *ingresso);

#endif  /* TIMER_H_INCLUDED */
