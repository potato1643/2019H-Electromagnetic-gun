#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
//ALIENTEK Mini STM32�����巶������8
//PWM���ʵ��   
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾

 int main(void)
 {	
	  
	
	delay_init();	    	 //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	TIM1_PWM_Init(199,7199);//����Ƶ��PWMƵ��===(7200*200)/72000000=0.02=20ms
   	while(1)
	{
		
    TIM_SetCompare1(TIM1,190); //-45,1ms

     delay_ms(20000); 

    TIM_SetCompare1(TIM1,185); //0,1.5ms

     delay_ms(6000);		
//		TIM_SetCompare1(TIM1,180); //45,2ms
//     delay_ms(20000);
//		
//		TIM_SetCompare1(TIM1,175); 

//     delay_ms(2000);
//		
//		TIM_SetCompare1(TIM1,170); 
//		
//		 delay_ms(1500);
//		 
//		TIM_SetCompare1(TIM1,175); 
//		
//		 delay_ms(1500);
//		 
//		 TIM_SetCompare1(TIM1,180); 
//		
//		 delay_ms(1500);
//		 
//		 TIM_SetCompare1(TIM1,185); 
//		
//		 delay_ms(1500);
//		 
//		
		
	} 
}

