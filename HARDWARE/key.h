#ifndef __KEY_H
#define __KEY_H	

 
#include "stm32f4xx.h"
#include "sys.h"

//�����˿ڶ���
#define KEY0 PBin(12)   //PB12
#define KEY1 PBin(13)	//PB13 
#define KEY2 PBin(14)	//PB14
#define KEY3 PBin(15)   //PB15
//����ģ��SU03T������˿ڶ���
#define VOC1 PBin(4)    //PB4
#define VOC2 PBin(5)	  //PB5 
#define VOC3 PBin(6)	  //PB6
#define VOC4 PBin(7)    //PB7

extern  u8   VCState;  //wya ����һ����������״̬ȫ�ֱ���



void KEY_Init(void);//IO��ʼ��
				    
#endif
