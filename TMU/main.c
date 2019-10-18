/*
 * TMU.c
 *
 * Created: 10/12/2019 7:53:19 PM
 * Author : omara
 */ 

#include "REG_Lib.h"
#include "DIO.h"
#include "tmu.h"
#define F_CPU 8000000UL 

void processor_sleep();
void togg_led();
void togg_led2();



int main(void)
{
	DDRA = 0xff;

	//DDRC = 0xff; //for testing
	DIO_init();
	TMU_init(TIMER0, 1);
	TMU_start(togg_led,PERIODIC, (uint16)1000 );
	TMU_start(togg_led2,PERIODIC, (uint16)3000 );

    while (1) 
    {
		TMU_dispatcher();
		
	//	processor_sleep();
    }
}


void togg_led()
{
	uint8 result;
	DIO_read(led0, &result);
	if(result)
	{
		DIO_write(led0, LOW);
	}
	else
	{
		DIO_write(led0, HIGH);
	}
}
void togg_led2()
{
	uint8 result;
	DIO_read(led1, &result);
	if(result)
	{
		DIO_write(led1, LOW);
	}
	else
	{
		DIO_write(led1, HIGH);
	}
}

void processor_sleep()
{
	// stub to act as a sleep function
}