#ifndef __QUEUE_H
#define __QUEUE_H	
#include "sys.h"

void initQueue(u8 *q);		//初始化/清除数组
void resetQueue(void);
bool isEmpty(u8 *q);			//是否为空？
void enqueue(u8 *q, u8 elem);//入队
u8 dequeue(u8 *q);//出队
u8 getLenQueue(u8 *q);

#endif 
