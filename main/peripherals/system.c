// PIC24FJ256GA705 Configuration Bit Settings

// 'C' source line config statements

// FSEC
#pragma config BWRP = OFF               // Boot Segment Write-Protect bit (Boot Segment may be written)
#pragma config BSS = DISABLED           // Boot Segment Code-Protect Level bits (No Protection (other than BWRP))
#pragma config BSEN = OFF               // Boot Segment Control bit (No Boot Segment)
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GSS = DISABLED           // General Segment Code-Protect Level bits (No Protection (other than GWRP))
#pragma config CWRP = OFF               // Configuration Segment Write-Protect bit (Configuration Segment may be written)
#pragma config CSS = DISABLED           // Configuration Segment Code-Protect Level bits (No Protection (other than CWRP))
#pragma config AIVTDIS = OFF            // Alternate Interrupt Vector Table bit (Disabled AIVT)

// FBSLIM
#pragma config BSLIM = 0x1FFF         // Boot Segment Flash Page Address Limit bits (Enter Hexadecimal value)

// FOSCSEL
#pragma config FNOSC = FRCPLL           // Oscillator Source Selection (Fast RC Oscillator with divide-by-N with PLL module (FRCPLL) )
#pragma config PLLMODE = DISABLED       // PLL Mode Selection (No PLL used; PLLEN bit is not available)
#pragma config IESO = OFF               // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Mode Select bits (Primary Oscillator disabled)
#pragma config OSCIOFCN = ON           // OSC2 Pin Function bit (OSC2 is GPIO)
#pragma config SOSCSEL = OFF            // SOSC Power Selection Configuration bits (Digital (SCLKI) mode)
#pragma config PLLSS = PLL_PRI          // PLL Secondary Selection Configuration bit (PLL is fed by the Primary oscillator)
#pragma config IOL1WAY = ON             // Peripheral pin select configuration bit (Allow only one reconfiguration)
#pragma config FCKSM = CSDCMD           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPS = PS1024            // Watchdog Timer Postscaler bits (1:1024)
#pragma config FWPSA = PR128            // Watchdog Timer Prescaler bit (1:128)
#pragma config FWDTEN = ON              // Watchdog Timer Enable bits (WDT Enabled)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config WDTWIN = WIN25           // Watchdog Timer Window Select bits (WDT Window is 25% of WDT period)
#pragma config WDTCMX = WDTCLK          // WDT MUX Source Select bits (WDT clock source is determined by the WDTCLK Configuration bits)
#pragma config WDTCLK = LPRC            // WDT Clock Source Select bits (WDT uses LPRC)

// FPOR
#pragma config BOREN = ON               // Brown Out Enable bit (Brown Out Enable Bit)
#pragma config LPCFG = OFF              // Low power regulator control (No Retention Sleep)
#pragma config DNVPEN = ENABLE          // Downside Voltage Protection Enable bit (Downside protection enabled using ZPBOR when BOR is inactive)

// FICD
#pragma config ICS = PGD2               // ICD Communication Channel Select bits (Communicate on PGEC2 and PGED2)
#pragma config JTAGEN = OFF              // JTAG Enable bit (JTAG is enabled)

// FDEVOPT1
#pragma config ALTCMPI = DISABLE        // Alternate Comparator Input Enable bit (C1INC, C2INC, and C3INC are on their standard pin locations)
#pragma config TMPRPIN = OFF            // Tamper Pin Enable bit (TMPRN pin function is disabled)
#pragma config SOSCHP = ON              // SOSC High Power Enable bit (valid only when SOSCSEL = 1 (Enable SOSC high power mode (default))
#pragma config ALTI2C1 = ALTI2CEN       // Alternate I2C pin Location (SDA1 and SCL1 on RB9 and RB8)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "system.h"


#define TCY_NS      (1000000000ULL / FCY)
#define MINIMUM_ADCS (120 / TCY_NS)
#define ADCS_SET     10


// Il +2 e' per stare larghi
#if ADCS_SET < (MINIMUM_ADCS + 2)
#error "Impostazione troppo veloce per il TAD dell'ADC"
#endif


void system_init(void) {
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    
    ANSELBbits.ANSB12 = 1;  
    ANSELBbits.ANSB13 = 1;
    ANSELCbits.ANSC1  = 1;
    ANSELCbits.ANSC2  = 1;

    // ASAM disabled; ADDMABM disabled; ADSIDL disabled; DONE disabled; SIMSAM Sequential; FORM Absolute decimal result,
    // unsigned, right-justified; SAMP disabled; SSRC Clearing sample bit ends sampling and starts conversion; AD12B
    // 12-bit; ADON enabled; SSRCG disabled;
    AD1CON1 = 0x0000;
    
    // CSCNA disabled; VCFG0 AVDD; VCFG1 AVSS; ALTS disabled; BUFM disabled; SMPI Generates interrupt after completion
    // of every sample/conversion operation; CHPS 1 Channel;
    AD1CON2 = 0x00;
    // SAMC 0; ADRC FOSC/2; ADCS 0;
    AD1CON3          = 0x0;
    AD1CON3bits.ADCS = ADCS_SET;
    // CH0SA AN0; CH0SB AN0; CH0NB AVSS; CH0NA AVSS;
    AD1CHS0 = 0x00;
    // CSS26 disabled; CSS25 disabled; CSS24 disabled; CSS31 disabled; CSS30 disabled;
    AD1CSSH = 0x00;
    // CSS2 disabled; CSS1 disabled; CSS0 disabled; CSS8 disabled; CSS7 disabled; CSS6 disabled; CSS5 disabled; CSS4
    // disabled; CSS3 disabled;
    AD1CSSL = 0x00;
    // DMABL Allocates 1 word of buffer to each analog input; ADDMAEN disabled;
    AD1CON4 = 0x00;
    // CH123SA disabled; CH123SB CH1=OA2/AN0,CH2=AN1,CH3=AN2; CH123NA disabled; CH123NB CH1=VREF-,CH2=VREF-,CH3=VREF-;
    //AD1CHS123      = 0x00;
    IFS0bits.AD1IF = 0;
    
    
#define  PPSUnLock                   __builtin_write_OSCCONL(OSCCON & 0xbf) 
#define  PPSLock                     __builtin_write_OSCCONL(OSCCON | 0x40)

    PPSUnLock;
    //RPOR2bits.RP5R = 16; //pwm out 2a
    RPOR11bits.RP23R = 3; //uart1 tx
    RPINR18bits.U1RXR = 25; //uart1 rx
    PPSLock;
    
#undef PPSUnLock
#undef PPSLock
}
