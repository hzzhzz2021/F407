#include <string.h>
#include "stm32f4xx.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"

#define  RXD1_MAX_NUM  200

u8 USART1_RX_BUF[RXD1_MAX_NUM];     //���ջ���,���200���ֽ�.  	
u8 USART2_RX_BUF[MAX_NUM];     //���ջ���,���8���ֽ�.

u8 USART1_TX_BUF[MAX_NUM]="Hello!";     //����1���ͻ���,���8���ֽ�.
//����״̬
//bit7:�����Ƿ���ɱ�־
u8 flag1=0,flag2=0,rx2zt_flag=0;   //���ڽ���״̬;
u8 flag3=0;		 //add by ygm

u8 rxd1_over_flag=0;  // ������ɱ�־

u8 USART_RX_STA=0;       // ����״̬���	 
u8 RX_num1=0,RX_num2=0;     // ���յ�����Ч�ֽ���Ŀ
u8 RX2_MAX=8;			// ������յ���ֽ�
u8 USART1_RXNUM=0;      // ���崮��1������Ч���ݳ���


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: USART1_IRQHandler
** ��������: Wifi-����1�жϴ�����

** ����������void
** ���ߣ�����wya

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void USART1_IRQHandler(void)
{			

  u8 res;


	rxd1_timer4_1ms=0;
	res=USART1->DR;

	if(rxd1_timer4_flag==1) //  2msʱ�� ����֡���¿�ʼ
	{
		 rxd1_timer4_flag=0;
		 TIM4->CR1|=0x01;     //ʹ�ܶ�ʱ��4
		 USART1_RXNUM=0;
		 USART1_RX_BUF[USART1_RXNUM]=res;

	}
	else if(USART1_RXNUM<RXD1_MAX_NUM)	
	{
		  USART1_RX_BUF[++USART1_RXNUM]=res;	 // ��������֡
	}

	else   // ���ݳ������������ݳ���ʱ����ȥ��Ҫ
	{
			res=0;
	}  

	//LED8=!LED8;  //wya ���ڴ���1���ղ���
	 								 
}  

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: USART2_IRQHandler
** ��������:  ZigBee-����2�жϴ�����

** ����������void
** ���ߣ�����wya

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void USART2_IRQHandler(void)
{

								 
} 


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: uart1_init
** ��������:  WiFi-����1��ʼ��
              PA9--TX�� PA10-RX
              ��Ϊ�˺����ڲ��ж�ȡϵͳʱ�ӵĹ��ܣ���pclk2��������
** ����������bound-������
** ���ߣ�����wya

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void uart1_init(u32 pclk2,u32 bound)
{  	 
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
  //wya ʹ�ܽ����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����


}			


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: uart2_init
** ��������: ZigBee-����2��ʼ��.ԭSTM32F103 ��ѧʵ�����ZigBee�ӿ����ô���
              PD5--TX�� PD6-RX
              ��Ϊ�˺����ڲ��ж�ȡϵͳʱ�ӵĹ��ܣ���pclk2��������
** ����������bound-������
** ���ߣ�����wya

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void uart2_init(u32 pclk2,u32 bound)
{  	 
//	float temp;
//	u16 mantissa;
//	u16 fraction;	   
//	temp=(float)(pclk2*1000000)/(bound*32);//�õ�USARTDIV  ����1ʱ(bound*16)
//	mantissa=temp;				 //�õ���������
//	fraction=(temp-mantissa)*16; //�õ�С������	 
//    mantissa<<=4;
//	mantissa+=fraction; 

//	RCC->APB2ENR|=1<<0;   //ʹ��AFIO��ʱ��
//	RCC->APB2ENR|=1<<5;   //ʹ��PORT5��ʱ��  
//	RCC->APB1ENR|=1<<17;  //ʹ�ܴ���2ʱ��
//	
//	AFIO->MAPR=0X0008; 	  // ����2��ӳ�书��TX-PD5  RX-PD6
//	GPIOD->CRL=0X44B44444;//IO״̬����
//		  
//	RCC->APB1RSTR|=1<<17;   //��λ����2
//	RCC->APB1RSTR&=~(1<<17);//ֹͣ��λ	   	   
//	//����������
// 	USART2->BRR=mantissa; // ����������	 
//	USART2->CR1|=0X200C;  //1λֹͣ,��У��λ.
//#ifdef EN_USART2_RX		  //���ʹ���˽���
//	//ʹ�ܽ����ж�
//	USART2->CR1|=1<<8;    //PE�ж�ʹ��
//	USART2->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
//	MY_NVIC_Init(1,2,USART2_IRQn,1);//��1��������ȼ� 
//#endif


}


void Delay(vu32 nCount) 
{
  for(; nCount != 0; nCount--);
}


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������:  U1SendChar
** ��������:  WiFi-����1����һ���ַ�
              
** ����������ch-�����ַ�
** ���ߣ�����wya

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
int U1SendChar(int ch) 
{

  while (!(USART1->SR & USART_SR_TXE ));    //wya SR_TXEλ F407��F103�޲���
  USART1->DR = (ch & 0x1FF);

  return (ch);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������:  U2SendChar
** ��������:  ZigBee-����2����һ���ַ�
              
** ����������ch-�����ַ�
** ���ߣ�����wya

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
int U2SendChar(int ch) 
{

  while (!(USART2->SR & USART_SR_TXE ));
  USART2->DR = (ch & 0x1FF);

  return (ch);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������:  GetKey
** ��������:  ZigBee-����2����һ���ַ�
              
** ����������void
** ���ߣ�����wya

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
int GetKey(void) 
{
  while (!(USART2->SR & USART_SR_TXE ));  

  return ((int)(USART2->DR & 0x1FF));
}
