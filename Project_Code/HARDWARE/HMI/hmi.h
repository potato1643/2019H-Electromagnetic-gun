#ifndef __HMI_H
#define __HMI_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//蜂鸣器驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
//蜂鸣器端口定义
//#define BEEP PBout(8)	// BEEP,蜂鸣器接口		   

//void BEEP_Init(void);	//初始化
void HMISendString(char *buf1);
void HMISendb(u8 buf);
void beepms(u16 va);

		 				    
#endif
