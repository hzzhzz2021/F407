#ifndef __USART_MY_H
#define __USART_MY_H


#include <stdint.h>
#include <string.h>
#include "stm32f4xx.h"	  //ͷ�ļ�����
#include "sys.h"
#include "usart.h"


#ifdef __UART_MY_C__
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL void USART_WIFI_JX( u8 *buf );	// ���ڴ����е������
GLOBAL void usart1_data(void);			// ����1������������8�ֽ�
GLOBAL void usart1_data_abnormal(void);	 // ����1���ݽ����쳣
GLOBAL void send_data_zigbee(u8 len);	 // ����1������ת��������2 zigbee

#undef GLOBAL
#endif

