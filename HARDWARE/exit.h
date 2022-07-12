#ifndef __EXIT_H
#define __EXIT_H	 
#include "sys.h"

#define CSB_TX PEout(0)             // PE0���ͳ���������

#define INT0      PBin(9)				//���峬�����ж���������
#define SPEED     PBin(10)			    //���������ź���������
#define SPEED_R   PBin(0)			    //���������ź���������

extern u16 CodedDisk;   //����ֵͳ��
extern u16 tempMP;	   //��������ֵ
extern u16 MP;			   //��������ֵ


extern u16 dis;

void Init_PORT(void);
void EXTI9_5_IRQHandler(void);    // �ⲿ�ж�13�������
void EXTI15_10_IRQHandler(void);
void EXTIX_Init(void);     // �ⲿ�жϳ�ʼ������
void Timer3_Init(u16 arr,u16 psc);	// ͨ�ö�ʱ���жϳ�ʼ��
void TIM3_IRQHandler(void);         // ��ʱ�ж�3�������     
void tran(void); 					// ��������������
	 				    
#endif


