#ifndef _WAVE_H
#define _WAVE_H


#include "sys.h"


void Wave_SRD_Init(void);        //超声波模块的初始化
//void Hcsr04GetLength(void);
void Wave_SRD_Strat(void);         //超声波模块本省的触发条件，大于10us的触发

void Timer_SRD_Init(u16 arr,u16 psc);
	
void TIM5_IRQHander(void);

#endif

