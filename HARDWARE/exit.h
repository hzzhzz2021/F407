#ifndef __EXIT_H
#define __EXIT_H	 
#include "sys.h"

#define CSB_TX PEout(0)             // PE0发送超声波引脚

#define INT0      PBin(9)				//定义超声波中断输入引脚
#define SPEED     PBin(10)			    //定义码盘信号输入引脚
#define SPEED_R   PBin(0)			    //定义码盘信号输入引脚

extern u16 CodedDisk;   //码盘值统计
extern u16 tempMP;	   //接收码盘值
extern u16 MP;			   //控制码盘值


extern u16 dis;

void Init_PORT(void);
void EXTI9_5_IRQHandler(void);    // 外部中断13服务程序
void EXTI15_10_IRQHandler(void);
void EXTIX_Init(void);     // 外部中断初始化程序
void Timer3_Init(u16 arr,u16 psc);	// 通用定时器中断初始化
void TIM3_IRQHandler(void);         // 定时中断3服务程序     
void tran(void); 					// 左发生超声波函数
	 				    
#endif


