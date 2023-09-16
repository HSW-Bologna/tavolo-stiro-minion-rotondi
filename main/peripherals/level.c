#include "xc.h"
#include "hardwareprofile.h"
#include "level.h"
#include "system.h"

#define NUM_SAMPLES 1

#define NUM_READINGS 10


#define PWM_PERIOD (0xFFFF/16)


static unsigned long level_readings[LEVEL_NUM][NUM_READINGS]={{0},{0}};
static int index=0;
static int first_loop=1;




// Routine che richiede fino a 50 microsecondi, da usare con parsimonia.
static unsigned long read_input(int channel) {
    unsigned long value = 0, i, tmp, result;
    
    AD1CON1bits.ADON   = 0;
    AD1CON1bits.MODE12 = 1;
    AD1CHS0bits.CH0SA  = channel;
    AD1CON1bits.ADON   = 1;
    __delay_us(20);     // TODO: riduci
    
    for (i = 0; i < NUM_SAMPLES; i++)
    {
        AD1CON1bits.SAMP = 1;
        __delay_us(2);     // Attesa di TAD*3 (due microsecondi sono abbondanti)
        AD1CON1bits.SAMP = 0;
        
        while (!IFS0bits.AD1IF)
        {
            __delay_us(1);
        }
        IFS0bits.AD1IF = 0;
        tmp            = ADC1BUF0;
        value += tmp;
    }
    AD1CON1bits.ADON = 0;
    
    result = value / NUM_SAMPLES;
    return result;
}



void level_take_readings(void) {
    level_readings[LEVEL_1][index] = read_input(HAP_LIVELLO1_H2O_AN);
    level_readings[LEVEL_2][index] = read_input(HAP_LIVELLO2_H2O_AN);
    
    if (index==NUM_READINGS-1) {
        first_loop=0;
    }
    index=(index+1)%NUM_READINGS;
}



uint16_t level_get_adc_value(level_t level) {
    unsigned long sum=0;
    unsigned long temp=0;
    int i;
    int num_readings = first_loop ? index : NUM_READINGS;
    for (i=0; i<num_readings; i++) {
        sum+=level_readings[level][i];
    }
    if (num_readings==0) {
        return 0;
    }
    else {
        temp = sum/num_readings;
        return (uint16_t)temp;
    }
}



void level_init(void) {
    HAP_PWM_LIVELLI_TRIS = SYSTEM_TRIS_OUTPUT;
    
    // Set MCCP operating mode
    CCP1CON1Lbits.CCSEL = 0;         // Set MCCP operating mode (OC mode)
    CCP1CON1Lbits.MOD = 0b0101;      // Set mode (Buffered Dual-Compare/PWM mode)
    
    //Configure MCCP Timebase
    CCP1CON1Lbits.T32 = 0;           // Set timebase width (16-bit)
    CCP1CON1Lbits.TMRSYNC = 0;       // Set timebase synchronization (Synchronized)
    CCP1CON1Lbits.CLKSEL = 0b000;    // Set the clock source (Tcy)
    CCP1CON1Lbits.TMRPS = 0b00;      // Set the clock pre-scaler (1:1)
    CCP1CON1Hbits.TRIGEN = 0;        // Set Sync/Triggered mode (Synchronous)
    CCP1CON1Hbits.SYNC = 0b00000;    // Select Sync/Trigger source (Self-sync)
    
    //Configure MCCP output for PWM signal
    CCP1CON2Hbits.OCAEN = 0;
    CCP1CON2Hbits.OCBEN = 0;
    CCP1CON2Hbits.OCCEN = 0;
    CCP1CON2Hbits.OCDEN = 0;
    CCP1CON2Hbits.OCFEN = 1;          // Enable desired output signals (OC1F)
    CCP1CON2Hbits.OCEEN = 0;          // Enable desired output signals (OC1F)

    CCP1CON3Hbits.OUTM = 0b000;       // Set advanced output modes (Standard output)
    CCP1CON3Hbits.POLACE = 0;         // Configure output polarity (Active High)
    CCP1TMRL = 0x0000;                // Initialize timer prior to enable module.
    CCP1PRL = PWM_PERIOD;                 // Configure timebase period 
    // CCP1RA = 0x0000;                  // Set the rising edge compare value
    CCP1RA = 1;
    CCP1RB = 0;                       // Set the falling edge compare value
    CCP1CON1Lbits.CCPON = 1;          // Turn on MCCP module
    
    CCP1RA = PWM_PERIOD/2;
}
