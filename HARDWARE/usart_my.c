/**************************************************************/
/*                    串口命令解析函数　　　　　　　　　　　　*/
/**************************************************************/


#define __UART_MY_C__
#include "usart_my.h"
#include "led.h"

/***************************************************************************************/
/*****************     串口传递给机械臂数据解析函数       *****************************/
/**************************************************************************************/

void  USART_WIFI_JX( u8 *buf )
{
	 u8 sum=0,len=0,i=0;  //初始赋值  sum
	 u16 t = 0;

	 len = buf[3];
	 for(i=0;i<len;i++)				 // 求和校验，取低八位
	 {
	 	 //sum=(sum+buf[i+3])%256;
	 	  t += buf[i+3];
	 }
		  sum =(u8)(~(t&0x0ff)) ;

	  // sum = ((u8)0xff)(~(sum));
//	   U1SendChar(sum);
	   if(sum==buf[len+3])			//校验通过
	   {
//			rx_len = buf[2];
//	   		for(i=0;i<rx_len;i++)		//数据通过串口1打包发出
//			{
//				 U1SendChar(buf[i]);
//			}
//		  send_Flag=1;
	   }	  

}


void usart1_data(void)	  // 直接接收8字节控制指令
{
	u8 sum=0;

	if(USART1_RX_BUF[7]==0xbb)	 // 判断包尾
	 {									  
		//主指令与三位副指令左求和校验
		//注意：在求和溢出时应该对和做256取余。
		 sum=(USART1_RX_BUF[2]+USART1_RX_BUF[3]+USART1_RX_BUF[4]+USART1_RX_BUF[5])%256;
		if(sum==USART1_RX_BUF[6])
		{
			   flag1=1;
			
			LED8=!LED8;  //wya 用于串口1接收测试
			
		}
		else flag1=0;
		}
}


void usart1_data_abnormal(void)	  //数据异常处理
{
	u8 i,j;
	u8 sum=0;
	
	if(USART1_RXNUM<8)			// 异常数据字节数小于8字节不做处理
	{
	   flag1=0;
	}
	else {
		for(i=0;i<=(USART1_RXNUM-7);i++)  
		{
			if(USART1_RX_BUF[i]==0x55)	   // 寻找包头
			{
			   if(USART1_RX_BUF[i+7]==0xbb)	// 判断包尾				 
			   {
					   //LED8=!LED8;  //wya 用于串口1接收测试
			   	    sum=(USART1_RX_BUF[i+2]+USART1_RX_BUF[i+3]+USART1_RX_BUF[i+4]+USART1_RX_BUF[i+5])%256;

		           if(sum==USART1_RX_BUF[i+6])	 // 判断求和
	              {
			         for(j=0;j<8;j++)
					 {
					 	 USART1_RX_BUF[j]=USART1_RX_BUF[j+i];	 // 数据搬移
					 }
					flag1=1;
		          }
	  	           else flag1=0;
		       }
			}
	    }

	}
	

}


void send_data_zigbee(u8 len)   // 将数据转发给Zigbee
{
	 u8 i;
	 for(i=0;i<len;i++)
	 {
	 	U2SendChar(USART1_RX_BUF[i]);
	 }
	flag1=0;
}


