#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
#define PI 3.14159265
#define ZHONGZHI 0


#define car_stop 'C'	//ֹͣ
#define car_move 'F'	//ǰ��
#define car_back 'K'	//����
#define car_move_right 'G'	//ǰ����ת
#define car_move_left 'E'		//ǰ����ת
#define car_back_right 'L'	//������ת
#define car_back_left 'J'		//������ת
#define car_big_right 'I'		//����ת
#define car_big_left 'H'		//����ת
#define high_speed 'A'			//����
#define low_speed 'B'				//����
//#define auto_parking 'D'           //�Զ�ͣ��
#define on2off 'M'					
#define off2on 'N'

void Kinematic_Analysis(float velocity,float angle);
void TIM2_IRQHandler(void) ;
void Set_Pwm(int motor_a,int motor_b,int servo);
void Key(void);
void Xianfu_Pwm(void);
u8 Turn_Off( int voltage);
void Get_Angle(u8 way);
int myabs(int a);
int Incremental_PI_A (int Encoder,int Target);
int Incremental_PI_B (int Encoder,int Target);
int Position_PID_A (int Encoder,int Target);
int Position_PID_B (int Encoder,int Target);
void Get_RC(void);
void Find_CCD_Zhongzhi(void);

void Car_Stop(void);
void Car_Move(int Speed);
void Car_Back(int Speed);
void Car_Move_Right(int Speed);
void Car_Move_Left(int Speed);
void Car_Back_Right(int Speed);
void Car_Back_Left(int Speed);
void Car_Big_Right(void);
void Car_Big_Left(void);
void Get_Flag(void);

#endif
