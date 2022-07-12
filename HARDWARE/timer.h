#ifndef __TIMER_H
#define __TIMER_H	 

#include "stm32f4xx.h"
#include "usart.h"
#include "sys.h"
#include "led.h"
#include "Test_Build_Reg.h"
#include "djqd.h"



#ifdef __TIMER_C__
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL u8 	rxd1_timer4_1ms,rxd1_timer4_flag;
GLOBAL u8   bias_timer4_flag;
GLOBAL u32 	bias_timer4_1ms;  //wya 安卓端偏差信息计时,每接收一次，清零计数值 


GLOBAL void Timer2_Init(u16 arr,u16 psc);	// 通用定时器中断初始化
GLOBAL void TIM2_IRQHandler(void);         // 定时中断2服务程序     

GLOBAL void Timer3_Init(u16 arr,u16 psc);	// 通用定时器中断初始化
GLOBAL void TIM3_IRQHandler(void);         // 定时中断3服务程序

GLOBAL void Timer4_Init(u16 arr,u16 psc);	// 通用定时器中断初始化
GLOBAL void TIM4_IRQHandler(void);         // 定时中断4服务程序 	 				    

#undef GLOBAL
	 				    
#endif


