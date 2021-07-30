#include "hcsr04.h"
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"


#define TRIG_Send  PAout(0)
#define ECHO_Reci  PAin(1)

u16 msHcCount = 0;//ms计数

/*  对应IO口初始化  */
void Hcsr04Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;     
    GPIO_InitTypeDef GPIO_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   //使能对应RCC时钟
	
    //PA0_CH1  Trig
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       	  	 //发送电平引脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	
		//PA1_CH2  Echo	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;    		 			 //返回电平引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    GPIO_ResetBits(GPIOA,GPIO_Pin_1);    
    
		TIM_DeInit(TIM2);
		TIM_TimeBaseStructure.TIM_Period = (1000-1); 									//设置在下一个更新事件装入活动的自动重装载寄存器周期的值,计数到1000为1ms
		TIM_TimeBaseStructure.TIM_Prescaler =(72-1);								  //设置用来作为TIMx时钟频率除数的预分频值  1M的计数频率 1US计数
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;					//不分频
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//TIM向上计数模式
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 				       
		
		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);   									  //打开定时器更新中断
		
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;           	  //选择串口1中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     //抢占式中断优先级设置为1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            //响应式中断优先级设置为1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //使能中断
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2,DISABLE);     
}


static void OpenTimerForHc()        //打开定时器
{
        TIM_SetCounter(TIM2,0);			//清除计数
        msHcCount = 0;
        TIM_Cmd(TIM2, ENABLE);  		//使能TIM2外设
}
 
static void CloseTimerForHc()        //关闭定时器
{
        TIM_Cmd(TIM2, DISABLE);  //使能TIM2外设
}
 
 

//定时器1中断服务程序
void TIM2_IRQHandler(void)   //TIM2中断
{
        if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM2更新中断发生与否
        {
                TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIM2更新中断标志 
                msHcCount++;
        }
}
 

//获取定时器时间
u32 GetEchoTimer(void)
{
        u32 t = 0;
        t = msHcCount*1000;					//得到us
        t += TIM_GetCounter(TIM2);  //得到us
        TIM2->CNT = 0;  					  //将TIM2计数寄存器的计数值清零
        delay_ms(50);
        return t;
}
 

//一次获取超声波测距数据 两次测距之间需要相隔一段时间，隔断回响信号
//为了消除余震的影响，取五次数据的平均值进行加权滤波。
float Hcsr04GetLength(void)
{
        u32 t = 0;
        int i = 0;
        float lengthTemp = 0;
        float sum = 0;
        while(i!=5)
        {
					TRIG_Send = 1;      					//发送口高电平输出
					delay_us(20);									//20us脉冲
					TRIG_Send = 0;								//脉冲发送完成
					/*等待接收口高电平输出*/
					while(ECHO_Reci == 0);      	
					OpenTimerForHc();        			//打开定时器
					i = i + 1;
					/*等待高电平输出结束*/
					while(ECHO_Reci == 1);
					CloseTimerForHc();        		//关闭定时器
					t = GetEchoTimer();        		//获取时间,分辨率为1US
					lengthTemp = ((float)t/58.0);	//cm
					sum = lengthTemp + sum ;	
				}
        lengthTemp = sum/5.0; 				  //平均滤波
        return lengthTemp;
}

