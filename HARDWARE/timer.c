
#define __TIMER_C__
#include "timer.h"
#include "key.h"
/***************************************************************
** 功能：     定时器2中断服务程序
** 参数：	  无参数
** 返回值：    无
****************************************************************/ 
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//溢出中断
	{
		 //LED1=!LED1;     //wya  测试定时器频率用 for debug
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
	TIM2->SR&=~(1<<0);//清除中断标志位 	    
}
/***************************************************************
** 功能：     定时器2中断初始化
** 参数：	  arr：自动重装值
**			  psc：时钟预分频数
** 返回值：    无
****************************************************************/
void Timer2_Init(u16 arr,u16 psc)
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///使能TIM2时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM32
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM2,ENABLE); //使能定时器2
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //定时器2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
}
  
/***************************************************************
** 功能：     定时器3中断服务函数
** 参数：	  无参数
** 返回值：    无
****************************************************************/
void TIM3_IRQHandler(void)
{ 	    		  			    
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		if (VCState==1)   //wya 如果语音控制状态为启动追踪
		{
			if(bias_timer4_1ms>250000)		 //wya 如果未接收到安卓端周期偏差信息，超时2.5s钟则强制小车stop
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
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位    
}

/***************************************************************
** 功能：     定时器3中断初始化
** 参数：	  arr：自动重装值
      		  psc：时钟预分频数
** 返回值：    无
//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
****************************************************************/
void Timer3_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
								 
}


/***************************************************************
** 功能：     定时器4中断服务函数
** 参数：	  无参数
** 返回值：    无
****************************************************************/
void TIM4_IRQHandler(void)
{ 	    		  			    

	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
	{	
	
		 rxd1_timer4_1ms++;
		 if(rxd1_timer4_1ms>2)	   // 2ms时间已到，数据帧接收完成	  //sfq  如果4ms内未再次接受到数据，将停止本次数据接收
		 {
		 	  rxd1_timer4_flag=1;
			  TIM4->CR1&=~(0x01);       // wya  F407 关定时器4计数器使能
		    rxd1_over_flag=1;
		 }
    }
		 LED0=!LED0;		   
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位    

}


/***************************************************************
** 功能：     定时器4中断初始化
** 参数：	  arr：自动重装值
**			  psc：时钟预分频数
** 返回值：    无
****************************************************************/
void Timer4_Init(u16 arr,u16 psc)
{
	
//	RCC->APB1ENR|=1<<2;//TIM4时钟使能    wya  stm32f103的写法
// 	TIM4->ARR=arr;  //设定计数器自动重装值
//	TIM4->PSC=psc;  //预分频器7200,得到10Khz的计数时钟
//	//这两个东东要同时设置才可以使用中断
//	TIM4->DIER|=1<<0;   //允许更新中断				
//	TIM4->DIER|=1<<6;   //允许触发中断	   
//	TIM4->CR1|=0x01;    //使能定时器4
//  MY_NVIC_Init(1,1,TIM4_IRQn,3);//抢占1，子优先级2，组3	

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///使能TIM4时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//初始化TIM4
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许定时器4更新中断
	TIM_Cmd(TIM4,ENABLE); //使能定时器4
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //定时器4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}


