#include "control.h"	
#include "motor.h"
#include "red.h"
#include "sys.h"
#define T 0.156f    //后轮轮距
#define L 0.1445f  //前后轮中心距
#define K 800.8f   //小车整体系数
u8 Flag_Target;
int Voltage_Temp,Voltage_Count,Voltage_All,sum;

/**************************************************************************
函数功能：小车运动数学模型
入口参数：速度和转角
返回  值：无
**************************************************************************/
void Kinematic_Analysis(float velocity,float angle)
{
		Target_A=velocity*(1+T*tan(angle)/2/L); 
		Target_B=velocity*(1-T*tan(angle)/2/L);      //后轮差速
		Servo=SERVO_INIT+angle*K;                    //舵机转向   
}

/**************************************************************************
函数功能：TIM1定时中断		 
**************************************************************************/
void TIM1_UP_IRQHandler(void)  
{    
	if(TIM1->SR&0X0001)//定时中断
	{   
		
		  TIM1->SR&=~(1<<0);                                       							//===清除定时器中断标志位	       
			//所有控制代码应该放在这里面
			if(delay_flag==1)
			{
				 if(++delay_50==5)	 delay_50=0,delay_flag=0;                      //给主函数提供50ms的精准延时  10ms
			}
			Encoder_Left=Read_Encoder(2);                                       //===读取编码器的值							 //为了保证M法测速的时间基准，首先读取编码器数据
			Encoder_Right=-Read_Encoder(3);                                      //===读取编码器的值 
						
			if(Flag_Red==0)					//标准模式
			{
				Get_Flag();
				Kinematic_Analysis(Velocity,Angle);     															//小车运动学分析   根据标志位改变方向以及速度
				Motor_A=Incremental_PI_A(Encoder_Left,Target_A);                   //===速度闭环控制计算电机A最终PWM
				Motor_B=Incremental_PI_B(Encoder_Right,Target_B);                  //===速度闭环控制计算电机B最终PWM 
				Xianfu_Pwm();                                                      //===PWM限幅
				Set_Pwm(Motor_A,Motor_B,Servo);                                 	 //===赋值给PWM寄存器  
			}
			
			if(Flag_Red==1)					//红外避障模式
			{
				if(delay_1>121)	 
				{
					Red_detect();
					delay_1=0;
				}
				
	            //0:检测到，1：未检测到
				//左：1 中：1 右：1 前进
				
				if(obsatacle0==1 && obsatacle1==1 && obsatacle2==1)
				{
					Car_Direction=car_move;
					delay_1=delay_1+20;
				}
				//左：0 中：1 右：1 右转				
				if(obsatacle0==0 && obsatacle1==1 && obsatacle2==1)
				{
					Car_Direction=car_move_right;
					delay_1=delay_1+1;
				}
					
				//左：0 中：0 右：1 右后转
				if(obsatacle0==0 && obsatacle1==0 && obsatacle2==1)
				{
					Velocity=-Speed_Target;
					Car_Direction=car_big_right;
					delay_1=delay_1+1;
				}
				//左：1 中：0 右：0 左后转
				if(obsatacle0==1 && obsatacle1==0 && obsatacle2==0)
				{
					Velocity=-Speed_Target;
					Car_Direction=car_big_left;
					delay_1=delay_1+1;
				}				
				//左：1 中：1 右：0 左转
				if(obsatacle0==1 && obsatacle1==1 && obsatacle2==0)
				{
					Car_Direction=car_move_left;
					delay_1=delay_1+1;
				}				
				
				//左：0 中：0 右：0
				//左：0 中：1 右：03
				//左：1 中：0 右：1
				//后退左转
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
				Kinematic_Analysis(Velocity,Angle);     						    //小车运动学分析   根据标志位改变方向以及速度
				Motor_A=Incremental_PI_A(Encoder_Left,Target_A);                   //===速度闭环控制计算电机A最终PWM
				Motor_B=Incremental_PI_B(Encoder_Right,Target_B);                  //===速度闭环控制计算电机B最终PWM 
				Xianfu_Pwm();                                                      //===PWM限幅
				Set_Pwm(Motor_A,Motor_B,Servo);    
			}


}		 




/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：左轮PWM、右轮PWM
返回  值：无
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
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
**************************************************************************/
void Xianfu_Pwm(void)
{	
	  int Amplitude=6900;    //===PWM满幅是7200 限制在6900
      if(Motor_A<-Amplitude) Motor_A=-Amplitude;	
	  if(Motor_A>Amplitude)  Motor_A=Amplitude;	
	  if(Motor_B<-Amplitude) Motor_B=-Amplitude;	
		if(Motor_B>Amplitude)  Motor_B=Amplitude;		
		if(Servo<(SERVO_INIT-500))     Servo=SERVO_INIT-500;	  //舵机限幅
		if(Servo>(SERVO_INIT+500))     Servo=SERVO_INIT+500;		  //舵机限幅
}

/**************************************************************************
函数功能：增量PI控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI_A (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Target-Encoder;                //计算偏差
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //增量式PI控制器
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
int Incremental_PI_B (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Target-Encoder;                //计算偏差
	 Pwm+=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //增量式PI控制器
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}



/**************************************************************************
函数功能：计算角度的PWM值
入口参数，角度(-45°~+45°)
返回  值：控制该角度所需要的PWM值
注意事项：需要在一开始判断角度是否在范围内，如果不在范围内，限幅，如果输入
的参数不合法，则返回值为1500.
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
函数功能：单独设置电机PWM
入口参数，左轮PWM、右轮PWM
返回  值：无
注意事项：也许会出现两边电机，一边正转，一边反转的情况，以实际情况为准
**************************************************************************/
void Set_Motor_Pwm(int motor_a,int motor_b)
{
    	if(motor_a<0)			PWMA1=7200,PWMA2=7200+motor_a;
			else 	            PWMA2=7200,PWMA1=7200-motor_a;
		
		  if(motor_b<0)			PWMB1=7200,PWMB2=7200+motor_b;
			else 	            PWMB2=7200,PWMB1=7200-motor_b;
}


/**************************************************************************
函数功能：单独设置舵机PWM
入口参数，舵机角度
返回  值：无
**************************************************************************/
void Set_Servo_Pwm(int angle)
{			
     SERVO=Angle_To_PWM(angle);	
}


/**************************************************************************
函数功能：遥控
入口参数：无
返回  值：无
**************************************************************************/
void Get_Flag()
{
	
			switch(Car_Direction)
			{
				//小车前进
				case car_move:
				{
					Velocity=Speed_Target,Angle=0;  //前进
					break;
				}	
				
				//小车停止
				case car_stop:
				{
					Velocity=0,Angle=0;   //停止
					break;
				}		
				
				//小车后退
				case car_back:			
				{
					Velocity=-Speed_Target,Angle=0;
					break;
				}				
				//小车前进右转
				case car_move_right:			
				{
					Velocity=Speed_Target,Angle=PI/6;
					break;
				}					

				//小车前进左转
				case car_move_left:				
				{
					Velocity=Speed_Target,Angle=-PI/6;
					break;
				}				

				//小车后退右转
				case car_back_right:			
				{
					Velocity=-Speed_Target,Angle=PI/6;
					break;
				}			

				//小车后退左转
				case car_back_left:			
				{
					Velocity=-Speed_Target,Angle=-PI/6;
					break;
				}		
				
				//小车大右转
				case car_big_right:			
				{
					Angle=PI/4;
					break;
				}			

				//小车大左转
				case car_big_left:			
				{
					Angle=-PI/4;
					break;
				}								
				default:  break;
			}
}


