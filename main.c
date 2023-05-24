#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "encoder.h"
#include "control.h"
#include "usart3.h"
#include "esp8266.h"
#include "led.h"
#include "red.h"
#include "wave.h"

u8 Flag_Way=0,Flag_Show=0,Flag_Stop=1,Flag_Next;                 //停止标志位和 显示标志位 默认停止 显示打开
int Encoder_Left,Encoder_Right;             //左右编码器的脉冲计数
int Encoder_A_EXTI,Flag_Direction;
u8 Flag_Red=0;		//红外避障标志
unsigned char Car_Direction;  
int Encoder_Temp;
float Velocity,Velocity_Set,Angle,Angle_Set;
int Motor_A,Motor_B,Servo=1450,Target_A,Target_B,Speed_Target;  //电机舵机控制相关           
int flag;                                //电池电压采样相关的变量
u8 delay_50,delay_flag; 										//延时变量
u16 delay_1;
float Velocity_KP=20,Velocity_KI=0;	       //速度控制PID参数
float Position_KP=500,Position_KI=20,Position_KD=400;  //Position_PID
int ahead,right,back;

extern float obsatacle0,obsatacle1,obsatacle2;
extern float lengthTemp;
extern float Distance;
u8 waring[]={50};				 //默认的报警范围
u8 *p=waring;
u16 DIS;


int main(void)
 {	
	delay_init();                 //=====延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	Encoder_Init_TIM2();            //=====编码器接口
	Encoder_Init_TIM3();            //=====初始化编码器 
	uart_init(115200);						 //串口初始化为115200 
	usart3_init(115200);	 				//串口3初始化为115200
	esp8266_start_trans();							//esp8266进行初始化
	
	Servo_PWM_Init(9999,71);   		//=====初始化PWM50HZ驱动 舵机
	delay_ms(3000);                  //=====延时启动

	Motor_PWM_Init(7199,0);  				//=====初始化PWM 10KHZ，用于驱动电机 
	 
	Red_Init();                   //红外初始化
//	
//	 Timer_SRD_Init(5000,7199);
//	 Wave_SRD_Init();              //超声波初始化
	 
	 
 	while(1)
	{		
		                      
		
		if(USART3_RX_STA&0x8000)				//串口数据标志位 置1
		{
			switch(USART3_RX_BUF[11])
			{
				//高速档
				case high_speed:	
				{
					LED0=0;
					Speed_Target=500;
					break;
				}                        	
				//低速档
				case low_speed:
				{
					LED0=1;
					Speed_Target=80;
					break;
				}
				
				//前进左转
				case car_move_left:	
				{
					Car_Direction=car_move_left;
					break;
				}  

				//前进
				case car_move:	
				{
					Car_Direction=car_move;
					break;
				}   

				//前进右转
				case car_move_right:	
				{
					Car_Direction=car_move_right;
					break;
				}   

				//大左转
				case car_big_left:	
				{
					Car_Direction=car_big_left;
					break;
				}   

				//大右转
				case car_big_right:	
				{
					Car_Direction=car_big_right;
					break;
				}   

				//后退左转
				case car_back_left:	
				{
					Car_Direction=car_back_left;
					break;
				}   

				//后退
				case car_back:	
				{
					Car_Direction=car_back;
					break;
				}   	

				//后退右转
				case car_back_right:	
				{
					Car_Direction=car_back_right;
					break;
				}   		
				
				//小车停止
				case car_stop:	
				{
					Car_Direction=car_stop;
					break;
				}   
				
				case 'D':
				{
				    USART3_RX_STA=0;
					Red_R_detect();
					while(obsatacle_R==0&&obsatacle_L==0)
					{
						Car_Direction=car_back;
						Red_R_detect();
					}
                       if(obsatacle_R==0&&obsatacle_L==1)
					   {
								     Car_Direction=car_stop;
					                   delay_ms(500);
                                      Velocity=-Speed_Target;
						             Car_Direction=car_big_left;
                                       delay_ms(1000);
									   Car_Direction=car_back;
                                       delay_ms(300);
									   Car_Direction=car_stop;
					   }
					  else
					   {
						             Car_Direction=car_stop;
					                   delay_ms(500);
                                    Velocity=-Speed_Target;
						             Car_Direction=car_big_right;
                                       delay_ms(1000);
									   Car_Direction=car_back;
                                       delay_ms(300);
									   Car_Direction=car_stop;
					   }
						   
                     break;
				
				}

				//开启红外避障
				case on2off:
				{
					Red_detect();
					Speed_Target=80;
					Flag_Red=1;
					break;
				}
				
				//关闭红外避障
				case off2on:
				{
					Flag_Red=0;
					Car_Direction=car_stop;
					break;
				}
				default:  break;
		  }

			USART3_RX_STA=0;
		}
		
				
	}

 
}	 
 
 
 
