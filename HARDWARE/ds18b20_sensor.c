#include "ds18b20_sensor.h" 
#include "usart.h"  
#include "usually.h"
#include "delay.h"  
#include "smg.h"


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: SetDsOut
** 功能描述: to set PC12 as output pin
** 参数描述：无
   返回值：　void
   作者：　　wya
   日期：    20171115
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void SetDsOut(void) 
{
    GPIO_InitTypeDef  GPIO_InitStructure;  
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz  
    GPIO_Init(GPIOC, &GPIO_InitStructure);//   
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: SetDsInpu
** 功能描述: to set PC12 as input pin
** 参数描述：无
   返回值：　void
   作者：　　wya
   日期：    20171115
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void SetDsInput(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;  
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//??GPIOG??  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//??????  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz  
    GPIO_Init(GPIOC, &GPIO_InitStructure);//???  
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: dsInit
** 功能描述: to reset DS18B20
** 参数描述：无
   返回值：　void
   作者：　　wya
   日期：    20171115
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
  void dsInit(void)
 {
    SetDsOut();
    ds_out= 0;
                               //???783us, ???????480us??  960us??
    Delay_Us(600);
  
    ds_out = 1;              //???????, ????????
 			                         //??????
    Delay_Us(4);
    
 }
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: dsCheck
** 功能描述: to check  DS18B20 exist  or not
** 参数描述：无
   返回值：　unsigned char; 1--not exist,0-- exist
   作者：　　wya
   日期：    20171115
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
uint8_t dsCheck(void)
{
	uint8_t retry=0;
	
	SetDsInput();//SET PC12 INPUT      
	
	while ((!ds_in)&&retry<200)
	{
		 retry++;
		 Delay_Us(1);

	};   
 
	if(retry>=200) return 1;
	else retry=0;

	while (ds_in&&retry<240)
	{
     retry++;
		Delay_Us(1);
	};

	if(retry>=240)return 1;     
	else 
	{
	  LED2=!LED2;                            //wya  18b20 exist
	  return 0;
	}		
   
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: readBit
** 功能描述: read a bit from DS18B20
** 参数描述：无
   返回值：　unsigned char
   作者：　　wya
   日期：    20171115
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
uint8_t readBit(void)
{    
   uint8_t b;
   SetDsOut();
   ds_out = 0;
 
	 Delay_Us(2);
                               
   ds_out=1; 	     	              
   SetDsInput();
     
   Delay_Us(9);
   						   
   b=ds_in;
   SetDsOut();
   ds_out=1;
   Delay_Us(60);                      
   return b;
  
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: readByte
** 功能描述: read a byte from DS18B20
** 参数描述：无
   返回值：　unsigned char
   作者：　　wya
   日期：    20171115
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
unsigned char readByte(void)
{
   unsigned int i;
   unsigned char j, dat;
   dat = 0;
   for(i=0; i<8; i++)
   {
      j = readBit();                      
      dat = (j << 7) | (dat >> 1);
   }
   return dat;
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: writeByte
** 功能描述: write one byte to DS18B20
** 参数描述：无
   返回值：　无
   作者：　　wya
   日期：    20171115
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void writeByte(unsigned char dat)
{
   unsigned char j;
   uint8_t b;
   SetDsOut();
   ds_out=1;
   for(j = 0; j < 8; j++)
   {
		 b = dat & 0x01;
		 dat >>= 1;
														 //?"1", ?DQ??15us?, ?15us~60us??DQ??, ????1
			if(b) 
			{
					ds_out = 0;
					
					Delay_Us(12); 
														 //???15us, ?? ?? ??15us?????
					ds_out=1;              
					Delay_Us(90);
														 //???100us, ???1 ?????60us??
			}
			else                   //?"0", ?DQ??60us~120us
			{
				
					ds_out = 0;
				
					Delay_Us(60);  
				 
					ds_out=1;													 

					Delay_Us(20);             
			}
     
    }
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: sendReadCmd
** 功能描述: send temprature convert command
** 参数描述：无
   返回值：　无
   作者：　　wya
   日期：    20171115
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void sendChangeCmd(void)
{
   dsInit();                   //???DS18B20, ??????, ?????????
   dsCheck();                   //??DS18B20??
   Delay_Us(1000); 
   writeByte(0xcc);            //?????????? Skip Rom
   writeByte(0x44);            //????????? Convert T
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: sendReadCmd
** 功能描述: 发送温度读取命令
** 参数描述：无
   返回值：　无
   作者：　　wya
   日期：    20171115
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void sendReadCmd(void)
{
   dsInit();
   dsCheck();
   Delay_Us(1000);
   writeByte(0xcc);            //?????????? Skip Rom
   writeByte(0xbe);            //???????? Read Scratchpad
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: getTmpValue
** 功能描述: LED IO引脚配置
** 参数描述：无
   返回值：　short
   作者：　　wya
   日期：    20171115
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
int getTmpValue(void)
{
    uint16_t tmpvalue=0;
	  unsigned char TH,TL;
    int16_t value=0;               
    float t;
	
	  dsInit();                             //wya 检测DS18B20是否存在，如果不存在返回9999
    if(dsCheck()) 
		return 9999;	  
		 
		Delay_Us(500);	
		//dspbuf(dsbuf,2);	
		
    sendChangeCmd(); 
		Delay_Us(500);

    sendReadCmd();                         
    TL = readByte();        
    TH  =readByte();	   
		
		value= (TH<<8)|TL;
 		
    t = value * 0.0625;                               
    value = t * 100;     
		
    return value;
   
}


