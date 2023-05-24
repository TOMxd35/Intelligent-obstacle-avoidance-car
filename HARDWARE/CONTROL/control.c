#include "control.h"	
#include "motor.h"
#include "red.h"
#include "sys.h"
#define T 0.156f    //�����־�
#define L 0.1445f  //ǰ�������ľ�
#define K 800.8f   //С������ϵ��
u8 Flag_Target;
int Voltage_Temp,Voltage_Count,Voltage_All,sum;

/**************************************************************************
�������ܣ�С���˶���ѧģ��
��ڲ������ٶȺ�ת��
����  ֵ����
**************************************************************************/
void Kinematic_Analysis(float velocity,float angle)
{
		Target_A=velocity*(1+T*tan(angle)/2/L); 
		Target_B=velocity*(1-T*tan(angle)/2/L);      //���ֲ���
		Servo=SERVO_INIT+angle*K;                    //���ת��   
}

/**************************************************************************
�������ܣ�TIM1��ʱ�ж�		 
**************************************************************************/
void TIM1_UP_IRQHandler(void)  
{    
	if(TIM1->SR&0X0001)//��ʱ�ж�
	{   
		
		  TIM1->SR&=~(1<<0);                                       							//===�����ʱ���жϱ�־λ	       
			//���п��ƴ���Ӧ�÷���������
			if(delay_flag==1)
			{
				 if(++delay_50==5)	 delay_50=0,delay_flag=0;                      //���������ṩ50ms�ľ�׼��ʱ  10ms
			}
			Encoder_Left=Read_Encoder(2);                                       //===��ȡ��������ֵ							 //Ϊ�˱�֤M�����ٵ�ʱ���׼�����ȶ�ȡ����������
			Encoder_Right=-Read_Encoder(3);                                      //===��ȡ��������ֵ 
						
			if(Flag_Red==0)					//��׼ģʽ
			{
				Get_Flag();
				Kinematic_Analysis(Velocity,Angle);     															//С���˶�ѧ����   ���ݱ�־λ�ı䷽���Լ��ٶ�
				Motor_A=Incremental_PI_A(Encoder_Left,Target_A);                   //===�ٶȱջ����Ƽ�����A����PWM
				Motor_B=Incremental_PI_B(Encoder_Right,Target_B);                  //===�ٶȱջ����Ƽ�����B����PWM 
				Xianfu_Pwm();                                                      //===PWM�޷�
				Set_Pwm(Motor_A,Motor_B,Servo);                                 	 //===��ֵ��PWM�Ĵ���  
			}
			
			if(Flag_Red==1)					//�������ģʽ
			{
				if(delay_1>121)	 
				{
					Red_detect();
					delay_1=0;
				}
				
	            //0:��⵽��1��δ��⵽
				//��1 �У�1 �ң�1 ǰ��
				
				if(obsatacle0==1 && obsatacle1==1 && obsatacle2==1)
				{
					Car_Direction=car_move;
					delay_1=delay_1+20;
				}
				//��0 �У�1 �ң�1 ��ת				
				if(obsatacle0==0 && obsatacle1==1 && obsatacle2==1)
				{
					Car_Direction=car_move_right;
					delay_1=delay_1+1;
				}
					
				//��0 �У�0 �ң�1 �Һ�ת
				if(obsatacle0==0 && obsatacle1==0 && obsatacle2==1)
				{
					Velocity=-Speed_Target;
					Car_Direction=car_big_right;
					delay_1=delay_1+1;
				}
				//��1 �У�0 �ң�0 ���ת
				if(obsatacle0==1 && obsatacle1==0 && obsatacle2==0)
				{
					Velocity=-Speed_Target;
					Car_Direction=car_big_left;
					delay_1=delay_1+1;
				}				
				//��1 �У�1 �ң�0 ��ת
				if(obsatacle0==1 && obsatacle1==1 && obsatacle2==0)
				{
					Car_Direction=car_move_left;
					delay_1=delay_1+1;
				}				
				
				//��0 �У�0 �ң�0
				//��0 �У�1 �ң�03
				//��1 �У�0 �ң�1
				//������ת
				if((obsatacle0==0 && obsatacle1==0 && obsatacle2==0)||
					(obsatacle0==0 && obsatacle1==1 && obsatacle2==0)||
				   (obsatacle0==1 && obsatacle1==0 && obsatacle2==1))
				    {
					    Velocity=-Speed_Target;
						Car_Direction=car_big_left;
						delay_1=delay_1+1;
					}
								
				}	
				
				Get_Flag();
				Kinematic_Analysis(Velocity,Angle);     						    //С���˶�ѧ����   ���ݱ�־λ�ı䷽���Լ��ٶ�
				Motor_A=Incremental_PI_A(Encoder_Left,Target_A);                   //===�ٶȱջ����Ƽ�����A����PWM
				Motor_B=Incremental_PI_B(Encoder_Right,Target_B);                  //===�ٶȱջ����Ƽ�����B����PWM 
				Xianfu_Pwm();                                                      //===PWM�޷�
				Set_Pwm(Motor_A,Motor_B,Servo);    
			}


}		 




