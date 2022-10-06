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


    // Boucle Principale
    while (1) {

        if (ADCIsConversionFinished() == 1) { //si la conversion est fini
            unsigned int * result = ADCGetResult(); //on mets result dans la meme emplacement memoire que ADCGetResult
            ADCClearConversionFinishedFlag(); //permet de remettre le flag à 0            
            float volts = ((float) result [0]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExtGauche = 34 / volts - 5;
            volts = ((float) result [1]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
            volts = ((float) result [2]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result [3]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            volts = ((float) result [4]) * 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExtDroit = 34 / volts - 5;

        }
    } // f i n main
}

unsigned char stateRobot;

void OperatingSystemLoop(void) {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;
        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;

        case STATE_AVANCE:
            PWMSetSpeedConsigne(20, MOTEUR_DROIT);
            PWMSetSpeedConsigne(20, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(30, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_PETIT_GAUCHE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_PETIT_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_PETIT_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(30, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_PETIT_DROIT:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_PETIT_DROIT_EN_COURS;
            break;
        case STATE_TOURNE_PETIT_DROIT_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}

unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode() {

    unsigned char positionObstacle = PAS_D_OBSTACLE;

    //Détermination de la position des obstacles en fonction des télémètres
    if (robotState.distanceTelemetreExtGauche < 15) //Obstacle à l'extreme droite
       positionObstacle = positionObstacle + 16; //attribue un 1 au 5eme bit de positionObstacle
    if (robotState.distanceTelemetreGauche < 18) //Obstacle à droite
       positionObstacle = positionObstacle + 8; //attribue un 1 au 4eme bit de positionObstacle
    if (robotState.distanceTelemetreCentre < 20) //Obstacle au centre
       positionObstacle = positionObstacle + 4; //attribue un 1 au 3eme bit de positionObstacle
    if (robotState.distanceTelemetreDroit < 18) //Obstacle à gauche
       positionObstacle = positionObstacle + 2; //attribue un 1 au 2eme bit de positionObstacle
    if (robotState.distanceTelemetreExtDroit < 15) //Obstacle à l'extreme gauche
       positionObstacle = positionObstacle + 8; //attribue un 1 au 1er bit de positionObstacle
       
    //Détermination de l?état à venir du robot
    //TABLE DE VERITE.exe
    switch (positionObstacle) {
        case 0b00000:
            nextStateRobot = STATE_AVANCE;
            break;
        case 0b00001:
            nextStateRobot = STATE_TOURNE_PETIT_GAUCHE;
            break;
        case 0b00010:
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b00011:
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b00100:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b00101:
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b00110:
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b00111:
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b01000:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b01001:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b01010:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b01011:
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b01100:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b01101:
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b01110:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b01111:
            nextStateRobot = STATE_TOURNE_GAUCHE;
            break;
        case 0b10000:
            nextStateRobot = STATE_TOURNE_PETIT_DROIT;
            break;
        case 0b10001:
            nextStateRobot = STATE_AVANCE;
            break;
        case 0b10010:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b10011:
            nextStateRobot = STATE_TOURNE_PETIT_GAUCHE;
            break;
        case 0b10100:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b10101:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b10110:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b10111:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;
        case 0b11000:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b11001:
            nextStateRobot = STATE_TOURNE_PETIT_DROIT;
            break;
        case 0b11010:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b11011:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b11100:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b11101:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
            break;
        case 0b11110:
            nextStateRobot = STATE_TOURNE_DROITE;
            break;
        case 0b11111:
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
            break;

        default:
            stateRobot = STATE_ATTENTE;
            break;

            //Si l?on n?est pas dans la transition de l?étape en cours
            if (nextStateRobot != stateRobot - 1) //si l'action n'est pas deja en cours
                stateRobot = nextStateRobot; // alors faire l'action
    }
}
