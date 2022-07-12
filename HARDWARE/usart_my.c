/**************************************************************/
/*                    ���������������������������������������*/
/**************************************************************/


#define __UART_MY_C__
#include "usart_my.h"
#include "led.h"

/***************************************************************************************/
/*****************     ���ڴ��ݸ���е�����ݽ�������       *****************************/
/**************************************************************************************/

void  USART_WIFI_JX( u8 *buf )
{
	 u8 sum=0,len=0,i=0;  //��ʼ��ֵ  sum
	 u16 t = 0;

	 len = buf[3];
	 for(i=0;i<len;i++)				 // ���У�飬ȡ�Ͱ�λ
	 {
	 	 //sum=(sum+buf[i+3])%256;
	 	  t += buf[i+3];
	 }
		  sum =(u8)(~(t&0x0ff)) ;

	  // sum = ((u8)0xff)(~(sum));
//	   U1SendChar(sum);
	   if(sum==buf[len+3])			//У��ͨ��
	   {
//			rx_len = buf[2];
//	   		for(i=0;i<rx_len;i++)		//����ͨ������1�������
//			{
//				 U1SendChar(buf[i]);
//			}
//		  send_Flag=1;
	   }	  

}


void usart1_data(void)	  // ֱ�ӽ���8�ֽڿ���ָ��
{
	u8 sum=0;

	if(USART1_RX_BUF[7]==0xbb)	 // �жϰ�β
	 {									  
		//��ָ������λ��ָ�������У��
		//ע�⣺��������ʱӦ�öԺ���256ȡ�ࡣ
		 sum=(USART1_RX_BUF[2]+USART1_RX_BUF[3]+USART1_RX_BUF[4]+USART1_RX_BUF[5])%256;
		if(sum==USART1_RX_BUF[6])
		{
			   flag1=1;
			
			LED8=!LED8;  //wya ���ڴ���1���ղ���
			
		}
		else flag1=0;
		}
}


void usart1_data_abnormal(void)	  //�����쳣����
{
	u8 i,j;
	u8 sum=0;
	
	if(USART1_RXNUM<8)			// �쳣�����ֽ���С��8�ֽڲ�������
	{
	   flag1=0;
	}
	else {
		for(i=0;i<=(USART1_RXNUM-7);i++)  
		{
			if(USART1_RX_BUF[i]==0x55)	   // Ѱ�Ұ�ͷ
			{
			   if(USART1_RX_BUF[i+7]==0xbb)	// �жϰ�β				 
			   {
					   //LED8=!LED8;  //wya ���ڴ���1���ղ���
			   	    sum=(USART1_RX_BUF[i+2]+USART1_RX_BUF[i+3]+USART1_RX_BUF[i+4]+USART1_RX_BUF[i+5])%256;

		           if(sum==USART1_RX_BUF[i+6])	 // �ж����
	              {
			         for(j=0;j<8;j++)
					 {
					 	 USART1_RX_BUF[j]=USART1_RX_BUF[j+i];	 // ���ݰ���
					 }
					flag1=1;
		          }
	  	           else flag1=0;
		       }
			}
	    }

	}
	

}


void send_data_zigbee(u8 len)   // ������ת����Zigbee
{
	 u8 i;
	 for(i=0;i<len;i++)
	 {
	 	U2SendChar(USART1_RX_BUF[i]);
	 }
	flag1=0;
}


