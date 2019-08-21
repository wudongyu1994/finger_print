#include "exti.h"
#include "lock.h"
#include "timer.h"
#include "delay.h"

extern bool flagFP;

//外部中断0服务程序
void EXTIX_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
   	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB,ENABLE);	//使能复用功能时钟

	//GPIOB.0
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//GPIOB.0	 
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//0--->1
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;			//子优先级1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  
}

//外部中断0服务程序
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);  //清除LINE0上的中断标志位
		
		delay_ms(50);
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))
		{
			flagFP=true;
		}
		
	}
}

