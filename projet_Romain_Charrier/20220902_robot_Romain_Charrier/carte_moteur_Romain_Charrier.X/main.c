#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "adc.h"

unsigned int result;

int main(void) {
    // Initialisation de l'oscillateur
    InitOscillator();
    // Configuration des éntres sorties
    InitIO();
    // Initialisation des Timers
    InitTimer23();
    InitTimer1();
    // Initialisation de la PWM
    InitPWM();

    InitADC1();
            
    LED_BLANCHE = 1;
    LED_BLEUE = 1;
    LED_ORANGE = 1;

    // Boucle Principale
    while (1) {

        if (ADCIsConversionFinished() == 1) { //si la conversion est fini
            unsigned int * result = ADCGetResult(); //on mets result dans la meme emplacement memoire que ADCGetResult
            ADCClearConversionFinishedFlag(); //permet de remettre le flag à 0     
        }

    } // f i n main
}