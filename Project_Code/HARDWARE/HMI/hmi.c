
#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "hmi.h"


u8 idfind(u8 *buf,u8 *val,u8 len);
void HMISendString(char *buf1);
void HMISendb(u8 buf);
void beepms(u16 va);
u8 key,beep;


void beepms(u16 va)
{
	beep=1;
	delay_ms(va);
	beep=0;
}

void HMISendstart(void)
{
	delay_ms(200);
	HMISendb(0xff);
	delay_ms(200);
}

void HMISendString(char *buf1)		  //字符串发送函数
{
	u8 i=0;
	while(1)
	{
	if(buf1[i]!=0)
	 	{
			USART_SendData(USART2,buf1[i]);  //发送一个字节
			while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){};//等待发送结束
		 	i++;
		}
	else 
	return ;

	}
}

void HMISendb(u8 k)		         //字节发送函数
{		 
	u8 i;
	for(i=0;i<3;i++)
	{
	if(k!=0)
	 	{
			USART_SendData(USART2,k);  //发送一个字节
			while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){};//等待发送结束
		}
	else 
	return ;

	} 
} 



