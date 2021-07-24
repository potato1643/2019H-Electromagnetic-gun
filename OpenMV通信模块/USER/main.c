/******************************
SDA:PA2
SCL:PA1
******************************/

#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "uart.h"
#include "stdio.h"


int main(void)
{		

		OLED_Init();
		delay_init();	    	                              //延时函数初始化	  
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
		USART1_Init();
		OLED_ColorTurn(0);          //0正常显示，1 反色显示
		OLED_DisplayTurn(0);        //0正常显示 1 屏幕翻转显示
	 
		while(1) 
		{
			OLED_Refresh();
	
		}
	
}
