/*
 * DIO.c
 *
 * Created: 9/20/2019 3:41:49 PM
 *  Author: omara
 */ 

#include "DIO.h"

func_state DIO_init()
{
	func_state state = OK;
	uint8 loop_index = 0;
	if (NUM_OF_PINS <= MAX_NUM_OF_PINS)
	{
		for(;loop_index < NUM_OF_PINS; loop_index++)
		{
			DIO_cnfig_list[loop_index].IS_initialized = INITIALIZED;
			switch(DIO_cnfig_list[loop_index].PORT)
			{
				case PORT_A:
				if(DIO_cnfig_list[loop_index].DIR == OUTPUT)
				{
					DDRA |=(1u<<DIO_cnfig_list[loop_index].PIN);
					if (DIO_cnfig_list[loop_index].OUT == HIGH)
					{
						PORTA |=(1u<<DIO_cnfig_list[loop_index].PIN);
					}
					else if (DIO_cnfig_list[loop_index].OUT == LOW)
					{
						PORTA &=~(1u<<DIO_cnfig_list[loop_index].PIN);
					}
					else
					{
						DIO_cnfig_list[loop_index].IS_initialized = NOT_INITIALIZED;
						state = NOK;
					}
				}
				else if(DIO_cnfig_list[loop_index].DIR == INPUT)
				{
					DDRA &=~(1u<<DIO_cnfig_list[loop_index].PIN);
					if (DIO_cnfig_list[loop_index].RES == PULLUP)
					{
						PORTA |=(1u<<DIO_cnfig_list[loop_index].PIN);
					}
					else if (DIO_cnfig_list[loop_index].RES == EXTRES)
					{
						PORTA &=~(1u<<DIO_cnfig_list[loop_index].PIN);
					}
					else
					{
						DIO_cnfig_list[loop_index].IS_initialized = NOT_INITIALIZED;
						state = NOK;
					}
				}
				else
				{
					DIO_cnfig_list[loop_index].IS_initialized = NOT_INITIALIZED;
					state = NOK;
				}
				break;
				case PORT_B:
				if(DIO_cnfig_list[loop_index].DIR == OUTPUT)
				{
					DDRB |=(1u<<DIO_cnfig_list[loop_index].PIN);
					if (DIO_cnfig_list[loop_index].OUT == HIGH)
					{
						PORTB |=(1u<<DIO_cnfig_list[loop_index].PIN);
					}
					else if (DIO_cnfig_list[loop_index].OUT == LOW)
					{
						PORTB &=~(1u<<DIO_cnfig_list[loop_index].PIN);
					}
					else
					{
						DIO_cnfig_list[loop_index].IS_initialized = NOT_INITIALIZED;
						state = NOK;
					}
				}
				else if(DIO_cnfig_list[loop_index].DIR == INPUT)
				{
					DDRB &=~(1u<<DIO_cnfig_list[loop_index].PIN);
					if (DIO_cnfig_list[loop_index].RES == PULLUP)
					{
						PORTB |=(1u<<DIO_cnfig_list[loop_index].PIN);
					}
					else if (DIO_cnfig_list[loop_index].RES == EXTRES)
					{
						PORTB &=~(1u<<DIO_cnfig_list[loop_index].PIN);
					}
					else
					{
						DIO_cnfig_list[loop_index].IS_initialized = NOT_INITIALIZED;
						state = NOK;
					}
				}
				else
				{
					DIO_cnfig_list[loop_index].IS_initialized = NOT_INITIALIZED;
					state = NOK;
				}
				break;
				case PORT_C:
				if(DIO_cnfig_list[loop_index].DIR == OUTPUT)
				{
					DDRC |=(1u<<DIO_cnfig_list[loop_index].PIN);
					if (DIO_cnfig_list[loop_index].OUT == HIGH)
					{
						PORTC |=(1u<<DIO_cnfig_list[loop_index].PIN);
					}
					else if (DIO_cnfig_list[loop_index].OUT == LOW)
					{
						PORTC &=~(1u<<DIO_cnfig_list[loop_index].PIN);
					}
					else
					{
						DIO_cnfig_list[loop_index].IS_initialized = NOT_INITIALIZED;
						state = NOK;
					}
				}
				else if(DIO_cnfig_list[loop_index].DIR == INPUT)
				{
					DDRC &=~(1u<<DIO_cnfig_list[loop_index].PIN);
					if (DIO_cnfig_list[loop_index].RES == PULLUP)
					{
						PORTC |=(1u<<DIO_cnfig_list[loop_index].PIN);
					}
					else if (DIO_cnfig_list[loop_index].RES == EXTRES)
					{
						PORTC &=~(1u<<DIO_cnfig_list[loop_index].PIN);
					}
					else
					{
						DIO_cnfig_list[loop_index].IS_initialized = NOT_INITIALIZED;
						state = NOK;
					}
				}
				else
				{
					DIO_cnfig_list[loop_index].IS_initialized = NOT_INITIALIZED;
					state = NOK;
				}
				break;
				case PORT_D:
				if(DIO_cnfig_list[loop_index].DIR == OUTPUT)
				{
					DDRD |=(1u<<DIO_cnfig_list[loop_index].PIN);
					if (DIO_cnfig_list[loop_index].OUT == HIGH)
					{
						PORTD |=(1u<<DIO_cnfig_list[loop_index].PIN);
					}
					else if (DIO_cnfig_list[loop_index].OUT == LOW)
					{
						PORTD &=~(1u<<DIO_cnfig_list[loop_index].PIN);
					}
					else
					{
						DIO_cnfig_list[loop_index].IS_initialized = NOT_INITIALIZED;
						state = NOK;
					}
				}
				else if(DIO_cnfig_list[loop_index].DIR == INPUT)
				{
					DDRD &=~(1u<<DIO_cnfig_list[loop_index].PIN);
					if (DIO_cnfig_list[loop_index].RES == PULLUP)
					{
						PORTD |=(1u<<DIO_cnfig_list[loop_index].PIN);
					}
					else if (DIO_cnfig_list[loop_index].RES == EXTRES)
					{
						PORTD &=~(1u<<DIO_cnfig_list[loop_index].PIN);
					}
					else
					{
						DIO_cnfig_list[loop_index].IS_initialized = NOT_INITIALIZED;
						state = NOK;
					}
				}
				else
				{
					DIO_cnfig_list[loop_index].IS_initialized = NOT_INITIALIZED;
					state = NOK;
				}
				break;
				default:
				DIO_cnfig_list[loop_index].IS_initialized = NOT_INITIALIZED;
				state = NOK;
			}
		}
	}
	else
	{
		for (loop_index = 0; loop_index <NUM_OF_PINS ;loop_index++)
		{
			DIO_cnfig_list[loop_index].IS_initialized = NOT_INITIALIZED;
		}
		state = NOK;
	}
	return state;
}



