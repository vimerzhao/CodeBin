//2_3_part3.cpp
/**
author:zhaoyu
email:zhaoyu1995.com@gmail.com
date:2016-6-5
note:realize my textbook <<数据结构（C语言版）>>
*/
#include "head.h"
//----线性表的双向链表存储结构----
/**
My Code
to make the paragram run correctlly
*/
#define ElemType int

typedef struct DuLNode{
	ElemType data;
	struct DuLNode *prior;
	struct DuLNode *next;
}DuLNode, *DuLinkList;
/**
My Code
*/
DuLinkList GetElemP_DuL(DuLinkList L, int i)
{

}

/**
algorithm 2.18
*/
Status ListInsert_DuL(DuLinkList &L, int i, ElemType e)
{
	//在带头结点的双链循环线性表 L 中第 i 个位置之前插入元素 e
	// i 的合法值为 [1, 表长+1]
	if (!(p = GetElemP_DuL(L, i)))//在 L 中确定插入位置指针 p
	{
		retrurn ERROR;//i 等于表长+1时，p指向头结点；i 大于表长+1时，p=NULL
	}
	DuLinkList s = (DuLinkList)malloc(sizeof(DuLNode));
	if (!s)
	{
		return ERROR;
	}
	s->data = e;
	s->prior = p->prior;
	p->prior->next = s;
	s->next = p;
	p->prior = s;
	return OK;
}//ListInsert_DuL