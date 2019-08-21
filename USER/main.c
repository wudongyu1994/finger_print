#include "sys.h"
#include "led.h"
#include "lock.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "exti.h"
#include "as608.h"
#include "finger.h"		

extern bool flagFP;		//0---FingerPress����ָ���£�1--û����ָ����
extern u16 ValidN;
extern SysPara AS608Para;
extern u16 USART2_RX_STA; 

int main(void)
{	
	SearchResult search;
	//��ʼ��
	delay_init();	    	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	uart_init(115200);	 
	
	uart2_init(57600);			//����3�б����������
	TIM2_Int_Init(99,7199);		//10ms�ж�
	USART2_RX_STA=0;			//����
	TIM_Cmd(TIM2,DISABLE);		//�رն�ʱ��2
	
	LED_Init();			     
	EXTIX_Init();
	
	PS_ValidTempleteNum(&ValidN);		//��ȡĿǰ��Чָ�Ƹ���
	PS_ReadSysPara(&AS608Para);  		//��ȡָ��ģ����������� 
	printf("Start!\n");					//��ʼ
	while(1)	
	{
		if(checkFinger(&search)){
			printf("finger exist!#ID=%d--score=%d\n",search.pageID,search.mathscore);
		}else{
			printf("no your finger!\n");
			Add_FR();
		}
	}	 
}





