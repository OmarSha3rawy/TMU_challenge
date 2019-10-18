/*
 * TIMER.c
 *
 *  Created on: Sep 20, 2019
 *      Author: alx
 */
#include "TIMER.h"
#include "Interrupts.h"



static uint8 Init_check[NUM_OF_Timers];

static volatile void (*g_TIMER0_OVF_callBackPtr)(void);
static volatile void (*g_TIMER0_CMP_callBackPtr)(void);
static volatile void (*g_TIMER1_OVF_callBackPtr)(void);
static volatile void (*g_TIMER1_CMPA_callBackPtr)(void);
static volatile void (*g_TIMER1_CMPB_callBackPtr)(void);
static volatile void (*g_TIMER2_OVF_callBackPtr)(void);
static volatile void (*g_TIMER2_CMP_callBackPtr)(void);

/*********************************************************
 *					Timer0 COMP Interrupt				 *
 *********************************************************/
void __vector_10 (void)
{
	if(g_TIMER0_CMP_callBackPtr != NULL)
	{
		g_TIMER0_CMP_callBackPtr();
	}
}

/*********************************************************
 *					Timer0 OVF Interrupt				 *
 *********************************************************/
void __vector_11 (void)
{
	if(g_TIMER0_OVF_callBackPtr != NULL)
	{
		g_TIMER0_OVF_callBackPtr();
	}
}

/*********************************************************
 *					Timer1 OVF Interrupt				 *
 *********************************************************/
void __vector_9 (void)
{
	if(g_TIMER1_OVF_callBackPtr != NULL)
	{
		g_TIMER1_OVF_callBackPtr();
	}

}

/*********************************************************
 *					Timer1 COMPB Interrupt				 *
 *********************************************************/
void __vector_8 (void)
{
	if(g_TIMER1_CMPB_callBackPtr != NULL)
	{
		g_TIMER1_CMPB_callBackPtr();
	}

}

/*********************************************************
 *					Timer1 COMPA Interrupt				 *
 *********************************************************/
void __vector_7 (void)
{
	if(g_TIMER1_CMPA_callBackPtr != NULL)
	{
		g_TIMER1_CMPA_callBackPtr();
	}

}

/*********************************************************
 *					Timer1 ICU Interrupt				 *
 *********************************************************/
void __vector_6 (void)
{

}

/*********************************************************
 *					Timer2 OVF Interrupt				 *
 *********************************************************/
void __vector_5 (void)
{
	if(g_TIMER2_OVF_callBackPtr != NULL)
	{
		g_TIMER2_OVF_callBackPtr();
	}
}

/*********************************************************
 *					Timer2 CMP Interrupt				 *
 *********************************************************/
void __vector_4 (void)
{
	if(g_TIMER2_CMP_callBackPtr != NULL)
	{
		g_TIMER2_CMP_callBackPtr();
	}
}



