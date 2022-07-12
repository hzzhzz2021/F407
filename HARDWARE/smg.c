//头文件调用
#include "stm32f4xx.h"
//#include "hardware.h"
#include  "smg.h"	
#include "delay.h"
                           /*display 0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F,-*/
unsigned char tab[]={0x03,0x9f,0x25,0x0d,0x99,0x49,0x41,0x1f,0x01,0x09,0x11,0xc1,0x63,0x85,0x61,0xb1,0xfd};  //wya 0xfd-'-'
unsigned int  table[] = {0x0010,0x0020,0x0040,0x0080};


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称:  SMG_INIT
** 功能描述:  数码管位选和段选引脚初始化
             位选使用PD4-PD7
             段选使用PD0-PD1 通过SN74HC164进行串入并出8位段码
              
** 参数描述：void
** 作者：　　wya

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void SMG_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//定义一个GPIO结构体变量

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOD,ENABLE);    //wya For F407 使能各个端口时钟，重要！！！
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6| GPIO_Pin_7;  //wya 数码管位选引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        //输出方向
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_Init(GPIOE,&GPIO_InitStructure);              //初始化GPIOE4，5，6，7
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;  //段码串入引脚PD0,PD1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //输出方向
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_Init(GPIOD, &GPIO_InitStructure);				   	   //根据参数初始化GPIOD寄存器	
	
}
//	  函数名：spend
//	  功能：发送数据
//	  时间：20170105
//    作者：SYH
//	  参数：byte为要发送的数据
void spend(char byte)
{
	char i,num;
	num=tab[byte];
	for(i=0;i<8;i++)
	{
		CP = 0;
    delay_us(10);
		B  = num&0x01;
		delay_us(10);
		CP = 1;
		num>>= 1 ;
	}
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: disp
** 功能描述: Display a 16bit integer
** 参数描述：int count
** 作者：　　wya

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void disp(int count)
{
	char a;
	a=0;
	for(a=0;a<4;a++)
	{
		GPIOE->ODR = GPIOE->ODR&0xff0f;
		spend(count%10);
		count/=10;

    GPIOE->ODR = GPIOE->ODR | table[3-a];
    delay_ms(1);
	}
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: dispbf
** 功能描述: Display a 4-byte buf
** 参数描述：1.Display buf-char *buf
             2.Num of fractional part (1-3)
** 作者：　　wya
   日期:     2017-11-17

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void dspbuf(char *buf,uint8_t frac_num)
{
	char i,j,num;
	i=0;
	for(i=0;i<4;i++)
	{			
		num=tab[buf[3-i]];
		
		if((frac_num>=1)&&(frac_num<=3))
		{
			if (i==(frac_num)) num&=0xfe;
		}		
		
		for(j=0;j<8;j++)
		{
			CP = 0;
			delay_us(10);
			B  = num&0x01;
			delay_us(10);
			CP = 1;
			num>>= 1 ;
		}		
			
    GPIOE->ODR = GPIOE->ODR | table[3-i];    //wya 位选
	//	delay_ms(1);
    delay_ms(1);	   
	  GPIOE->ODR = GPIOE->ODR&0xff0f;         //wya 显示完后，最后一个数码管及时关闭掉，以免与其他几位数字亮度不一致
	}
}

