#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
#define PI 3.14159265
#define ZHONGZHI 0


#define car_stop 'C'	//停止
#define car_move 'F'	//前进
#define car_back 'K'	//后退
#define car_move_right 'G'	//前进右转
#define car_move_left 'E'		//前进左转
#define car_back_right 'L'	//后退右转
#define car_back_left 'J'		//后退左转
#define car_big_right 'I'		//大右转
#define car_big_left 'H'		//大左转
#define high_speed 'A'			//高速
#define low_speed 'B'				//低速
//#define auto_parking 'D'           //自动停车
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
