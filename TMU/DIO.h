/*
 * DIO.h
 *
 * Created: 9/20/2019 3:42:23 PM
 *  Author: Ashry & sha3rawy
 * content: the DIO driver APIs
 */ 


#ifndef DIO_H_
#define DIO_H_

#include <avr/io.h> // rewrite it
#include "DIO_configuration.h"
#include "std_types.h"

#define MAX_NUM_OF_PINS	(uint8)32

typedef enum{NOK,OK}func_state;

/*
 * Function:  DIO_init
 * --------------------
 * initializes the DIO ports :
 *
 *  returns: error flag in case of wrong configuration
 */
func_state DIO_init();

/*
 * Function:  DIO_read
 * --------------------
 * reads the value of a specified input pin:
 *
 *  device: the ID of the pin in the configration array of structures
 *  result: holds the pin value
 *
 *  returns: error flag in case of wrong configuration
 */
func_state DIO_read(pintype device, uint8* result);

/*
 * Function:  DIO_write
 * --------------------
 * assign the pin value to a spicific value:
 *
 *  device: the ID of the pin in the configration array of structures
 *  value: the value to be assigned to the pin
 *
 *  returns: the value of the summation in the normal case and the value after the overflow in overflow cases
 *           prints that an overflow detected if exists
 */
func_state DIO_write(pintype device, uint8 value);

#endif /* DIO_H_ */