uint8 TIMER_init(void)
{
	uint8 retval=OK;
	uint8 Loop_index=0;
	if(NUM_OF_Timers<=MAX_NUM_OF_TIMERS)
	{
		for(Loop_index=0;Loop_index<NUM_OF_Timers;Loop_index++)
		{
			Init_check[Loop_index]=INITIALIZED;
			switch(TIMER_cnfg_arr[Loop_index].TIMER_ID)
			{
			case TIMER0:
				/*	uint8 TIMER_Mode  				done;
				 * 	uint8 OC_ChA_Mode				done;
					uint16 Normal_PreloadedValue  	done;
					uint8 Prescaler					done;
					uint8 Interrupt_ChA				done with Q;
					uint8 PWM_DUTYCYCLE;
					OCR chA							done*/


				switch(TIMER_cnfg_arr[Loop_index].TIMER_Mode)
				{
				/****************************************************************
				 * 						  Timer0 Normal Mode					*
				 ****************************************************************/
				case NORMAL_MODE:
					/*
					 * 1- FOC0 bit = 1 for non-PWM mode
					 * 2- WGM00 and WGM01 bits = 0 for Normal Mode
					 * 3- COM00 and COM01 bits = 0 to disconnect the OC pin as it's not used in Normal mode
					 * 4- CS00:1 bits = 0 for no clk source at the beginning of the initialization (it will be defined later with the prescaler)
					 */
					TCCR0 = 0x80;

					/*
					 * Setting Timer Preloaded value
					 */
					if((TIMER_cnfg_arr[Loop_index].Normal_PreloadedValue >= 0)\
							&& (TIMER_cnfg_arr[Loop_index].Normal_PreloadedValue < 0xFF))
					{
						TCNT0 = TIMER_cnfg_arr[Loop_index].Normal_PreloadedValue;
					}
					else if(TIMER_cnfg_arr[Loop_index].Normal_PreloadedValue==NA)
					{

					}
					else
					{
						Init_check[Loop_index]=NOT_INITIALIZED;
						retval = NOK;
					}



					/*
					 * Setting weather the timer operates in Interrupt or pooling mode
					 */
					if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == INTERRUPT)
					{
						TIMSK |= 0x01;
						SREG |= (1<<SREG_I);
					}
					else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == POOLING)
					{
						TIMSK &= ~0x01;
					}
					else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == NA)
					{
						TIMSK &= ~0x01;
					}
					else
					{
						Init_check[Loop_index]=NOT_INITIALIZED;
						retval = NOK;
					}


					/*
					 * Setting Timer0 Prescaler
					 */
					switch(TIMER_cnfg_arr[Loop_index].Prescaler)
					{
					case NO_CLK_SRC:
					case F_CPU_1:
					case F_CPU_8:
					case F_CPU_64:
					case F_CPU_256:
					case F_CPU_1024:
					case EXT_CLK_FALLING:
					case EXT_CLK_RISING:

						TCCR0 |= TIMER_cnfg_arr[Loop_index].Prescaler;
						break;
					default:
						Init_check[Loop_index]=NOT_INITIALIZED;
						retval = NOK;
					}

					break;

					/****************************************************************
					 * 						  Timer0 CTC Mode						*
					 ****************************************************************/

					case CTC_MODE:
						/*
						 * 1- FOC0 bit = 1 for non-PWM mode
						 * 2- WGM00 bit = 0 and WGM01 bit = 1 for CTC Mode
						 * 3- COM00 and COM01 bits = 0 to disconnect the OC pin at the beginning of the initialization
						 * 4- CS00:1 bits = 0 for no clk source at the beginning of the initialization (it will be defined later with the prescaler)
						 */
						TCCR0 = 0x88;
						/*
						 * Setting the Compare value
						 */
						if(TIMER_cnfg_arr[Loop_index].OCR_ChA <= 0xFF)
						{
							OCR0 = TIMER_cnfg_arr[Loop_index].OCR_ChA;
						}
						else
						{
							Init_check[Loop_index]=NOT_INITIALIZED;
							retval = NOK;
						}

						/*
						 * Setting the OC pin Mode
						 */
						switch(TIMER_cnfg_arr[Loop_index].OC_ChA_Mode)
						{
						case NA:
							break;
						case OC_DISABLED:
							break;

						case OC_TOGGLE:
							TCCR0 |= (1<<TCCR0_COM00);
							break;

						case OC_CLEAR:
							TCCR0 |= (1<<TCCR0_COM01);
							break;

						case OC_SET:
							TCCR0 |= (3<<TCCR0_COM00);
							break;

						default:
							Init_check[Loop_index]=NOT_INITIALIZED;
							retval = NOK;
						}

						/*
						 * Setting weather the timer operates in Interrupt or pooling mode
						 */
						if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == INTERRUPT)
						{
							TIMSK |= 0x02;
							SREG |= (1<<SREG_I);
						}
						else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == POOLING)
						{
							TIMSK &= ~0x02;
						}
						else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA==NA)
						{
							TIMSK &= ~0x02;		
						}
						else
						{
							Init_check[Loop_index]=NOT_INITIALIZED;
							retval = NOK;
						}


						/*
						 * Setting Timer0 Prescaler
						 */
						switch(TIMER_cnfg_arr[Loop_index].Prescaler)
						{
						case NO_CLK_SRC:
						case F_CPU_1:
						case F_CPU_8:
						case F_CPU_64:
						case F_CPU_256:
						case F_CPU_1024:
						case EXT_CLK_FALLING:
						case EXT_CLK_RISING:

							TCCR0 |= TIMER_cnfg_arr[Loop_index].Prescaler;
							break;
						default:
							Init_check[Loop_index]=NOT_INITIALIZED;
							retval = NOK;
						}

						break;

						/****************************************************************
						 * 						  Timer0 FAST PWM Mode					*
						 ****************************************************************/

						case FAST_PWM_MODE:
							/*
							 * 1- FOC0 bit = 0 for PWM mode
							 * 2- WGM00 bit = 1 and WGM01 bit = 1 for Fast PWM Mode
							 * 3- COM00 and COM01 bits = 0 to disconnect the OC pin at the beginning of the initialization
							 * 4- CS00:1 bits = 0 for no clk source at the beginning of the initialization (it will be defined later with the prescaler)
							 */
							TCCR0 = 0x48;

							/*
							 * Setting the Duty Cycle
							 */
							if(TIMER_cnfg_arr[Loop_index].PWM_DUTYCYCLE_chA <= 100)
							{

								OCR0 = (uint8)((255 * TIMER_cnfg_arr[Loop_index].PWM_DUTYCYCLE_chA)/100);
							}
							else
							{
								Init_check[Loop_index]=NOT_INITIALIZED;
								retval = NOK;
							}

							/*
							 * Setting OC pin Mode
							 */
							switch(TIMER_cnfg_arr[Loop_index].OC_ChA_Mode)
							{
							case OC_DISABLED:
								break;

							case OC_NONINVERTING_PWM:
								TCCR0 |= (1<<TCCR0_COM01);
								break;

							case OC_INVERTING_PWM:
								TCCR0 |= (3<<TCCR0_COM00);
								break;

							case NA:
								break;

							default:
								Init_check[Loop_index]=NOT_INITIALIZED;
								retval = NOK;
							}

							/*
							 * Setting weather the timer operates in Interrupt or pooling mode
							 */
							if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == INTERRUPT)
							{
								TIMSK |= 0x02;
								SREG |= (1<<SREG_I);
							}
							else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == POOLING)
							{
								TIMSK &= ~0x02;
							}
							else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == NA)
							{
								TIMSK &= ~0x02;

							}
							else
							{
								Init_check[Loop_index]=NOT_INITIALIZED;
								retval = NOK;
							}

							/*
							 * Setting Timer0 Prescaler
							 */
							switch(TIMER_cnfg_arr[Loop_index].Prescaler)
							{
							case NO_CLK_SRC:
							case F_CPU_1:
							case F_CPU_8:
							case F_CPU_64:
							case F_CPU_256:
							case F_CPU_1024:
							case EXT_CLK_FALLING:
							case EXT_CLK_RISING:

								TCCR0 |= TIMER_cnfg_arr[Loop_index].Prescaler;
								break;
							default:
								Init_check[Loop_index]=NOT_INITIALIZED;
								retval = NOK;
							}


							break;

							/****************************************************************
							 * 					Timer0 PWM Phase Correct Mode				*
							 ****************************************************************/
							case PHASE_PWM_MODE:
								/*
								 * 1- FOC0 bit = 0 for PWM mode
								 * 2- WGM00 bit = 1 and WGM01 bit = 0 for PWM Phase Correct Mode
								 * 3- COM00 and COM01 bits = 0 to disconnect the OC pin at the beginning of the initialization
								 * 4- CS00:1 bits = 0 for no clk source at the beginning of the initialization (it will be defined later with the prescaler)
								 */
								TCCR0 = 0x40;


								/*
								 * Setting the Duty Cycle
								 */
								if(TIMER_cnfg_arr[Loop_index].PWM_DUTYCYCLE_chA <= 100)
								{

									OCR0 = (uint8)((255 * TIMER_cnfg_arr[Loop_index].PWM_DUTYCYCLE_chA)/100);
								}
								else
								{
									Init_check[Loop_index]=NOT_INITIALIZED;
									retval = NOK;
								}

								/*
								 * Setting the OC pin Mode
								 */
								switch(TIMER_cnfg_arr[Loop_index].OC_ChA_Mode)
								{
								case NA:
									break;
								case OC_DISABLED:
									break;

								case OC_NONINVERTING_PWM:
									TCCR0 |= (1<<TCCR0_COM01);
									break;

								case OC_INVERTING_PWM:
									TCCR0 |= (3<<TCCR0_COM00);
									break;

								default:
									Init_check[Loop_index]=NOT_INITIALIZED;
									retval = NOK;
								}

								/*
								 * Setting weather the timer operates in Interrupt or pooling mode
								 */
								if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == INTERRUPT)
								{
									TIMSK |= 0x02;
									SREG |= (1<<SREG_I);
								}
								else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == POOLING)
								{
									TIMSK &= ~0x02;
								}
								else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == NA)
								{
									TIMSK &= ~0x02;
								}
								else
								{
									Init_check[Loop_index]=NOT_INITIALIZED;
									retval = NOK;
								}

								/*
								 * Setting Timer0 Prescaler
								 */
								switch(TIMER_cnfg_arr[Loop_index].Prescaler)
								{
								case NO_CLK_SRC:
								case F_CPU_1:
								case F_CPU_8:
								case F_CPU_64:
								case F_CPU_256:
								case F_CPU_1024:
								case EXT_CLK_FALLING:
								case EXT_CLK_RISING:

									TCCR0 |= TIMER_cnfg_arr[Loop_index].Prescaler;
									break;
								default:
									Init_check[Loop_index]=NOT_INITIALIZED;
									retval = NOK;
								}


								break;


								default:
									Init_check[Loop_index]=NOT_INITIALIZED;
									retval = NOK;
				}

				break;


				/********************************************************************************
				 * 									Timer1										*
				 ********************************************************************************/
				case TIMER1:

					/*	uint8 TIMER_Mode  				;
					 * 	uint8 OC_ChA_Mode				;
					 * 	uint8 OC_ChB_Mode				;
									uint16 Normal_PreloadedValue  	;
									uint8 Prescaler					;
									uint8 Interrupt_ChA				;
									uint8 Interrupt_ChB 			;
									uint8 PWM_DUTYCYCLE             ;
									uint16 OCR chA	   				;
									uint16 OCR chB					;
									uint16 ICR						;
					 */
					switch(TIMER_cnfg_arr[Loop_index].TIMER_Mode)
					{
					/****************************************************************
					 * 						  Timer1 Normal Mode					*
					 ****************************************************************/
					case NORMAL_MODE:
						/*
						 * 1- FOC0 bit = 1 for non-PWM mode
						 * 2- WGM10, WGM11,WGM12 and WGM13 bits = 0 for Normal Mode
						 * 3- Check for prescaler and set CS12, CS11, CS10 values
						 */
						TCCR1A = 0b00000011;
						switch(TIMER_cnfg_arr[Loop_index].Prescaler)
						{
						case NO_CLK_SRC:
							TCCR1B=0x00;
							break;
						case F_CPU_1:
							TCCR1B=0x01;
							break;
						case F_CPU_8:
							TCCR1B=0x02;
							break;
						case F_CPU_64:
							TCCR1B=0x03;
							break;
						case F_CPU_256:
							TCCR1B=0x04;
							break;
						case F_CPU_1024:
							TCCR1B=0x05;
							break;
						case EXT_CLK_FALLING:
							TCCR1B=0x06;
							break;
						case EXT_CLK_RISING:
							TCCR1B=0x07;
							break;
						default:
							Init_check[Loop_index]=NOT_INITIALIZED;
							retval = NOK;
							break;
						}

						/*
						 * 4- Setting Timer preloaded value
						 */
						if((TIMER_cnfg_arr[Loop_index].Normal_PreloadedValue >= 0)\
								&& (TIMER_cnfg_arr[Loop_index].Normal_PreloadedValue < 0xffff ))
						{
							TCNT1L = TIMER_cnfg_arr[Loop_index].Normal_PreloadedValue;
						}
						else if(TIMER_cnfg_arr[Loop_index].Normal_PreloadedValue >= 0xffff )
						{
							TCNT1L = 0x00;
						}
						else
						{
							Init_check[Loop_index]=NOT_INITIALIZED;
							retval = NOK;
						}
						/*
						 * 5- Setting weather the timer operates in Interrupt or pooling mode
						 */
						if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == INTERRUPT)
						{
							TIMSK |= 0x04;
							SREG |= (1u<<SREG_I);

						}
						else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == POOLING)
						{
							TIMSK &= ~0x04;
						}
						else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == NA)
						{
							TIMSK &= ~0x04;
						}
						else
						{
							Init_check[Loop_index]=NOT_INITIALIZED;
							retval = NOK;
						}
						break;

						/****************************************************************
						 * 						  Timer1 CTC Mode						*
						 ****************************************************************/
						case CTC_MODE:
							/*
							 * 1- FOC1A bit = 1 and FOC1B= 1 for non-PWM mode
							 * 2- WGM10 bit = 0 , WGM11= 0 ,WGM12= 1 and WGM13 bit = 0 for CTC Mode
							 * 3  Setting the OC pin Mode
							 */
							TCCR1A=0b00001100;

							/*
							 * Setting the OC pin Mode for Channel A
							 */
							switch(TIMER_cnfg_arr[Loop_index].OC_ChA_Mode)
							{
							case OC_DISABLED:
								break;
							case NA:
								break;
							case OC_TOGGLE:
								TCCR1A |= (1u<<TCCR1A_COM1A0);
								break;
							case OC_CLEAR:
								TCCR1A |= (1u<<TCCR1A_COM1A1);
								break;
							case OC_SET:
								TCCR1A |= (3u<<TCCR1A_COM1A0);
								break;
							default:
								Init_check[Loop_index]=NOT_INITIALIZED;
								retval = NOK;
								break;
							}
							/*
							 * Setting the OC pin Mode for Channel B
							 */
							switch(TIMER_cnfg_arr[Loop_index].OC_ChB_Mode)
							{
							case OC_DISABLED:
								break;
							case NA:
								break;							
							case OC_TOGGLE:
								TCCR1A |= (1u<<TCCR1A_COM1B0);
								break;
							case OC_CLEAR:
								TCCR1A |= (1u<<TCCR1A_COM1B1);
								break;
							case OC_SET:
								TCCR1A |= (3u<<TCCR1A_COM1B0);
								break;
							default:
								Init_check[Loop_index]=NOT_INITIALIZED;
								retval = NOK;
								break;
							}
							/*
							 * Setting Timer1 Prescaler
							 */
							switch(TIMER_cnfg_arr[Loop_index].Prescaler)
							{
							case NO_CLK_SRC:
								TCCR1B=0b00001000;
								break;
							case F_CPU_1:
								TCCR1B=0b00001001;
								break;
							case F_CPU_8:
								TCCR1B=0b00001010;
								break;
							case F_CPU_64:
								TCCR1B=0b00001011;
								break;
							case F_CPU_256:
								TCCR1B=0b00001100;
								break;
							case F_CPU_1024:
								TCCR1B=0b00001101;
								break;
							case EXT_CLK_FALLING:
								TCCR1B=0b00001110;
								break;
							case EXT_CLK_RISING:
								TCCR1B=0b00001111;
								break;
							default:
								Init_check[Loop_index]=NOT_INITIALIZED;
								retval = NOK;
								break;
							}
							/*
							 * Setting the Compare value of Timer1 Channel A
							 */
							if(TIMER_cnfg_arr[Loop_index].OCR_ChA < 0xFFFF)
							{
								OCR1AL = TIMER_cnfg_arr[Loop_index].OCR_ChA;
							}
							else if(TIMER_cnfg_arr[Loop_index].OCR_ChA== NA_16)
							{

							}
							else
							{
								Init_check[Loop_index]=NOT_INITIALIZED;
								retval = NOK;
							}

							/*
							 * Setting the Compare value of Timer1 Channel A
							 */
							if(TIMER_cnfg_arr[Loop_index].OCR_ChB < 0xFFFF)
							{
								OCR1BL = TIMER_cnfg_arr[Loop_index].OCR_ChB;
							}
							else if(TIMER_cnfg_arr[Loop_index].OCR_ChB== NA_16)
							{

							}
							else
							{
								Init_check[Loop_index]=NOT_INITIALIZED;
								retval = NOK;
							}

							/*
							 * Setting weather the timer1 CHA operates in Interrupt or pooling mode
							 */
							if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == INTERRUPT)
							{
								TIMSK |= 0x10;
								SREG |= (1u<<SREG_I);

							}
							else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == POOLING)
							{
								TIMSK &= ~0x10;
							}
							else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == NA)
							{
								TIMSK &= ~0x10;
							}
							else
							{
								Init_check[Loop_index]=NOT_INITIALIZED;
								retval = NOK;
							}
							/*
							 * Setting weather the timer1 CHB operates in Interrupt or pooling mode
							 */
							if(TIMER_cnfg_arr[Loop_index].Interrupt_ChB == INTERRUPT)
							{
								TIMSK |= 0x08;
								SREG |= (1u<<SREG_I);
							}
							else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChB == POOLING)
							{
								TIMSK &= ~0x08;
							}
							else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChB == NA)
							{
								TIMSK &= ~0x08;
							}
							else
							{
								Init_check[Loop_index]=NOT_INITIALIZED;
								retval = NOK;
							}
							break;


							/****************************************************************
							 * 						  Timer1 FAST PWM Mode					*
							 ****************************************************************/
							/*
							 * 1- FOC1A bit = 0 and FOC1B= 0 for non-PWM mode
							 * 2- WGM10 bit = 0 , WGM11= 0 ,WGM12= 1 and WGM13 bit = 0 for Fast PWM Mode
							 * 3  Setting the OC pin Mode
							 */
							case FAST_PWM_MODE:
								TCCR1A=0b00000011;
								/*setting of the oc pin mode in case of fast PWM*/
								switch(TIMER_cnfg_arr[Loop_index].OC_ChA_Mode)
								{
								case OC_DISABLED:
									break;
								case NA:
									break;
								case OC_TOGGLE:
									TCCR1A |= (1u<<TCCR1A_COM1A0);
									break;
								case OC_NONINVERTING_PWM:
									TCCR1A |= (1u<<TCCR1A_COM1A1);
									break;
								case OC_INVERTING_PWM:
									TCCR1A |= (3u<<TCCR1A_COM1A0);
									break;
								default:
									Init_check[Loop_index]=NOT_INITIALIZED;
									retval = NOK;
									break;
								}


								switch(TIMER_cnfg_arr[Loop_index].OC_ChB_Mode)
								{
								case OC_DISABLED:
									break;
								case NA:
									break;
								case OC_TOGGLE:
									TCCR1A |= (1u<<TCCR1A_COM1B0);
									break;
								case OC_NONINVERTING_PWM:
									TCCR1A |= (1u<<TCCR1A_COM1B1);
									break;
								case OC_INVERTING_PWM:
									TCCR1A |= (3u<<TCCR1A_COM1B0);
									break;
								default:
									Init_check[Loop_index]=NOT_INITIALIZED;
									retval = NOK;
									break;
								}
								/*
								 * Setting Timer1 Prescaler
								 */
								switch(TIMER_cnfg_arr[Loop_index].Prescaler)
								{
								case NO_CLK_SRC:
									TCCR1B=0b00001000;
									break;
								case F_CPU_1:
									TCCR1B=0b00001001;
									break;
								case F_CPU_8:
									TCCR1B=0b00001010;
									break;
								case F_CPU_64:
									TCCR1B=0b00001011;
									break;
								case F_CPU_256:
									TCCR1B=0b00001100;
									break;
								case F_CPU_1024:
									TCCR1B=0b00001101;
									break;
								case EXT_CLK_FALLING:
									TCCR1B=0b00001110;
									break;
								case EXT_CLK_RISING:
									TCCR1B=0b00001111;
									break;
								default:
									Init_check[Loop_index]=NOT_INITIALIZED;
									retval = NOK;
									break;
								}

								/*
								 * Setting the Compare value of Timer1 Channel A
								 */
								if(TIMER_cnfg_arr[Loop_index].PWM_DUTYCYCLE_chA <= 100 &&TIMER_cnfg_arr[Loop_index].OCR_ChA<=0xffff )
								{

									TIMER_cnfg_arr[Loop_index].OCR_ChA=(uint16) ((0x03FF * TIMER_cnfg_arr[Loop_index].PWM_DUTYCYCLE_chA)/100);
									OCR1AL = TIMER_cnfg_arr[Loop_index].OCR_ChA;
								}
								else
								{
									Init_check[Loop_index]=NOT_INITIALIZED;
									retval = NOK;
								}

								/*
								 * Setting the Compare value of Timer1 Channel B
								 */
								if(TIMER_cnfg_arr[Loop_index].PWM_DUTYCYCLE_chB <= 100 &&TIMER_cnfg_arr[Loop_index].OCR_ChB<=0xffff )
								{

									TIMER_cnfg_arr[Loop_index].OCR_ChB=(uint16) ((0x03FF * TIMER_cnfg_arr[Loop_index].PWM_DUTYCYCLE_chB)/100);
									OCR1BL = TIMER_cnfg_arr[Loop_index].OCR_ChB;
								}
								else
								{
									Init_check[Loop_index]=NOT_INITIALIZED;
									retval = NOK;
								}

								/*
								 * Setting weather the timer1 CHA operates in Interrupt or pooling mode
								 */
								if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == INTERRUPT)
								{
									TIMSK |= 0x10;
									SREG |= (1u<<SREG_I);

								}
								else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == POOLING)
								{
									TIMSK &= ~0x10;
								}
								else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == NA)
								{
									TIMSK &= ~0x10;
								}
								else
								{
									Init_check[Loop_index]=NOT_INITIALIZED;
									retval = NOK;
								}

								/*
								 * Setting weather the timer1 CHB operates in Interrupt or pooling mode
								 */
								if(TIMER_cnfg_arr[Loop_index].Interrupt_ChB == INTERRUPT)
								{
									TIMSK |= 0x08;
									SREG |= (1u<<SREG_I);
								}
								else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChB == POOLING)
								{
									TIMSK &= ~0x08;
								}
								else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChB == NA)
								{
									TIMSK &= ~0x10;
								}
								else
								{
									Init_check[Loop_index]=NOT_INITIALIZED;
									retval = NOK;
								}


								break;

								/****************************************************************
								 * 					Timer1 PWM Phase Correct Mode				*
								 ****************************************************************/

								case PHASE_PWM_MODE:
									/*
									 * 1- FOC1A bit = 0 and FOC1B= 0 for PWM mode
									 * 2- WGM10 bit = 0 , WGM11= 1 ,WGM12= 0 and WGM13 bit = 1 for Fast PWM Mode
									 * 3  Setting the OC pin Mode
									 */
									TCCR1A=0b00000011;
									/*setting of the oc pin mode in case of fast PWM*/
									switch(TIMER_cnfg_arr[Loop_index].OC_ChA_Mode)
									{
									case OC_DISABLED:
										break;
									case NA:
										break;
									case OC_TOGGLE:
										TCCR1A |= (1u<<TCCR1A_COM1A0);
										break;
									case OC_NONINVERTING_PWM:
										TCCR1A |= (1u<<TCCR1A_COM1A1);
										break;
									case OC_INVERTING_PWM:
										TCCR1A |= (3u<<TCCR1A_COM1A0);
										break;
									default:
										Init_check[Loop_index]=NOT_INITIALIZED;
										retval = NOK;
										break;
									}


									switch(TIMER_cnfg_arr[Loop_index].OC_ChB_Mode)
									{
									case OC_DISABLED:
										break;
									case NA:
										break;
									case OC_TOGGLE:
										TCCR1A |= (1u<<TCCR1A_COM1B0);
										break;
									case OC_NONINVERTING_PWM:
										TCCR1A |= (1u<<TCCR1A_COM1B1);
										break;
									case OC_INVERTING_PWM:
										TCCR1A |= (3u<<TCCR1A_COM1B0);
										break;
									default:
										Init_check[Loop_index]=NOT_INITIALIZED;
										retval = NOK;
										break;
									}


									/*
									 * Setting Timer1 Prescaler
									 */
									switch(TIMER_cnfg_arr[Loop_index].Prescaler)
									{
									case NO_CLK_SRC:
										TCCR1B=0b00000000;
										break;
									case F_CPU_1:
										TCCR1B=0b00000001;
										break;
									case F_CPU_8:
										TCCR1B=0b00000010;
										break;
									case F_CPU_64:
										TCCR1B=0b00000011;
										break;
									case F_CPU_256:
										TCCR1B=0b00000100;
										break;
									case F_CPU_1024:
										TCCR1B=0b00000101;
										break;
									case EXT_CLK_FALLING:
										TCCR1B=0b00000110;
										break;
									case EXT_CLK_RISING:
										TCCR1B=0b00000111;
										break;
									default:
										Init_check[Loop_index]=NOT_INITIALIZED;
										retval = NOK;
										break;
									}
									/*
									 * Setting the Compare value of Timer1 Channel A
									 */
									if(TIMER_cnfg_arr[Loop_index].PWM_DUTYCYCLE_chA <= 100 && TIMER_cnfg_arr[Loop_index].OCR_ChA<=0xffff)
									{

										TIMER_cnfg_arr[Loop_index].OCR_ChA=(uint16) ((0x03FF * TIMER_cnfg_arr[Loop_index].PWM_DUTYCYCLE_chA)/100);
										OCR1AL = TIMER_cnfg_arr[Loop_index].OCR_ChA;
									}
									else
									{
										Init_check[Loop_index]=NOT_INITIALIZED;
										retval = NOK;
									}

									/*
									 * Setting the Compare value of Timer1 Channel B
									 */
									if(TIMER_cnfg_arr[Loop_index].PWM_DUTYCYCLE_chB <= 100 && TIMER_cnfg_arr[Loop_index].OCR_ChB<=0xffff )
									{

										TIMER_cnfg_arr[Loop_index].OCR_ChB=(uint16) ((0x03FF * TIMER_cnfg_arr[Loop_index].PWM_DUTYCYCLE_chB)/100);
										OCR1BL = TIMER_cnfg_arr[Loop_index].OCR_ChB;
									}
									else
									{
										Init_check[Loop_index]=NOT_INITIALIZED;
										retval = NOK;
									}

									/*
									 * Setting weather the timer1 CHA operates in Interrupt or pooling mode
									 */
									if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == INTERRUPT)
									{
										TIMSK |= 0x10;
										SREG |= (1u<<SREG_I);

									}
									else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == POOLING)
									{
										TIMSK &= ~0x10;
									}
									else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == NA)
									{
										TIMSK &= ~0x10;
									}
									else
									{
										Init_check[Loop_index]=NOT_INITIALIZED;
										retval = NOK;
									}


									/*
									 * Setting weather the timer1 CHB operates in Interrupt or pooling mode
									 */
									if(TIMER_cnfg_arr[Loop_index].Interrupt_ChB == INTERRUPT)
									{
										TIMSK |= 0x08;
										SREG |= (1u<<SREG_I);
									}
									else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChB == POOLING)
									{
										TIMSK &= ~0x08;
									}
									else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChB == NA)
									{
										TIMSK &= ~0x10;
									}
									else
									{
										Init_check[Loop_index]=NOT_INITIALIZED;
										retval = NOK;
									}

									break;
									case ICU_MODE:
										break;
									default:
										Init_check[Loop_index]=NOT_INITIALIZED;
										retval = NOK;
										break;
					}

					break;

					/********************************************************************************
					 * 									Timer2										*
					 ********************************************************************************/
					case TIMER2:

						switch(TIMER_cnfg_arr[Loop_index].TIMER_Mode)
						{
						/****************************************************************
						 * 						  Timer2 Normal Mode					*
						 ****************************************************************/
						case NORMAL_MODE:
							/*
							 * 1- FOC0 bit = 1 for non-PWM mode
							 * 2- WGM00 and WGM01 bits = 0 for Normal Mode
							 * 3- COM00 and COM01 bits = 0 to disconnect the OC pin as it's not used in Normal mode
							 * 4- CS00:1 bits = 0 for no clk source at the beginning of the initialization (it will be defined later with the prescaler)
							 */
							ASSR &= ~0x0F;
							TCCR2 = 0x80;

							/*
							 * Setting Timer Preloaded value
							 */
							if((TIMER_cnfg_arr[Loop_index].Normal_PreloadedValue >= 0)\
									&& (TIMER_cnfg_arr[Loop_index].Normal_PreloadedValue < 0xFF))
							{
								TCNT2 = TIMER_cnfg_arr[Loop_index].Normal_PreloadedValue;
							}
							else if(TIMER_cnfg_arr[Loop_index].Normal_PreloadedValue>=0xff)
							{

							}
							else
							{
								Init_check[Loop_index]=NOT_INITIALIZED;
								retval = NOK;
							}



							/*
							 * Setting weather the timer operates in Interrupt or pooling mode
							 */
							if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == INTERRUPT)
							{
								TIMSK |= 0x40;
								SREG |= (1<<SREG_I);
							}
							else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == POOLING)
							{
								TIMSK &= ~0x40;
							}
							else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == NA)
							{
								TIMSK &= ~0x40;
							}
							else
							{
								Init_check[Loop_index]=NOT_INITIALIZED;
								retval = NOK;
							}


							/*
							 * Setting Timer0 Prescaler
							 */
							switch(TIMER_cnfg_arr[Loop_index].Prescaler)
							{
							case F_CPU_64:
								TCCR2 |= TIMER_cnfg_arr[Loop_index].Prescaler+1;
								break;

							case F_CPU_256:
							case F_CPU_1024:
								TCCR2 |= TIMER_cnfg_arr[Loop_index].Prescaler+2;
								break;


							case NO_CLK_SRC:
							case F_CPU_1:
							case F_CPU_8:
								TCCR2 |= TIMER_cnfg_arr[Loop_index].Prescaler;
								break;

							case F_CPU_32:
								TCCR2 |= 3;
								break;
							case F_CPU_128:
								TCCR2 |= 5;
								break;
							default:
								Init_check[Loop_index]=NOT_INITIALIZED;
								retval = NOK;
							}

							break;

							/****************************************************************
							 * 						  Timer2 CTC Mode						*
							 ****************************************************************/

							case CTC_MODE:
								/*
								 * 1- FOC0 bit = 1 for non-PWM mode
								 * 2- WGM00 bit = 0 and WGM01 bit = 1 for CTC Mode
								 * 3- COM00 and COM01 bits = 0 to disconnect the OC pin at the beginning of the initialization
								 * 4- CS00:1 bits = 0 for no clk source at the beginning of the initialization (it will be defined later with the prescaler)
								 */
								TCCR2 = 0x88;

								/*
								 * Setting the Compare value
								 */
								if(TIMER_cnfg_arr[Loop_index].OCR_ChA < 0xFF)
								{
									OCR2 = TIMER_cnfg_arr[Loop_index].OCR_ChA;
								}
								else if(TIMER_cnfg_arr[Loop_index].OCR_ChA == 0xFF)
								{

								}
								else
								{
									Init_check[Loop_index]=NOT_INITIALIZED;
									retval = NOK;
								}

								/*
								 * Setting the OC pin Mode
								 */
								switch(TIMER_cnfg_arr[Loop_index].OC_ChA_Mode)
								{
								case OC_DISABLED:
									break;
								case NA:
									break;
								case OC_TOGGLE:
									TCCR2 |= (1<<TCCR2_COM20);
									break;

								case OC_CLEAR:
									TCCR2 |= (1<<TCCR2_COM21);
									break;

								case OC_SET:
									TCCR2 |= (3<<TCCR2_COM20);
									break;

								default:
									Init_check[Loop_index]=NOT_INITIALIZED;
									retval = NOK;
								}

								/*
								 * Setting weather the timer operates in Interrupt or pooling mode
								 */
								if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == INTERRUPT)
								{
									TIMSK |= 0x80;
									SREG |= (1<<SREG_I);
								}
								else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == POOLING)
								{
									TIMSK &= ~0x80;
								}
								else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == NA)
								{
									TIMSK &= ~0x80;
								}
								else
								{
									Init_check[Loop_index]=NOT_INITIALIZED;
									retval = NOK;
								}


								/*
								 * Setting Timer0 Prescaler
								 */
								switch(TIMER_cnfg_arr[Loop_index].Prescaler)
								{
								case F_CPU_64:
									TCCR2 |= TIMER_cnfg_arr[Loop_index].Prescaler+1;
									break;

								case F_CPU_256:
								case F_CPU_1024:
									TCCR2 |= TIMER_cnfg_arr[Loop_index].Prescaler+2;
									break;


								case NO_CLK_SRC:
								case F_CPU_1:
								case F_CPU_8:
									TCCR2 |= TIMER_cnfg_arr[Loop_index].Prescaler;
									break;

								case F_CPU_32:
									TCCR2 |= 3;
									break;
								case F_CPU_128:
									TCCR2 |= 5;
									break;
								default:
									Init_check[Loop_index]=NOT_INITIALIZED;
									retval = NOK;
								}

								break;

								/****************************************************************
								 * 						  Timer2 FAST PWM Mode					*
								 ****************************************************************/

								case FAST_PWM_MODE:
									/*
									 * 1- FOC0 bit = 0 for PWM mode
									 * 2- WGM00 bit = 1 and WGM01 bit = 1 for Fast PWM Mode
									 * 3- COM00 and COM01 bits = 0 to disconnect the OC pin at the beginning of the initialization
									 * 4- CS00:1 bits = 0 for no clk source at the beginning of the initialization (it will be defined later with the prescaler)
									 */
									TCCR2 = 0x48;

									/*
									 * Setting the Duty Cycle
									 */
									if(TIMER_cnfg_arr[Loop_index].PWM_DUTYCYCLE_chA <= 100)
									{

										OCR2 = (uint8)((255 * TIMER_cnfg_arr[Loop_index].PWM_DUTYCYCLE_chA)/100);
									}
									else
									{
										Init_check[Loop_index]=NOT_INITIALIZED;
										retval = NOK;
									}

									/*
									 * Setting OC pin Mode
									 */
									switch(TIMER_cnfg_arr[Loop_index].OC_ChA_Mode)
									{
									case OC_DISABLED:
										break;
									case NA:
										break;

									case OC_NONINVERTING_PWM:
										TCCR2 |= (1<<TCCR2_COM21);
										break;

									case OC_INVERTING_PWM:
										TCCR2 |= (3<<TCCR2_COM20);
										break;

									default:
										Init_check[Loop_index]=NOT_INITIALIZED;
										retval = NOK;
									}

									/*
									 * Setting weather the timer operates in Interrupt or pooling mode
									 */
									if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == INTERRUPT)
									{
										TIMSK |= 0x80;
										SREG |= (1<<SREG_I);
									}
									else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == POOLING)
									{
										TIMSK &= ~0x80;
									}
									else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == NA)
									{
										TIMSK &= ~0x80;
									}
									else
									{
										Init_check[Loop_index]=NOT_INITIALIZED;
										retval = NOK;
									}

									/*
									 * Setting Timer0 Prescaler
									 */
									switch(TIMER_cnfg_arr[Loop_index].Prescaler)
									{
									case F_CPU_64:
										TCCR2 |= TIMER_cnfg_arr[Loop_index].Prescaler+1;
										break;

									case F_CPU_256:
									case F_CPU_1024:
										TCCR2 |= TIMER_cnfg_arr[Loop_index].Prescaler+2;
										break;


									case NO_CLK_SRC:
									case F_CPU_1:
									case F_CPU_8:
										TCCR2 |= TIMER_cnfg_arr[Loop_index].Prescaler;
										break;

									case F_CPU_32:
										TCCR2 |= 3;
										break;
									case F_CPU_128:
										TCCR2 |= 5;
										break;
									default:
										Init_check[Loop_index]=NOT_INITIALIZED;
										retval = NOK;
									}


									break;

									/****************************************************************
									 * 					Timer2 PWM Phase Correct Mode				*
									 ****************************************************************/
									case PHASE_PWM_MODE:
										/*
										 * 1- FOC0 bit = 0 for PWM mode
										 * 2- WGM00 bit = 1 and WGM01 bit = 0 for PWM Phase Correct Mode
										 * 3- COM00 and COM01 bits = 0 to disconnect the OC pin at the beginning of the initialization
										 * 4- CS00:1 bits = 0 for no clk source at the beginning of the initialization (it will be defined later with the prescaler)
										 */
										TCCR2 = 0x40;


										/*
										 * Setting the Duty Cycle
										 */
										if(TIMER_cnfg_arr[Loop_index].PWM_DUTYCYCLE_chA <= 100)
										{

											OCR2 = (uint8)((255 * TIMER_cnfg_arr[Loop_index].PWM_DUTYCYCLE_chA)/100);
										}
										else
										{
											Init_check[Loop_index]=NOT_INITIALIZED;
											retval = NOK;
										}

										/*
										 * Setting the OC pin Mode
										 */
										switch(TIMER_cnfg_arr[Loop_index].OC_ChA_Mode)
										{
										case OC_DISABLED:
											break;
										case NA:
											break;

										case OC_NONINVERTING_PWM:
											TCCR2 |= (1<<TCCR2_COM21);
											break;

										case OC_INVERTING_PWM:
											TCCR2 |= (3<<TCCR2_COM20);
											break;

										default:
											Init_check[Loop_index]=NOT_INITIALIZED;
											retval = NOK;
										}

										/*
										 * Setting weather the timer operates in Interrupt or pooling mode
										 */
										if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == INTERRUPT)
										{
											TIMSK |= 0x80;
											SREG |= (1<<SREG_I);
										}
										else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == POOLING)
										{
											TIMSK &= ~0x80;
										}
										else if(TIMER_cnfg_arr[Loop_index].Interrupt_ChA == NA)
										{
											TIMSK &= ~0x80;
										}
										else
										{
											Init_check[Loop_index]=NOT_INITIALIZED;
											retval = NOK;
										}

										/*
										 * Setting Timer0 Prescaler
										 */
										switch(TIMER_cnfg_arr[Loop_index].Prescaler)
										{
										case F_CPU_64:
											TCCR2 |= TIMER_cnfg_arr[Loop_index].Prescaler+1;
											break;

										case F_CPU_256:
										case F_CPU_1024:
											TCCR2 |= TIMER_cnfg_arr[Loop_index].Prescaler+2;
											break;


										case NO_CLK_SRC:
										case F_CPU_1:
										case F_CPU_8:
											TCCR2 |= TIMER_cnfg_arr[Loop_index].Prescaler;
											break;

										case F_CPU_32:
											TCCR2 |= 3;
											break;
										case F_CPU_128:
											TCCR2 |= 5;
											break;
										default:
											Init_check[Loop_index]=NOT_INITIALIZED;
											retval = NOK;
										}


										break;


										default:
											Init_check[Loop_index]=NOT_INITIALIZED;
											retval = NOK;
						}

						break;

						/*
						 * In case of wrong timer ID
						 * return not initialized timer and NOT OK
						 */
						default:
							Init_check[Loop_index]=NOT_INITIALIZED;
							retval = NOK;
			}

		}
	}

	/*
	 * In case that the number of timers to be configured more than 3
	 * return Not OK
	 */
	else{
		retval = NOK;
	}


	return retval;
}



