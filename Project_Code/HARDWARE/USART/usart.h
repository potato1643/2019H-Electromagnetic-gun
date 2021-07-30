#ifndef _USART_H_
#define _USART_H_


#include "stm32f10x.h"


#define USART_DEBUG		USART2		//���Դ�ӡ��ʹ�õĴ�����
//#define USART4  UART4


void Usart1_Init(unsigned int baud);
void Usart2_Init(unsigned int baud);
void Usart3_Init(unsigned int baud);

void Usart4_Init(unsigned int baud);

void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);

void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);



#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void Usart1_Init(unsigned int baud);




#endif
