#include "finger.h"
#include "as608.h"
#include "delay.h"

void ShowErrMessage(u8 ensure)
{
	#if DEBUG
	printf((const char *)EnsureMessage(ensure));
	#endif
};
extern u16 ValidN;	
extern SysPara AS608Para;
extern bool flagFhave;	
//录指纹
void Add_FR(void)
{
	u8 i=0,ensure ,processnum=0;
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
//					delay_ms(1500);
					return ;
				}else {processnum=0;ShowErrMessage(ensure);}					
				break;				
		}
		delay_ms(400);
		if(i==5)//超过5次没有按手指则退出
			break;	
	}
}

//刷指纹
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	char *str;
	ensure=PS_GetImage();
	if(ensure==0x00)//获取图像成功 
	{	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //生成特征成功
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,AS608Para.PS_max,&seach);
			if(ensure==0x00)//搜索成功
			{	
				flagFhave=true;
				printf("finger exist!#ID=%d--score=%d",seach.pageID,seach.mathscore);
			}
			else 
				ShowErrMessage(ensure);					
	  }
		else
			ShowErrMessage(ensure);
	 delay_ms(600);
	}
		
}

