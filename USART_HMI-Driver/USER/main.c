/****************************************
PA15:AD0   PB10:IIC_SCL   PB11:IIC_SDA
PA1:IIC_SCL_OLED      PC3:IIC_SDA_OLED
PA2:LED0
****************************************/
#include "oled.h"
#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "usart.h"
#include "stdio.h"
#include "led.h"
#include "beep.h"
#include "timer.h"
#include "hmi.h"
#include "key.h"


//USART_DEBUG �������� USART1

int main(void)
{	 
	float pitch,roll,yaw; 		//ŷ����
	short aacx,aacy,aacz;		  //���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;	//������ԭʼ����
	//short temp;								//�¶�	
	u16 angle_yaw;
	u16 angle_roll;
	s16 angle;
	s16 distance;
	//u16 data = 986;
	u16 len;
	//int b;
	u16 t,i;
	u8 men[5];
	vu8 key=0;

	delay_init();				       //��ʱ��ʼ��

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	//LED_Init();         //LED��ʼ��
	BEEP_Init();        //BEEP��ʼ��
	KEY_Init();         //������ʼ��
	OLED_Init();
	OLED_ColorTurn(0);         //0������ʾ��1 ��ɫ��ʾ
  	OLED_DisplayTurn(0);       //0������ʾ 1 ��Ļ��ת��ʾ
	MPU_Init();					       //��ʼ��MPU6050
	
	//Usart1_Init(115200);		//USART1��ʼ������USART_DEBUG
	uart_init(115200);
	Usart2_Init(115200);		//USART2��ʼ������������USART HMI������ͨ��

	
	TIM3_PWM_Init(199, 7199);	
	TIM1_PWM_Init(199, 7199); 

	OLED_ShowString(15,0,"Waiting",24,1);
	OLED_ShowString(0,28,"Initialise",24,1);
	OLED_Refresh();
	
	//Servo��ʼ����������ת90��
	TIM_SetCompare1(TIM1, 17);			//TIM1_CH1_PA8_��ֱ����_ROLL
	//����ʵ������Ϊ17; ������Ϊ15;
	
	delay_ms(5000);
	TIM_SetCompare2(TIM3, 16);			//TIM3_CH2_PB5_ˮƽ����_YAW    //SetCompare���ﶨ��������Ĺ�
	//����ʵ������Ϊ16; ������Ϊ15;
	delay_ms(5000);

	while(mpu_dmp_init())
 	{
		OLED_Clear();
		OLED_Refresh();
	}
	OLED_Clear();
	OLED_Refresh();
	//OLED_ShowString(0,0,"PITCH:",16,1);
	OLED_ShowString(0, 15,"ROLL:", 16, 1);
	OLED_ShowString(0, 31, "YAW:", 16, 1);
	//OLED_ShowString(0,47,"TEMP:",16,1);
	OLED_Refresh();

 	while(1)
	{
		
		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{
			//temp=MPU_Get_Temperature();							  //�õ��¶�ֵ
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
			
			//LED0 = 0;//LED����
			BEEP = 1;
			//BEEP = 1;//BEEP�ر�
			//printf("Pitch:  %f\r\n",(float)pitch);

			//UsartPrintf(USART2, "Roll:  %f\r\n", (float)roll);
			//UsartPrintf(USART2, "yaw:  %f\r\n", (float)yaw);
			//printf("temp:  %f\r\n",(float)temp);
			//UsartPrintf(USART2, " \r\n");

			if(USART_RX_STA&0x8000)
			{					   
				len=USART_RX_STA&0x3fff;//�õ��˴ν������ݵĳ���
				UsartPrintf(USART2, "\r\n�����͵���ϢΪ:\r\n\r\n");
				for(t = 0; t < len; t++)
				{
					men[t] = USART_RX_BUF[t];//�򴮿�һ��������
					//USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�һ��������
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
				}
				for(i = 0; i < len; ++i)
				{
					men[i] = men[i] & 0x000f;
					//UsartPrintf(USART2, "%x",men[i]);
				}

				// key=KEY_Scan(0);	//�õ���ֵ
				// if(key)
				// {
				// 	switch(key)
				// 	{
				// 		case KEY0_PRES:	//yaw
				// 			if(men[0] == 1)
				// 			angle = 10*men[1] + men[2];
				// 			else
				// 			{
				// 				angle = 10*men[1] + men[2];
				// 				angle = -angle;
				// 			}
				// 			break;

				// 		case KEY1_PRES:   //roll
				// 			if(men[0] == 1)
				// 			distance = 100*men[0] + 10*men[1] + men[2];
				// 			else
				// 			{
				// 				distance = 100*men[0] + 10*men[1] + men[2];
				// 				distance = -distance;
				// 			}
				// 			break;
				// 	}
				// }
				if(men[0] == 1)
				angle = 10*men[1] + men[2];
				else
				{
					angle = 10*men[1] + men[2];
					angle = -angle;
				}
			
				// if(men[0] == 1)
				// distance = 100*men[1] + 10*men[2] + men[3];
				// else
				// {
				// 	distance = 100*men[1] + 10*men[2] + men[3];
				// 	distance = -distance;
				// }


				//UsartPrintf(USART2, "Yaw: %d\r\n", angle);
				//UsartPrintf(USART2, "Distance: %d\r\n", distance);
				//b=(USART_RX_BUF[0]);
				//UsartPrintf(USART2, "Sucess Received!!!!\r\n");
				//UsartPrintf(USART2, "%d",b);
				//UsartPrintf(USART2, "\r\n\r\n");
				USART_RX_STA=0;
			}

			//UsartPrintf(USART_DEBUG, "n1.val=%d\xff\xff\xff", data);
			//UsartPrintf(USART_DEBUG, "t0.txt=\"World\"\xff\xff\xff");

			//OLED_ShowFloat(50, 0,pitch,1,16,1);
			OLED_ShowFloat(50,15,roll,5,16,1);
			OLED_ShowFloat(50,31,yaw,5,16,1);
			//OLED_ShowFloat(50,47,(float)temp/100,5,16,1);
			OLED_Refresh();

			switch (angle)   //90��Ϊ�������ߡ�������16
			{
			case 30://1-30(1-����; 0-����)    //��Ӧ120��
				angle_yaw = 20;
				break;
			case -30://0-30       //��Ӧ60��
				angle_yaw = 13;
				break;
			case 15:
				angle_yaw = 18;
				break;
			case -15:
				angle_yaw = 14;
				break;
			default: 
				angle_yaw = 16;
				break;
			}

			switch (distance)		////90��Ϊ�������ߡ�������17; 120cm
			{
			case -175://-10�㡪������100
				angle_roll = 18;
				break;
			case -225://-15��������105
				angle_roll = 19;
				break;
			case -230://-20��������110
				angle_roll = 20;
				break;
			case -295://-30��������120
				angle_roll = 21;
				break;
			case -315://-35��������125
				angle_roll = 22;
				break;
			case -350://-40��������130
				angle_roll = 23;
				break;
			case -325://-45��������135
				angle_roll = 24;
				break;
			
			
			default:
				angle_roll = 17;
				break;
			}
			
			
			TIM_SetCompare2(TIM3, angle_yaw);
			//delay_ms(5000);
			//TIM_SetCompare1(TIM1, angle_roll);


			
		}
		


	} 	
}
