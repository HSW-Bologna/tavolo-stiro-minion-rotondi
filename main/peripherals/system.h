#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#define FOSC 32000000UL
#define FCY (FOSC/2)

void system_init(void);

#include <libpic30.h>
#include <xc.h>


#define SYSTEM_TRIS_OUTPUT 0
#define SYSTEM_TRIS_INPUT  1


#endif
