//3_4_part1.h
/**
author:zhaoyu
email:zhaoyu1995.com@gmail.com
date:2016-6-9
note:realize my textbook <<数据结构（C语言版）>>
*/
//Page 61
#include <cstdio>
#include "head.h"
#define QElemType int
//----单链队列：队列的链式存储结构----
typedef struct QNode{
	QElemType data;
	struct QNode *next;        
}QNode, *QueuePtr;
typedef struct{
	QueuePtr front;//队列头指针
	QueuePtr rear;//队列尾指针
}LinkQueue;
//基本操作的声明及实现
Status InitQueue(LinkQueue &Q)
{
	//构造一个空队列
	Q.front = Q.rear = (QueuePtr)malloc(sizeof(QNode));
	if (!Q.front)
	{
		exit(OVERFLOW);
	}
	Q.front->next = NULL;
	return OK;
}
Status DestroyQueue(LinkQueue &Q)
{
	//销毁队列 Q ，Q 不存在
	while (Q.front)
	{
		Q.rear = Q.front->next;
		free(Q.front);
		Q.front = Q.rear;
	}
	return OK;
}
Status ClearQueue(LinkQueue &Q)
{
	//将 Q 清为空队列
}
Status QueueEmpty(LinkQueue Q)
{
	//若队列 Q 为空，则返回 TRUE，否则返回 FALSE
}
int QueueLength(LinkQueue Q)
{
	//返回 Q 的元素个数
}
Status GetHead(LinkQueue Q, QElemType &e)
{
	//若队列不空，则用 e 返回 Q 的队列头元素， 并返回 OK，
	//否则返回 ERROR

}
Status EnQueue(LinkQueue &Q, QElemType e)
{
	//插入 e 为 Q 的新的队尾元素 
	QueuePtr p = (QueuePtr)malloc(sizeof(QNode));
	if (!p)
	{
		exit(OVERFLOW);
	}
	p->data = e;
	p->next = NULL;
	Q.rear->next = p;
	Q.rear = p;
	return OK;
}
Status DeQueue(LinkQueue &Q, QElemType &e)
{
	//若队列不空，则删除 Q 的队头元素，用 e 返回其值，
	//并返回 OK，否则返回 ERROR
	if (Q.front == Q.rear)
	{
		return ERROR;
	}
	QueuePtr p = Q.front->next;
	e = p->data;
	Q.front->next = p->next;
	if (Q.rear == p)
	{
		Q.rear = Q.front;
	}
	free(p);
	return OK;
}
void visit(void)
{

}
Status QueueTraverse(LinkQueue Q, void visit())
{

}
void PrintQueue(LinkQueue Q)
{
	//QueuePtr p = Q.front;注意头结点、头指针
	QueuePtr p = Q.front->next;
	while (p)
	{
		printf("%d\t", p->data);
		if (p == Q.rear)
		{
			break;
		}
		p = p->next;
	}
	printf("\n");
}