func_state DIO_write(pintype device, uint8 value)
{
    func_state state;
    if ( DIO_cnfig_list[device].DIR == OUTPUT  && (value == HIGH || value == LOW))
    {
		switch(DIO_cnfig_list[device].PORT)
        {
            case PORT_A:
				if (value == HIGH)
                {
                    PORTA |= (1u << DIO_cnfig_list[device].PIN);
                }
                else
                {
					PORTA &= ~(1u << DIO_cnfig_list[device].PIN);                    
                }                                        
            break;
            
            case PORT_B:
            if (value == HIGH)
            {
                PORTB |= (1u << DIO_cnfig_list[device].PIN);
            }
            else
            {
                PORTB &= ~(1u << DIO_cnfig_list[device].PIN);
            }
            break;
            
            case PORT_C:
            if (value == HIGH)
            {
                PORTC |= (1u << DIO_cnfig_list[device].PIN);
            }
            else
            {
                PORTC &= ~(1u << DIO_cnfig_list[device].PIN);
            }
            break;
            
            case PORT_D:
            if (value == HIGH)
            {
                PORTD |= (1u << DIO_cnfig_list[device].PIN);
            }
            else
            {
                PORTD &= ~(1u << DIO_cnfig_list[device].PIN);
            }
            break;       
        }            
         
        state = OK;
    }
    else
    {
        state = NOK;
    }
    
	return state;
}


func_state DIO_read(pintype device, uint8* result)
{
	func_state state = OK;
	if (DIO_cnfig_list[device].IS_initialized == INITIALIZED)
	{
		switch(DIO_cnfig_list[device].PORT)
		{
			case PORT_A:
			*result = (1u & (PINA >> DIO_cnfig_list[device].PIN));
			break;

			case PORT_B:
			*result = (1u & (PINB >> DIO_cnfig_list[device].PIN));
			break;
			
			case PORT_C:
			*result = (1u & (PINC >> DIO_cnfig_list[device].PIN));
			break;
			
			case PORT_D:
			*result = (1u & (PIND >> DIO_cnfig_list[device].PIN));
			break;
		}
	}
	else
	{
		state = NOK;
		*result = 0x00;			
	}
	return state;
}
