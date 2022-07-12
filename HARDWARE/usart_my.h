#ifndef __USART_MY_H
#define __USART_MY_H


#include <stdint.h>
#include <string.h>
#include "stm32f4xx.h"	  //头文件包含
#include "sys.h"
#include "usart.h"


#ifdef __UART_MY_C__
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL void USART_WIFI_JX( u8 *buf );	// 串口处理机械臂数据
GLOBAL void usart1_data(void);			// 串口1数据正常接收8字节
GLOBAL void usart1_data_abnormal(void);	 // 串口1数据接收异常
GLOBAL void send_data_zigbee(u8 len);	 // 串口1将数据转发给串口2 zigbee

#undef GLOBAL
#endif

