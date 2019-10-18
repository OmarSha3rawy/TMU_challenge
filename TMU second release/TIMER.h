/*
 * TIMER.h
 *
 *  Created on: Oct 13, 2019
 *      Author: Mohammed Nassar
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "TIMER_cfg.h"

typedef struct
{
	uint8 TIMER_ID;
	uint8 WGM_mode;
	uint8 COM_mode;
	uint8 prescaler;
	uint8 interrupt;
	uint8 IS_init;
} TIMER_cnfg_t ;

#include "TIMER_Pbcfg.h"


extern void TIMER_init(uint8);
extern void TIMER_start(void);
extern void TIMER_stop(void);

extern void TIMER0_OVF_setCallBack(void (*COPY_TIMER0_OVF_callBackPtr)(void));
extern void TIMER0_CMP_setCallBack(void (*COPY_TIMER0_CMP_callBackPtr)(void));
extern void TIMER1_OVF_setCallBack(void (*COPY_TIMER1_OVF_callBackPtr)(void));
extern void TIMER1_CMPA_setCallBack(void (*COPY_TIMER1_CMPA_callBackPtr)(void));
extern void TIMER1_CMPB_setCallBack(void (*COPY_TIMER1_CMPB_callBackPtr)(void));
extern void TIMER2_OVF_setCallBack(void (*COPY_TIMER2_OVF_callBackPtr)(void));
extern void TIMER2_CMP_setCallBack(void (*COPY_TIMER2_CMP_callBackPtr)(void));

#endif /* TIMER_H_ */
