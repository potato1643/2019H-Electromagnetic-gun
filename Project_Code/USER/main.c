#include "sys.h"
#include "delay.h"
#include "usart.h"	//system�ļ����е�ͷ�ļ�

#include "led.h"    //LEDͷ�ļ�
#include "oled.h"   //OLEDͷ�ļ�
//#include "bmp.h"��ʾ��Ƭ
 
 
/************************************************
Time��2021/07/23
Aim��2019����H�⡪��ģ������
1.1	����LED-Driver
1.2	����0.96OLED-Driver 
0.96OLED		128��64(������/�ֱ���)

************************************************/





/**
*****************����ע�ӵĴ�����ͨ�����ÿ⺯����ʵ��IO���Ƶķ���*****************************************/
int main(void)
{
	u8 t;
 
	delay_init();		  	//��ʼ����ʱ����
	LED_Init();		      //��ʼ��LED�˿�
	OLED_Init();				//��ʼ��OLED�˿�
	
	OLED_ColorTurn(0);      //0������ʾ��1 ��ɫ��ʾ
	OLED_DisplayTurn(0);    //0������ʾ 1 ��Ļ��ת��ʾ
	OLED_Refresh();         //�����Դ浽OLED
	t=' ';
	LED0 = 0;
	//LED0 = ~LED0;
	
	while(1)
	{
		LED0 = ~LED0;
		//LED0 = 0;                            //LED0��Ӧ��������͵�ƽ��LED0����
		//GPIO_ResetBits(GPIOA, GPIO_Pin_2);  //LED0��Ӧ����GPIOA.2���ͣ���  ��ͬLED0=0;
		
		//delay_ms(300);  		             //��ʱ300ms

		//LED0 = 1;                          //LED0��Ӧ��������ߵ�ƽ��LED0Ϩ��
		//GPIO_SetBits(GPIOA,GPIO_Pin_2);	   //LED0��Ӧ����GPIOB.5���ߣ���  ��ͬLED0=1;
		//delay_ms(300);                     //��ʱ300ms
		//OLED_ShowPicture(0,0,128,8,BMP1);
		//delay_ms(500);
		//OLED_Clear();
		// OLED_ShowChinese(0,0,0,16);//��
		// OLED_ShowChinese(18,0,1,16);//��
		// OLED_ShowChinese(36,0,2,16);//԰
		// OLED_ShowChinese(54,0,3,16);//��
		// OLED_ShowChinese(72,0,4,16);//��
		// OLED_ShowChinese(90,0,5,16);//��
		// OLED_ShowChinese(108,0,6,16);//��
		OLED_ShowString(0, 0, "ABCDEFGHIJKLMNOP", 16);          //128 / 8 = 16;
		//OLED_ShowString(0, 32, "12345678910111213", 16);          //128 / 16 = 8;
	
		//OLED_ShowString(20,32,"2014/05/01",16);
		//OLED_ShowString(20,32,"2020/10/02",16);
		
		OLED_ShowString(0,48,"ASCII:",16);  
		OLED_ShowChar(48,48,t,16);//��ʾASCII�ַ�	   

		t++;
		OLED_ShowString(64, 48, "CODE:", 16);     //64,ǰ���ܹ�56,��һ��
		if(t > '~') t=' ';
		OLED_ShowNum(104,48,t,3,16);    //63 + 5*8 = 103; 103 + 3*8 = 127
		OLED_Refresh();				//�����Դ�
		delay_ms(500);

		// OLED_Clear();				//����������ҳ
		// OLED_ShowChinese(0,0,0,16);  //16*16 ��
	    // OLED_ShowChinese(16,0,0,24); //24*24 ��
		// OLED_ShowChinese(24,20,0,32);//32*32 ��
	    // OLED_ShowChinese(64,0,0,64); //64*64 ��
		// OLED_Refresh();
	    // delay_ms(500);

    	// OLED_Clear();
		// OLED_ShowString(0,0,"ABC",12);//6*12 ��ABC��
	    // OLED_ShowString(0,12,"ABC",16);//8*16 ��ABC��
		// OLED_ShowString(0,28,"ABC",24);//12*24 ��ABC��
	    // OLED_Refresh();
		// delay_ms(500);
	}
	
} 
 

 
   

	
 

	
/**
*******************����ע�͵��Ĵ�����ͨ�� ֱ�Ӳ����Ĵ��� ��ʽʵ��IO�ڿ���**************************************
int main(void)
{ 
 
	delay_init();		  //��ʼ����ʱ����
	LED_Init();		        //��ʼ��LED�˿�
	while(1)
	{
     GPIOB->BRR=GPIO_Pin_5;//LED0��
	   GPIOE->BSRR=GPIO_Pin_5;//LED1��
		 delay_ms(300);
     GPIOB->BSRR=GPIO_Pin_5;//LED0��
	   GPIOE->BRR=GPIO_Pin_5;//LED1��
		 delay_ms(300);

	 }
 }
**************************************************************************************************
**/

