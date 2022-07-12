#include "stm32f4xx.h"
#include "djqd.h"

/***************************************************************
** 功能：     电机驱动端口初始化
** 参数：	  无参数 方向控制引脚
** 返回值：    无
****************************************************************/
void DJ_Init(void)
{
	GPIOE->MODER&= 0XF000FFFF;  //PE9/PE10/PE11/PE12 通用推挽输出  	 
	GPIOE->MODER|= 0X01540000;  
	GPIOE->OTYPER&=0XFFFF21FF;
	GPIOE->ODR|=0X0000;
	
//	GPIOE->CRH&=0XFF000000; 
//	GPIOE->CRH|=0X00333333;   //PE8/PE9/PE10/PE11/PE12/PE13推挽输出  	 
//  GPIOE->ODR|=0X0000;

	LEFT111=1;
	LEFT112=1;
	RIGHT112=1;
	RIGHT111=1;

}

/***************************************************************
** 功能：     停车
** 参数：	  无
** 返回值：    无	  
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
** 功能：     PWM初始化： 
              1.TIM1的CH1N使用PE8输出
              2.TIM1的CH3使用PE13输出
** 参数：	  frequency：自动重装值
**            psc：时钟预分频数
** 返回值：    无	  
****************************************************************/
void PWM_Init(void)
{		 	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1时钟使能   	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//使能PORTE时钟	
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_TIM1); //GPIOE8复用为定时器1输出引脚
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1); //GPIOE13复用为定时器1输出引脚
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_13;           //GPIOE8,13 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //初始化GPIOE8,13
	
	TIM_TimeBaseStructure.TIM_Prescaler=83;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=100;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器1
	
		//初始化TIM1 Channel1N PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low; //输出极性:TIM输出比较极性低
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC3
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器	
	
	//初始化TIM1 Channel3 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC3
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器
 
  TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1
	
	//TIM1->BDTR |=1<<15;  //MOE使能，开启OC和OCN输出 【注意】：【此处极易忽略，造成PWM输出不了！】 不必需在此处使能
 	
	TIM_SetCompare1(TIM1,10);	//修改比较值，修改占空比,要注意在别的地方也可能有修改占空比的代码
	TIM_SetCompare3(TIM1,10);	//修改比较值，修改占空比	
	
}


/***************************************************************
** 功能：     电机控制函数
** 参数：	    L_Spend：电机左轮速度
**            R_Spend：电机右轮速度
** 返回值：    无	  
****************************************************************/
void Control(int L_Spend,int R_Spend)
{
	STOP();
	if(L_Spend>=0)
	{	
		if(L_Spend>100)L_Spend=100;if(L_Spend<5)L_Spend=5;		//限制速度参数
		LEFT111=0;LEFT112=1;TIM1->CCR1=L_Spend;                  //左轮正转计算公式
	}
	else
	{	
		if(L_Spend<-80)L_Spend=-80;if(L_Spend>-5)L_Spend=-5;     //限制速度参数
		LEFT111=1;LEFT112=0;TIM1->CCR1=-L_Spend;				 //左轮反转计算公式
	}
	if(R_Spend>=0)
	{	
		if(R_Spend>100)R_Spend=100;if(R_Spend<5)R_Spend=5;		//限制速度参数
		RIGHT112=0;RIGHT111=1;TIM1->CCR3=R_Spend;				//右轮正转计算公式
	}
	else
	{	
		if(R_Spend<-80)R_Spend=-80;if(R_Spend>-5)R_Spend=-5;		//限制速度参数		
		RIGHT112=1;RIGHT111=0;TIM1->CCR3=-R_Spend;					//右轮反转计算公式
	}

	TIM1->BDTR |=1<<15;  //MOE使能，开启OC和OCN输出 【注意】：【此处极易忽略，造成PWM输出不了！】
}


void TIM4_PWM_Init(u16 arr,u16 psc)//PD12 PD13 PD14 PD15

{

RCC->APB1ENR|=1<<2;//使能TIM4

RCC->AHB1ENR|=1<<3;//使能GPIOD

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

TIM4->CCMR1|=1<<3;//CH1预装载使能

    TIM4->CCMR1|=1<<10;//CH2

    TIM4->CCMR2|=1<<3;//CH3

    TIM4->CCMR2|=1<<10;//CH4

    TIM4->CCMR2|=6<<4;

    TIM4->CCMR2|=6<<12;

TIM4->CCER|=0<<1;//OC1 高电平有效

TIM4->CCER|=1<<0;//OC1使能

    TIM4->CCER|=1<<4;//OC2

    TIM4->CCER|=0<<5;//OC2

    TIM4->CCER|=1<<8;//OC3

    TIM4->CCER|=0<<9;//OC3

    TIM4->CCER|=1<<12;//OC4

    TIM4->CCER|=0<<13;//OC4 

TIM4->CR1|=1<<7;//ARPE使能

TIM4->CR1|=1<<0;//使能定时器4

   

}



