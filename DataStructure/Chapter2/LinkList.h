//LinkList.h
/**
《数据结构（C语言版）》 Page 37
....为此，从实际应用角度出发重新定义线性链表及其基本操作....
*/
/**
author:zhaoyu
email:zhaoyu1995.com@gmail.com
date:2016-6-4
note:realize my textbook <<数据结构（C语言版）>>
*/
#ifndef _LINKLIST_H_
#define _LINKLIST_H_
#include "head.h"
#include <cstdlib>
#define ElemType int
typedef struct LNode{
	ElemType data;
	struct LNode *next;
}*Link, *Position;
typedef struct{//链表类型
	Link head, tail;//分别指向线性链表中的头结点和最后一个结点
	int len;//指示线性链表中数据元素的个数
}LinkList;
Status MakeNode(Link &p, ElemType e)
{
	//分配由 p 指向的值为 e 的结点，并返回 OK；若分配失败，则返回 ERROR
	p = (Link)malloc(sizeof(struct LNode));
	if (!p)
	{
		return ERROR;
	}
	p->data = e;
	p->next = NULL;
	return OK;
}
Status FreeNode(Link &p)
{
	//释放 p 所指节点
	p = NULL;//便于回收???释放后再次使用
	free(p);
}
Status InitList(LinkList &L)
{
	//构造一个空的线性表L
	L.head = (Link)malloc(sizeof(struct LNode));
	L.head->data = 100;
	L.head->next = NULL;
	L.tail = L.head->next;
	L.len = 0;
}
Status ClearList(LinkList &L)
{
	//将线性链表置为空表并释放原链表的结点空间
}
Status InsFirst(LinkList &L, Link &h, Link &s)
{
	//已知 h 指向线性链表的头结点，将 s 所指节点插入在第一个结点之前
	//note:这里的参数与书上有些不一样，LinkList &L是我添加的
	//因为插入新的元素之后，链表长度应该同步变化
	//链表的插入是基于地址的，所以我全部加了引用
	//这一个函数与书上参数完全不同
	//更新 2016-5-5 16:46
	//通过分析算法2.20发现 h 不是绝对的头结点，而是接受一个 h
	//就以它为头结点作参考操作其它元素
	//这个函数花了不少时间，值得反复思考
	if (NULL == h)
	{
		return ERROR;
	}
	else
	{
		s->next = h->next;
		h->next = s;
	}
	L.len++;
	return OK;
}
Status DelFirst(Link &h, Link &q)
{
	//已知 h 指向线性链表的头结点，删除链表中的第一个节点并以 q 返回
	if (h == NULL)
	{
		return ERROR;
	}
	q = h->next;
	h->next = q->next;
	q->next = NULL;
	return OK;
}
Status Append(LinkList &L, Link &s)
{
	//将指针 s 所指（彼此以指针相链）的一串结点
	//链接在线性链表 L 最后一个结点
	Link q = L.head;
	while (q->next != NULL)
	{
		q = q->next;
	}
	q->next = s;
	int cnt = 0;
	Link temp = s;
	while (temp != NULL)
	{
		cnt++;
		if (NULL == temp->next)
		{
			L.tail = temp;//注意更新尾指针
		}
		temp = temp->next;
	}

	L.len += cnt;
	//注意要根据这一串结点长度增加链表长度
	return OK;
}
Status Remove(LinkList &L, Link &q)
{
	//删除线性链表 L 中的尾结点并以 q 返回，改变链表的尾指针指向新的尾结点
}
Status InsBefore(LinkList &L, Link &p, Link s)
{
	//已知 p 指向线性链表 L 中的一个结点，将 s 所指结点插入 p 所指节点之前
	//并修改指针 p 指向新插入的节点
}
Status InsAfter(LinkList &L, Link &p, Link s)
{
	//已知 p 指向线性链表 L 中的一个结点，将 s 所指结点插入 p 所指节点之后
	//并修改指针 p 指向新插入的节点
}
Status SetCurElem(Link &p, ElemType e)
{
	//已知 p 指向线性链表中的一个结点，用 e 更新 p 所指结点中数据元素的值
}
ElemType GetCurElem(Link p)
{
	//已知 p 指向线性链表中的一个节点，返回 p 所指结点中数据元素的值
	if (p != NULL)
	{
		return p->data;
	}
	else
	{
		exit(ERROR);
	}
}
Status ListEmpty(LinkList L)
{
	//若线性链表 L 为空，则返回 TRUE，否则返回 FALSE
}
int ListLength(LinkList L)
{
	//返回线性链表 L 中元素的个数

}
Position GetHead(LinkList L)
{
	//返回线性链表 L 中头结点的位置
	return L.head;
}
Position GetLast(LinkList L)
{
	//返回线性链表 L 中最后一个结点的位置
}
Position PriorPos(LinkList L, Link p)
{
	//已知 p 指向线性链表 L 中的一个结点，返回 p 所指结点直接前驱的位置
	//若无前驱，则返回NULL
}
Position NextPos(LinkList L, Link p)
{
	//已知 p 指向线性链表 L 中的一个节点，返回 p 所指结点
	//的直接后继的位置，若无后继，返回 NULL
	Link q = L.head;
	while (q != NULL)
	{
		if(q == p)
		{
			return p->next;
		}
		q = q->next;
	}
	return NULL;
}
Status LocatePos(LinkList L, int i, Link &p)
{
	//返回 p 指示线性链表 L 中第 i 个结点的位置并返回 OK
	//i 值不合法时返回ERROR
	int cnt = 2;
	p = L.head;
	if (i > L.len || i < 1)
	{
		return ERROR;
	}
	while (cnt <= i)
	{
		p = p->next;
		cnt++;
	}
	return OK;

}
Position LocateElem(LinkList L, ElemType e, Status (* compare)(ElemType, ElemType))
{
	//返回线性链表中第一个与 e 满足函数compare() 判定关系的元素的位置
	//若不存在这样的元素，返回 NULL
}
Status ListTraverse(LinkList L, Status (* visit)())
{
	//依次对 L 中每个元素调用函数 visit()，一旦visit() 失败，则操作失败。
}
/**
My Code
*/
void PrintList(LinkList L)
{
	Link p = L.head->next;
	int cnt = 1;
	while (p != NULL && cnt <= L.len)
	{
		printf("%d\t", p->data);
		p = p->next;
		cnt++;
	}
	printf("\n");
}
int compare(ElemType a, ElemType b)
{
	if (a < b)
	{
		return -1;
	}
	else if (a == b)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
#endif