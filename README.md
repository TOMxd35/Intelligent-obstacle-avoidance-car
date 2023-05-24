# Intelligent-obstacle-avoidance-car
This is a C++ language design of intelligent obstacle avoidance car program, compiler platform for Keil uVision5

main.c为主程序文件，驱动STM32单片机以及红外避障模块、WIFI模块、编码器、舵机、直流电机的运行，并包含自动避障的方案和用手机APP操控小车速度和方向的方案。

USER文件夹内包含MAIN函数以及STM32单片机的一些驱动文件

HARDWARE文件夹内包含编码器、直流电机驱动、舵机驱动、WIFI模块、红外避障模块的设置文件

SYSTEM文件夹内包含STM32单片机自带的延时函数和串口驱动设置文件

CORE文件夹存放核心文件和启动文件

FWLib文件夹用来存放 ST 官方提供的库函数源码文件
