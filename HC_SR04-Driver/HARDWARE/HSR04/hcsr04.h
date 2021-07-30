#ifndef __HCSR04_H
#define __HCSR04_H

#include "sys.h"

void Hcsr04Init(void);
void TIM2_RQHandler(void);  
u32 GetEchoTimer(void);
float Hcsr04GetLength(void);


#endif
