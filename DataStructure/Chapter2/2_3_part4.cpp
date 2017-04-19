#include "LinkList.h"
//2_3_part4.cpp
/**
author:zhaoyu
email:zhaoyu1995.com@gmail.com
date:2016-6-5
note:realize my textbook <<数据结构（C语言版）>>
*/
/**
algrithm 2.20
*/
Status ListInsert_L(LinkList &L, int i, ElemType e)
{
	//在带头结点的单链线性表 L 的第 i 个元素之前插入元素 e
	Link h, s;
	if (!LocatePos(L, i-1, h))
	{
		return ERROR;// i 值不合法
	}
	if (!MakeNode(s, e))
	{
		return ERROR;//结点存储分配失效
	}
	InsFirst(L, h, s);//对于从第 i 个结点开始的链表，第 i-1 个结点是它的头结点
	return OK;
}//ListInsert_L
/**
algorithm 2.21
*/
Status MergeList_L(LinkList &La, LinkList &Lb, LinkList &Lc, 
	int(*compare)(ElemType, ElemType))
{
	//已知单链线性表 La 和 Lb的元素按值非递减排列
	//归并La 和 Lb 得到新的单链线性表 Lc，Lc 的元素也按值非递减
	if (!InitList(Lc))
	{
		return ERROR;//存储空间分配失败
	}
	Link ha = GetHead(La);
	Link hb = GetHead(Lb);//ha 和 hb 分别指向 La 和 Lb 的头结点
	Link pa = NextPos(La, ha);
	Link pb = NextPos(Lb, hb);//pa 和 pb 分别指向 La 和 Lb 中当前节点
	while (pa && pb)//pa 和 pb 均非空
	{
		ElemType a = GetCurElem(pa);
		ElemType b = GetCurElem(pb);
		Link q = NULL;
		if ((* compare)(a, b) <= 0)//a 小于等于 b
		{
			DelFirst(ha, q);
			Append(Lc, q);
			pa = NextPos(La, ha);
		}
		else//a > b
		{
			DelFirst(hb, q);
			Append(Lc, q);
			pb = NextPos(Lb, hb);
		}
	}//while
	if (pa)
	{
		Append(Lc, pa);
	}
	else
	{
		Append(Lc, pb);
	}
	FreeNode(ha);
	FreeNode(hb);
	return OK;
}//MergeList_L
int main(int argc, char const *argv[])
{
	LinkList L;
	InitList(L);
	printf("length of L:%d\n", L.len);
	Link p;
	for (int i = 0; i < 5; i++)
	{
		MakeNode(p, i);
		InsFirst(L, L.head, p);
		//free(p);
		//注意这里不能释放结点，p结点虽然没用，但指向确是头结点
	}
	printf("length of L:%d\n", L.len);
	printf("L\t");
	PrintList(L);
	//测试 MergeList_L函数
	LinkList La, Lb, Lc;
	InitList(La);
	InitList(Lb);
	for (int i = 0; i < 5; i++)
	{
		MakeNode(p, 10-i);
		InsFirst(La, La.head, p);
		MakeNode(p, 15-i*2);
		InsFirst(Lb, Lb.head, p);		
	}
	printf("La:\t");
	PrintList(La);
	printf("length\t%d\n", La.len);
	printf("Lb:\t");
	PrintList(Lb);
	printf("length\t%d\n", Lb.len);
	MergeList_L(La, Lb, Lc, compare);
	printf("Lc\t\n");
	PrintList(Lc);
	printf("length\t%d", Lc.len);
	return 0;
}