//2_3_part2.cpp
/**
author:zhaoyu
email:zhaoyu1995.com@gmail.com
date:2016-6-5
note:realize my textbook <<数据结构（C语言版）>>
*/
#include "head.h"
//----线性表的静态单链表存储结构----
/**
这种描述方法便于在不设指针类型的高级程序设计语言中使用链表
静态链表需要用户自己实现malloc和free函数，为了辨明数组中哪些
分量未被使用，解决的办法是将所有未被使用过以及被删除的分量
用游标链成一个备用的链表。
*/
/**
My Code
to make the paragram run correctlly
*/
#define ElemType int

#define MAXSIZE 1000//链表的最大长度
typedef struct 
{
	ElemType data;
	int cur;
}component, SLinkList[MAXSIZE];
/**
algorithm 2.13
*/
int LocateElem_SL(SLinkList S, ElemType e)
{
	//在静态单链线性表 L 中查找第一个值为 e 的元素
	//若找到，则返回他在L中的位序，否则返回 0 
	int i = S[0].cur;
	while (0 != i && S[i].data != e)
	{
		return i;
	}
}//LocateElem_SL
/**
algorithm 2.14
*/
void InitSpace_SL(SLinkList &space)
{
	//将一维数组 space 中各分量链成一个备用链表，space[0].cur 为头指针
	//"0"表示空指针
	for (int i = 0; i < MAXSIZE - 1; i++)
	{
		space[i].cur = i + 1;
	}
	space[MAXSIZE-1].cur = 0;
}//InitSpace_SL
/**
algorithm 2.15
*/
int Malloc_SL(SLinkList &space)
{
	//若备用空间链表非空，则返回分配的结点下标，否则返回 0
	int i = space[0].cur;
	if (space[0].cur)
	{
		space[0].cur = space[i].cur;
	}
	return i;
}//Malloc_SL

/**
algorithm 2.16
*/
void Free_SL(SLinkList &space, int k)
{
	//将下标为 k 的空闲结点回收到备用链表
	space[k].cur = space[0].cur;
	space[0].cur = k;
}//Free_SL
/**
algorithm 2.17
*/
void difference(SLinkList &space, int &S)
{
	//依次输入集合 A 和 B 的元素， 在一维数组 space 中建立表示集合 (A-B)U(B-A)
	//的静态链表，S 为其头指针。假设备用空间足够大，space[0].cur为其头指针
}

/**
My Code
*/
void PrintList(SLinkList S)
{
	int i = S[0].cur;
	for (;i && i < 10;i = S[i].cur)
	{
		printf("%d\t", S[i].data);
	}
	printf("\n");
}

int main(int argc, char const *argv[])
{
	SLinkList S;
	//打印未分配空间的静态链表
	PrintList(S);
	//分配空间
	InitSpace_SL(S);
	//打印分配空间后的链表
	PrintList(S);
	printf("%d\n", Malloc_SL(S));
	return 0;
}






















