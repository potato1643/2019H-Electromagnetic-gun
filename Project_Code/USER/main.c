/*******************************************************
Pin��
PA15:AD0   PB10:IIC_SCL_MPU6050   PB11:IIC_SDA_MPU6050
PA1:IIC_SCL_OLED      PC3:IIC_SDA_OLED
PA2:LED0
********************************************************/
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
//#include "hmi.h"
#include "key.h"
#include "openmv.h"
//#include "raspberry.h"
#include "math.h"

//ȫ�ֱ���
extern u8 CX, CY, CW, CH;		//Openmv����
//extern u8 USART2_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
//extern u8 USART2_RX_STA;         		//����״̬���	

int main(void)
{	 
	float pitch,roll,yaw; 		//ŷ����
	short aacx,aacy,aacz;		  //���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;	//������ԭʼ����
	//short temp;								//�¶�	
	u16 angle_yaw;
	u16 angle_roll;
	//u16 yaw_flag = 0;
	//u16 roll_flag = 0;
	s16 angle = 0;
	s16 distance = 0;
	s16 angle_distance = 0;
	//u16 data = 986;
	u16 len;
	u16 usart2_len;
	//int b;
	u16 t,i;
	u8 men[5];
	u8 usart2[5];
	//u16 usart2[200];
	vu8 key=0;
	u16 openmv_flag = 0;
	u16 yaw_flag = 0;
	u16 roll_flag = 0;
	u32 Intermediate_Variable;
	u32 Distance_From_Center;
	float Angle_K = 0;
	u16 Angle = 0;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

	delay_init();		//��ʱ��ʼ��
	LED_Init();         //LED��ʼ��
	BEEP_Init();        //BEEP��ʼ��
	KEY_Init();         //������ʼ��
	OLED_Init();		//OLED��ʼ��
	OLED_ColorTurn(0);         //0������ʾ��1 ��ɫ��ʾ
  	OLED_DisplayTurn(0);       //0������ʾ 1 ��Ļ��ת��ʾ
	MPU_Init();					       //��ʼ��MPU6050
	
	Usart1_Init(115200);		//USART1��ʼ����������������USART HMI������ͨ��
	Usart2_Init(115200);		//USART2��ʼ����������������raspberry-piͨ��
	Usart3_Init(115200);		//USART3��ʼ����������������Openmvͨ��
	
	//UART5_Init(115200);			//UART5��ʼ��������������UART_DEBUG
	UART5_Configuration();		//UART5��ʼ��������������UART_DEBUG


	TIM3_PWM_Init(7199, 199);	//72000000 / 200 = 360kHz; 360kHz / 7200 = 50Hz = 0.02s = 20ms
							 	//7200����20ms; 1ms����360; 0.5ms����180

	OLED_ShowString(15,0,"Waiting",24,1);
	OLED_ShowString(0,28,"Initialise",24,1);
	OLED_Refresh();

	//180��Servo��ʼ����������ת90��
	//180����0��; 4������1��
	TIM_SetCompare1(TIM3, 590);			//TIM3_CH1_PA6_��ֱ����_ROLL
	//����ʵ������Ϊ590; ������Ϊ540;
	
	//delay_ms(5000);
	TIM_SetCompare2(TIM3, 580);			//TIM3_CH2_PA7_ˮƽ����_YAW    
	//����ʵ������Ϊ580; ������Ϊ540;
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


	LED0 = 0;//LED����

 	while(1)
	{

		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{
			//temp=MPU_Get_Temperature();				//�õ��¶�ֵ
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������
			
			
			//BEEP = 1;//BEEP�ر�
			//printf("Pitch:  %f\r\n",(float)pitch);

			//UsartPrintf(USART_DEBUG, "Roll:  %f\r\n", (float)roll);
			//UsartPrintf(USART_DEBUG, "yaw:  %f\r\n", (float)yaw);
			//UsartPrintf(USART4, "Roll:  %f\r\n", (float)roll);
			//printf("temp:  %f\r\n",(float)temp);
			//UsartPrintf(USART2, " \r\n");

			//UsartPrintf(USART_DEBUG, "n1.val=%d\xff\xff\xff", data);
			//UsartPrintf(USART_DEBUG, "t0.txt=\"World\"\xff\xff\xff");

			//OLED_ShowFloat(50, 0,pitch,1,16,1);
			OLED_ShowFloat(50,15,roll,5,16,1);
			OLED_ShowFloat(50,31,yaw,5,16,1);
			//OLED_ShowFloat(50,47,(float)temp/100,5,16,1);
			OLED_Refresh();
			//printf("Roll:  %f\r\n", (float)roll);

			if(USART_RX_STA&0x8000)//HMI��������������
			{					   
				len=USART_RX_STA&0x3fff;//�õ��˴ν������ݵĳ���
				//UsartPrintf(USART2, "\r\n�����͵���ϢΪ:\r\n\r\n");
				for(t = 0; t < len; t++)
				{
					men[t] = USART_RX_BUF[t];//�򴮿�һ��������
					//USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�һ��������
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
				}
				for(i = 0; i < len; ++i)
				{
					men[i] = men[i] & 0x000f;
					//printf("%d",men[i]);
					//printf("\r\n");
				}

				/*ģʽѡ��*/
				if(men[0] == 9)//���̿���yaw
				{
					if(men[1] == 1) {
					angle = 10*men[2] + men[3];
					
					}
					else
					{
						angle = 10*men[2] + men[3];
						angle = -angle;
					}
				}
			
				if(men[0] == 7)//���̿���roll����distance
				{
					distance = 100*men[1] + 10*men[2] + men[3];
					//angle_distance = ;
				}

				if(men[0] == 1)//���̿���openmv
				{
					if(men[1] == 1)
					openmv_flag = 1;
					else
					openmv_flag = 0;
				}
				
				//printf("Roll:  %f\r\n", (float)roll);
				//UsartPrintf(USART_DEBUG, "Yaw: %d\r\n", angle);
				//UsartPrintf(USART2, "Distance: %d\r\n", distance);
				//b=(USART_RX_BUF[0]);
				//UsartPrintf(USART2, "Sucess Received!!!!\r\n");
				//UsartPrintf(USART2, "%d",b);
				//UsartPrintf(USART2, "\r\n\r\n");
				USART_RX_STA = 0;
			}

			// if(USART2_RX_STA & 0x8000)//Raspberry��������
			// {					   
			// 	usart2_len = USART2_RX_STA & 0x3fff;//�õ��˴ν������ݵĳ���
			// 	//UsartPrintf(USART2, "\r\n�����͵���ϢΪ:\r\n\r\n");
			// 	for(t = 0; t < usart2_len; t++)
			// 	{
			// 		usart2[t] = USART2_RX_BUF[t];//�򴮿�һ��������
			// 		//USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�һ��������
			// 		while(USART_GetFlagStatus(USART2,USART_FLAG_TC) != SET);//�ȴ����ͽ���
			// 	}
			// 	for(i = 0; i < 5; ++i)
			// 	{
			// 		//men[i] = men[i] & 0x000f;
			// 		printf("%c",usart2[i]);
			// 		//printf("\r\n");
					
			// 	}
			// 	USART2_RX_STA = 0;
			// }
				

			//��ʱ��ö��
			switch (distance)		////90��Ϊ�������ߡ�������600; 120cm
			{
			case 175://-10�㡪������100
				angle_distance = 10;
				break;
			case 225://-15��������105
				angle_distance = 15;
				break;
			case 230://-20��������110
				angle_distance = 20;
				break;
			case 295://-30��������120
				angle_distance = 30;
				break;
			case 315://-35��������125
				angle_distance = 35;
				break;
			case 350://-40��������130
				angle_distance = 40;
				break;
			case 325://-45��������135
				angle_distance = 45;
				break;
			default:
				angle_distance = 0;
				break;
			}

			if(angle >= 0) {	//û��angle����ʱ��angle = 0
			angle_yaw = 580 + angle*4;
			yaw_flag = 1;
			}
			else
			angle_yaw = 580 + angle*4;

			if(angle_distance >= 0) {
				angle_roll = 590 + angle_distance*4;
				roll_flag = 1;
			}
			//printf("angle_roll: %d\r\n", angle_roll);

			//TIM_SetCompare2(TIM3, angle_yaw);
			//delay_ms(5000);
			//����������ʱ����ȻMPU6050���ݴ����ٶȲ������ת�٣������MPU6050�޷�׼ȷ��ʾyaw������
			//TIM_SetCompare1(TIM3, angle_roll);


			if(yaw_flag == 1)
			{
				TIM_SetCompare2(TIM3, angle_yaw);
				//yaw_flag = 0;

			}

			if(roll_flag == 1)
			{
				TIM_SetCompare1(TIM3, angle_roll);
				//roll_flag = 0;
			}
			
			if (openmv_flag == 1)
			{
				//�ȸ�λ
				TIM_SetCompare1(TIM3, 590);			//TIM3_CH1_PA6_��ֱ����_ROLL
				//����ʵ������Ϊ590; ������Ϊ540;
	
				//delay_ms(5000);
				TIM_SetCompare2(TIM3, 580);			//TIM3_CH2_PA7_ˮƽ����_YAW    
				delay_ms(1000);

				//-30�㡪�� +30��
				TIM_SetCompare2(TIM3, 460);
				delay_ms(1000);
				TIM_SetCompare2(TIM3, 700);
				delay_ms(1000);
				printf("CX = %d\r\n", CX);
				printf("CY = %d\r\n", CY);
				printf("CW = %d\r\n", CW);
				printf("CH = %d\r\n", CH);
				printf("\r\n");
				Intermediate_Variable = (CX-80)*(CX-80) + (CY-60)*(CY-60);
				//printf("Intermediate_Variable: %d\r\n",Intermediate_Variable);
				Distance_From_Center = sqrt(Intermediate_Variable);
				//printf("Distance_From_Center: %d\r\n", Distance_From_Center);
				Angle_K = 20 / 56.0357;
				//printf("Angle_K: %f", Angle_K);
				Angle = Distance_From_Center*Angle_K;
				printf("Angle: %d��\r\n", Angle);

				if(Angle >= 65)
				{
					openmv_flag = 2;
				}

			}
			if (openmv_flag == 2)
			{
				TIM_SetCompare1(TIM3, 590);			//TIM3_CH1_PA6_��ֱ����_ROLL
				//����ʵ������Ϊ590; ������Ϊ540;
	
				//delay_ms(5000);
				TIM_SetCompare2(TIM3, 700);	
				delay_ms(5000);
				openmv_flag = 0;
			}
			
			
		}

		
		//�����Ŀ��Ҫ���
		

		


	} 	
}
	
