/*
 * TIMER_Pbcfg.c
 *
 *  Created on: Oct 13, 2019
 *      Author: Mohammed
 */


#include "TIMER_Pbcfg.h"



//TIMER_cnfg_t TIMER = {TIMER_ID,WGM_mode,COM_mode,Prescaler,interrupt,INITIALIZATION};

TIMER_cnfg_t TIMER = {TIMER0,CTC_MODE,NORMAL,PRESCALER8,INTERRUPT,NOT_INITIALIZED};

