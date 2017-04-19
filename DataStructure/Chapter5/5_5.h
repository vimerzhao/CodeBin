//5_5.h
/**
author:zhaoyu
date；2016-6-16
*/
//----广义表的头尾链表存储表示----
#include "4_2_part1.h"
#define AtomType char
typedef enum {ATOM, LIST} ElemTag;//ATOM==0：原子，LIST==1子表
typedef struct GLNode
{
	ElemTag tag;//公共部分，用于区分原子结点和表节点
	union{//原子结点和表节点的公共部分
		AtomType atom;//atom 是原子结点的值域，AtomType由用户定义
		struct {struct GLNode *hp, *tp;}ptr;//ptr 是表结点
		//的指针域，ptr.hp 和 ptr.tp 分别指向表头和表尾
	};
}*GList;//广义表类型
/**
algorithm 5.5
*/
int GListDepth(GList L)
{//采用头尾链表存储结构，求广义表 L 深度
	if (!L)
	{
		return 1;//空表深度为 1
	}
	if (L->tag == ATOM)
	{
		return 0;//原子深度为 0
	}
	int MAX = 0;
	for (GList pp = L; pp; pp = pp->ptr.tp)
	{
		int t = GListDepth(pp->ptr.hp);//求以 ptr.hp 为头指针的子表深度
		MAX = MAX>t?MAX:t;
	}
	return MAX+1;
}
/**
algorithm 5.8
*/
Status sever(SString &str, SString &hstr)
{//将非空串 str 分割成两部分：hsub为第一个','之前的字串，str 为之后的子串
	int n = StrLength(str);
	int i = 0, k = 0;//k 记尚未配对的左括号的个数
	char ch;
	SString CH;
	do{
		++i;
		SubString(CH, str, i, 1);
		ch = CH[1];
		if ('(' == ch)
		{
			++k;
		}
		else if (')' == ch){
			--k;
		}
	}while (i < n && (',' != ch || 0 != k));
	if (i < n)
	{
		SubString(hstr, str, 1, i-1);
		SubString(str, str, i+1, n-i);
	}
	else
	{
		StrCopy(hstr, str);
		ClearString(str);
	}
}

/**
algorithm 5.7
*/
Status CreateGList(GList &L, SString S)
{//采用头尾链表存储结构，由广义表的书写形式串 S 创建广义表 L,设emp="()"
	GList q = NULL, p = NULL;
	SString emp = { 2, '(', ')', '\0'};
	SString sub, hsub;
	if ( 0 == StrCompare(S, emp))
	{//创建空表
		L = NULL;
	}
	else
	{
		if (!(L = (GList)malloc(sizeof(GLNode))))
		{
			exit(OVERFLOW);//建表结点
		}
		if (1 == StrLength(S))
		{//创建单原子广义表
			L->tag = ATOM;
			L->atom = S[1];
		}
		else
		{
			L->tag = LIST;
			p = L;
			SubString(sub, S, 2, StrLength(S)-2);
			do{//重复建 n 个子表
				sever(sub, hsub);//从 sub 中分离出表头串 hsub
				CreateGList(p->ptr.hp, hsub);
				q = p;
				if (!StrEmpty(sub))
				{//表尾不空
					if (!(p = (GList)malloc(sizeof(GLNode))))
					{
						exit(OVERFLOW);
					}
					p->tag = LIST;
					q->ptr.tp = p;
				}//if
			}while (!StrEmpty(sub));
			q->ptr.tp = NULL;
		}//else
	}
}
/**
algorithm 5.6
*/
Status CopyGList(GList &T, GList L)
{//采用头尾链表存储结构，由广义表 L 复制得到广义表 T
	if (!L)
	{
		T = NULL;
	}
	else
	{
		if (!(T = (GList)malloc(sizeof(GLNode))))
		{
			exit(OVERFLOW);
		}
		T->tag = L->tag;
		if (ATOM == L->tag)
		{
			T->atom = L->atom;
		}
		else
		{
			CopyGList(T->ptr.hp, L->ptr.hp);
			CopyGList(T->ptr.tp, L->ptr.tp);
		}
	}
	return OK;
}

/**
my code
*/
int cnt = 0;
void PrintGList(GList L)
{
	if (NULL == L)
	{
		printf("()");
	}
	else
	{
		if (ATOM == L->tag)
		{
			printf("%c", L->atom);
		}
		else
		{
			if (NULL == L->ptr.hp)
			{
				printf("(");
			}
			if (NULL != L->ptr.hp && LIST == L->ptr.hp->tag)
			{
				printf("(");
			}
			PrintGList(L->ptr.hp);·		
			if (NULL != L->ptr.tp && LIST == L->ptr.tp->tag)
			{
				printf(",");
			}
			if (NULL == L->ptr.tp)
			{
				printf(")");
			}
			else
			{
				PrintGList(L->ptr.tp);
			}
			
			
		}
	}
}
