#ifndef __QUEUE_H
#define __QUEUE_H	
#include "sys.h"

void initQueue(u8 *q);		//��ʼ��/�������
void resetQueue(void);
bool isEmpty(u8 *q);			//�Ƿ�Ϊ�գ�
void enqueue(u8 *q, u8 elem);//���
u8 dequeue(u8 *q);//����
u8 getLenQueue(u8 *q);

#endif 
