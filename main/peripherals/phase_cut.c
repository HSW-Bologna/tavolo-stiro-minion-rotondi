#include <stdint.h>
#include "hardwareprofile.h"
#include "timer.h"
#include "system.h"
#include "phase_cut.h"


#define MIN_DELAY 1400
#define MAX_DELAY 9000


static void phase_cut_control(phase_cut_t phase_cut, uint8_t value);


// Delay in microseconds to be applied to the corresponding percentage (as index)
static const uint16_t sine_percentage_linearization[100] = {
    9999, 9361, 9095, 8890, 8716, 8563, 8423, 8294, 8173, 8058, 7950, 7846, 7746, 7650, 7556, 7466, 7378,
    7292, 7208, 7126, 7046, 6967, 6889, 6813, 6738, 6664, 6591, 6519, 6447, 6377, 6307, 6238, 6169, 6101,
    6034, 5967, 5900, 5834, 5768, 5703, 5638, 5573, 5508, 5444, 5380, 5315, 5252, 5188, 5124, 5060, 4996,
    4933, 4869, 4805, 4741, 4677, 4613, 4549, 4485, 4420, 4355, 4290, 4225, 4159, 4093, 4026, 3959, 3892,
    3824, 3755, 3686, 3616, 3546, 3474, 3402, 3329, 3255, 3180, 3104, 3026, 2947, 2867, 2785, 2701, 2615,
    2527, 2437, 2343, 2247, 2147, 2043, 1935, 1820, 1699, 1570, 1430, 1276, 1103, 898,  632,
};


uint8_t phase_cut_active[PHASE_CUT_NUM] = {0};
uint16_t phase_cut_period[PHASE_CUT_NUM] = {0};
uint16_t phase_cut_counter[PHASE_CUT_NUM] = {0};
uint16_t phase_cut_triac[PHASE_CUT_NUM] = {0};


void phase_cut_set_percentage(phase_cut_t phase_cut, uint8_t percentage) { 
    if (percentage > 100) {
        percentage = 100;
    } else if (percentage == 0) {
        percentage = 1;
        phase_cut_active[phase_cut] = 0;
    } else {
        phase_cut_active[phase_cut] = 1;
    }
    phase_cut_period[phase_cut] = sine_percentage_linearization[percentage-1];
}

void phase_cut_init(void)
{
    HAP_INT0_TRIS = SYSTEM_TRIS_INPUT;
    
    HAP_ASP_TRIS = SYSTEM_TRIS_OUTPUT;
    HAP_SOFF_TRIS = SYSTEM_TRIS_OUTPUT;
    
    //    CNENBbits.CNIEB7 = 1;
    //    CNPUB = 0;
    //    CNPDB = 0;
    //    IFS1bits.CNIF = 0;
    //    IPC4bits.CNIP = 0x06;
    //    IEC1bits.CNIE = 1;
    
    
    
    IOCPBbits.IOCPB7 = 1; /* Interrupt trigger su transizioni low-to-high */
    IOCNBbits.IOCNB7 = 1; /* Interrupt trigger su transizioni high-to-low */

    IOCPUBbits.IOCPUB7 = 0; /* Disabilitare il pull-up interno */
    IOCPDBbits.IOCPDB7 = 0; /* Disabilitare il pull-down interno */

    IPC4bits.IOCIP = 0x05; /* maximum priority */

    IFS1bits.IOCIF   = 0; /* Interrupt flag */
    PADCONbits.IOCON = 1; /* Interrupt-on-change enable bit */
    IEC1bits.IOCIE   = 1; /* Interrupt enable bit globale */

    //    CORCONbits.IPL3     = 0;    /* CPU Priority Level ???? !!!! ToDO */
    
    TMR2            = 0;
    T2CONbits.TCS   = 0;            // Internal Clock
    T2CONbits.TCKPS = 0;            // Clock source prescaler
    PR2             = 1603 - 1;    // Period, interrupt every 100us at 32MHz
    T2CONbits.TON   = 1;            // Enable timer 2
    IPC1bits.T2IP   = 4;            // timer 2 interrupt priority
    IFS0bits.T2IF   = 0;
    IEC0bits.T2IE   = 1;            // Enable timer 2 interrupt
}



void __attribute__((interrupt, no_auto_psv)) _IOCInterrupt()
// void __attribute__((interrupt, no_auto_psv)) _CNInterrupt()
{
    static uint8_t zero_xing_old = 0;

    uint8_t zero_xing = HAP_INT0_PORT;
    if (zero_xing != zero_xing_old)
    {
        phase_cut_t i = 0;
        for (i = 0; i < PHASE_CUT_NUM; i++) {
            if (phase_cut_active[i]) {
                if (phase_cut_period[i] < MIN_DELAY) 
                { // Full
                    phase_cut_counter[i] = 0;
                    phase_cut_control(i, 1);
                } // Empty
                else if (phase_cut_period[i] > MAX_DELAY) {
                    phase_cut_counter[i] = 0;
                    phase_cut_control(i, 0);
                } // Normal
                else {
                    phase_cut_counter[i] = phase_cut_period[i];
                }
            } else {
                phase_cut_control(i, 0);
            }
        }

        zero_xing_old = zero_xing;
    }

    IFS1bits.IOCIF = 0;
}


void __attribute__((interrupt, auto_psv)) _T2Interrupt(void)
{
    phase_cut_t i = 0;
    for (i = 0; i < PHASE_CUT_NUM; i++) {
        if (phase_cut_triac[i] > 0) {
            phase_cut_triac[i]--;
            if (phase_cut_triac[i] == 0) {
                phase_cut_control(i, 0);
            }
        }
        
        if (phase_cut_counter[i] > 0) {
            if (phase_cut_counter[i] > 100) {
                phase_cut_counter[i] -= 100;
            } else {
                phase_cut_counter[i] = 0;
            }
            if (phase_cut_counter[i] == 0) {
                phase_cut_control(i, 1);
                phase_cut_triac[i] = 5;
            }
        }
    }
    
    IFS0bits.T2IF = 0;
}


static void phase_cut_control(phase_cut_t phase_cut, uint8_t value) {
    switch (phase_cut) {
        case PHASE_CUT_ASPIRATION:
            HAP_ASP_LAT = value > 0;
            break;
        case PHASE_CUT_BLOW:
            HAP_SOFF_LAT = value > 0;
            break;
    }
}
