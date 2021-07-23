#include "sys.h"
#include "delay.h"
#include "usart.h"	//system文件夹中的头文件

#include "led.h"    //LED头文件
#include "oled.h"   //OLED头文件
//#include "bmp.h"显示照片
 
 
/************************************************
Time：2021/07/23
Aim：2019电赛H题——模拟电磁炮
1.1	加入LED-Driver
1.2	加入0.96OLED-Driver 
0.96OLED		128×64(像素率/分辨率)

************************************************/





/**
*****************下面注视的代码是通过调用库函数来实现IO控制的方法*****************************************/
int main(void)
{
	u8 t;
 
	delay_init();		  	//初始化延时函数
	LED_Init();		      //初始化LED端口
	OLED_Init();				//初始化OLED端口
	
	OLED_ColorTurn(0);      //0正常显示，1 反色显示
	OLED_DisplayTurn(0);    //0正常显示 1 屏幕翻转显示
	OLED_Refresh();         //更新显存到OLED
	t=' ';
	LED0 = 0;
	//LED0 = ~LED0;
	
	while(1)
	{
		LED0 = ~LED0;
		//LED0 = 0;                            //LED0对应引脚输出低电平，LED0点亮
		//GPIO_ResetBits(GPIOA, GPIO_Pin_2);  //LED0对应引脚GPIOA.2拉低，亮  等同LED0=0;
		
		//delay_ms(300);  		             //延时300ms

		//LED0 = 1;                          //LED0对应引脚输出高电平，LED0熄灭
		//GPIO_SetBits(GPIOA,GPIO_Pin_2);	   //LED0对应引脚GPIOB.5拉高，灭  等同LED0=1;
		//delay_ms(300);                     //延时300ms
		//OLED_ShowPicture(0,0,128,8,BMP1);
		//delay_ms(500);
		//OLED_Clear();
		// OLED_ShowChinese(0,0,0,16);//中
		// OLED_ShowChinese(18,0,1,16);//景
		// OLED_ShowChinese(36,0,2,16);//园
		// OLED_ShowChinese(54,0,3,16);//电
		// OLED_ShowChinese(72,0,4,16);//子
		// OLED_ShowChinese(90,0,5,16);//科
		// OLED_ShowChinese(108,0,6,16);//技
		OLED_ShowString(0, 0, "ABCDEFGHIJKLMNOP", 16);          //128 / 8 = 16;
		//OLED_ShowString(0, 32, "12345678910111213", 16);          //128 / 16 = 8;
	
		//OLED_ShowString(20,32,"2014/05/01",16);
		//OLED_ShowString(20,32,"2020/10/02",16);
		
		OLED_ShowString(0,48,"ASCII:",16);  
		OLED_ShowChar(48,48,t,16);//显示ASCII字符	   

		t++;
		OLED_ShowString(64, 48, "CODE:", 16);     //64,前面总共56,空一个
		if(t > '~') t=' ';
		OLED_ShowNum(104,48,t,3,16);    //63 + 5*8 = 103; 103 + 3*8 = 127
		OLED_Refresh();				//更新显存
		delay_ms(500);

		// OLED_Clear();				//清屏——换页
		// OLED_ShowChinese(0,0,0,16);  //16*16 中
	    // OLED_ShowChinese(16,0,0,24); //24*24 中
		// OLED_ShowChinese(24,20,0,32);//32*32 中
	    // OLED_ShowChinese(64,0,0,64); //64*64 中
		// OLED_Refresh();
	    // delay_ms(500);

    	// OLED_Clear();
		// OLED_ShowString(0,0,"ABC",12);//6*12 “ABC”
	    // OLED_ShowString(0,12,"ABC",16);//8*16 “ABC”
		// OLED_ShowString(0,28,"ABC",24);//12*24 “ABC”
	    // OLED_Refresh();
		// delay_ms(500);
	}
	
} 
 

 
   

	
 

	
/**
*******************下面注释掉的代码是通过 直接操作寄存器 方式实现IO口控制**************************************
int main(void)
{ 
 
	delay_init();		  //初始化延时函数
	LED_Init();		        //初始化LED端口
	while(1)
	{
     GPIOB->BRR=GPIO_Pin_5;//LED0亮
	   GPIOE->BSRR=GPIO_Pin_5;//LED1灭
		 delay_ms(300);
     GPIOB->BSRR=GPIO_Pin_5;//LED0灭
	   GPIOE->BRR=GPIO_Pin_5;//LED1亮
		 delay_ms(300);

	 }
 }
**************************************************************************************************
**/

