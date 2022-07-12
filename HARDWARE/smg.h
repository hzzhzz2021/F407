#ifndef __SMG_H
#define __SMG_H	
//头文件调用
#include "stm32f4xx.h"
#include "sys.h"

#define B  PDout(0)// PD0
#define CP PDout(1)// PD1
#define S1 PEout(4)// PD1
#define S2 PEout(5)// PD1
#define S3 PEout(6)// PD1
#define S4 PEout(7)// PD1

#define FU 16
#define SMG_A  10
#define SMG_B  11
#define SMG_C  12
#define SMG_D  13
#define SMG_E  14
#define SMG_F  15

extern void disp(int count);
extern void dspbuf(char *buf,uint8_t frac_num);
extern void SMG_INIT(void);//初始化		 				    
#endif