uint8 TIMER_start(uint8 TIMER_ID)
{
	uint8 funcStatus = NOK;
	uint8 Loop_index;
	for(Loop_index=0;Loop_index<NUM_OF_Timers;Loop_index++)
	{
		if(TIMER_cnfg_arr[Loop_index].TIMER_ID == TIMER_ID)
		{
			if(Init_check[Loop_index] == INITIALIZED)
			{
				funcStatus = OK;
				switch(TIMER_cnfg_arr[Loop_index].TIMER_ID)
				{
				case TIMER0:

					switch(TIMER_cnfg_arr[Loop_index].Prescaler)
					{
					case NO_CLK_SRC:
					case F_CPU_1:
					case F_CPU_8:
					case F_CPU_64:
					case F_CPU_256:
					case F_CPU_1024:
					case EXT_CLK_FALLING:
					case EXT_CLK_RISING:

						TCCR0 |= TIMER_cnfg_arr[Loop_index].Prescaler;
						break;
					default:
						Init_check[Loop_index]=NOT_INITIALIZED;
						funcStatus = NOK;
					}

					break;

					case TIMER1:

						switch(TIMER_cnfg_arr[Loop_index].Prescaler)
						{
						case NO_CLK_SRC:
							TCCR1B |= 0b00000000;
							break;
						case F_CPU_1:
							TCCR1B |= 0b00000001;
							break;
						case F_CPU_8:
							TCCR1B |= 0b00000010;
							break;
						case F_CPU_64:
							TCCR1B |= 0b00000011;
							break;
						case F_CPU_256:
							TCCR1B |= 0b00000100;
							break;
						case F_CPU_1024:
							TCCR1B |= 0b00000101;
							break;
						case EXT_CLK_FALLING:
							TCCR1B |= 0b00000110;
							break;
						case EXT_CLK_RISING:
							TCCR1B |= 0b00000111;
							break;
						default:
							Init_check[Loop_index]=NOT_INITIALIZED;
							funcStatus = NOK;
							break;
						}

						break;

						case TIMER2:

							switch(TIMER_cnfg_arr[Loop_index].Prescaler)
							{
							case F_CPU_64:
								TCCR2 |= TIMER_cnfg_arr[Loop_index].Prescaler+1;
								break;

							case F_CPU_256:
							case F_CPU_1024:
								TCCR2 |= TIMER_cnfg_arr[Loop_index].Prescaler+2;
								break;


							case NO_CLK_SRC:
							case F_CPU_1:
							case F_CPU_8:
								TCCR2 |= TIMER_cnfg_arr[Loop_index].Prescaler;
								break;

							case F_CPU_32:
								TCCR2 |= 3;
								break;
							case F_CPU_128:
								TCCR2 |= 5;
								break;
							default:
								Init_check[Loop_index]=NOT_INITIALIZED;
								funcStatus = NOK;
							}

							break;
				}
			}
			Loop_index = MAX_NUM_OF_TIMERS+1;
		}
	}

	return funcStatus;
}



