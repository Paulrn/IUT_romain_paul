#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "adc.h"

int main(void) {
    // I n i t i a l i s a t i o n de l ? o s c i l l a t e u r
    InitOscillator();
    // Configuration des éntres sorties
    InitIO();

    InitTimer23();
    InitTimer1();

    InitPWM();
    
    LED_BLANCHE = 1;
    LED_BLEUE = 1;
    LED_ORANGE = 1;

    // Boucle P r i n c i p a l e
    while (1) {

    } // f i n main
}