/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ���������PWM������PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int motor_a,int motor_b,int servo)
{
			
    	if(motor_a<0)			PWMA1=7200,PWMA2=7200+motor_a;
			else 	            PWMA2=7200,PWMA1=7200-motor_a;
		
		  if(motor_b<0)			PWMB1=7200,PWMB2=7200+motor_b;
			else 	            PWMB2=7200,PWMB1=7200-motor_b;
     SERVO=servo;	
}

/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Xianfu_Pwm(void)
{	
	  int Amplitude=6900;    //===PWM������7200 ������6900
      if(Motor_A<-Amplitude) Motor_A=-Amplitude;	
	  if(Motor_A>Amplitude)  Motor_A=Amplitude;	
	  if(Motor_B<-Amplitude) Motor_B=-Amplitude;	
		if(Motor_B>Amplitude)  Motor_B=Amplitude;		
		if(Servo<(SERVO_INIT-500))     Servo=SERVO_INIT-500;	  //����޷�
		if(Servo>(SERVO_INIT+500))     Servo=SERVO_INIT+500;		  //����޷�
}

/**************************************************************************
�������ܣ�����PI������
��ڲ���������������ֵ��Ŀ���ٶ�
����  ֵ�����PWM
��������ʽ��ɢPID��ʽ 
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
pwm�����������
�����ǵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI_A (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Target-Encoder;                //����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
int Incremental_PI_B (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Target-Encoder;                //����ƫ��
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}



/**************************************************************************
�������ܣ�����Ƕȵ�PWMֵ
��ڲ������Ƕ�(-45��~+45��)
����  ֵ�����ƸýǶ�����Ҫ��PWMֵ
ע�������Ҫ��һ��ʼ�жϽǶ��Ƿ��ڷ�Χ�ڣ�������ڷ�Χ�ڣ��޷����������
�Ĳ������Ϸ����򷵻�ֵΪ1500.
**************************************************************************/

int Angle_To_PWM(int angle)
{
	int Angle_PWM;
	if(angle<-45)
	{
		Angle_PWM=1000;
	}
	else if(angle<-45)
	{
		Angle_PWM=2000;
	}
	else 
		Angle_PWM=SERVO_INIT+angle*11.1;
	
	return Angle_PWM;
}


/**************************************************************************
�������ܣ��������õ��PWM
��ڲ���������PWM������PWM
����  ֵ����
ע�����Ҳ���������ߵ����һ����ת��һ�߷�ת���������ʵ�����Ϊ׼
**************************************************************************/
void Set_Motor_Pwm(int motor_a,int motor_b)
{
    	if(motor_a<0)			PWMA1=7200,PWMA2=7200+motor_a;
			else 	            PWMA2=7200,PWMA1=7200-motor_a;
		
		  if(motor_b<0)			PWMB1=7200,PWMB2=7200+motor_b;
			else 	            PWMB2=7200,PWMB1=7200-motor_b;
}


/**************************************************************************
�������ܣ��������ö��PWM
��ڲ���������Ƕ�
����  ֵ����
**************************************************************************/
void Set_Servo_Pwm(int angle)
{			
     SERVO=Angle_To_PWM(angle);	
}


/**************************************************************************
�������ܣ�ң��
��ڲ�������
����  ֵ����
**************************************************************************/
void Get_Flag()
{
	
			switch(Car_Direction)
			{
				//С��ǰ��
				case car_move:
				{
					Velocity=Speed_Target,Angle=0;  //ǰ��
					break;
				}	
				
				//С��ֹͣ
				case car_stop:
				{
					Velocity=0,Angle=0;   //ֹͣ
					break;
				}		
				
				//С������
				case car_back:			
				{
					Velocity=-Speed_Target,Angle=0;
					break;
				}				
				//С��ǰ����ת
				case car_move_right:			
				{
					Velocity=Speed_Target,Angle=PI/6;
					break;
				}					

				//С��ǰ����ת
				case car_move_left:				
				{
					Velocity=Speed_Target,Angle=-PI/6;
					break;
				}				

				//С��������ת
				case car_back_right:			
				{
					Velocity=-Speed_Target,Angle=PI/6;
					break;
				}			

				//С��������ת
				case car_back_left:			
				{
					Velocity=-Speed_Target,Angle=-PI/6;
					break;
				}		
				
				//С������ת
				case car_big_right:			
				{
					Angle=PI/4;
					break;
				}			

				//С������ת
				case car_big_left:			
				{
					Angle=-PI/4;
					break;
				}								
				default:  break;
			}
}


