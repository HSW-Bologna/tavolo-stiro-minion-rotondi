#include "xc.h"
#include "hardwareprofile.h"
#include "ptc.h"
#include "system.h"

#define MINIMUM_AD_VALUE 880L
#define MAXIMUM_AD_VALUE 1620L
#define MINIMUM_TEMP_VALUE 0L
#define MAXIMUM_TEMP_VALUE 130L

#define COEFF_M_TIMES(x) ((x * (MAXIMUM_TEMP_VALUE-MINIMUM_TEMP_VALUE))/(MAXIMUM_AD_VALUE-MINIMUM_AD_VALUE))
#define COEFF_Q (-MINIMUM_AD_VALUE*(MAXIMUM_TEMP_VALUE-MINIMUM_TEMP_VALUE)/(MAXIMUM_AD_VALUE-MINIMUM_AD_VALUE)+MINIMUM_TEMP_VALUE)

#define NUM_SAMPLES 1


#define NUM_READINGS 10


static unsigned long ptc_readings[PTC_NUM][NUM_READINGS]={{0},{0}};
static int index=0;
static int first_loop=1;


// Routine che richiede fino a 50 microsecondi, da usare con parsimonia.
static unsigned long ptc_read_input(int channel) {
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



void ptc_take_readings(void) {
    ptc_readings[PTC_1][index] = ptc_read_input(HAP_TEMP1_AN);
    ptc_readings[PTC_2][index] = ptc_read_input(HAP_TEMP2_AN);
    
    if (index==NUM_READINGS-1) {
        first_loop=0;
    }
    index=(index+1)%NUM_READINGS;
}



uint16_t ptc_get_adc_value(ptc_t ptc) {
    unsigned long temperature_sum=0;
    unsigned long temp=0;
    int i;
    int num_readings = first_loop ? index : NUM_READINGS;
    for (i=0; i<num_readings; i++) {
        temperature_sum+=ptc_readings[ptc][i];
    }
    if (num_readings==0) {
        return 0;
    }
    else {
        temp = temperature_sum/num_readings;
        return (uint16_t)temp;
    }
}



int ptc_get_temperature(ptc_t ptc) {
    int32_t temp = ptc_get_adc_value(ptc);
    if (temp<=MINIMUM_AD_VALUE)
        return MINIMUM_TEMP_VALUE;
    else if (temp>=MAXIMUM_AD_VALUE)
        return MAXIMUM_TEMP_VALUE;
    else
        return (int) (COEFF_M_TIMES(temp)+COEFF_Q);
}
