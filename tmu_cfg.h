/*
 * tmu_cfg.h
 *
 * Created: 10/12/2019 7:57:55 PM
 *  Author: omara
 */ 


#ifndef TMU_CFG_H_
#define TMU_CFG_H_

#define MAXNUMBEROfTASKS 10
#include "std_types.h"

//typedef enum {motor0, motor1, seg0, seg1, seg2, seg3, led_flag, NUM_OF_PINS} pintype;

typedef enum {
	ONE_SHOT,
	PERIODIC,
	} periodicity;
	
typedef enum {
	FINISHED,
	RUNNING,
	} task_state;

typedef enum {
	TIMER0,
	TIMER1,
	TIMER2,
	} timer_id;

typedef struct
{
	void (*app)(void);
	periodicity per;
	uint8 freq;
	task_state state;
}buff;

extern buff buffer [MAXNUMBEROfTASKS];


#endif /* TMU_CFG_H_ */