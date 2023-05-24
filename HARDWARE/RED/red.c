#include "red.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#define RED0 GPIO_Pin_4
#define RED1 GPIO_Pin_5
#define RED2 GPIO_Pin_8
#define RED_R GPIO_Pin_3
#define RED_L GPIO_Pin_9



//u16 DIS;
float obsatacle0,obsatacle1,obsatacle2,obsatacle_R,obsatacle_L;

void Red_Init(void)
{
 GPIO_InitTypeDef GPIO_InitStructure; 
 GPIO_InitStructure.GPIO_Pin = RED0;   //将PA4于OUT相连
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   //推挽输出模式
 GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
 GPIO_InitStructure.GPIO_Pin = RED1;   //将PA5于OUT相连
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   //推挽输出模式
 GPIO_Init(GPIOA, &GPIO_InitStructure);
	
 GPIO_InitStructure.GPIO_Pin = RED2;   //将PA8于OUT相连
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   //推挽输出模式
 GPIO_Init(GPIOA, &GPIO_InitStructure);
	
 GPIO_InitStructure.GPIO_Pin = RED_L;   //将PA9于OUT相连
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   //推挽输出模式
 GPIO_Init(GPIOA, &GPIO_InitStructure);
 
 GPIO_InitStructure.GPIO_Pin = RED_R;   //将PA3于OUT相连
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   //推挽输出模式
 GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void Red_detect(void)
{
  
	//while(GPIO_ReadInputDataBit(GPIOA,OUT)); //等待低电平
  
	obsatacle0 = GPIO_ReadInputDataBit(GPIOA,RED0);
    obsatacle1 = GPIO_ReadInputDataBit(GPIOA,RED1);
	obsatacle2 = GPIO_ReadInputDataBit(GPIOA,RED2);
	
}

void Red_R_detect(void)
{
	obsatacle_R = GPIO_ReadInputDataBit(GPIOA,RED_R);
	obsatacle_L = GPIO_ReadInputDataBit(GPIOA,RED_L);
}



//void Parking(void)
//{                   
//	while(1)
//	{
//	Red_detect(); 
//    OBS=obsatacle;
//		if(OBS==0)
//			{
//				LED1=1;
//				Speed_Target=0;
//				Servo=1800;
//			}
//		else
//			{
//			    LED1=0;
//				Speed_Target=200;
//				Servo=1500;
//			}
//	}
//}

