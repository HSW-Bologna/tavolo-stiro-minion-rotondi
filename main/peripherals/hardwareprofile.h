#ifndef HARDWAREPROFILE_H_INCLUDED
#define HARDWAREPROFILE_H_INCLUDED

#include <xc.h>


#define HAP_RUN_TRIS TRISAbits.TRISA4
#define HAP_RUN_LAT  LATAbits.LATA4


// OUTPUT ----------------------------------------------------------------------
//
#define HAP_REL1_TRIS  TRISAbits.TRISA12         // OUT RL
#define HAP_REL1_LAT   LATAbits.LATA12
#define HAP_REL2_TRIS  TRISAbits.TRISA0
#define HAP_REL2_LAT   LATAbits.LATA0
#define HAP_REL3_TRIS  TRISAbits.TRISA1
#define HAP_REL3_LAT   LATAbits.LATA1
#define HAP_REL4_TRIS  TRISBbits.TRISB0
#define HAP_REL4_LAT   LATBbits.LATB0
#define HAP_REL5_TRIS  TRISBbits.TRISB1
#define HAP_REL5_LAT   LATBbits.LATB1
#define HAP_REL6_TRIS  TRISBbits.TRISB2
#define HAP_REL6_LAT   LATBbits.LATB2
#define HAP_REL7_TRIS  TRISBbits.TRISB3
#define HAP_REL7_LAT   LATBbits.LATB3
#define HAP_AUX_M_TRIS TRISAbits.TRISA13
#define HAP_AUX_M_LAT  LATAbits.LATA13
#define HAP_TAGLIOLA_A_TRIS TRISAbits.TRISA8
#define HAP_TAGLIOLA_A_LAT  LATAbits.LATA8
#define HAP_TAGLIOLA_B_TRIS TRISAbits.TRISA3
#define HAP_TAGLIOLA_B_LAT  LATAbits.LATA3

#define HAP_POMPA_TRIS TRISCbits.TRISC3         // TRIAC POMPA ON/OFF
#define HAP_POMPA_LAT  LATCbits.LATC3

// ZERO CROSS - VENTOLE --------------------------------------------------------
//
#define HAP_INT0_TRIS TRISBbits.TRISB7          // IN
#define HAP_INT0_PORT PORTBbits.RB7

// OUTPUT VENTOLE --------------------------------------------------------------
//
#define HAP_ASP_TRIS  TRISCbits.TRISC5          // TRIAC VEL. VENTOLA ASP
#define HAP_ASP_LAT   LATCbits.LATC5
#define HAP_SOFF_TRIS TRISCbits.TRISC4          // TRIAC VEL. VENTOLA SOF
#define HAP_SOFF_LAT  LATCbits.LATC4



// INPUT -----------------------------------------------------------------------
//
#define HAP_IN1_TRIS TRISAbits.TRISA10          // INPUT 1 # FC DX - ASPIRAZIONE
#define HAP_IN1_PORT PORTAbits.RA10
#define HAP_IN2_TRIS TRISAbits.TRISA7           // INPUT 2 # FC SX - ASPIRAZIONE
#define HAP_IN2_PORT PORTAbits.RA7
#define HAP_IN3_TRIS TRISBbits.TRISB14          // INPUT 2 # PEDALE - SOFFIO
#define HAP_IN3_PORT PORTBbits.RB14
#define HAP_IN4_TRIS TRISBbits.TRISB15          // INPUT 4 # LIVELLO RECUPERATORE PIENO
#define HAP_IN4_PORT PORTBbits.RB15

#define HAP_VAP_TRIS  TRISAbits.TRISA9          // INPUT 5 # FERRO 220 VAC
#define HAP_VAP_PORT  PORTAbits.RA9



// LIVELLI H2O -----------------------------------------------------------------
//
#define HAP_PWM_LIVELLI_TRIS TRISCbits.TRISC0   // OUT SEGNALE PWM LIV1 e LIV2

#define HAP_LIVELLO1_H2O_AN 12                  // IN LIV1 # LIVELLO H2O MAX SERBATOIO VAPORE
#define HAP_LIVELLO2_H2O_AN 11                  // IN LIV2



// TEMPERATURE -----------------------------------------------------------------
//
#define HAP_TEMP1_AN 7                          // PTC 1 # MISURA TEMPERATURA PIANO
#define HAP_TEMP2_AN 8                          // PTC 2 # MISURA TEMPERATURA BRACCIOLO



// TRASMISSIONE - RS485 # MODBUS -----------------------------------------------
//
#define HAP_TX_TRIS  TRISCbits.TRISC7
#define HAP_RX_TRIS  TRISCbits.TRISC9
#define HAP_DIR_TRIS TRISCbits.TRISC8
#define HAP_DIR_LAT  LATCbits.LATC8

#define HAP_TX_1_TRIS  TRISBbits.TRISB8
#define HAP_RX_1_TRIS  TRISCbits.TRISC6
#define HAP_DIR_1_TRIS TRISBbits.TRISB9
#define HAP_DIR_1_LAT  LATBbits.LATB9

#endif
