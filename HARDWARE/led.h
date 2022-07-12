#ifndef __LED_H
#define __LED_H	 

#include "stm32f4xx.h"  
#include "sys.h"


//LED端口定义
#define LED0 PDout(8)// PD8
#define LED1 PDout(9)// PD9
#define LED2 PDout(10)// PD10
#define LED3 PDout(11)// PD11
#define LED4 PDout(12)// PD12
#define LED5 PDout(13)// PD13
#define LED6 PDout(14)// PD14
#define LED7 PDout(15)// PD15

#define LED8 PFout(9)// PF9
#define LED9 PFout(10)// PF10



void LED_Init(void);//初始化
		 				    
#endif

















