#include "init.h"
#include "led.h"
#include "key.h"
#include "djqd.h"
#include "timer.h"
#include "exit.h"

void Speak_Init(void);
/***************************************************************
** 功能：     硬件端口初始化  使能端口时钟
** 参数：	  无参数
** 返回值：   无
****************************************************************/
void YJ_INIT(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);					//中断分组2
	
	//wya RCC 端口时钟控制寄存器 F407 与 F103 不同
	RCC->AHB1ENR|=1<<0;    //使能PORTA时钟  
	RCC->AHB1ENR|=1<<1;    //使能PORTB时钟
	RCC->AHB1ENR|=1<<2;    //使能PORTC时钟
	RCC->AHB1ENR|=1<<3;    //使能PORTD时钟	
  RCC->AHB1ENR|=1<<4;    //使能PORTE时钟   	 

//  RCC->APB2ENR|=1<<0;       //开启辅助时钟   wya  reved
//  AFIO->MAPR&=0XF8FFFFFF;   //清除MAPR的［26：24］ wya reved
//  AFIO->MAPR|=0X04000000;   //关闭JTAG   wya reved
	Speak_Init();        //wya 测温播报控制引脚初始化 
	LED_Init();			    //LED硬件初始化 
	KEY_Init();			    //按键初始化
	DJ_Init();				  //电机驱动初始化
	PWM_Init();             //TIM1_PWM初始化 
	Timer2_Init(10,83);     // 1Mhz的计数频率，计数到10为10us
	Timer3_Init(10,83);     // 1Mhz的计数频率，计数到10为10us  
	Timer4_Init(1000,83);   // 1Mhz的计数频率，计数到1000为1000ms
  EXTIX_Init();         	// 外部中断初始化

}


/***************************************************************
** 功能：     核心板测温播报控制端口初始化
** 参数：	  无参数
** 返回值：    无
****************************************************************/
void Speak_Init(void)
{	   	 
	
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//使能GPIOG时钟

  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化	
	
}