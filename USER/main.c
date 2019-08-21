#include "sys.h"
#include "led.h"
#include "lock.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "exti.h"
#include "as608.h"
#include "finger.h"

bool flagFhave=false;
extern bool flagLed;
extern bool flagFP;		//0---FingerPress����ָ���£�1--û����ָ����
extern u16 ValidN;
extern u32 AS608Addr;
extern SysPara AS608Para;
extern u16 USART2_RX_STA; 

//�ⲿ�ж���������
//en:true������;false���ر�;  
void EXTI_Set(u8 n,bool en)
{
    EXTI->PR=1<<n;  //���LINEN�ϵ��жϱ�־λ
    if(en)EXTI->IMR|=1<<n;//������lineN�ϵ��ж�
    else EXTI->IMR&=~(1<<n);//����lineN�ϵ��ж�   
}

int main(void)
{	
	u8 ensure,i;	
	
	delay_init();	    	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	uart_init(115200);	 
	
	uart2_init(57600);
	TIM2_Int_Init(99,7199);		//10ms�ж�
	USART2_RX_STA=0;			//����
	TIM_Cmd(TIM2,DISABLE);		//�رն�ʱ��2
	
	LED_Init();			     
	EXTIX_Init();
	
	PS_ValidTempleteNum(&ValidN);
	ensure=PS_ReadSysPara(&AS608Para);  //������ 
	if(ensure==0x00)
	{
		printf("Valid N=%d\n",ValidN);
	}
	else
		ShowErrMessage(ensure);		
	delay_ms(1000);
	printf("Start!\n");
	while(1)	
	{
		
		if(flagFP)
		{
			
			EXTI_Set(0,false);	//�ر�Line0�ж�
			for(i=0;i<3;i++)
			{
				if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))
					break;
				press_FR();
				if(flagFhave)
				{
					flagFhave=false;
					break;
				}
				printf("##please try again!\n");
//				delay_ms(100);
			}
			if(i>2)
			{
				printf("no your finger!\n");
				Add_FR();
			}
			EXTI_Set(0,true);	//����Line0�ж�
			flagFP=false;
		}
		
		/*
		if(flagLed)
			GPIO_SetBits(GPIOB,GPIO_Pin_12);	
		else
			GPIO_ResetBits(GPIOB,GPIO_Pin_12);	
		flagLed=!flagLed;
		*/
		
//		delay_ms(100);
	}	 
}





