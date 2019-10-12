/*
 * tmu.h
 *
 * Created: 10/12/2019 7:56:40 PM
 *  Author: omara
 */ 


#ifndef TMU_H_
#define TMU_H_

#include "tmu_cfg.h"

typedef enum{NOK,OK}func_state;

func_state TMU_init(timer_id id, uint8 resolution);
func_state TMU_start(void (*app)(void), periodicity per, uint8 freq);
func_state TMU_stopTimer(void (*app)(void));
func_state TMU_dispatcher();

#endif /* TMU_H_ */
