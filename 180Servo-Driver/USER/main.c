#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
 

	
int main(void)
{		
 	//u16 led0pwmval=0;
	u16 duty_cycle_high = 15;//占空比参数————高电平持续时间; 占空比 = 高电平时间 / 周期
	u8 dir=1;	
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();			     //LED端口初始化

 	TIM3_PWM_Init(199,7199);	 
	//分频;  72000000 / 7200 = 10Khz; 
	//10kHz / 200 = 50Hz; T = 0.02s = 20ms
	//自动重装载值arr用来控制PWM的周期; psc为预分频系数
	//arr————PWM周期
   	while(1)
	{
 		// delay_ms(10);	 
		// if(dir)led0pwmval++;
		// else led0pwmval--;

 		// if(led0pwmval>300)dir=0;
		// if(led0pwmval==0)dir=1;										 
		TIM_SetCompare2(TIM3,10);
		delay_ms(1000);
		TIM_SetCompare2(TIM3,15);
		delay_ms(1000);

		//捕获比较寄存器的来设定占空比	
		//450	   
	}	 
}

