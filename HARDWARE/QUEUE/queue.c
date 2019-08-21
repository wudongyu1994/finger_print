 #include "queue.h"

u8 head,tail;   
u8 cmd[400];

extern u8 qLen;
struct Queue  *myQueue;

void initQueue(u8 *q)
{
	int i;
	head=0;tail=0;
	for(i=0;i<256;i++)
		cmd[i]=0;
}

void resetQueue(void)
{
	head=0;tail=0;
}

bool isEmpty(u8 *q)
{
	if(head==tail)
		return true;
	else
		return false;
}

void enqueue(u8 *q, u8 elem)
{ 
	q[++tail]=elem;
} 

u8 dequeue(u8 *q)
{    
	if(!isEmpty(q))
	{
		return q[head++];
	}	
	else
		return 0xff;
}  

u8 getLenQueue(u8 *q)
{
	return tail-head;
}

//u8 queueLen(struct Queue *q){ 
//	int sum,i;
//	for(i=q->top+1;i<q->rear;i++)
//		sum++;
//	return sum;
//}

