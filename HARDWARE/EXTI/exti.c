#include "exti.h"
#include "lock.h"
#include "timer.h"
#include "delay.h"

extern bool flagFP;

//�ⲿ�ж�0�������
void EXTIX_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
   	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB,ENABLE);	//ʹ�ܸ��ù���ʱ��

	//GPIOB.0
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//GPIOB.0	 
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;	//0--->1
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;			//�����ȼ�1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  
}

//�ⲿ�ж�0�������
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);  //���LINE0�ϵ��жϱ�־λ
		
		delay_ms(50);
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))
		{
			flagFP=true;
		}
		
	}
}

