#ifndef __SYS_H
#define __SYS_H	
#include "stm32f10x.h"
#include <stdbool.h>
#include <stdio.h>
 
#define CLI()  	__set_PRIMASK(1)  	//�����ж�
#define SEI() 	__set_PRIMASK(0)  	//��
#define DEBUG	1					//0---����ģʽ; 1--����ģʽ

//����Ϊ��ຯ��
void WFI_SET(void);		//ִ��WFIָ��
void INTX_DISABLE(void);//�ر������ж�
void INTX_ENABLE(void);	//���������ж�
void MSR_MSP(u32 addr);	//���ö�ջ��ַ

#endif
