//3_1.h
/**
author:zhaoyu
email:zhaoyu1995.com@gmail.com
date:2016-6-7
note:realize my textbook <<数据结构（C语言版）>>
*/
//Page 46
#ifndef _3_1_H_
#define _3_1_H_
#include <cstdio>
#include <cstdlib>
#include "head.h"
/**
My Code
*/
#define SElemType int
//----栈的顺序存储表示----
#define STACK_INIT_SIZE 100//存储空间的初始分配值
#define STACKINCREMENT 10//存储空间分配增量
typedef struct{
	SElemType *base;//在栈构造之前和销毁之后，base 值为 NULL
	SElemType *top;//栈顶指针
	int stacksize;//当前已分配的存储空间，以元素为单位
}SqStack;
//----基本操作的函数原型说明及部分实现----
Status InitStack(SqStack &S)
{
	//构造一个空栈 S
	S.base = (SElemType *)malloc(STACK_INIT_SIZE*sizeof(SElemType));
	if (!S.base)
	{
		exit(OVERFLOW);
	}
	S.top = S.base;
	S.stacksize = STACK_INIT_SIZE;
	return OK;
}//InitStack
Status DestroyStack(SqStack &S)
{
	//销毁栈 S，S 不再存在
}
Status ClearStack(SqStack &S)
{
	//将 S 置为空栈
	S.top = S.base;
	return OK;
}
Status StackEmpty(SqStack S)
{
	//若 S 为空栈， 则返回 TRUE， 否则返回 FALSE
	if (S.base == S.top)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
int StackLength(SqStack S)
{
	//返回 S 的元素的个数，即栈的长度
}
Status GetTop(SqStack S, SElemType &e)
{
	//若栈不空，则用 e 返回 S 的栈顶元素，并返回 OK；
	//否则返回ERROR
	if (S.top == S.base)
	{
		return ERROR;
	}
	e = *(S.top - 1);
	return OK;
}//GetTop
Status Push(SqStack &S, SElemType e)
{
	//插入元素 e 为新的栈顶元素
	if (S.top - S.base >= S.stacksize)
	{//栈满，追加存储空间
		S.base = (SElemType *)realloc(S.base,
			(S.stacksize+STACKINCREMENT)*sizeof(SElemType));
		if (!S.base)
		{
			exit(OVERFLOW);
		}
		S.top = S.base + S.stacksize;
		S.stacksize += STACKINCREMENT;
	}
	*S.top++ = e;
	return OK;
}//Push
Status Pop(SqStack &S, SElemType &e)
{
	//若栈不空，则删除 S 的栈顶元素，用 e 返回其
	//值，并返回OK；否则返回ERROR
	if (S.top == S.base)
	{
		return ERROR;
	}
	e = *--S.top;
	return OK;
}//Pop
Status StackTraverse(SqStack S, Status (* visit)())
{
	//从栈底到栈顶一次对栈中每个元素调用函数visit()
	//一旦visit() 失败，则操作失败
}

//----栈的应用举例-----
//注意代码之间的数据类型冲突，所以编译时要根据要编译的部分
//注释掉阈值冲突的部分
/**
algorithm 3.1
*/
void conversion()
{
	//对于输入的任意一个非负十进制整数，
	//打印输出与其等值的 n 进制数
	//note:书上是8进制，这里稍作拓展
	SqStack S;
	InitStack(S);
	int N, n, e;
	scanf("%d%d", &N, &n);
	while (N)
	{
		Push(S, N%n);
		N = N/n;
	}
	while (!StackEmpty(S))
	{//while (!StackEmpty)竟然不报错
		//StackEmppty是一个非NULL的指针！！！
		Pop(S, e);
		printf("%d", e);
	}
}//conversion


//note:以下两个函数不能与conversion()同时编译
//因为具体处理的数据类型不同
/**
algorithm 3.4
*/
/*
void PrintStack_LineEdit(SqStack S)
{
	char ch;
	SqStack TempStack;
	InitStack(TempStack);
	while (!StackEmpty(S))
	{
		Pop(S, ch);
		Push(TempStack, ch);
	}
	while (!StackEmpty(TempStack))
	{
		Pop(TempStack, ch);
		putchar(ch);
	}
	printf("\n");
}
void LineEdit()
{
	//利用字符栈 S 从终端接收一行并传送至调用过程的数据区
	//编译时要将 SElemType 设置成 char（而不是 int）
	SqStack S;
	InitStack(S);
	char c,ch;
	ch = getchar();
	while (EOF != ch)//EOF 为全文结束符
	{
		while (EOF != ch && '\n' != ch)
		{
			switch (ch)
			{
				case '#':
				{
					Pop(S, c);
					break;//仅当栈非空时退栈
				}
				case '@':
				{
					ClearStack(S);//重置 S 为空栈
					break;
				}
				default:
				{
					Push(S, ch);//有效字符进栈
					break;
				}
			}
			ch = getchar();//从终端接收下一个字符
		}
		//将从栈底到栈顶的栈内字符传送至调用过程的数据区
		PrintStack_LineEdit(S);
		ClearStack(S);//重置 S 为空栈
		if (EOF != ch)
		{
			ch = getchar();
		}
	}
	DestroyStack(S);
}//LineEdit

*/
#endif