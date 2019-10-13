/*
 * TIMER_cnfg.h
 *
 *  Created on: Sep 20, 2019
 *      Author: Mahmoud Rezk
 */

#ifndef TIMER_CNFG_H_
#define TIMER_CNFG_H_

#include "STD_TYPES.h"
#include "REG_Lib.h"

typedef enum {T1,NUM_OF_Timers}Timers_Name;


									/*TIMERS IDs */

#define TIMER0 0
#define TIMER1 1
#define TIMER2 2


									/*TIMERS MODES */

#define NORMAL_MODE 0
#define CTC_MODE 1
#define FAST_PWM_MODE 2
#define PHASE_PWM_MODE 3
#define ICU_MODE 4

									/*TIMERS OC MODES */

#define OC_DISABLED 0
#define OC_TOGGLE 1
#define OC_CLEAR 2
#define OC_SET 3
#define OC_INVERTING_PWM 4
#define OC_NONINVERTING_PWM 5

									/*TIMERS PRESCALER */

#define NO_CLK_SRC 0
#define F_CPU_1 1
#define F_CPU_8 2
#define F_CPU_32 6
#define F_CPU_64 3
#define F_CPU_128 7
#define F_CPU_256 4
#define F_CPU_1024 5
#define EXT_CLK_FALLING 6
#define EXT_CLK_RISING 7

									/*INTERRUPTS STATE */
#define INTERRUPT 0
#define POOLING 1

#define INITIALIZED			1
#define NOT_INITIALIZED		0


#define NA 0xff
#define NA_16 0xffff

					/*******************************************************/


typedef struct
{
	uint8 TIMER_ID;
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
	float32 PWM_DUTYCYCLE_chA;
	float32 PWM_DUTYCYCLE_chB;
}TIMER_SConfg;


extern TIMER_SConfg TIMER_cnfg_arr [NUM_OF_Timers];







#endif /* TIMER_CNFG_H_ */
