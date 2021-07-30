#include "sys.h"
#include "uart4.h"	  
#include "uart.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.



/*
************************************************************
*	函数名称：	Uart4_Init
*
*	函数功能：	串口4初始化
*
*	入口参数：	baud：设定的波特率
*
*	返回参数：	无
*
*	说明：		TX-PC10		RX-PC11		(PartialRemap)
************************************************************
*/





 
 
#if EN_UART4_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 UART4_RX_BUF[UART4_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 UART4_RX_STA=0;       //接收状态标记	  

void Uart4_Init(unsigned int baud)
{

    GPIO_InitTypeDef gpioInitStruct;
    USART_InitTypeDef uart4InitStruct;
    NVIC_InitTypeDef nvicInitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4, ENABLE);
    
    //PC10	TXD
    gpioInitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    gpioInitStruct.GPIO_Pin = GPIO_Pin_10;
    gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &gpioInitStruct);
    
    //PC11	RXD
    gpioInitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpioInitStruct.GPIO_Pin = GPIO_Pin_11;
    gpioInitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &gpioInitStruct);

    //Usart1 NVIC 配置
    nvicInitStruct.NVIC_IRQChannel = UART4_IRQn;
    nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
    nvicInitStruct.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级3
    nvicInitStruct.NVIC_IRQChannelSubPriority = 3;
    NVIC_Init(&nvicInitStruct);
    
    //USART 初始化设置
    uart4InitStruct.USART_BaudRate = baud;
    uart4InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控
    uart4InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//接收和发送
    uart4InitStruct.USART_Parity = USART_Parity_No;									//无校验
    uart4InitStruct.USART_StopBits = USART_StopBits_1;								//1位停止位
    uart4InitStruct.USART_WordLength = USART_WordLength_8b;							//8位数据位
    USART_Init(UART4, &uart4InitStruct);
    
    USART_Cmd(UART4, ENABLE);														//使能串口
    
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);									//使能接收中断
	

}


void UART4_IRQHandler(void)                	//串口4中断服务程序
{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(UART4);	//读取接收到的数据
		
		if((UART4_RX_STA&0x8000)==0)//接收未完成
			{
			if(UART4_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)UART4_RX_STA=0;//接收错误,重新开始
				else UART4_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)UART4_RX_STA|=0x4000;
				else
					{
					UART4_RX_BUF[UART4_RX_STA&0X3FFF]=Res ;
					UART4_RX_STA++;
					if(UART4_RX_STA>(UART4_REC_LEN-1))UART4_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
}
#endif	

