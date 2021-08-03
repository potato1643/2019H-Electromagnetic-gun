#include "usart.h"
#include "oled.h"
#include "stdio.h"
#include "openmv.h"

static u8 Cx = 0,Cy = 0,Cw = 0,Ch = 0;
u8 CX = 0, CY = 0, CW = 0, CH = 0;

//USART3 全局中断服务函数
void USART3_IRQHandler(void)			 
{
		u8 com_data; 
		u8 i;
		static u8 RxCounter3=0;
		static u16 RxBuffer3[10]={0};
		static u8 RxState = 0;	
		static u8 RxFlag1 = 0;

		if(USART_GetITStatus(USART3,USART_IT_RXNE) != RESET)  	   //接收中断  
		{
				USART_ClearITPendingBit(USART3,USART_IT_RXNE);   //清除中断标志
				com_data = USART_ReceiveData(USART3);
			
				if(RxState == 0 && com_data == 0x2C)  //0x2c帧头
				{
					RxState=1;
					RxBuffer3[RxCounter3++]=com_data;
					//OLED_Refresh();
				}
		
				else if(RxState==1&&com_data==0x12)  //0x12帧头
				{
					RxState=2;
					RxBuffer3[RxCounter3++]=com_data;
				}
		
				else if(RxState == 2)
				{
					RxBuffer3[RxCounter3++]=com_data;

					if(RxCounter3 >= 10 || com_data == 0x5B)       //RxBuffer1接受满了,接收数据结束
					{
						RxState=3;
						RxFlag1=1;
						Cx=RxBuffer3[RxCounter3-5];
						Cy=RxBuffer3[RxCounter3-4];
						Cw=RxBuffer3[RxCounter3-3];
						Ch=RxBuffer3[RxCounter3-2];

					}
				}
		
				else if(RxState==3)		//检测是否接受到结束标志
				{
						if(RxBuffer3[RxCounter3-1] == 0x5B)
						{
									USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);//关闭DTSABLE中断
									if(RxFlag1)
									{
                                    //OLED_Clear();
									//OLED_Refresh();
									CX = Cx;
									CY = Cy;
									CW=  Cw;
									CH = Ch;
									//OLED_ShowNum(0, 0,Cx,3,16,1);
									//OLED_ShowNum(0,17,Cy,3,16,1);
									//OLED_ShowNum(0,33,Cw,3,16,1);
									//OLED_ShowNum(0,49,Ch,3,16,1);
									}
									RxFlag1 = 0;
									RxCounter3 = 0;
									RxState = 0;
									USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
						}
						else   //接收错误
						{
									RxState = 0;
									RxCounter3=0;
									for(i=0;i<10;i++)
									{
											RxBuffer3[i]=0x00;      //将存放数据数组清零
									}
						}
				} 
	
				else   //接收异常
				{
						RxState = 0;
						RxCounter3=0;
						for(i=0;i<10;i++)
						{
								RxBuffer3[i]=0x00;      //将存放数据数组清零
						}
				}

		}
		
}
