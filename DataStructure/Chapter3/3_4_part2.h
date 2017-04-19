//3_4_part1.h
/**
author:zhaoyu
email:zhaoyu1995.com@gmail.com
date:2016-6-9
note:realize my textbook <<数据结构（C语言版）>>
*/
//Page 64
#include <cstdio>
#include "head.h"
#define QElemType int
//----循环队列：队列的顺序存储结构----
#define MAXQSIZE 10 //最大队列长度
typedef struct{
	QElemType *base;
	int front;
	int rear;
}SqQueue;
//----循环队列的基本操作说明及实现----
Status InitQueue(SqQueue &Q)
{
	//构造一个空队列 Q
	Q.base = (QElemType *)malloc(MAXQSIZE*sizeof(QElemType));
	if (!Q.base)
	{
		exit(OVERFLOW);
	}
	Q.front = Q.rear = 0;
	return OK;
}
int QueueLength(SqQueue Q)
{
	//返回 Q 的元素个数，即队列的长度
	return (Q.rear - Q.front + MAXQSIZE) % MAXQSIZE;
}
Status EnQueue(SqQueue &Q, QElemType e)
{
	//插入元素 e 为 Q 的新的队尾元素
	if ((Q.rear+1)%MAXQSIZE == Q.front)
	{
		return ERROR;//队列满
	}
	Q.base[Q.rear] = e;
	Q.rear = (Q.rear+1)%MAXQSIZE;
	return OK;
}
Status DeQueue(SqQueue &Q, QElemType &e)
{
	//若队列不空，则删除 Q 的队列头元素，用 e 返回其值，
	//并返回 OK，否则返回 ERROR
	if (Q.front == Q.rear)
	{
		return ERROR;
	}
	e = Q.base[Q.front];
	Q.front = (Q.front+1)%MAXQSIZE;
	return OK;
}
void PrintQueue(SqQueue Q)
{
	int cnt = Q.front;
	if (Q.front == Q.rear)
	{
		printf("void\n");
		return;
	}
	while ((cnt+1)%MAXQSIZE != Q.rear)
	{	
		//printf("%d\t%d\n",Q.base[cnt++], cnt);输出好奇怪
		printf("%d\t", Q.base[cnt]);
		cnt++;
	}
	printf("%d\n", Q.base[cnt]);
}