/******************************************************************************/
/*                                                                            */
/*      H   H      SSSS     W   W                                             */
/*      H   H     S         W   W                                             */
/*      HHHHH      SSS      W   W                                             */
/*      H   H         S     W W W                                             */
/*      H   H  *  SSSS   *   W W   *  snc                                     */
/*                                                                            */
/*      *********************************                                     */
/*                                                                            */
/*  HSW snc - Casalecchio di Reno (BO) ITALY                                  */
/*  ----------------------------------------                                  */
/*                                                                            */
/*  programma:  TAVOLO STIRO MNION (Derivato da TAVOLO COCCHI ma con 2 SCHEDE */
/*                                                                            */
/*  cliente:    ROTONDI GROUP - Settimo Milanese (MI)                         */
/*                                                                            */
/*  ver. 00.0:  05/05/2021  dalla da MiniEco V:17.4   D:11/04/2021            */
/*                                                                            */
/*  ver. att.:  04/08/2023  00.1                                              */
/*                                                                            */
/*  BY:         Maldus (Mattia MALDINI) & Matteo COSTA & Massimo ZANNA        */
/*                                                                            */
/*  NNB:        SENSO DEGLI INGRESSI: DRITTO                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*  micro:      MicroChip PIC PIC24FJ256GA705 - 32MHz                         */
/*              xxxK Flash - xxK Auxiliary Flash - xxxxxxb Ram                */
/*              xxxxxxxxb ?xxxxxxxx b ?            xxxxxxxb                   */
/*                                                                            */
/* ************************************************************************** */



//                                    12345678901234567890
const unsigned char versione_prg[] = "V:00.1 D:17/09/2023";

// NNB: OCCHIO ALLE TRADUZIONI NELLE LINGUE CON LE ACCENTATE !!!!!!!!!!!!!!!! //



/* ************************************************************************** */
/*                                                                            */
/*  PASSWORD                                                                  */
/*  --------                                                                  */
/*                                                                            */
/*  72346                                                                     */
/*                                                                            */
/*============================================================================*/
/*                                                                            */
/*                                                                            */
/*                                                                            */
/* ************************************************************************** */



/******************************************************************************/
/*                                                                            */
/*  rev.:       05/05/2021 (00.0)                                             */
/*                                                                            */
/*      - versione di partenza dal TAVOLO COCCHI su una scheda (ORA 2)        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*  rev.:       04/08/2023 (00.1)                                             */
/*                                                                            */
/*      - prima versione con interfaccia quasi completa e gestione completa   */
/*        di tutte le periferiche presenti sulla scheda                       */
/*                                                                            */
/******************************************************************************/



#include "peripherals/system.h"
#include "peripherals/relay.h"
#include "peripherals/rs485_display.h"
#include "peripherals/ptc.h"
#include "peripherals/phase_cut.h"
#include "controller/controller.h"
#include "peripherals/input.h"
#include "peripherals/level.h"
#include "peripherals/timer.h"
#include "services/system_time.h"


int main(void) {
    timestamp_t input_ts = 0;
    timestamp_t adc_ts = 0;

    system_init();
    relay_init();
    input_init();
    timer_init();
    level_init();
    rs485_display_init();
    phase_cut_init();
    
    controller_init();
    
    for (;;)
    {
        if (is_expired(input_ts, get_millis(), 10))
        {
            if (input_take_reading())
            {
                //Inputs changed
            }
            
            input_ts = get_millis();
        }
        
        if (is_expired(adc_ts, get_millis(), 100))
        {
            level_take_readings();
            ptc_take_readings();
           
            adc_ts = get_millis();
        }
        
        rs485_display_manage();
        controller_manage();
        Idle();
    }
    return 0;
}
