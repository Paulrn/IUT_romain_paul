#ifndef MAIN_H
#define	MAIN_H

// C o n fi g u r a ti o n de s �p a r am t re s du c hi p
#define FCY 40000000

//#define STATE_ATTENTE 0
//#define STATE_ATTENTE_EN_COURS 1
//#define STATE_AVANCE 2
//#define STATE_AVANCE_EN_COURS 3
//#define STATE_TOURNE_GAUCHE 4
//#define STATE_TOURNE_GAUCHE_EN_COURS 5
//#define STATE_TOURNE_DROITE 6
//#define STATE_TOURNE_DROITE_EN_COURS 7
//#define STATE_TOURNE_SUR_PLACE_GAUCHE 8
//#define STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS 9
//#define STATE_TOURNE_SUR_PLACE_DROITE 10
//#define STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS 11
//#define STATE_ARRET 12
//#define STATE_ARRET_EN_COURS 13
//#define STATE_RECULE 14
//#define STATE_RECULE_EN_COURS 15
//
//#define PAS_D_OBSTACLE 0
//#define OBSTACLE_A_GAUCHE 1
//#define OBSTACLE_A_DROITE 2
//#define OBSTACLE_EN_FACE 3

//void OperatingSystemLoop(void);
//void SetNextRobotStateInAutomaticMode();

void SendMessage(unsigned char* message, int length);
unsigned char CB_RX1_Get(void);
int CB_RX1_GetDataSize(void);

#endif	/* MAIN_H */

