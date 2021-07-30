#include "sys.h"
#include "uart4.h"	  
#include "uart.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.



/*
************************************************************
*	�������ƣ�	Uart4_Init
*
*	�������ܣ�	����4��ʼ��
*
*	��ڲ�����	baud���趨�Ĳ�����
*
*	���ز�����	��
*
*	˵����		TX-PC10		RX-PC11		(PartialRemap)
************************************************************
*/





 
 
#if EN_UART4_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 UART4_RX_BUF[UART4_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 UART4_RX_STA=0;       //����״̬���	  

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

    //Usart1 NVIC ����
    nvicInitStruct.NVIC_IRQChannel = UART4_IRQn;
    nvicInitStruct.NVIC_IRQChannelCmd = ENABLE;
    nvicInitStruct.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�3
    nvicInitStruct.NVIC_IRQChannelSubPriority = 3;
    NVIC_Init(&nvicInitStruct);
    
    //USART ��ʼ������
    uart4InitStruct.USART_BaudRate = baud;
    uart4InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//��Ӳ������
    uart4InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;						//���պͷ���
    uart4InitStruct.USART_Parity = USART_Parity_No;									//��У��
    uart4InitStruct.USART_StopBits = USART_StopBits_1;								//1λֹͣλ
    uart4InitStruct.USART_WordLength = USART_WordLength_8b;							//8λ����λ
    USART_Init(UART4, &uart4InitStruct);
    
    USART_Cmd(UART4, ENABLE);														//ʹ�ܴ���
    
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);									//ʹ�ܽ����ж�
	

}


void UART4_IRQHandler(void)                	//����4�жϷ������
{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(UART4);	//��ȡ���յ�������
		
		if((UART4_RX_STA&0x8000)==0)//����δ���
			{
			if(UART4_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)UART4_RX_STA=0;//���մ���,���¿�ʼ
				else UART4_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)UART4_RX_STA|=0x4000;
				else
					{
					UART4_RX_BUF[UART4_RX_STA&0X3FFF]=Res ;
					UART4_RX_STA++;
					if(UART4_RX_STA>(UART4_REC_LEN-1))UART4_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
}
#endif	

