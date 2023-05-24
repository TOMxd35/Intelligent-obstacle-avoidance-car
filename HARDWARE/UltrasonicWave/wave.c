#include "wave.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"

#define Trig GPIO_Pin_3

#define Echo GPIO_Pin_2

#define Trig_Send PAout (3)
#define Echo_Reci PAout (2)

//float Distance;
//float  lengthTemp = 0; 
//u16 msHcCount = 0; //毫秒计数 
//void Wave_SRD_Init(void)
//{
//	GPIO_InitTypeDef  GPIO_InitSture;
////	NVIC_InitTypeDef  NVIC_InitSture;
////	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;     //生成用于定时器设置的结构体 
////  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA,ENABLE);
//	
//	//配置IO端口
//	GPIO_InitSture.GPIO_Mode=GPIO_Mode_Out_PP;   //推挽输出模式
//	GPIO_InitSture.GPIO_Pin=Trig;                //将PA3于Trig相连
//	GPIO_InitSture.GPIO_Speed=GPIO_Speed_50MHz;  
//	GPIO_Init(GPIOA,&GPIO_InitSture);
//	//GPIO_ResetBits(GPIO_A,Trig);  
//	
//	GPIO_InitSture.GPIO_Mode=GPIO_Mode_IPD;      //拉输入模式
//	GPIO_InitSture.GPIO_Pin=Echo;                //将PA2于Echo相连
//	GPIO_InitSture.GPIO_Speed=GPIO_Speed_50MHz;  
//	GPIO_Init(GPIOA,&GPIO_InitSture);
//	//GPIO_ResetBits(GPIO_A,Echo); 
//}

//void Timer_SRD_Init(u16 arr,u16 psc)
//{
//	TIM_TimeBaseInitTypeDef    TIM_TimeBaseInitSture;
//	NVIC_InitTypeDef           NVIC_InitSture;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);//使能TIM5
//	
//	//初始化定时器
//	TIM_TimeBaseInitSture.TIM_CounterMode=TIM_CounterMode_Up;
//	TIM_TimeBaseInitSture.TIM_Period=5000;
//	TIM_TimeBaseInitSture.TIM_Prescaler=7199;
//	TIM_TimeBaseInitSture.TIM_ClockDivision=0;
//	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitSture);
//	
//	//允许更新中断，触发方式中断
//	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
//	TIM_ITConfig(TIM5,TIM_IT_Trigger,ENABLE);
//	
//	//中断优先级管理
//	NVIC_InitSture.NVIC_IRQChannel=TIM5_IRQn;
//	NVIC_InitSture.NVIC_IRQChannelPreemptionPriority=4;
//	NVIC_InitSture.NVIC_IRQChannelSubPriority=4;
//	NVIC_InitSture.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitSture);
//	
//	TIM_Cmd(TIM5,ENABLE);
//}

//void Wave_SRD_Strat(void)
//{
//	GPIO_SetBits(GPIOA,Trig);   //将Trig设置为高电平
//	delay_us(12);               //持续大于10us触发，触发超声波模块工作
//	GPIO_ResetBits(GPIOA,Trig); 
//	
//		TIM_SetCounter(TIM5,0);
//		TIM_Cmd(TIM5,ENABLE);
//		
//		while(GPIO_ReadInputDataBit(GPIOA,Echo));  //等待低电平
//		
//		TIM_Cmd(TIM5,DISABLE);
//		//printf("TIM5:%d\r\n",TIM_GetCounter(TIM5));
//		Distance=(TIM_GetCounter(TIM5)*340)/200.0;

//}

//void TIM5_IRQHander(void)
//{
//  if(TIM_GetITStatus (TIM5,TIM_IT_Update)!=RESET)
//   {
//        TIM_ClearITPendingBit(TIM5,TIM_IT_CC1|TIM_IT_Update); 
//           
//  } 

//}

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
//	
//	TIM_DeInit(TIM5);
//	TIM_TimeBaseStructure .TIM_Period=(1000-1); //
//	TIM_TimeBaseStructure.TIM_Prescaler=(72-1);
//	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
//	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
//	
//	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
//	TIM_ClearFlag(TIM5,TIM_FLAG_Update);
//	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
//	
//	TIM_Cmd (TIM5,DISABLE);
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	NVIC_InitSture.NVIC_IRQChannel=TIM5_IRQn;
//	NVIC_InitSture.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_InitSture.NVIC_IRQChannelPreemptionPriority=3;
//	NVIC_InitSture.NVIC_IRQChannelSubPriority=3;
//	NVIC_InitSture.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitSture);
//}


//static void OpenTimerForHc()
//{
//      TIM_SetCounter(TIM5,0);
//      msHcCount=0;
//      TIM_Cmd(TIM5,ENABLE);
//}

//static void CloseTimerForHc()
//{
//   TIM_Cmd(TIM5,DISABLE);

//}

//void TIM6_IRQHander(void)
//{
//  if(TIM_GetITStatus (TIM5,TIM_IT_Update)!=RESET)
//   {
//	       TIM_ClearITPendingBit (TIM5,TIM_IT_Update);
//         
//         msHcCount ++;		 
//	 
//	 }	

//}

//u32 GetEchoTimer(void)
//{ 
//	u32 t=0;
//	t=msHcCount *1000;
//	t+=TIM_GetCounter (TIM5);
//	TIM5->CNT =0;
//	delay_ms(50);
//  return t;
//}

//void Hcsr04GetLength(void)
//{
//	u32 t=0;
//	int i=0;
//	float lengthTemp=0;
//	float sum=0;
//	      if(i!=5)
//	           {
//	             Trig_Send=1;
//		           delay_us(20);
//		           Trig_Send=0;
//		              while(Echo_Reci==0);
//		              OpenTimerForHc();
//		                 i=i+1;
//		
//		               while(Echo_Reci==1);
//		               CloseTimerForHc();
//		               t=GetEchoTimer();
//		               lengthTemp=((float)t/58.0);
//		               sum=lengthTemp+sum;
//		
//	}
//	     lengthTemp=sum/5.0;
//	    
//}


/*void Wave_SRD_Strat(void)
 {

      int  i = 0;  
     
       float  sum = 0;  
      // if(i!= 5) 
			// {				 
	          GPIO_SetBits(GPIOA,Trig);   //将Trig设置为高电平
	          delay_us(15);               //持续大于10us触发，触发超声波模块工作
	           GPIO_ResetBits(GPIOA,Trig); 
	
		delay_us(10);
	
		TIM_SetCounter(TIM3,0);
		TIM_Cmd(TIM3,ENABLE);
		
		while(GPIO_ReadInputDataBit(GPIOA,Echo));  //等待低电平
		
		TIM_Cmd(TIM3,DISABLE);
		
		lengthTemp=TIM_GetCounter(TIM3)*340/200.0;
	  sum = lengthTemp + sum;
		lengthTemp = sum / 5.0;

}
*/








