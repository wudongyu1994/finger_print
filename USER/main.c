#include "sys.h"
#include "led.h"
#include "lock.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "exti.h"
#include "as608.h"
#include "finger.h"		

extern bool flagFP;		//0---FingerPress有手指按下；1--没有手指按下
extern u16 ValidN;
extern SysPara AS608Para;
extern u16 USART2_RX_STA; 

int main(void)
{	
	SearchResult search;
	//初始化
	delay_init();	    	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	uart_init(115200);	 
	
	uart2_init(57600);			//下面3行必须紧跟这行
	TIM2_Int_Init(99,7199);		//10ms中断
	USART2_RX_STA=0;			//清零
	TIM_Cmd(TIM2,DISABLE);		//关闭定时器2
	
	LED_Init();			     
	EXTIX_Init();
	
	PS_ValidTempleteNum(&ValidN);		//读取目前有效指纹个数
	PS_ReadSysPara(&AS608Para);  		//读取指纹模块里各个参数 
	printf("Start!\n");					//开始
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





