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
	printf("���ڳ�ʼ���ɹ���\r\n");
	Hcsr04Init();
	printf("HcSr04��ʼ���ɹ���\r\n");
	while(1)
	{
		length = Hcsr04GetLength();
		printf("����Ϊ %f cm\r\n",length);
	}
	
}
