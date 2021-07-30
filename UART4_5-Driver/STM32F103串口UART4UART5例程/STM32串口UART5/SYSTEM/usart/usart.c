#include "sys.h"
#include "usart.h"
#include "led.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/5/27
//版本：V1.3
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((UART5->SR&0X40)==0);//循环发送,直到发送完毕   
	UART5->DR = (u8) ch;      
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////

#ifdef EN_UART5_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[64];     //接收缓冲,最大64个字节.
//接收状态
//bit7，接收完成标志
//bit6，接收到0x0d
//bit5~0，接收到的有效字节数目
u8 USART_RX_STA=0;       //接收状态标记	  

void UART5_IRQHandler(void)
{
	u8 res;	    
	if(UART5->SR&(1<<5))//接收到数据
	{	 
		res=UART5->DR; 
		if((USART_RX_STA&0x80)==0)//接收未完成
		{
			if(USART_RX_STA&0x40)//接收到了0x0d
			{
				if(res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x80;	//接收完成了 
			}else //还没收到0X0D
			{	
				if(res==0x0d)USART_RX_STA|=0x40;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3F]=res;
					USART_RX_STA++;
					if(USART_RX_STA>63)USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}  		 									     
	}  											 
} 
#endif

   void USART5_Configuration(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        USART_InitTypeDef USART_InitStructure;
	    NVIC_InitTypeDef NVIC_InitStructure;        

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	         	//USART5 TX 
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    	//复用推挽输出 
  	GPIO_Init(GPIOC, &GPIO_InitStructure);	    	 //c端口  

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         	 //USART5 RX 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //复用开漏输入 
  	GPIO_Init(GPIOD, &GPIO_InitStructure);	         	 //D端口  
   
  	USART_InitStructure.USART_BaudRate   = 115200; 
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位8位 
  	USART_InitStructure.USART_StopBits   = USART_StopBits_1;	//停止位1位 
  	USART_InitStructure.USART_Parity     = USART_Parity_No;	//无校验位 
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件流控 
  	USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;	//收发模式 
   
  	/* Configure USART3 */ 
  	USART_Init(UART5, &USART_InitStructure);	//配置串口参数函数  
   
	    NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;     //UART4_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);

  	/*USART3 receive data register is not empty */ 
  	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);   
  	/* Enable the USART3 */ 
  	USART_Cmd(UART5, ENABLE);	   
}



