#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "adc.h"
#include "Robot.h"
#include "main.h"

unsigned int result;

int main(void) {
    // Initialisation de l'oscillateur
    InitOscillator();
    // Configuration des éntres sorties
    InitIO();
    // Initialisation des Timers
    InitTimer23();
    InitTimer1();
    InitTimer4();
    // Initialisation de la PWM
    InitPWM();

    InitADC1();

    //LED_BLANCHE = 1;
    //LED_BLEUE = 1;
    //LED_ORANGE = 1;

    // Boucle Principale
    while (1) {

        if (ADCIsConversionFinished() == 1) { //si la conversion est fini
            unsigned int * result = ADCGetResult(); //on mets result dans la meme emplacement memoire que ADCGetResult
            ADCClearConversionFinishedFlag(); //permet de remettre le flag à 0     
        }

        if (robotState.distanceTelemetreDroit >= 30) {
            LED_BLANCHE = 1;
        } else {
            LED_BLANCHE = 0;
        }
//        if (robotState.distanceTelemetreCentre >= 30) {
//            LED_BLEUE = 1;
//        } else {
//            LED_BLEUE = 0;
//        }
//        if (robotState.distanceTelemetreGauche >= 30) {
//            LED_ORANGE = 1;
//        } else {
//            LED_ORANGE = 0;
//        }


        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result [2]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            volts = ((float) result [1]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result [0]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
        }

    } // f i n main
}