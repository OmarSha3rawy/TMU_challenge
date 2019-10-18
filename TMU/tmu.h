/*
 * tmu.h
 *
 * Created: 10/12/2019 7:56:40 PM
 *  Author: omara
 */ 


#ifndef TMU_H_
#define TMU_H_

#include "tmu_cfg.h"

//typedef enum{NOK,OK}func_state;

uint8 TMU_init(uint8 id, uint8 resolution);
//uint8 TMU_start(void (*application)(void), periodicity per, uint32 freq);
uint8 TMU_start(void (*application)(void), periodicity period, uint16 frequency);

uint8 TMU_stopTimer(void (*application)(void));
uint8 TMU_dispatcher();

#endif /* TMU_H_ */
