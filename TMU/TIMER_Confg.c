/*
 * TIMER_Confg.c
 *
 *  Created on: Sep 20, 2019
 *      Author: alx
 */
#include "TIMER_cnfg.h"




				/******************TIMERS PARAMETERS***********************/
/*	uint8 TIMER_ID;
	uint8 TIMER_Mode;
	uint8 OC_ChA_Mode;
	uint8 OC_ChB_Mode;
	uint16 Normal_PreloadedValue;
	uint8 Prescaler;
	uint16 OCR_ChA;
	uint16 OCR_ChB;
	uint16 ICR;
	uint8 Interrupt_ChA;
	uint8 Interrupt_ChB;
	uint8 PWM_DUTYCYCLE;*/

TIMER_SConfg TIMER_cnfg_arr [NUM_OF_Timers] =
{
	{TIMER0,CTC_MODE,NA,NA,0,F_CPU_8,250,NA,NA,INTERRUPT,NA,NA,NA}
//	{TIMER1,PHASE_PWM_MODE,OC_INVERTING_PWM,OC_NONINVERTING_PWM,0,F_CPU_1024,0,0,NA_16,POOLING,POOLING,20,50}
};
