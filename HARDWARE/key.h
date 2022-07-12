#ifndef __KEY_H
#define __KEY_H	

 
#include "stm32f4xx.h"
#include "sys.h"

//按键端口定义
#define KEY0 PBin(12)   //PB12
#define KEY1 PBin(13)	//PB13 
#define KEY2 PBin(14)	//PB14
#define KEY3 PBin(15)   //PB15
//语音模块SU03T输出检测端口定义
#define VOC1 PBin(4)    //PB4
#define VOC2 PBin(5)	  //PB5 
#define VOC3 PBin(6)	  //PB6
#define VOC4 PBin(7)    //PB7

extern  u8   VCState;  //wya 定义一个语音控制状态全局变量



void KEY_Init(void);//IO初始化
				    
#endif
