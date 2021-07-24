#include "hcsr04.h"
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"


#define TRIG_Send  PAout(0)
#define ECHO_Reci  PAin(1)

u16 msHcCount = 0;//ms����

/*  ��ӦIO�ڳ�ʼ��  */
void Hcsr04Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;     
    GPIO_InitTypeDef GPIO_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   //ʹ�ܶ�ӦRCCʱ��
	
    //PA0_CH1  Trig
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       	  	 //���͵�ƽ����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	
		//PA1_CH2  Echo	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;    		 			 //���ص�ƽ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    GPIO_ResetBits(GPIOA,GPIO_Pin_1);    
    
		TIM_DeInit(TIM2);
		TIM_TimeBaseStructure.TIM_Period = (1000-1); 									//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ,������1000Ϊ1ms
		TIM_TimeBaseStructure.TIM_Prescaler =(72-1);								  //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  1M�ļ���Ƶ�� 1US����
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;					//����Ƶ
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//TIM���ϼ���ģʽ
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 				       
		
		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);   									  //�򿪶�ʱ�������ж�
		
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;           	  //ѡ�񴮿�1�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     //��ռʽ�ж����ȼ�����Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            //��Ӧʽ�ж����ȼ�����Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //ʹ���ж�
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2,DISABLE);     
}


static void OpenTimerForHc()        //�򿪶�ʱ��
{
        TIM_SetCounter(TIM2,0);			//�������
        msHcCount = 0;
        TIM_Cmd(TIM2, ENABLE);  		//ʹ��TIM2����
}
 
static void CloseTimerForHc()        //�رն�ʱ��
{
        TIM_Cmd(TIM2, DISABLE);  //ʹ��TIM2����
}
 
 

//��ʱ��1�жϷ������
void TIM2_IRQHandler(void)   //TIM2�ж�
{
        if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM2�����жϷ������
        {
                TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIM2�����жϱ�־ 
                msHcCount++;
        }
}
 

//��ȡ��ʱ��ʱ��
u32 GetEchoTimer(void)
{
        u32 t = 0;
        t = msHcCount*1000;					//�õ�us
        t += TIM_GetCounter(TIM2);  //�õ�us
        TIM2->CNT = 0;  					  //��TIM2�����Ĵ����ļ���ֵ����
        delay_ms(50);
        return t;
}
 

//һ�λ�ȡ������������� ���β��֮����Ҫ���һ��ʱ�䣬���ϻ����ź�
//Ϊ�����������Ӱ�죬ȡ������ݵ�ƽ��ֵ���м�Ȩ�˲���
float Hcsr04GetLength(void)
{
        u32 t = 0;
        int i = 0;
        float lengthTemp = 0;
        float sum = 0;
        while(i!=5)
        {
					TRIG_Send = 1;      					//���Ϳڸߵ�ƽ���
					delay_us(20);									//20us����
					TRIG_Send = 0;								//���巢�����
					/*�ȴ����տڸߵ�ƽ���*/
					while(ECHO_Reci == 0);      	
					OpenTimerForHc();        			//�򿪶�ʱ��
					i = i + 1;
					/*�ȴ��ߵ�ƽ�������*/
					while(ECHO_Reci == 1);
					CloseTimerForHc();        		//�رն�ʱ��
					t = GetEchoTimer();        		//��ȡʱ��,�ֱ���Ϊ1US
					lengthTemp = ((float)t/58.0);	//cm
					sum = lengthTemp + sum ;	
				}
        lengthTemp = sum/5.0; 				  //ƽ���˲�
        return lengthTemp;
}

