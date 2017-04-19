#ifndef _2_3_part1_H_
#define _2_3_part1_H_
//2_3_part1.h
/**
author:zhaoyu
email:zhaoyu1995.com@gmail.com
date:2016-6-4
note:realize my textbook <<数据结构（C语言版）>>
*/
//----线性表的单链表存储结构----
/**
My Code
to make the paragram run correctlly
*/
#define ElemType int
typedef struct LNode{
	ElemType data;
	struct LNode *next;
}LNode, *LinkList;
/**
algorithm 2.8
*/
Status GetElem_L(LinkList L, int i, ElemType &e)
{
	//L为带头结点的单链表的头指针
	//当第i个元素存在时，其值赋给e并返回OK，否则返回ERROR
	LinkList p = L->next;
	int j = 1;//初始化，p指向第一个节点，j为计数器
	while (p && j<i)
	{//顺指针向后查找，直到p指向第i个元素或p为空
		p = p->next;
		++j;
	}
	if (!p || j > i)
	{//第i个元素不存在
		return ERROR;
	}
	e = p->data;//取第i个元素
	return OK;
}//GetElem_L
//关于指针的引用
/**
algorithm 2.9
*/
Status ListInsert_L(LinkList &L, int i, ElemType e)
{
	//在带头结点的单链线性表L中第i个位置之前插入元素
	LinkList p = L;
	int j = 0;
	while (p && j < i-1)
	{//寻找第i-1个节点
		p = p->next;
		++j;
	}
	if (!p || j>i-1)
	{//i小于 1 或大于表长加 1
		return ERROR;
	}
	LinkList s = (LinkList)malloc(sizeof(LNode));
	s->data = e;//插入 L 中
	s->next = p->next;//注意顺序
	p->next = s;
	return OK;
}//ListInsert_L

/**
algorithm 2.10
*/
Status ListDelete_L(LinkList &L, int i, ElemType &e)
{
	//在带头结点的单链线性表L中，删除第 i 个节点，并由 e 返回其值
	LinkList p = L;
	int j = 0;
	while (p->next && j < i-1)
	{//寻找第 i 个节点，并另 p 指向其前驱
		p = p->next;
		++j;
	}
	if (!(p->next) || j > i-1)
	{//删除位置不合理
		return ERROR;
	}
	LinkList q = p->next;
	p->next = q->next;//删除并释放节点
	e = q->data;
	free(q);
	return OK;
}//ListDelete_L
/**
algorithm 2.11
从表尾到表头逆向建立单链表的算法
*/
void CreateList_L(LinkList &L, int n)
{
	//逆位序输入 n 个元素的值，建立带表头节点的单链线性表 L
	L = (LinkList)malloc(sizeof(LNode));
	L->next = NULL;//先建立一个带头结点的单链表
	for (int i = n; i > 0; i--)
	{
		LinkList p = (LinkList)malloc(sizeof(LNode));//生成新节点
		scanf("%d", &p->data);//输入元素值
		p->next = L->next;
		L->next = p;//插入到表头
	}
}//CreateList_L
/**
algorithm 2.12
*/
void MergeList_L(LinkList &La, LinkList &Lb, LinkList &Lc)
{
	//已知单链线性表 La 和 Lb 的元素按值非递减排列
	//归并 La 和 Lb 得到新的单链线性表 Lc，Lc的元素也按值非递减排列
	LinkList pa = La->next, pb = Lb->next;
	LinkList pc =  Lc = La;//用 La 的头结点作为 Lc 的头结点
	while (pa && pb)
	{
		if (pa->data <= pb->data)
		{
			pc->next = pa;
			pc = pa;
			pa = pa->next;
		}
		else
		{
			pc->next = pb;
			pc = pb;
			pb = pb->next;
		}
	}
	pc->next = pa ? pa : pb;//插入剩余节点
	free(Lb);
}//MergeList_L

/**
My Code
*/
void PrintList(LinkList L)
{
	LinkList p = L->next;
	while (p != NULL)
	{
		printf("%d\t", p->data);
		p = p->next;
	}
	printf("\n");
}
#endif