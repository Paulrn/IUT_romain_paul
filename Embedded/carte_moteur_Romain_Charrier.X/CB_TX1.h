/* 
 * File:   CB_TX1.h
 * Author: TP-EO-5
 *
 * Created on 9 décembre 2022, 14:06
 */

#ifndef CB_TX1_H
#define	CB_TX1_H

void SendMessage(unsigned char* message, int length);
void CB_TX1_Add(unsigned char value);
void SendOne();
unsigned char CB_TX1_Get(void);
unsigned char CB_TX1_IsTranmitting(void);
int CB_TX1_GetDataSize(void);
int CB_TX1_GetRemainingSize(void);

#endif	/* CB_TX1_H */

