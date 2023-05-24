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

u8 Flag_Way=0,Flag_Show=0,Flag_Stop=1,Flag_Next;                 //ֹͣ��־λ�� ��ʾ��־λ Ĭ��ֹͣ ��ʾ��
int Encoder_Left,Encoder_Right;             //���ұ��������������
int Encoder_A_EXTI,Flag_Direction;
u8 Flag_Red=0;		//������ϱ�־
unsigned char Car_Direction;  
int Encoder_Temp;
float Velocity,Velocity_Set,Angle,Angle_Set;
int Motor_A,Motor_B,Servo=1450,Target_A,Target_B,Speed_Target;  //�������������           
int flag;                                //��ص�ѹ������صı���
u8 delay_50,delay_flag; 										//��ʱ����
u16 delay_1;
float Velocity_KP=20,Velocity_KI=0;	       //�ٶȿ���PID����
float Position_KP=500,Position_KI=20,Position_KD=400;  //Position_PID
int ahead,right,back;

extern float obsatacle0,obsatacle1,obsatacle2;
extern float lengthTemp;
extern float Distance;
u8 waring[]={50};				 //Ĭ�ϵı�����Χ
u8 *p=waring;
u16 DIS;


int main(void)
 {	
	delay_init();                 //=====��ʱ��ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	Encoder_Init_TIM2();            //=====�������ӿ�
	Encoder_Init_TIM3();            //=====��ʼ�������� 
	uart_init(115200);						 //���ڳ�ʼ��Ϊ115200 
	usart3_init(115200);	 				//����3��ʼ��Ϊ115200
	esp8266_start_trans();							//esp8266���г�ʼ��
	
	Servo_PWM_Init(9999,71);   		//=====��ʼ��PWM50HZ���� ���
	delay_ms(3000);                  //=====��ʱ����

	Motor_PWM_Init(7199,0);  				//=====��ʼ��PWM 10KHZ������������� 
	 
	Red_Init();                   //�����ʼ��
//	
//	 Timer_SRD_Init(5000,7199);
//	 Wave_SRD_Init();              //��������ʼ��
	 
	 
 	while(1)
	{		
		                      
		
		if(USART3_RX_STA&0x8000)				//�������ݱ�־λ ��1
		{
			switch(USART3_RX_BUF[11])
			{
				//���ٵ�
				case high_speed:	
				{
					LED0=0;
					Speed_Target=500;
					break;
				}                        	
				//���ٵ�
				case low_speed:
				{
					LED0=1;
					Speed_Target=80;
					break;
				}
				
				//ǰ����ת
				case car_move_left:	
				{
					Car_Direction=car_move_left;
					break;
				}  

				//ǰ��
				case car_move:	
				{
					Car_Direction=car_move;
					break;
				}   

				//ǰ����ת
				case car_move_right:	
				{
					Car_Direction=car_move_right;
					break;
				}   

				//����ת
				case car_big_left:	
				{
					Car_Direction=car_big_left;
					break;
				}   

				//����ת
				case car_big_right:	
				{
					Car_Direction=car_big_right;
					break;
				}   

				//������ת
				case car_back_left:	
				{
					Car_Direction=car_back_left;
					break;
				}   

				//����
				case car_back:	
				{
					Car_Direction=car_back;
					break;
				}   	

				//������ת
				case car_back_right:	
				{
					Car_Direction=car_back_right;
					break;
				}   		
				
				//С��ֹͣ
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

				//�����������
				case on2off:
				{
					Red_detect();
					Speed_Target=80;
					Flag_Red=1;
					break;
				}
				
				//�رպ������
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
 
 
 
