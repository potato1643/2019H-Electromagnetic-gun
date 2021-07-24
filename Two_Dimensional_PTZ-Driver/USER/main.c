#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
 
/******
����180�ȶ�����ԣ���Ƕ�����λ����һһ��Ӧ��
***/
	
int main(void)
{		
 	//u16 led0pwmval=0;
	u16 angle_init_yaw = 16;
	u16 angle_init_roll = 18;

	u16 duty_cycle_high = 15;//ռ�ձȲ������������ߵ�ƽ����ʱ��; ռ�ձ� = �ߵ�ƽʱ�� / ����
	u8 dir=1;	
	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��

 	TIM3_PWM_Init(199, 7199);	
	TIM1_PWM_Init(199, 7199);  

	//��Ƶ;  72000000 / 7200 = 10Khz; 
	//10kHz / 200 = 50Hz; T = 0.02s = 20ms
	//�Զ���װ��ֵarr��������PWM������; pscΪԤ��Ƶϵ��
	//arr��������PWM����

	//Servo��ʼ����������ת90��
	TIM_SetCompare1(TIM1, 17);			//TIM1_CH1_PA8_��ֱ����_ROLL
	//����ʵ������Ϊ17; ������Ϊ15;
	
	delay_ms(5000);
	delay_ms(5000);
	TIM_SetCompare2(TIM3, 16);			//TIM3_CH2_PB5_ˮƽ����_YAW    //SetCompare���ﶨ��������Ĺ�
	//����ʵ������Ϊ16; ������Ϊ15;
	delay_ms(5000);


   	while(1)
	{
 		// delay_ms(10);	 
		// if(dir)led0pwmval++;
		// else led0pwmval--;

 		// if(led0pwmval>300)dir=0;
		// if(led0pwmval==0)dir=1;										 
		// TIM_SetCompare2(TIM3,10);			//TIM3_CH2_PB5_ˮƽ����_YAW
		// delay_ms(500);
		// TIM_SetCompare2(TIM3,15);
		// delay_ms(500);

		// TIM_SetCompare1(TIM1, 10);			//TIM1_CH1_PA8_��ֱ����_ROLL
		// delay_ms(500);
		// TIM_SetCompare1(TIM1, 15);
		// delay_ms(500);
		//����ȽϼĴ��������趨ռ�ձ�	
		//450	   
	}	 
}

