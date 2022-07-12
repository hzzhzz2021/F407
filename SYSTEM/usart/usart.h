#ifndef __USART_H
#define __USART_H
#include "stm32f4xx.h"
#include "stdio.h"	 

//Mini STM32������
//����1��ʼ��	

#define  MAX_NUM  40		   
 
extern u8 USART1_RX_BUF[];     //���ջ���  
extern u8 USART1_TX_BUF[];     //���ͻ��� 
extern u8 USART2_RX_BUF[];     //���ջ���
extern u8 USART_RX_STA;         //����״̬���	
extern u8 flag1,flag2;
extern u8 flag3;
extern u8 rxd1_over_flag;
extern u8 USART1_RXNUM;
extern void UART_Send_Str(char *s);
extern int U1SendChar(int ch);
extern int U2SendChar(int ch);

#define EN_USART1_RX //ʹ�ܴ���1����
#define EN_USART2_RX //ʹ�ܴ���2����
void uart1_init(u32 pclk2,u32 bound);
void uart2_init(u32 pclk2,u32 bound);

#endif	   



