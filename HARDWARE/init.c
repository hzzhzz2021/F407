#include "init.h"
#include "led.h"
#include "key.h"
#include "djqd.h"
#include "timer.h"
#include "exit.h"

void Speak_Init(void);
/***************************************************************
** ���ܣ�     Ӳ���˿ڳ�ʼ��  ʹ�ܶ˿�ʱ��
** ������	  �޲���
** ����ֵ��   ��
****************************************************************/
void YJ_INIT(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);					//�жϷ���2
	
	//wya RCC �˿�ʱ�ӿ��ƼĴ��� F407 �� F103 ��ͬ
	RCC->AHB1ENR|=1<<0;    //ʹ��PORTAʱ��  
	RCC->AHB1ENR|=1<<1;    //ʹ��PORTBʱ��
	RCC->AHB1ENR|=1<<2;    //ʹ��PORTCʱ��
	RCC->AHB1ENR|=1<<3;    //ʹ��PORTDʱ��	
  RCC->AHB1ENR|=1<<4;    //ʹ��PORTEʱ��   	 

//  RCC->APB2ENR|=1<<0;       //��������ʱ��   wya  reved
//  AFIO->MAPR&=0XF8FFFFFF;   //���MAPR�ģ�26��24�� wya reved
//  AFIO->MAPR|=0X04000000;   //�ر�JTAG   wya reved
	Speak_Init();        //wya ���²����������ų�ʼ�� 
	LED_Init();			    //LEDӲ����ʼ�� 
	KEY_Init();			    //������ʼ��
	DJ_Init();				  //���������ʼ��
	PWM_Init();             //TIM1_PWM��ʼ�� 
	Timer2_Init(10,83);     // 1Mhz�ļ���Ƶ�ʣ�������10Ϊ10us
	Timer3_Init(10,83);     // 1Mhz�ļ���Ƶ�ʣ�������10Ϊ10us  
	Timer4_Init(1000,83);   // 1Mhz�ļ���Ƶ�ʣ�������1000Ϊ1000ms
  EXTIX_Init();         	// �ⲿ�жϳ�ʼ��

}


/***************************************************************
** ���ܣ�     ���İ���²������ƶ˿ڳ�ʼ��
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/
void Speak_Init(void)
{	   	 
	
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��GPIOGʱ��

  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��	
	
}