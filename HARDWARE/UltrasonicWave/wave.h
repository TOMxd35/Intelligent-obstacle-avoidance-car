#ifndef _WAVE_H
#define _WAVE_H


#include "sys.h"


void Wave_SRD_Init(void);        //������ģ��ĳ�ʼ��
//void Hcsr04GetLength(void);
void Wave_SRD_Strat(void);         //������ģ�鱾ʡ�Ĵ�������������10us�Ĵ���

void Timer_SRD_Init(u16 arr,u16 psc);
	
void TIM5_IRQHander(void);

#endif

