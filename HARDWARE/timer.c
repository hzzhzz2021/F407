
#define __TIMER_C__
#include "timer.h"
#include "key.h"
/***************************************************************
** ���ܣ�     ��ʱ��2�жϷ������
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/ 
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//����ж�
	{
		 //LED1=!LED1;     //wya  ���Զ�ʱ��Ƶ���� for debug
	  	if(G_Flag && (MP > tempMP))
		{
			G_Flag = 0;
			Stop_Flag = 3;
			STOP();
		} 	
		if(B_Flag && (MP > tempMP))
		{
			B_Flag = 0;
			Stop_Flag = 8;
			STOP();
		} 
		if(L_Flag && (MP > tempMP))
		{
			L_Flag = 0;
			Stop_Flag = 2;
			STOP();
		} 
		if(R_Flag && (MP > tempMP))
		{
			R_Flag = 0;
			Stop_Flag = 7;
			STOP();
		} 			     	    	
	}				   
	TIM2->SR&=~(1<<0);//����жϱ�־λ 	    
}
/***************************************************************
** ���ܣ�     ��ʱ��2�жϳ�ʼ��
** ������	  arr���Զ���װֵ
**			  psc��ʱ��Ԥ��Ƶ��
** ����ֵ��    ��
****************************************************************/
void Timer2_Init(u16 arr,u16 psc)
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///ʹ��TIM2ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//��ʼ��TIM32
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM2,ENABLE); //ʹ�ܶ�ʱ��2
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //��ʱ��2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
}
  
/***************************************************************
** ���ܣ�     ��ʱ��3�жϷ�����
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/
void TIM3_IRQHandler(void)
{ 	    		  			    
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		if (VCState==1)   //wya �����������״̬Ϊ����׷��
		{
			if(bias_timer4_1ms>250000)		 //wya ���δ���յ���׿������ƫ����Ϣ����ʱ2.5s����ǿ��С��stop
			{
				STOP();
				bias_timer4_1ms=0;
				LED9=!LED9;
			} 
			 else
			 {
				 bias_timer4_1ms++;
			 }
	 }
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ    
}

/***************************************************************
** ���ܣ�     ��ʱ��3�жϳ�ʼ��
** ������	  arr���Զ���װֵ
      		  psc��ʱ��Ԥ��Ƶ��
** ����ֵ��    ��
//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
****************************************************************/
void Timer3_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
								 
}


/***************************************************************
** ���ܣ�     ��ʱ��4�жϷ�����
** ������	  �޲���
** ����ֵ��    ��
****************************************************************/
void TIM4_IRQHandler(void)
{ 	    		  			    

	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //����ж�
	{	
	
		 rxd1_timer4_1ms++;
		 if(rxd1_timer4_1ms>2)	   // 2msʱ���ѵ�������֡�������	  //sfq  ���4ms��δ�ٴν��ܵ����ݣ���ֹͣ�������ݽ���
		 {
		 	  rxd1_timer4_flag=1;
			  TIM4->CR1&=~(0x01);       // wya  F407 �ض�ʱ��4������ʹ��
		    rxd1_over_flag=1;
		 }
    }
		 LED0=!LED0;		   
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //����жϱ�־λ    

}


/***************************************************************
** ���ܣ�     ��ʱ��4�жϳ�ʼ��
** ������	  arr���Զ���װֵ
**			  psc��ʱ��Ԥ��Ƶ��
** ����ֵ��    ��
****************************************************************/
void Timer4_Init(u16 arr,u16 psc)
{
	
//	RCC->APB1ENR|=1<<2;//TIM4ʱ��ʹ��    wya  stm32f103��д��
// 	TIM4->ARR=arr;  //�趨�������Զ���װֵ
//	TIM4->PSC=psc;  //Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��
//	//����������Ҫͬʱ���òſ���ʹ���ж�
//	TIM4->DIER|=1<<0;   //��������ж�				
//	TIM4->DIER|=1<<6;   //�������ж�	   
//	TIM4->CR1|=0x01;    //ʹ�ܶ�ʱ��4
//  MY_NVIC_Init(1,1,TIM4_IRQn,3);//��ռ1�������ȼ�2����3	

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///ʹ��TIM4ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//��ʼ��TIM4
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //����ʱ��4�����ж�
	TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ��4
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //��ʱ��4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}


