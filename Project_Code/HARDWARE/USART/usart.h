#ifndef _USART_H_
#define _USART_H_


#include "stm32f10x.h"


//#define USART_DEBUG		USART2		//调试打印所使用的串口组
//#define USART4  UART4


void Usart1_Init(unsigned int baud);
void Usart2_Init(unsigned int baud);
void Usart3_Init(unsigned int baud);

//void Usart4_Init(unsigned int baud);

void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);

void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);



#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收

	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         		//接收状态标记	

extern u8  USART2_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART2_RX_STA;         		//接收状态标记	

//如果想串口中断接收，请不要注释以下宏定义
void Usart1_Init(unsigned int baud);
void Usart2_Init(unsigned int baud);





#endif
