#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
//ALIENTEKս��STM32������ʵ��4
//����ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 
 int main(void)
 {		
 	u8 t;
	u8 len;	
	u16 times=0;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	//uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
    USART5_Configuration();
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
 	while(1)
	  {
			printf("����ԭ��@ALIENTEK\n\n\n");
	     //printf("����ԭ��@ALIENTEK\n\n\n");
		// printf("\n�����͵���ϢΪ:\n");
//		 if(USART_RX_STA&0x80)
//		{					   
//			len=USART_RX_STA&0x3f;//�õ��˴ν��յ������ݳ���
//		//	printf("\n�����͵���ϢΪ:\n");
//			for(t=0;t<len;t++)
//			{
//				UART5->DR=USART_RX_BUF[t];
//				while((UART5->SR&0X40)==0);//�ȴ����ͽ���
//			}
//		//	printf("\n\n");//���뻻��
//			LED1=!LED1;
//			USART_RX_STA=0;
//		}else
//		{
//			times++;
//			if(times%5000==0)
//			{
//				printf("\nMiniSTM32������ ����ʵ��\n");
//				printf("����ԭ��@ALIENTEK\n\n\n");
//			}
//			if(times%200==0)printf("����������,�Իس�������\n");  
//			if(times%30==0)LED0=!LED0;//��˸LED,��ʾϵͳ��������.
//			delay_ms(10);   
//		}
//		    LED0=!LED0;//��˸LED,��ʾϵͳ��������.
//			delay_ms(50);   
		}
		 
 }

