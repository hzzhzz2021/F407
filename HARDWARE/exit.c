#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
//#include "stm32f10x_exti.h"
#include "sys.h"
#include "delay.h"		
#include "exit.h"	
//#include "stm32f10x_tim.h"
#include "led.h"

// 变量 
u32 status=0;						// 计数值
u8 tx=0,txr=0;
float  real_time;	 				// 读回值

u16	dis;						// 距离计算值
		  
u16 CodedDisk=0;   //码盘值统计
u16 tempMP=0;	   //接收码盘值
u16 MP;			   //控制码盘值

/***************************************************************
** 功能：     计算超声波所测得距离
** 参数：	  无参数
** 返回值：    无
****************************************************************/ 	
void jscsb()
{
   real_time = status ;
	
   real_time=(float)real_time*1.72-20;     // 计算距离	定时10us，S=Vt/2（减20是误差补尝）
   dis=(u16)real_time;

}

/***************************************************************
** 功能：     外部中断9-5服务程序 使用了中断9  超声波接收 INT0 (PB9)线中断
** 参数：	  无参数
** 返回值：    无
****************************************************************/
void EXTI9_5_IRQHandler(void)
{ 
	if(INT0==0)	
	{
	  TIM3->CR1&=~(0x01);                   // 关使能定时器3
	  jscsb();
     
	} 
	  									 
	EXTI->PR=1<<9;                       // 清除LINE9上的中断标志位
}

# define SPPED_INT_MODE 1   //  1 左侧码盘计数   0右侧


/***************************************************************
** 功能：     外部中断15-10服务程序 使用了中断10, 引脚 PB10 接收码盘脉冲
** 参数：	  无参数
** 返回值：    无
****************************************************************/
void EXTI15_10_IRQHandler(void)
{ 
		 EXTI->PR=1<<10;  //清除LINE10上的中断标志位
	   LED2=!LED2;
	  if(SPEED==0)      //wya 确定是码盘的 SPEED 线(PB10)产生中断
	  {
	    if(tx>0)
		{
		  tx=0;
			
		  CodedDisk++;
	    MP++;
		}
	  	else tx++; 
	  }
	   
}



/***************************************************************
** 功能：     外部中断0服务程序 使用了中断0,  引脚 PB0 接收码盘脉冲
** 参数：	  无参数
** 返回值：    无
****************************************************************/
void EXTI0_IRQHandler(void)
{ 
	  EXTI->PR=1<<0;  //清除LINE0上的中断标志位
	
	LED1=!LED1;

	if(SPEED_R==0)     //wya 确定是码盘的 A/B (PB0) 线产生中断
	  {
	    if(txr>0)
		{
			
		  txr=0;
		  CodedDisk++;
	      MP++;
		}
	  	else txr++; 
	  }
	 
}


/***************************************************************
** 功能：   外部中断初始化
            码盘中断引脚： PB11, PB10 ; 超声波中断接收引脚：PB9 ; 超声波发射引脚：PE0
** 参数：	  无参数
** 返回值：    无
****************************************************************/
void EXTIX_Init(void)
{

	Init_PORT();   //wya 初始化中断及超声波发射引脚

	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
		
  /* 配置超声波中断引脚 PB9 -- EXTI_Line9 */	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource9);   //PB0 连接到中断线9
	
  EXTI_InitStructure.EXTI_Line = EXTI_Line9;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
  EXTI_Init(&EXTI_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//外部中断9
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	
	
	/* 配置码盘引脚 PB10 -- EXTI_Line10 */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource10);  //PB10 连接到中断线10
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断10
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;//子优先级3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
		
}


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: Init_PORT
** 功能描述: LED 中断IO引脚配置
** 参数描述：无
** 作  　者: hnpi
** 日　  期: 2017年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_PORT(void)
{
	   	              
	/*将端口GPIOB进行初始化配置*/	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOB,GPIOE时钟

  //超声波中断引脚--GPIOB9,码盘中断引脚B10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	
	//GPIO PE0超声波发射引脚-INC 初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化
	
}

/***************************************************************
** 功能：     超声波发生函数
** 参数：	  无参数
** 返回值：    无
****************************************************************/ 
void tran(void)
{
  

    CSB_TX = 1;            
	  delay_us(3);
    CSB_TX = 0;              //wya 使能超声波发送

	  TIM3->CR1|=0x01;         // 使能定时器3
    TIM3->SR&=~(1<<0);       // 清除中断标志位    
    status  = 0;			       // 定时器清零

	  delay_us(200) ;			     //wya 发射一个200us的超声波短脉冲
	  CSB_TX = 1;              //wya 停止超声波发送
    delay_ms(5);

}
