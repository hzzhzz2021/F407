#include "stm32f4xx.h"
#include "djqd.h"

/***************************************************************
** ���ܣ�     ��������˿ڳ�ʼ��
** ������	  �޲��� �����������
** ����ֵ��    ��
****************************************************************/
void DJ_Init(void)
{
	GPIOE->MODER&= 0XF000FFFF;  //PE9/PE10/PE11/PE12 ͨ���������  	 
	GPIOE->MODER|= 0X01540000;  
	GPIOE->OTYPER&=0XFFFF21FF;
	GPIOE->ODR|=0X0000;
	
//	GPIOE->CRH&=0XFF000000; 
//	GPIOE->CRH|=0X00333333;   //PE8/PE9/PE10/PE11/PE12/PE13�������  	 
//  GPIOE->ODR|=0X0000;

	LEFT111=1;
	LEFT112=1;
	RIGHT112=1;
	RIGHT111=1;

}

/***************************************************************
** ���ܣ�     ͣ��
** ������	  ��
** ����ֵ��    ��	  
****************************************************************/
void STOP(void)
{
	u16 st;
	for(st=0;st<10;st++)
	{
	  TIM1->CCR1=st*10;
    TIM1->CCR3=st*10;
 
	}

    LEFT111=0;LEFT112=0;
    RIGHT112=0;RIGHT111=0;

}

					 		
/***************************************************************
** ���ܣ�     PWM��ʼ���� 
              1.TIM1��CH1Nʹ��PE8���
              2.TIM1��CH3ʹ��PE13���
** ������	  frequency���Զ���װֵ
**            psc��ʱ��Ԥ��Ƶ��
** ����ֵ��    ��	  
****************************************************************/
void PWM_Init(void)
{		 	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1ʱ��ʹ��   	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//ʹ��PORTEʱ��	
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_TIM1); //GPIOE8����Ϊ��ʱ��1�������
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1); //GPIOE13����Ϊ��ʱ��1�������
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_13;           //GPIOE8,13 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //��ʼ��GPIOE8,13
	
	TIM_TimeBaseStructure.TIM_Prescaler=83;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=100;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//��ʼ����ʱ��1
	
		//��ʼ��TIM1 Channel1N PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC3
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���	
	
	//��ʼ��TIM1 Channel3 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC3
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���
 
  TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
	
	//TIM1->BDTR |=1<<15;  //MOEʹ�ܣ�����OC��OCN��� ��ע�⡿�����˴����׺��ԣ����PWM������ˣ��� �������ڴ˴�ʹ��
 	
	TIM_SetCompare1(TIM1,10);	//�޸ıȽ�ֵ���޸�ռ�ձ�,Ҫע���ڱ�ĵط�Ҳ�������޸�ռ�ձȵĴ���
	TIM_SetCompare3(TIM1,10);	//�޸ıȽ�ֵ���޸�ռ�ձ�	
	
}


/***************************************************************
** ���ܣ�     ������ƺ���
** ������	    L_Spend����������ٶ�
**            R_Spend����������ٶ�
** ����ֵ��    ��	  
****************************************************************/
void Control(int L_Spend,int R_Spend)
{
	STOP();
	if(L_Spend>=0)
	{	
		if(L_Spend>100)L_Spend=100;if(L_Spend<5)L_Spend=5;		//�����ٶȲ���
		LEFT111=0;LEFT112=1;TIM1->CCR1=L_Spend;                  //������ת���㹫ʽ
	}
	else
	{	
		if(L_Spend<-80)L_Spend=-80;if(L_Spend>-5)L_Spend=-5;     //�����ٶȲ���
		LEFT111=1;LEFT112=0;TIM1->CCR1=-L_Spend;				 //���ַ�ת���㹫ʽ
	}
	if(R_Spend>=0)
	{	
		if(R_Spend>100)R_Spend=100;if(R_Spend<5)R_Spend=5;		//�����ٶȲ���
		RIGHT112=0;RIGHT111=1;TIM1->CCR3=R_Spend;				//������ת���㹫ʽ
	}
	else
	{	
		if(R_Spend<-80)R_Spend=-80;if(R_Spend>-5)R_Spend=-5;		//�����ٶȲ���		
		RIGHT112=1;RIGHT111=0;TIM1->CCR3=-R_Spend;					//���ַ�ת���㹫ʽ
	}

	TIM1->BDTR |=1<<15;  //MOEʹ�ܣ�����OC��OCN��� ��ע�⡿�����˴����׺��ԣ����PWM������ˣ���
}


void TIM4_PWM_Init(u16 arr,u16 psc)//PD12 PD13 PD14 PD15

{

RCC->APB1ENR|=1<<2;//ʹ��TIM4

RCC->AHB1ENR|=1<<3;//ʹ��GPIOD

//   GPIO_Set(GPIOD,PIN12,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);

//    GPIO_Set(GPIOD,PIN13,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);

//    GPIO_Set(GPIOD,PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);

//    GPIO_Set(GPIOD,PIN15,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);

//GPIO_AF_Set(GPIOD,12,2);

//    GPIO_AF_Set(GPIOD,13,2);

//    GPIO_AF_Set(GPIOD,14,2);

//    GPIO_AF_Set(GPIOD,15,2);

TIM4->ARR=arr;

TIM4->PSC=psc;

TIM4->CCMR1|=6<<4;//PWM1 mode

    TIM4->CCMR1|=6<<12;

TIM4->CCMR1|=1<<3;//CH1Ԥװ��ʹ��

    TIM4->CCMR1|=1<<10;//CH2

    TIM4->CCMR2|=1<<3;//CH3

    TIM4->CCMR2|=1<<10;//CH4

    TIM4->CCMR2|=6<<4;

    TIM4->CCMR2|=6<<12;

TIM4->CCER|=0<<1;//OC1 �ߵ�ƽ��Ч

TIM4->CCER|=1<<0;//OC1ʹ��

    TIM4->CCER|=1<<4;//OC2

    TIM4->CCER|=0<<5;//OC2

    TIM4->CCER|=1<<8;//OC3

    TIM4->CCER|=0<<9;//OC3

    TIM4->CCER|=1<<12;//OC4

    TIM4->CCER|=0<<13;//OC4 

TIM4->CR1|=1<<7;//ARPEʹ��

TIM4->CR1|=1<<0;//ʹ�ܶ�ʱ��4

   

}



