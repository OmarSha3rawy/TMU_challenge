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

uint8 TMU_init(uint8 id);
uint8 TMU_DeInit();

uint8 TMU_start(func_Ptr application, periodicity period, uint16 frequency);

uint8 TMU_stopTimer(func_Ptr application);

uint8 TMU_dispatcher();

#endif /* TMU_H_ */

