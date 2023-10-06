
#ifndef PHASE_CUT_H_INCLUDED
#define PHASE_CUT_H_INCLUDED

#include <stdint.h>



typedef enum {
    PHASE_CUT_ASPIRATION = 0,
    PHASE_CUT_BLOW,
#define PHASE_CUT_NUM 2
} phase_cut_t;


void phase_cut_init(void);
void phase_cut_set_percentage(phase_cut_t phase_cut, uint8_t percentage);

#endif
