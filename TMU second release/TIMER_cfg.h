/*
 * TIMER_cfg.h
 *
 *  Created on: Oct 13, 2019
 *      Author: Mohammed
 */

#ifndef TIMER_CFG_H_
#define TIMER_CFG_H_

#include "std_types.h"


/* TIMERS IDs */

#define TIMER0 0
#define TIMER1 1
#define TIMER2 2

/* WGM MODE */
#define NORMAL_MODE        0
#define CTC_MODE           1

/* COM MODE */
#define NORMAL  0
#define TOGGLE  1
#define CLEAR   2
#define SET     3

/* PRESCALER */
#define PRESCALER0             0
#define PRESCALER8             1
#define PRESCALER64            2
#define PRESCALER256           3
#define PRESCALER1024          4
#define EXTERNAL_CLK_RISING    5
#define EXTERNAL_CLK_FALLING   6

/* PRESCALER TIMER 2 */
#define PRESCALER32             7
#define PRESCALER128            8

/* INTERRUPT */
#define INTERRUPT     1
#define POOLING       0

/* INITIALIZATION	 */
#define INITIALIZED			1
#define NOT_INITIALIZED		0


#endif /* TIMER_CFG_H_ */
