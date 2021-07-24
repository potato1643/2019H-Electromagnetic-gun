#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
 
/************************************************
 ALIENTEKս��STM32������ʵ��9
 PWM���ʵ��  
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

	
int main(void)
{		
 	u16 led0pwmval=0;
	u8 dir=1;	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
 	TIM3_PWM_Init(199, 7199);	//��Ƶ
	 							//PWMƵ��72000000 Hz / 7200 = 10000Hz; 10000 / 200 = 50Hz; T = 0.02s = 20ms
   	while(1)
	{
 		// delay_ms(10);	 
		// if(dir)led0pwmval++;
		// else led0pwmval--;

 		// if(led0pwmval>300)dir=0;
		// if(led0pwmval==0)dir=1;										 
		TIM_SetCompare2(TIM3, 190);		 
		delay_ms(1000);
		TIM_SetCompare2(TIM3, 185);		 
		delay_ms(1000);
	}	 
}

