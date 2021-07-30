# Openmv-Driver

## 外设模块接口说明

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

openmv.c

> void USART3_IRQHandler(void)

openmv.h



