#include <string.h>
#include "stm32f4xx.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"

#define  RXD1_MAX_NUM  200

u8 USART1_RX_BUF[RXD1_MAX_NUM];     //接收缓冲,最大200个字节.  	
u8 USART2_RX_BUF[MAX_NUM];     //接收缓冲,最大8个字节.

u8 USART1_TX_BUF[MAX_NUM]="Hello!";     //串口1发送缓冲,最大8个字节.
//接收状态
//bit7:接收是否完成标志
u8 flag1=0,flag2=0,rx2zt_flag=0;   //串口接收状态;
u8 flag3=0;		 //add by ygm

u8 rxd1_over_flag=0;  // 接收完成标志

u8 USART_RX_STA=0;       // 接收状态标记	 
u8 RX_num1=0,RX_num2=0;     // 接收到的有效字节数目
u8 RX2_MAX=8;			// 定义接收的最长字节
u8 USART1_RXNUM=0;      // 定义串口1接收有效数据长度


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: USART1_IRQHandler
** 功能描述: Wifi-串口1中断处理函数

** 参数描述：void
** 作者：　　wya

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void USART1_IRQHandler(void)
{			

  u8 res;


	rxd1_timer4_1ms=0;
	res=USART1->DR;

	if(rxd1_timer4_flag==1) //  2ms时间 数据帧重新开始
	{
		 rxd1_timer4_flag=0;
		 TIM4->CR1|=0x01;     //使能定时器4
		 USART1_RXNUM=0;
		 USART1_RX_BUF[USART1_RXNUM]=res;

	}
	else if(USART1_RXNUM<RXD1_MAX_NUM)	
	{
		  USART1_RX_BUF[++USART1_RXNUM]=res;	 // 接收数据帧
	}

	else   // 数据超出最大接收数据长度时，舍去不要
	{
			res=0;
	}  

	//LED8=!LED8;  //wya 用于串口1接收测试
	 								 
}  

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: USART2_IRQHandler
** 功能描述:  ZigBee-串口2中断处理函数

** 参数描述：void
** 作者：　　wya

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void USART2_IRQHandler(void)
{

								 
} 


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: uart1_init
** 功能描述:  WiFi-串口1初始化
              PA9--TX， PA10-RX
              因为此函数内部有读取系统时钟的功能，故pclk2参数不用
** 参数描述：bound-波特率
** 作者：　　wya

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void uart1_init(u32 pclk2,u32 bound)
{  	 
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART1, ENABLE);  //使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
  //wya 使能接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、


}			


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: uart2_init
** 功能描述: ZigBee-串口2初始化.原STM32F103 教学实验板上ZigBee接口所用串口
              PD5--TX， PD6-RX
              因为此函数内部有读取系统时钟的功能，故pclk2参数不用
** 参数描述：bound-波特率
** 作者：　　wya

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void uart2_init(u32 pclk2,u32 bound)
{  	 
//	float temp;
//	u16 mantissa;
//	u16 fraction;	   
//	temp=(float)(pclk2*1000000)/(bound*32);//得到USARTDIV  串口1时(bound*16)
//	mantissa=temp;				 //得到整数部分
//	fraction=(temp-mantissa)*16; //得到小数部分	 
//    mantissa<<=4;
//	mantissa+=fraction; 

//	RCC->APB2ENR|=1<<0;   //使能AFIO口时钟
//	RCC->APB2ENR|=1<<5;   //使能PORT5口时钟  
//	RCC->APB1ENR|=1<<17;  //使能串口2时钟
//	
//	AFIO->MAPR=0X0008; 	  // 串口2重映射功能TX-PD5  RX-PD6
//	GPIOD->CRL=0X44B44444;//IO状态设置
//		  
//	RCC->APB1RSTR|=1<<17;   //复位串口2
//	RCC->APB1RSTR&=~(1<<17);//停止复位	   	   
//	//波特率设置
// 	USART2->BRR=mantissa; // 波特率设置	 
//	USART2->CR1|=0X200C;  //1位停止,无校验位.
//#ifdef EN_USART2_RX		  //如果使能了接收
//	//使能接收中断
//	USART2->CR1|=1<<8;    //PE中断使能
//	USART2->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
//	MY_NVIC_Init(1,2,USART2_IRQn,1);//组1，最低优先级 
//#endif


}


void Delay(vu32 nCount) 
{
  for(; nCount != 0; nCount--);
}


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称:  U1SendChar
** 功能描述:  WiFi-串口1发送一个字符
              
** 参数描述：ch-发送字符
** 作者：　　wya

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
int U1SendChar(int ch) 
{

  while (!(USART1->SR & USART_SR_TXE ));    //wya SR_TXE位 F407与F103无差异
  USART1->DR = (ch & 0x1FF);

  return (ch);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称:  U2SendChar
** 功能描述:  ZigBee-串口2发送一个字符
              
** 参数描述：ch-发送字符
** 作者：　　wya

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
int U2SendChar(int ch) 
{

  while (!(USART2->SR & USART_SR_TXE ));
  USART2->DR = (ch & 0x1FF);

  return (ch);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称:  GetKey
** 功能描述:  ZigBee-串口2接收一个字符
              
** 参数描述：void
** 作者：　　wya

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
int GetKey(void) 
{
  while (!(USART2->SR & USART_SR_TXE ));  

  return ((int)(USART2->DR & 0x1FF));
}
