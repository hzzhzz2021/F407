#ifndef __INIT_H
#define __INIT_H	 
	
#include "stm32f4xx.h"	  //包含
#include "sys.h"
#define SPEAK PGout(1)  //wya PG1设置为测温语音播报控制端

void YJ_INIT(void);//初始化	
void Speak_Init(void);
	 				    
#endif