uint8 TIMER_stop(uint8 TIMER_ID)
{
	uint8 funcStatus = NOK;
	uint8 Loop_index;
	for(Loop_index=0;Loop_index<NUM_OF_Timers;Loop_index++)
	{
		if(TIMER_cnfg_arr[Loop_index].TIMER_ID == TIMER_ID)
		{
			funcStatus = OK;
			switch(TIMER_cnfg_arr[Loop_index].TIMER_ID)
			{
			case TIMER0:

				TCCR0 &= ~ 0x07;
				break;

			case TIMER1:

				TCCR1B &= ~ 0x07;
				break;

			case TIMER2:

				TCCR2 &= ~ 0x07;
				break;
			}

		}
	}

	return funcStatus;
}


uint8 CHANGE_PWM(uint8 TIMER_ID,uint8 PWM_DUTYCYCLE_chA)
{
	TIMER_stop(TIMER_ID);
	uint8 retval=OK;
	if((PWM_DUTYCYCLE_chA >= 0) && (PWM_DUTYCYCLE_chA <= 100))
	{
		switch(TIMER_ID)
		{
		case TIMER0:
			TCCR0 = 0x48;
			TCCR0 |= (1<<TCCR0_COM01);

			/*
			 * Setting the Duty Cycle
			 */
			OCR0 = (255.0 * PWM_DUTYCYCLE_chA)/100;
			break;

		case TIMER1:
			TCCR1A=0b00000011;
			TCCR1A |= (1u<<TCCR1A_COM1A1);
			/*
			 * Setting the Duty Cycle
			 */
			OCR1AL = (0x3ff * PWM_DUTYCYCLE_chA*1.0)/100;


			break;
		case TIMER2:
			TCCR2 = 0x48;
			TCCR2 |= (1<<TCCR2_COM21);
			OCR2 = (255.0 * PWM_DUTYCYCLE_chA)/100;


			break;

		default:
			retval = NOK;
			break;
		}

	}

	else
	{
		retval = NOK;
	}
	TIMER_start(TIMER_ID);
	return retval;
}



