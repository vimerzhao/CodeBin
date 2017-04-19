//2_2.cpp
/**
author:zhaoyu
email:zhaoyu1995.com@gmail.com
date:2016-6-4
note:realize my textbook <<数据结构（C语言版）>>
*/

#include "head.h"
//----线性表的动态分配顺序存储结构----
#define LIST_INIT_SIZE 100//线性表存储空间的初始分配量
#define LISTINCREMENT 10//线性表存储空间的分配增量
/**
My Code
to make the paragram run correctlly
*/
#define ElemType int

typedef struct {
	ElemType *elem;//存储空间基址
	int length;//当前长度
	int listsize;//当前分配的存容量（以sizeof(ElemType)位单位）
}SqList;
int compare(ElemType a, ElemType b)
{
	return a==b?1:0;
}
/**
algorithm 2.3
page 23
*/
Status InitList_Sq(SqList &L)
{
	//构造一个空的线性表
	L.elem = (ElemType *)malloc(LIST_INIT_SIZE * sizeof(ElemType));
	if (!L.elem)
	{
		exit(OVERFLOW);
	}
	L.length = 0;
	L.listsize = LIST_INIT_SIZE;
	return OK;
}//InitList_Sq

/**
algorithm 2.4
*/
Status ListInsert_Sq(SqList &L, int i, ElemType e)
{
	//在顺序线性表L中第i个位置之前插入新的元素e
	//i的合法为[1, ListLength_Sq(L)+1]
	if (i < 1 || i > L.length + 1)
	{
		return ERROR;//i值不合法
	}
	if (L.length >= L.listsize)
	{//当前存储空间已满，增加分配
		ElemType *newbase = (ElemType *)realloc(L.elem, (L.listsize + LISTINCREMENT)*sizeof(ElemType));
		if (!newbase)
		{
			exit(OVERFLOW);//存储分配失败
		}
		L.elem = newbase;//新基址
		L.listsize += LISTINCREMENT;//增加存储容量
	}
	ElemType *q = &(L.elem[i-1]);//q为插入位置
	for (ElemType *p = &L.elem[L.length-1]; p >= q; p--)
	{
		*(p+1) = *p;//插入位置及之后的元素右移
	}
	*q = e;
	++L.length;
	return OK;
}//ListInsert_Sq
/**
algorithm 2.5
*/
Status ListDelete_Sq(SqList &L, int i, ElemType &e)
{
	//在顺序线性表中删除第i个元素，并用e返回其值
	//i的合法值为[1, ListLength_Sq(L)]
	if ((i < 1) || (i > L.length))
	{
		return ERROR;//i为不合法值
	}
	ElemType *p = &(L.elem[i-1]);//p为被删除元素的位置
	e = *p;//被删除的元素赋值给e
	ElemType *q = L.elem + L.length - 1;//表位元素的位置
	for (++p; p <= q; ++p)
	{
		*(p-1) = *p;//被删除元素之后的元素左移
	}
	--L.length;//表长减 1
	return OK;
}//ListDelete_Sq
/**
algorithm 2.6
*/
int LocateElem_Sq(SqList L, ElemType e, Status(* compare)(ElemType, ElemType))
{
	//在顺序线性表L中查找第1个值与e满足compare()的元素位序
	//若找到返回其在L中的位序，否则返回0
	int i = 1;//i的初值为第一个元素的位序
	ElemType *p = L.elem;//p为第一个元素的存储位置
	while (i <= L.length && !(*compare)(*p++, e))
	{
		++i;
	}
	if (i <= L.length)
	{
		return i;
	}
	else
	{
		return 0;
	}
}//LocateElem_Sq
/**
algorithm 2.7
*/
void MergeList_Sq(SqList La, SqList Lb, SqList &Lc)
{
	//已知顺序线性表La和Lb的元素按值非递减排列
	//归并La和Lb得到新的顺序线性表Lc,Lc的元素也按值非递减排列
	ElemType *pa = La.elem;
	ElemType *pb = Lb.elem;
	Lc.listsize = Lc.length = La.length + Lb.length;
	ElemType *pc = Lc.elem = (ElemType *)malloc(Lc.listsize*sizeof(ElemType));
	if (!Lc.elem)
	{
		exit(OVERFLOW);
	}
	ElemType *pa_last = La.elem + La.length - 1;
	ElemType *pb_last = Lb.elem + Lb.length - 1;
	while (pa <= pa_last && pb <= pb_last)
	{
		if (*pa <= *pb)
		{
			*pc++ = *pa++;
		}
		else
		{
			*pc++ = *pb++;
		}
	}
	while (pa <= pa_last)
	{
		*pc++ = *pa++;
	}
	while (pb <= pb_last)
	{
		*pc++ = *pb++;
	}
}//MergeList_Sq
/**
My code
*/
void PrintList(SqList L)
{
	for (int i = 1; i <= L.length; i++)
	{
		printf("%d\t", L.elem[i-1]);
	}
	printf("\n");
}
/**
My Test
*/
int main(int argc, char const *argv[])
{
	SqList La, Lb, Lc;
	InitList_Sq(La);
	InitList_Sq(Lb);
	InitList_Sq(Lc);
	//创建一个1 2 3 4的线性表
	ListInsert_Sq(La, 1, 10);
	ListInsert_Sq(La, 2, 20);
	ListInsert_Sq(La, 3, 30);
	ListInsert_Sq(La, 4, 50);
	PrintList(La);
	//在位序4（即值为5的位置）插 4
	ListInsert_Sq(La, 4, 40);
	PrintList(La);
	//创建线性表Lb;10, 20, 5, 30
	ListInsert_Sq(Lb, 1, 15);
	ListInsert_Sq(Lb, 2, 25);
	ListInsert_Sq(Lb, 3, 5);
	ListInsert_Sq(Lb, 4, 35);
	PrintList(Lb);
	int temp;
	//删除位置3的元素，并返回给 temp
	ListDelete_Sq(Lb, 3, temp);
	PrintList(Lb);
	printf("%d\n", temp);
	//查找 30 在 Lb 的位置 
	printf("%d\n", LocateElem_Sq(Lb, 30, compare));
	printf("%d\n", LocateElem_Sq(Lb, 35, compare));
	//合并La， Lb 到 Lc，注意前提是有序的
	MergeList_Sq(La, Lb, Lc);
	PrintList(Lc);
	return 0;
}

