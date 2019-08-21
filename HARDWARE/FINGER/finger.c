#include "finger.h"
#include "as608.h"
#include "delay.h"
#include "exti.h"

extern bool flagFP;

void ShowErrMessage(u8 ensure)
{
	#if DEBUG
	printf((const char *)EnsureMessage(ensure));
	#endif
};
extern u16 ValidN;	
extern SysPara AS608Para;

//外部中断屏蔽设置
//en:true，开启;false，关闭;  
void EXTI_Set(u8 n,bool en)
{
    EXTI->PR=1<<n;  //清除LINEN上的中断标志位
    if(en)EXTI->IMR|=1<<n;//不屏蔽lineN上的中断
    else EXTI->IMR&=~(1<<n);//屏蔽lineN上的中断   
}
/*########################################################################
函数名：	bool Add_FR(void)
函数功能：	录入指纹，调用即可添加指纹，
返回值：	bool类，true--成功添加指纹；false--添加指纹失败
##########################################################################*/
bool Add_FR(void)
{
	u8 i=0,ensure ,processnum=0;
	bool isAddOk=false;
	while(1)
	{
		switch (processnum)
		{
			case 0:
				#if DEBUG
				printf("##press!\n");
				#endif
//				delay_ms(500);		
				i++;
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer1);//生成特征
					if(ensure==0x00)
					{
						#if DEBUG
						printf("finger ok\n##move finger away!\n");
						#endif
						i=0;
						processnum=1;//跳到第二步						
					}else ShowErrMessage(ensure);				
				}else ShowErrMessage(ensure);	
//				delay_ms(500);				
				break;
			
			case 1:
				#if DEBUG
				printf("##press again!\n");
				#endif
//				delay_ms(500);
				i++;
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer2);//生成特征
					if(ensure==0x00)
					{
						#if DEBUG
						printf("finger ok again\n##move finger away!\n");
						#endif
						i=0;
						processnum=2;//跳到第三步
					}else ShowErrMessage(ensure);	
				}else ShowErrMessage(ensure);
//				delay_ms(1200);	
				break;

			case 2:
				#if DEBUG
				printf("matching...\n");
				#endif
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					#if DEBUG
					printf("finger match ok\n");
					#endif
					processnum=3;//跳到第四步
				}
				else 
				{
					#if DEBUG
					printf("finger match failed.\n");
					#endif
					ShowErrMessage(ensure);
					i=0;
					processnum=0;//跳回第一步		
				}
//				delay_ms(1200);
				break;

			case 3:
				#if DEBUG
				printf("regModelling...\n");
				#endif
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					#if DEBUG
					printf("Model ok.\n");
					#endif
					processnum=4;//跳到第五步
				}else {processnum=0;ShowErrMessage(ensure);}
//				delay_ms(1200);
				break;
				
			case 4:	
				#if DEBUG
				printf("storing finger...\n");
				#endif
				ensure=PS_StoreChar(CharBuffer2,++ValidN);//储存模板
				if(ensure==0x00) 
				{		
					PS_ValidTempleteNum(&ValidN);//读库指纹个数
					#if DEBUG
					printf("store ok\n");
					printf("there is %d fingers\n",ValidN);
					#endif
					isAddOk=true;
//					delay_ms(1500);
					return isAddOk;
				}else {processnum=0;ShowErrMessage(ensure);}					
				break;				
		}
		delay_ms(400);
		if(i==5)//超过5次没有按手指则退出
			return isAddOk;	
	}
}


/*########################################################################
函数名：	void press_FR(void)
函数功能：	识别指纹1次
返回值：	bool类，true--成功搜索到该指纹；false--搜索指纹失败
##########################################################################*/
bool press_FR(SearchResult *seach)
{
	u8 ensure;
	
	ensure=PS_GetImage();
	if(ensure==0x00)//获取图像成功 
	{	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //生成特征成功
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,AS608Para.PS_max,seach);
			if(ensure==0x00)//搜索成功
			{	
				return true;
			}
			else 
			{
				ShowErrMessage(ensure);		
				return false;
			}				
		}
		else
		{
			ShowErrMessage(ensure);
			return false;
		}
	}
	else
	{
		ShowErrMessage(ensure);
		return false;
	}
	
}

/*########################################################################
函数名：	bool checkFinger(void)
函数功能：	识别指纹n=3次，调用即可判断是否存在该指纹，
返回值：	bool类，true--成功搜索到该指纹；false--搜索指纹失败
##########################################################################*/
bool checkFinger(SearchResult *search)
{
	u8 i;
	
	while(!flagFP);
	EXTI_Set(0,false);	//关闭Line0中断
	for(i=0;i<3;i++)
	{
		if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))
			break;
		if(press_FR(search))
		{
			EXTI_Set(0,true);	//开启Line0中断
			flagFP=false;
			return true;
		}
		else
			printf("##please try again!\n");
//		delay_ms(100);
	}
	EXTI_Set(0,true);	//开启Line0中断
	flagFP=false;
	return false;
}
