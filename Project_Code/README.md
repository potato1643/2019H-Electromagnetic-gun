# 外设模块接口说明

> OLED:
>
> SCL——PA1
>
> SDA——PA3
>
> MPU6050:
>
> SCL——PB10
>
> SDA——PB11
>
> Openmv：
>
> UART_RX——P5		PB11(STM32H743)
>
> UART_TX——P4		PB10(STM32H743)
>
> STM32F103：
>
> USART3_TX——PD8
>
> USART3_RX——PD9

## 底层驱动实现

OLED + MPU6050 + Beep + LED + Two_Dimensional_PTZ + USART1_3 + HMI + Button（舍弃了，直接用串口屏实现）+ 180°Servo 精确控制 + USART2/USART3端口重映射 + PWM端口更改 + Openmv + uart.c文件替换 + Raspberry-USART2通信

USART1-3	USART_COMMUNICATION

UART4/5	UART_DEBUG

一.接线
模块共有:Stm32f103C8T6、OLED、MPU6050
①OLED:          SCL->PA1     SDA->PA3                       供电为3.3V
②MPU6050:   SCL->PB10   SDA->PB11  AD0接PA15   供电为5V

二.MPU6050
1.mpuiic编写关于MPU6050的IIC的一系列函数；
2.mpu6050存放MPU6050寄存器的地址的宏定义、MPU6050初始化函数、
