#ifndef __SYS_H
#define __SYS_H	
#include "stm32f10x.h"
#include <stdbool.h>
#include <stdio.h>
 
#define CLI()  	__set_PRIMASK(1)  	//关总中断
#define SEI() 	__set_PRIMASK(0)  	//开
#define DEBUG	1					//0---正常模式; 1--调试模式

//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址

#endif