void TIMER0_OVF_setCallBack(void (*COPY_TIMER0_OVF_callBackPtr)(void))
{
	g_TIMER0_OVF_callBackPtr = COPY_TIMER0_OVF_callBackPtr;
}

void TIMER0_CMP_setCallBack(void (*COPY_TIMER0_CMP_callBackPtr)(void))
{
	g_TIMER0_CMP_callBackPtr = COPY_TIMER0_CMP_callBackPtr;
}

void TIMER1_OVF_setCallBack(void (*COPY_TIMER1_OVF_callBackPtr)(void))
{
	g_TIMER1_OVF_callBackPtr = COPY_TIMER1_OVF_callBackPtr;
}

void TIMER1_CMPA_setCallBack(void (*COPY_TIMER1_CMPA_callBackPtr)(void))
{
	g_TIMER1_CMPA_callBackPtr = COPY_TIMER1_CMPA_callBackPtr;
}

void TIMER1_CMPB_setCallBack(void (*COPY_TIMER1_CMPB_callBackPtr)(void))
{
	g_TIMER1_CMPB_callBackPtr = COPY_TIMER1_CMPB_callBackPtr;
}

void TIMER2_OVF_setCallBack(void (*COPY_TIMER2_OVF_callBackPtr)(void))
{
	g_TIMER2_OVF_callBackPtr = COPY_TIMER2_OVF_callBackPtr;
}

void TIMER2_CMP_setCallBack(void (*COPY_TIMER2_CMP_callBackPtr)(void))
{
	g_TIMER2_CMP_callBackPtr = COPY_TIMER2_CMP_callBackPtr;
}
