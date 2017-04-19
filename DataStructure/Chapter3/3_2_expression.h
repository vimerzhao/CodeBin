//3_2_maze.h
/**
author:zhaoyu
email:zhaoyu1995.com@gmail.com
date:2016-6-8
note:realize my textbook <<数据结构（C语言版）>>
*/
//Page 53
#ifndef _3_2_EXPRESSION_H_
#define _3_2_EXPRESSION_H_
#include "head.h"

#define SElemType char
#define OperandType float
#define STACK_INIT_SIZE 100//存储空间的初始分配值
#define STACKINCREMENT 10//存储空间分配增量
typedef struct{
	SElemType *base;//在栈构造之前和销毁之后，base 值为 NULL
	SElemType *top;//栈顶指针
	int stacksize;//当前已分配的存储空间，以元素为单位
}SqStack_Char;
typedef struct{
	OperandType *base;//在栈构造之前和销毁之后，base 值为 NULL
	OperandType *top;//栈顶指针
	int stacksize;//当前已分配的存储空间，以元素为单位
}SqStack_Float;
Status InitStack_Char(SqStack_Char &S)
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
Status InitStack_Float(SqStack_Float &S)
{
	//构造一个空栈 S
	S.base = (OperandType *)malloc(STACK_INIT_SIZE*sizeof(OperandType));
	if (!S.base)
	{
		exit(OVERFLOW);
	}
	S.top = S.base;
	S.stacksize = STACK_INIT_SIZE;
	return OK;
}//InitStack
Status Push_Char(SqStack_Char &S, SElemType e)
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
SElemType GetTop_Char(SqStack_Char S)
{
	//若栈不空，则用 e 返回 S 的栈顶元素，并返回 OK；
	//否则返回ERROR
	if (S.top == S.base)
	{
		return ERROR;
	}
	return *(S.top - 1);
}//GetTop
Status isInOPTR(SElemType c)
{
	switch (c)
	{
		case '+':
		case '-':
		case '*':
		case '/':
		case '(':
		case ')':
		case '#':
		{
			return TRUE;
			break;
		}
		default:
		{
			return FALSE;
			break;
		}
	}
}
Status Push_Float(SqStack_Float &S, OperandType e)
{
	//插入元素 e 为新的栈顶元素
	if (S.top - S.base >= S.stacksize)
	{//栈满，追加存储空间
		S.base = (OperandType *)realloc(S.base,
			(S.stacksize+STACKINCREMENT)*sizeof(OperandType));
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
char Precede(SElemType a, SElemType b)
{
	char R[7][7] =  {{'>','>','<','<','<','>','>'},
					{'>','>','<','<','<','>','>'},
					{'>','>','>','>','<','>','>'},
					{'>','>','>','>','<','>','>'},
					{'<','<','<','<','<','=','#'},
					{'>','>','>','>','#','>','>'},
					{'<','<','<','<','<','#','='}};
	int i,j;
	switch (a)
	{
		case '+':i=0;break;
		case '-':i=1;break;
		case '*':i=2;break;
		case '/':i=3;break;
		case '(':i=4;break;
		case ')':i=5;break;
		case '#':i=6;break;
		default:i=0;break;
	}
	switch (b)
	{
		case '+':j=0;break;
		case '-':j=1;break;
		case '*':j=2;break;
		case '/':j=3;break;
		case '(':j=4;break;
		case ')':j=5;break;
		case '#':j=6;break;
		default:j=0;break;
	}
	if ('#' == R[i][j])
	{
		printf("ERROR\n");
		exit(ERROR);
	}
	else
	{
		return R[i][j];
	}
}
Status Pop_Char(SqStack_Char &S, SElemType &e)
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
Status Pop_Float(SqStack_Float &S, OperandType &e)
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
OperandType Operate(OperandType a, SElemType theta, OperandType b)
{
	switch (theta)
	{
		case '+': return a+b; break;
		case '-': return a-b; break;
		case '*': return a*b; break;
		case '/': return a/b; break;
		default:return 0; break;
	}
}
OperandType GetTop_Float(SqStack_Float S)
{
	//若栈不空，则用 e 返回 S 的栈顶元素，并返回 OK；
	//否则返回ERROR
	if (S.top == S.base)
	{
		return ERROR;
	}
	return *(S.top - 1);
}//GetTop
OperandType EvaluateExpression()
{
	//算数表达式求值的算符优先算法。设 OPTR 和 OPND 
	//分别为运算符栈和运算数栈，OP 为运算符集合
	SqStack_Char OPTR;
	SqStack_Float OPND;
	InitStack_Char(OPTR);
	Push_Char(OPTR, '#');
	InitStack_Float(OPND);
	char c = getchar(), x, theta;
	float a, b;
	while ('#' != c || GetTop_Char(OPTR) != '#')
	{
		//if (!In(c, OP))
		if (!isInOPTR(c))
		{
			float temp = c-'0';
			Push_Float(OPND, temp);//不是运算符则进栈
			c = getchar();
		}
		else
		{
			switch (Precede(GetTop_Char(OPTR), c))
			{
				case '<'://栈顶元素优先权低
				{
					Push_Char(OPTR, c);
					c = getchar();
					break;
				}
				case '='://脱括号并接收下一字符
				{
					Pop_Char(OPTR, x);
					c = getchar();
					break;
				}
				case '>'://退栈并将运算符结果入栈
				{
					Pop_Char(OPTR, theta);
					Pop_Float(OPND, b);
					Pop_Float(OPND, a);
					Push_Float(OPND, Operate(a, theta, b));
					break;
				}
			}
		}
	}
	return GetTop_Float(OPND);
}
#endif