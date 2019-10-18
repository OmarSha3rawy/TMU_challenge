/*
 * TIMER.h
 *
 *  Created on: Sep 20, 2019
 *      Author: alx
 */

#ifndef TIMER_H_
#define TIMER_H_


#include "TIMER_cnfg.h"
//#include "Interrupts.h"


#define MAX_NUM_OF_TIMERS		(uint8)3
#define OK 1
#define NOK 0

uint8 TIMER_init(void);

uint8 TIMER_start(uint8 TIMER_ID);
uint8 TIMER_stop(uint8 TIMER_ID);

uint8 CHANGE_PWM(uint8 TIMER_ID,uint8 PWM_DUTYCYCLE_chA);

void TIMER0_OVF_setCallBack(void (*COPY_TIMER0_OVF_callBackPtr)(void));
void TIMER0_CMP_setCallBack(void (*COPY_TIMER0_CMP_callBackPtr)(void));
void TIMER1_OVF_setCallBack(void (*COPY_TIMER1_OVF_callBackPtr)(void));
void TIMER1_CMPA_setCallBack(void (*COPY_TIMER1_CMPA_callBackPtr)(void));
void TIMER1_CMPB_setCallBack(void (*COPY_TIMER1_CMPB_callBackPtr)(void));
void TIMER2_OVF_setCallBack(void (*COPY_TIMER2_OVF_callBackPtr)(void));
void TIMER2_CMP_setCallBack(void (*COPY_TIMER2_CMP_callBackPtr)(void));

#endif /* TIMER_H_ */
