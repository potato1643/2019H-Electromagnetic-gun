#include "led.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
//ALIENTEK Mini STM32开发板范例代码8
//PWM输出实验   
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司

 int main(void)
 {	
	  
	
	delay_init();	    	 //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口
	TIM1_PWM_Init(199,7199);//不分频。PWM频率===(7200*200)/72000000=0.02=20ms
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

