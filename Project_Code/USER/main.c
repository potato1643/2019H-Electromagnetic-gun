/****************************************
PA15:AD0   PB10:IIC_SCL   PB11:IIC_SDA
PA1:IIC_SCL_OLED      PA3:IIC_SDA_OLED
****************************************/
#include "oled.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "uart.h"
#include "stdio.h"
#include "led.h"
#include "beep.h"
#include "timer.h"


int main(void)
{	 
	float pitch,roll,yaw; 		//欧拉角
	short aacx,aacy,aacz;		  //加速度传感器原始数据
	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
	short temp;								//温度	
	u16 angle_yaw = 21;
	u16 angle_roll = 22;

	delay_init();				       //延时初始化

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	LED_Init();         //LED初始化
	BEEP_Init();        //BEEP初始化
	OLED_Init();
	OLED_ColorTurn(0);         //0正常显示，1 反色显示
  	OLED_DisplayTurn(0);       //0正常显示 1 屏幕翻转显示
	MPU_Init();					       //初始化MPU6050
	USART1_Init();
	 
	TIM3_PWM_Init(199, 7199);	
	TIM1_PWM_Init(199, 7199); 

	OLED_ShowString(15,0,"Waiting",24,1);
	OLED_ShowString(0,28,"Initialise",24,1);
	OLED_Refresh();
	
	//Servo初始化————转90度
	TIM_SetCompare1(TIM1, 17);			//TIM1_CH1_PA8_垂直方向_ROLL
	//经过实验这里为17; 理论上为15;
	
	delay_ms(5000);
	TIM_SetCompare2(TIM3, 16);			//TIM3_CH2_PB5_水平方向_YAW    //SetCompare这里定义里面更改过
	//经过实验这里为16; 理论上为15;
	delay_ms(5000);

	while(mpu_dmp_init())
 	{
		OLED_Clear();
		OLED_Refresh();
	}
	OLED_Clear();
	OLED_Refresh();
	//OLED_ShowString(0,0,"PITCH:",16,1);
	OLED_ShowString(0,15,"ROLL:",16,1);
	OLED_ShowString(0,31,"YAW:",16,1);
	//OLED_ShowString(0,47,"TEMP:",16,1);
	OLED_Refresh();

	
	
 	while(1)
	{

		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{
			temp=MPU_Get_Temperature();							  //得到温度值
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
			
			LED0 = 0;//LED点亮
			BEEP = 1;
			//BEEP = 1;//BEEP关闭
			//printf("Pitch:  %f\r\n",(float)pitch);
			printf("Roll:  %f\r\n",(float)roll);
			printf("yaw:  %f\r\n",(float)yaw);
			//printf("temp:  %f\r\n",(float)temp);
			printf(" \r\n");
			
			//OLED_ShowFloat(50, 0,pitch,1,16,1);
			OLED_ShowFloat(50,15,roll,5,16,1);
			OLED_ShowFloat(50,31,yaw,5,16,1);
			//OLED_ShowFloat(50,47,(float)temp/100,5,16,1);
			OLED_Refresh();
			
			TIM_SetCompare2(TIM3, angle_yaw);
			TIM_SetCompare1(TIM1, angle_roll);
			
			


		}

	} 	
}
