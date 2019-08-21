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

//�ⲿ�ж���������
//en:true������;false���ر�;  
void EXTI_Set(u8 n,bool en)
{
    EXTI->PR=1<<n;  //���LINEN�ϵ��жϱ�־λ
    if(en)EXTI->IMR|=1<<n;//������lineN�ϵ��ж�
    else EXTI->IMR&=~(1<<n);//����lineN�ϵ��ж�   
}
/*########################################################################
��������	bool Add_FR(void)
�������ܣ�	¼��ָ�ƣ����ü������ָ�ƣ�
����ֵ��	bool�࣬true--�ɹ����ָ�ƣ�false--���ָ��ʧ��
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
					ensure=PS_GenChar(CharBuffer1);//��������
					if(ensure==0x00)
					{
						#if DEBUG
						printf("finger ok\n##move finger away!\n");
						#endif
						i=0;
						processnum=1;//�����ڶ���						
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
					ensure=PS_GenChar(CharBuffer2);//��������
					if(ensure==0x00)
					{
						#if DEBUG
						printf("finger ok again\n##move finger away!\n");
						#endif
						i=0;
						processnum=2;//����������
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
					processnum=3;//�������Ĳ�
				}
				else 
				{
					#if DEBUG
					printf("finger match failed.\n");
					#endif
					ShowErrMessage(ensure);
					i=0;
					processnum=0;//���ص�һ��		
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
					processnum=4;//�������岽
				}else {processnum=0;ShowErrMessage(ensure);}
//				delay_ms(1200);
				break;
				
			case 4:	
				#if DEBUG
				printf("storing finger...\n");
				#endif
				ensure=PS_StoreChar(CharBuffer2,++ValidN);//����ģ��
				if(ensure==0x00) 
				{		
					PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
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
		if(i==5)//����5��û�а���ָ���˳�
			return isAddOk;	
	}
}


/*########################################################################
��������	void press_FR(void)
�������ܣ�	ʶ��ָ��1��
����ֵ��	bool�࣬true--�ɹ���������ָ�ƣ�false--����ָ��ʧ��
##########################################################################*/
bool press_FR(SearchResult *seach)
{
	u8 ensure;
	
	ensure=PS_GetImage();
	if(ensure==0x00)//��ȡͼ��ɹ� 
	{	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //���������ɹ�
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,AS608Para.PS_max,seach);
			if(ensure==0x00)//�����ɹ�
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
��������	bool checkFinger(void)
�������ܣ�	ʶ��ָ��n=3�Σ����ü����ж��Ƿ���ڸ�ָ�ƣ�
����ֵ��	bool�࣬true--�ɹ���������ָ�ƣ�false--����ָ��ʧ��
##########################################################################*/
bool checkFinger(SearchResult *search)
{
	u8 i;
	
	while(!flagFP);
	EXTI_Set(0,false);	//�ر�Line0�ж�
	for(i=0;i<3;i++)
	{
		if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0))
			break;
		if(press_FR(search))
		{
			EXTI_Set(0,true);	//����Line0�ж�
			flagFP=false;
			return true;
		}
		else
			printf("##please try again!\n");
//		delay_ms(100);
	}
	EXTI_Set(0,true);	//����Line0�ж�
	flagFP=false;
	return false;
}
