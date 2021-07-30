#include "stm32f10x.h"
#include "uart.h"
#include "delay.h"
#include "hcsr04.h"
#include "usart.h"
#include "stdio.h"


int main()
{
	float length = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		
	delay_init();
	uart_init(115200);
	printf("串口初始化成功！\r\n");
	Hcsr04Init();
	printf("HcSr04初始化成功！\r\n");
	while(1)
	{
		length = Hcsr04GetLength();
		printf("距离为 %f cm\r\n",length);
	}
	
}
