#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
//#include "stm32f10x_exti.h"
#include "sys.h"
#include "delay.h"		
#include "exit.h"	
//#include "stm32f10x_tim.h"
#include "led.h"

// ���� 
u32 status=0;						// ����ֵ
u8 tx=0,txr=0;
float  real_time;	 				// ����ֵ

u16	dis;						// �������ֵ
		  
u16 CodedDisk=0;   //����ֵͳ��
u16 tempMP=0;	   //��������ֵ
u16 MP;			   //��������ֵ

/***************************************************************
** ���ܣ�     ���㳬��������þ���
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/ 	
void jscsb()
{
   real_time = status ;
	
   real_time=(float)real_time*1.72-20;     // �������	��ʱ10us��S=Vt/2����20��������
   dis=(u16)real_time;

}

/***************************************************************
** ���ܣ�     �ⲿ�ж�9-5������� ʹ�����ж�9  ���������� INT0 (PB9)���ж�
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/
void EXTI9_5_IRQHandler(void)
{ 
	if(INT0==0)	
	{
	  TIM3->CR1&=~(0x01);                   // ��ʹ�ܶ�ʱ��3
	  jscsb();
     
	} 
	  									 
	EXTI->PR=1<<9;                       // ���LINE9�ϵ��жϱ�־λ
}

# define SPPED_INT_MODE 1   //  1 ������̼���   0�Ҳ�


/***************************************************************
** ���ܣ�     �ⲿ�ж�15-10������� ʹ�����ж�10, ���� PB10 ������������
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/
void EXTI15_10_IRQHandler(void)
{ 
		 EXTI->PR=1<<10;  //���LINE10�ϵ��жϱ�־λ
	   LED2=!LED2;
	  if(SPEED==0)      //wya ȷ�������̵� SPEED ��(PB10)�����ж�
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
** ���ܣ�     �ⲿ�ж�0������� ʹ�����ж�0,  ���� PB0 ������������
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/
void EXTI0_IRQHandler(void)
{ 
	  EXTI->PR=1<<0;  //���LINE0�ϵ��жϱ�־λ
	
	LED1=!LED1;

	if(SPEED_R==0)     //wya ȷ�������̵� A/B (PB0) �߲����ж�
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
** ���ܣ�   �ⲿ�жϳ�ʼ��
            �����ж����ţ� PB11, PB10 ; �������жϽ������ţ�PB9 ; �������������ţ�PE0
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/
void EXTIX_Init(void)
{

	Init_PORT();   //wya ��ʼ���жϼ���������������

	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
		
  /* ���ó������ж����� PB9 -- EXTI_Line9 */	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource9);   //PB0 ���ӵ��ж���9
	
  EXTI_InitStructure.EXTI_Line = EXTI_Line9;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
  EXTI_Init(&EXTI_InitStructure);//����
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//�ⲿ�ж�9
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	
	/* ������������ PB10 -- EXTI_Line10 */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource10);  //PB10 ���ӵ��ж���10
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//�ⲿ�ж�10
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;//�����ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
		
}


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: Init_PORT
** ��������: LED �ж�IO��������
** ������������
** ��  ����: hnpi
** �ա�  ��: 2017��6��20��
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_PORT(void)
{
	   	              
	/*���˿�GPIOB���г�ʼ������*/	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOB,GPIOEʱ��

  //�������ж�����--GPIOB9,�����ж�����B10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
	
	//GPIO PE0��������������-INC ��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
	
}

/***************************************************************
** ���ܣ�     ��������������
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/ 
void tran(void)
{
  

    CSB_TX = 1;            
	  delay_us(3);
    CSB_TX = 0;              //wya ʹ�ܳ���������

	  TIM3->CR1|=0x01;         // ʹ�ܶ�ʱ��3
    TIM3->SR&=~(1<<0);       // ����жϱ�־λ    
    status  = 0;			       // ��ʱ������

	  delay_us(200) ;			     //wya ����һ��200us�ĳ�����������
	  CSB_TX = 1;              //wya ֹͣ����������
    delay_ms(5);

}
