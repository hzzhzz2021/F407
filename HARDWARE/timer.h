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
GLOBAL u32 	bias_timer4_1ms;  //wya ��׿��ƫ����Ϣ��ʱ,ÿ����һ�Σ��������ֵ 


GLOBAL void Timer2_Init(u16 arr,u16 psc);	// ͨ�ö�ʱ���жϳ�ʼ��
GLOBAL void TIM2_IRQHandler(void);         // ��ʱ�ж�2�������     

GLOBAL void Timer3_Init(u16 arr,u16 psc);	// ͨ�ö�ʱ���жϳ�ʼ��
GLOBAL void TIM3_IRQHandler(void);         // ��ʱ�ж�3�������

GLOBAL void Timer4_Init(u16 arr,u16 psc);	// ͨ�ö�ʱ���жϳ�ʼ��
GLOBAL void TIM4_IRQHandler(void);         // ��ʱ�ж�4������� 	 				    

#undef GLOBAL
	 				    
#endif


