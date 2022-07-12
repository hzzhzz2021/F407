#ifndef __DS18B20_SENSOR_H
#define __DS18B20_SENSOR_H
 
#include "stm32f4xx.h"	  //°üº¬


#define ds_in PCin(12)
#define ds_out PCout(12)

void SetDsOut(void);
void SetDsInput(void);
void _Startup(void);
uint8_t dsCheck(void);
void dsInit(void);
uint8_t readBit(void);
void writeByte(unsigned char dat);

void sendChangeCmd(void);
extern int getTmpValue(void);


#endif


