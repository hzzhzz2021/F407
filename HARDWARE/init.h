#ifndef __INIT_H
#define __INIT_H	 
	
#include "stm32f4xx.h"	  //����
#include "sys.h"
#define SPEAK PGout(1)  //wya PG1����Ϊ���������������ƶ�

void YJ_INIT(void);//��ʼ��	
void Speak_Init(void);
	 				    
#endif

