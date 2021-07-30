#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
//ALIENTEK战舰STM32开发板实验4
//串口实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 
 int main(void)
 {		
 	u8 t;
	u8 len;	
	u16 times=0;
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	//uart_init(9600);	 //串口初始化为9600
    USART5_Configuration();
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
 	while(1)
	  {
			printf("正点原子@ALIENTEK\n\n\n");
	     //printf("正点原子@ALIENTEK\n\n\n");
		// printf("\n您发送的消息为:\n");
//		 if(USART_RX_STA&0x80)
//		{					   
//			len=USART_RX_STA&0x3f;//得到此次接收到的数据长度
//		//	printf("\n您发送的消息为:\n");
//			for(t=0;t<len;t++)
//			{
//				UART5->DR=USART_RX_BUF[t];
//				while((UART5->SR&0X40)==0);//等待发送结束
//			}
//		//	printf("\n\n");//插入换行
//			LED1=!LED1;
//			USART_RX_STA=0;
//		}else
//		{
//			times++;
//			if(times%5000==0)
//			{
//				printf("\nMiniSTM32开发板 串口实验\n");
//				printf("正点原子@ALIENTEK\n\n\n");
//			}
//			if(times%200==0)printf("请输入数据,以回车键结束\n");  
//			if(times%30==0)LED0=!LED0;//闪烁LED,提示系统正在运行.
//			delay_ms(10);   
//		}
//		    LED0=!LED0;//闪烁LED,提示系统正在运行.
//			delay_ms(50);   
		}
		 
 }

