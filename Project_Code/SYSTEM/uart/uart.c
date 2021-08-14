/**
	************************************************************
	************************************************************
	************************************************************
	*	�ļ����� 	uart.c
	*
	*	���ߣ� 		BXY
	*
	*	���ڣ� 		2021-07-30
	*
	*	�汾�� 		V1.0
	*
	*	˵���� 		��Ƭ�����������ʼ������ʽ����ӡ
	*
	*	�޸ļ�¼��	
	************************************************************
	************************************************************
	************************************************************
**/

#include "sys.h"
#include "uart.h"
#include "led.h"

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((UART5->SR&0X40)==0);//ѭ������,ֱ���������   
	UART5->DR = (u8) ch;      
	return ch;
}
#endif 

/*
************************************************************
*	�������ƣ�	Uart5_Init
*
*	�������ܣ�	����5��ʼ��
*
*	��ڲ�����	baud���趨�Ĳ�����
*
*	���ز�����	��
*
*	˵����		TX-PC12		RX-PD2		
************************************************************
*/
void UART5_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;        

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	         	//USART5 TX 
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    	//����������� 
  	GPIO_Init(GPIOC, &GPIO_InitStructure);	    	 //c�˿�  

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	         	 //USART5 RX 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //���ÿ�©���� 
  	GPIO_Init(GPIOD, &GPIO_InitStructure);	         	 //D�˿�  
   
  	USART_InitStructure.USART_BaudRate   = 115200; 
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//����λ8λ 
  	USART_InitStructure.USART_StopBits   = USART_StopBits_1;	//ֹͣλ1λ 
  	USART_InitStructure.USART_Parity     = USART_Parity_No;	//��У��λ 
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������ 
  	USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ 
   
  	/* Configure UART5 */ 
  	USART_Init(UART5, &USART_InitStructure);	//���ô��ڲ�������  
   
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;     //UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

  	/*USART5 receive data register is not empty */ 
  	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);   
  	/* Enable the USART5 */ 
  	USART_Cmd(UART5, ENABLE);	   
}

/*
************************************************************
*	�������ƣ�	UART5_IRQHandler
*
*	�������ܣ�	����5�շ��ж�
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
#ifdef EN_UART5_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART5_RX_BUF[USART_REC_LEN];     //���ջ���,���64���ֽ�.
//����״̬
//bit7��������ɱ�־
//bit6�����յ�0x0d
//bit5~0�����յ�����Ч�ֽ���Ŀ
u8 USART5_RX_STA = 0;       //����״̬���	  

void UART5_IRQHandler(void)
{
	u8 res;	    
	if(UART5->SR&(1<<5))//���յ�����
	{	 
		res = UART5->DR; 
		if((USART5_RX_STA&0x80)==0)//����δ���
		{
			if(USART5_RX_STA&0x40)//���յ���0x0d
			{
				if(res!=0x0a)USART5_RX_STA=0;//���մ���,���¿�ʼ
				else USART5_RX_STA|=0x80;	//��������� 
			}else //��û�յ�0X0D
			{	
				if(res==0x0d)USART5_RX_STA|=0x40;
				else
				{
					USART5_RX_BUF[USART5_RX_STA & 0X3F] = res;
					USART5_RX_STA++;
					if(USART5_RX_STA > (USART_REC_LEN-1))USART5_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}  		 									     
	}  											 
} 
#endif



