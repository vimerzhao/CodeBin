//2_4.h
/**
《数据结构（C语言版）》 Page 37
....为此，从实际应用角度出发重新定义线性链表及其基本操作....
*/
/**
author:zhaoyu
email:zhaoyu1995.com@gmail.com
date:2016-6-6
note:realize my textbook <<数据结构（C语言版）>>
*/
/**
抽象数据类型一元多项式的定义
ADT Polyomail{
	数据对象：D = {a[i]|a[i]属于TermSet, i = 1,2,3...,m,m>=0
					TermSet中每个元素包含一个表示系数的实数和表示指数的整数}
	数据关系
	基本操作：
	CreatPolyn(&P, m)
	操作结果：输入 m 项的系数和指数，建立一元多项式 P
	DestroyPolyn(&P)
	初始条件：一元多项式 P 已存在
	操作结果：销毁一元多项式P
	PrintPolyn(P)
	初始条件：一元多项式 P 已存在
	操作结果：打印输出一元多项式 P
	PolynLength(P)
	初始条件：一元多项式 P 已存在
	操作结果：返回一元多项式中 P 的项数
	AddPolyn(&Pa, &Pb)
	初始条件：一元多项式 Pa 和 Pb 已存在
	操作结果：完成多项式相加运算，即：Pa = Pa + Pb,并销毁一元多项式 Pb
	SubtractPolyn(&Pa, &Pb)
	初始条件：一元多项式 Pa 和 Pb 已存在
	操作结果：完成多项式相减运算，即 Pa = Pa - Pb,并销毁一元多项式 Pb
	MultiplyPolyn(&Pa, &Pb)
	初始条件：一元多项式 Pa 和 Pb 已存在
	操作结果：完成多项式相乘运算，即 Pa = Pa x Pb,并销毁一元多项式 Pb
}ADT Polynomial
*/
#ifndef _2_4_H_
#define _2_4_H_
#include "head.h"
typedef struct{//项的表示，多项式的项作为LinkList的数据元素
	float coef;//系数
	int expn;//指数
}term, ElemType;//两个类型名：term用于本ADT，ElemType为LinkList的数据对象名
typedef struct LNode{
	ElemType data;
	struct LNode *next;
}*Link, *Position;
typedef struct{//链表类型
	Link head, tail;//分别指向线性链表中的头结点和最后一个结点
	int len;//指示线性链表中数据元素的个数
}LinkList;
typedef LinkList polynomial;//用带表头结点的有序链表表示多项式

