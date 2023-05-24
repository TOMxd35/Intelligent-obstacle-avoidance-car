#ifndef _RED_H
#define _RED_H


 
void Red_Init(void);        //红外探测模块初始化

void Red_detect(void);         //红外模块工作
void Red_L_detect(void);         
void Red_R_detect(void);         //车库检测

void Parking(void);               //红外控制电机和舵机
#endif

