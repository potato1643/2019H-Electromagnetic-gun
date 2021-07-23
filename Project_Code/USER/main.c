#include "sys.h"
#include "delay.h"
#include "usart.h"	//system文件夹中的头文件


#include "led.h"    //LED头文件
#include "oled.h"   //OLED头文件
//#include "bmp.h"显示照片
#include "usmart.h"   

#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 


/************************************************
Time：2021/07/23
Aim：2019电赛H题——模拟电磁炮
1.1	加入LED-Driver
1.2	加入0.96OLED-Driver 
0.96OLED		128×64(像素率/分辨率)

yaw航向角————云台与中心轴线的偏离角度α
			 -30° —— +30°之间

roll横滚角————云台垂直方向的俯仰角,对应基本要求第二小问,距离d的变动
			  0° —— -30°之间（待定）

pitch俯仰角

************************************************/
//函数声明
void usart1_send_char(u8 c);
void usart1_niming_report(u8 fun,u8*data,u8 len);
void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz);
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw);



int main(void)
{
	u8 point = '.';   //char

	u8 t=0,report=1;			//默认开启上报
	u8 key;
	float pitch,roll,yaw; 		//欧拉角
	short aacx,aacy,aacz;		//加速度传感器原始数据
	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
	short temp;					//温度	


	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
	uart_init(115200);  //初始化串口uart
	delay_init();		  	//初始化延时函数
	usmart_dev.init(72);		//初始化USMART
	LED_Init();		      //初始化LED端口
	OLED_Init();				//初始化OLED端口
	MPU_Init();					//初始化MPU6050
	
	OLED_ColorTurn(0);      //0正常显示，1 反色显示
	OLED_DisplayTurn(0);    //0正常显示 1 屏幕翻转显示
	OLED_Refresh();         //更新显存到OLED
	LED0 = 0;
	
	//OLED_ShowString(0, 0, "Pitch->>", 16);//64
	OLED_ShowString(0, 16, "Roll->>", 16);//56
	OLED_ShowString(0, 32, "Yaw->>", 16);//48

	OLED_Refresh();				//更新显存
	//delay_ms(500);

	
	while(mpu_dmp_init())
 	{
		printf("MPU6050 Error");
		delay_ms(200);
	}  
	
	while(1)
	{
		//LED0 = !LED0;

		if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
		{ 
			temp=MPU_Get_Temperature();	//得到温度值
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
			//if(report)mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);//用自定义帧发送加速度和陀螺仪原始数据
			//if(report)usart1_report_imu(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*10));
			if((t%10)==0)
			{ 
				//printf("pitch→%f\r\n", pitch);
				printf("roll:%f\r\n", roll);
				printf("yaw:%f\r\n", yaw);
				printf("\r\n");
				
				//符号位
				if(roll < 0) {
					OLED_ShowString(56, 16, "-", 16);
					roll = -roll;
					OLED_ShowNum(64, 16, roll, 2, 16);//64 + 2*8 = 80;
					OLED_ShowString(80, 16, "C", 16);
				}
				else {
					OLED_ShowString(56, 16, "+", 16);
					OLED_ShowNum(64, 16, roll, 2, 16);//64 + 2*8 = 80;
					OLED_ShowString(80, 16, "C", 16);
				}
				OLED_Refresh();
				//OLED_ShowNum(64, 16, roll, 2, 16);//64 + 2*8 = 80;
				//OLED_ShowString(80, 16, "C", 16);
				//OLED_ShowChar(80, 16, point, 16);

				if(yaw < 0) {
					OLED_ShowString(48, 32, "-", 16);
					yaw = -yaw;
					OLED_ShowNum(56, 32, yaw, 2, 16);//56 + 2*8 = 72;
					OLED_ShowString(72, 32, "C", 16);
				}
				else {
					OLED_ShowString(48, 32, "+", 16);
					OLED_ShowNum(56, 32, yaw, 2, 16);//56 + 2*8 = 72;
					OLED_ShowString(72, 32, "C", 16);
				}
				//OLED_ShowNum(56, 32, yaw, 2, 16);//56 + 2*8 = 72;
				//OLED_ShowString(72, 32, "C", 16);
				//OLED_ShowChar(72, 32, point, 16);
				OLED_Refresh();				//更新显存
				
				t=0;
			}
		}
		t++; 	
	}
} 


//串口1发送1个字符 
//c:要发送的字符
void usart1_send_char(u8 c)
{   	
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
	USART_SendData(USART1,c);  
} 
//传送数据给匿名四轴上位机软件(V2.6版本)
//fun:功能字. 0XA0~0XAF
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数

void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//最多28字节数据 
	send_buf[len+3]=0;	//校验数置零
	send_buf[0]=0X88;	//帧头
	send_buf[1]=fun;	//功能字
	send_buf[2]=len;	//数据长度
	for(i=0;i<len;i++)send_buf[3+i]=data[i];			//复制数据
	for(i=0;i<len+3;i++)send_buf[len+3]+=send_buf[i];	//计算校验和	
	for(i=0;i<len+4;i++)usart1_send_char(send_buf[i]);	//发送数据到串口1 
}
//发送加速度传感器数据和陀螺仪数据
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值

void mpu6050_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
	u8 tbuf[12]; 
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
	usart1_niming_report(0XA1,tbuf,12);//自定义帧,0XA1
}	
//通过串口1上报结算后的姿态数据给电脑
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值
//roll:横滚角.单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
//pitch:俯仰角.单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
//yaw:航向角.单位为0.1度 0 -> 3600  对应 0 -> 360.0度
void usart1_report_imu(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz,short roll,short pitch,short yaw)
{
	u8 tbuf[28]; 
	u8 i;
	for(i=0;i<28;i++)tbuf[i]=0;//清0
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;	
	tbuf[18]=(roll>>8)&0XFF;
	tbuf[19]=roll&0XFF;
	tbuf[20]=(pitch>>8)&0XFF;
	tbuf[21]=pitch&0XFF;
	tbuf[22]=(yaw>>8)&0XFF;
	tbuf[23]=yaw&0XFF;
	usart1_niming_report(0XAF,tbuf,28);//飞控显示帧,0XAF
}  

 	
 