int cmp(term a, term b)
{
	//依据 a 的指数值 <(或=)(或>) b 的指数值 ，返回 -1 ，0， +1
	if(a.expn < b.expn)
	{
		return -1;
	}
	else if (a.expn == b.expn)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
Status InitList(LinkList &L)
{
	//构造一个空的线性表L
	L.head = (Link)malloc(sizeof(struct LNode));
	L.head->data.coef = 0.0;
	L.head->data.expn = -1;
	L.head->next = NULL;
	L.tail = L.head->next;
	L.len = 0;
}
Position GetHead(LinkList L)
{
	//返回线性链表 L 中头结点的位置
	return L.head;
}
Status SetCurElem(Link &p, ElemType e)
{
	//已知 p 指向线性链表中的一个结点，用 e 更新 p 所指结点中数据元素的值
	p->data.coef = e.coef;
	p->data.expn = e.expn;
	return OK;
}
Status LocateElem(LinkList L, ElemType e, Position &q, Status (* compare)(ElemType, ElemType))
{
	//若有序链表 L 中存在与 e 满足判定函数 compare() 取值为 0 的函数
	//则 q 指示 L 中第一个值为 e 的结点的位置，并返回 TRUE；否则 q 指示
	//第一个与 e 满足compare() 取值>0的元素的前驱的位置，并返回FLASE
	Link temp = L.head->next;
	while (temp != NULL)
	{
		if (0 == (* compare)(temp->data, e))
		{
			q = temp;
			return TRUE;
		}
		temp = temp->next;
	}
	temp = L.head;
	while (temp->next != NULL)
	{
		if ((* compare)(temp->next->data, e) > 0)
		{
			q = temp;
			return FALSE;
		}
		temp = temp->next;
	}
	q = temp;
	return FALSE;
}
Status MakeNode(Link &p, ElemType e)
{
	//分配由 p 指向的值为 e 的结点，并返回 OK；若分配失败，则返回 ERROR
	p = (Link)malloc(sizeof(struct LNode));
	if (!p)
	{
		return ERROR;
	}
	p->data.coef = e.coef;
	p->data.expn = e.expn;
	p->next = NULL;
	return OK;
}
Status InsFirst(Link &h, Link &s)
{
	//已知 h 指向线性链表的头结点，将 s 所指节点插入在第一个结点之前
	if (NULL == h)
	{
		return ERROR;
	}
	else
	{
		s->next = h->next;
		h->next = s;
	}
	return OK;
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
/**
My Code
*/
Position NextPos(LinkList L, Link p)
{
	//已知 p 指向线性链表 L 中的一个节点，返回 p 所指结点
	//的直接后继的位置，若无后继，返回 NULL
	Link q = L.head;
	while (q->next != NULL)
	{
		if(q == p)
		{
			return p->next;
		}
		q = q->next;
	}
	return NULL;
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
Status FreeNode(Link &p)
{
	//释放 p 所指节点
	p = NULL;//便于回收???释放后再次使用
	free(p);
}
Status ListEmpty(LinkList L)
{
	//若线性链表 L 为空，则返回 TRUE，否则返回 FALSE
	if (L.head->next == NULL)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
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
		temp = temp->next;//注意这一句要放在最后，否则可能访问非法内存
	}

	L.len += cnt;
	//注意要根据这一串结点长度增加链表长度
	return OK;
}
void PrintPolyn(polynomial P)
{
	Link temp = P.head->next;
	while (temp != NULL)
	{
		printf("%.1f\t", temp->data.coef);
		temp = temp->next;
	}
	printf("\n");
	temp = P.head->next;
	while (temp != NULL)
	{
		printf("%d\t", temp->data.expn);
		temp = temp->next;
	}
	printf("\n");
}
/**
algorithm 2.22 
*/
void CreatPolyn(polynomial &P, int m)
{
	//输入 m 项的系数和指数，建立表示一元多项式的有序链表 P
	InitList(P);
	Link h = GetHead(P), s, q;
	ElemType e;
	e.coef = 0.0;
	e.expn = -1;
	SetCurElem(h, e);//设置头结点中的数据元素
	for (int i = 1; i <= m; i++)
	{//依次输入 m 个非零项
		scanf("%f%d", &e.coef, &e.expn);
		if (!LocateElem(P, e, q, (* cmp)))
		{//当前链表中不存在该指数项
			if (MakeNode(s, e))
			{
				InsFirst(q, s);
			}
		}
	}
}//CreatePolyn
/**
algorithm 2.23
*/
void AddPolyn(polynomial &Pa, polynomial &Pb)
{
	//多项式加法：Pa = Pa + Pb，利用两个多项式构成和多项式
	Link ha = GetHead(Pa);
	Link hb = GetHead(Pb);//pa 和 pb 分别指向 Pa 和 Pb 的头结点
	Link qa = NextPos(Pa, ha);
	Link qb = NextPos(Pb, hb);//qa 和 qb 分别指向 Pa 和 Pb 中当前节点
	while (qa && qb)//qa 和 qb均非空
	{
		ElemType a = GetCurElem(qa);
		ElemType b = GetCurElem(qb);//a、b为两表中当前比较元素
		switch ((* cmp)(a, b))
		{
			case -1://多项式 Pa 中当前节点的指数值小
			{
				ha = qa;
				qa = NextPos(Pa, qa);//printf("%d\n", qa->data.expn);
				break;
			}
			case 0://两者的指数值相等
			{
				ElemType S = {a.coef + b.coef, a.expn};
				if(0 != S.coef)
				{//修改多项式 Pa 中当前节点
					SetCurElem(qa, S);
					ha = qa;
				}
				else
				{//删除多项式中 Pa 当前节点
					DelFirst(ha, qa);
					FreeNode(qa);
				}
				DelFirst(hb, qb);
				FreeNode(qb);
				qa = NextPos(Pa, ha);
				qb = NextPos(Pb, hb);
				break;
			}
			case 1://多项式 Pb 当前结点指数值小
			{
				DelFirst(hb, qb);
				InsFirst(ha, qb);
				qb = NextPos(Pb, hb);
				ha = NextPos(Pa, ha);
				break;
			}
		}//switch
	}//while
	if (!ListEmpty(Pb))
	{
		Append(Pa, qb);//链接 Pb 中剩余结点
	}
	FreeNode(hb);//释放 Pb 的头结点
}//AddPolyn
#endif