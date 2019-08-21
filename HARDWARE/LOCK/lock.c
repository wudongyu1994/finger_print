#include "lock.h"
#include "timer.h" 


void Lock_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //??PB,PE????

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //LED0-->PB.5 ????
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO????50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //?????????GPIOB.5
	GPIO_SetBits(GPIOA,GPIO_Pin_0);						 //PB.5 ???

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //LED0-->PB.5 ????
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //????
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //?????????GPIOB.5
}

void Unlock(void)
{
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	#if DEBUG
	printf("Unlock\n");
	#endif
}

void Lock(void)
{
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
	#if DEBUG
	printf("Lock\n");
	#endif
}
