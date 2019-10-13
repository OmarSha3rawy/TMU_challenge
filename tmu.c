/*
 * tmu.c
 *
 * Created: 10/12/2019 7:57:15 PM
 *  Author: omara
 */ 

#include "tmu.h"

buff buffer [MAXNUMBEROfTASKS];

flag_state tick_flag = DISABLED;
uint32 ov_counter = 0;
uint8 ov_counter_assistant = 0;

uint8 res;

void overflow_count()
{
	ov_counter_assistant++;
	if( (ov_counter_assistant) == 4 )
	{
		ov_counter++;
		ov_counter_assistant = 0;
		tick_flag = ACTIVE;
	}
}

uint8 TMU_init(uint8 id, uint8 resolution)
{
	res = resolution;
	switch(id)
	{
		case TIMER1:
			TIMER_cnfg_arr[0].TIMER_ID = TIMER1;
			TIMER1_CMPA_setCallBack(overflow_count);
		break;
		
		case TIMER2:
			TIMER_cnfg_arr[0].TIMER_ID = TIMER2;
			TIMER2_CMP_setCallBack(overflow_count);
		break;
		
		default:
			TIMER_cnfg_arr[0].TIMER_ID = TIMER0;
			TIMER0_CMP_setCallBack(overflow_count);
		break;
	}
	TIMER_init();
	return OK;
}

uint8 TMU_start(void (*application)(void), periodicity per, uint32 freq)
{
	uint8 i;
	for(i = 0; i < MAXNUMBEROfTASKS; i++)
	{
		if(buffer[i].app_ptr == NULL)
		{
			buffer[i].app_ptr = application;
			buffer[i].per = per;
			buffer[i].freq = freq;
			buffer[i].state = RUNNING ;
			buffer[i].start = ov_counter;
			return OK;
		}
	}
	return NOK;
}

uint8 TMU_stopTimer(void (*application)(void))
{
	uint8 i;
	for(i = 0; i < MAXNUMBEROfTASKS; i++)
	{
		if(buffer[i].app_ptr == application)
		{
			buffer[i].app_ptr = NULL;	
			buffer[i].state = FINISHED;
			return OK;
		}
	}
	return NOK;
}

uint8 TMU_dispatcher()
{
	uint8 i;
	if (tick_flag == ACTIVE)
	{
		for(i = 0; i < MAXNUMBEROfTASKS; i++)
		{
			//if( ( ((ov_counter - buffer[i].start) % buffer[i].freq == 0) || ((buffer[i].start - ov_counter) % buffer[i].freq == 0) ) && (buffer[i].state == RUNNING) )
			if( ( ((ov_counter - buffer[i].start) % buffer[i].freq == 0) ) && (buffer[i].state == RUNNING) )

			{
				if(buffer[i].per == ONE_SHOT)
				{
					(buffer[i].app_ptr)();
					buffer[i].app_ptr = NULL;
					buffer[i].state = FINISHED;					
				}
				else
				{
					(buffer[i].app_ptr)();
				}
			}
		}
		tick_flag = DISABLED;
	}
	return OK;
}

