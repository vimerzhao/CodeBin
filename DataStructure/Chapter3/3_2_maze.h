//3_2_maze.h
/**
author:zhaoyu
email:zhaoyu1995.com@gmail.com
date:2016-6-8
note:realize my textbook <<数据结构（C语言版）>>
*/
//Page 51
#ifndef _3_2_MAZE_H
#define _3_2_MAZE_H 
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "head.h"
#define STACK_INIT_SIZE 200//存储空间的初始分配值
#define STACKINCREMENT 10//存储空间分配增量
#define WALL -1
#define PATH 1
#define PASSED -2
#define UNREACHABLE -3 
#define FINALPATH -4 
#define NMAX 50
char Map[NMAX][NMAX];
typedef struct{
	int x;
	int y;
}PosType;
typedef struct node_1{
	int x, y;
	struct node_1 *next;
}MazeType;
typedef struct{
	int ord;//通道块在路径上的序号
	PosType seat;//通道块在迷宫中的位置
	int direction;//从此通道块走向下一通道块的方向
}SElemType;
typedef struct{
	SElemType *base;//在栈构造之前和销毁之后，base 值为 NULL
	SElemType *top;//栈顶指针
	int stacksize;//当前已分配的存储空间，以元素为单位
}SqStack;
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
void Assign_PosType(PosType &a, PosType b)
{
	a.x = b.x;
	a.y = b.y;
}//Assign_PosType
Status Pass(PosType pos)
{//可能写的不对，注意
	if (PATH == Map[pos.x][pos.y])
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}//Pass
void FoorPrint(PosType pos)
{
	Map[pos.x][pos.y] = PASSED;
}
void Assign_SELemType(SElemType &e, int x, PosType pos, int y)
{
	e.ord = x;
	Assign_PosType(e.seat, pos);
	e.direction = y;
}
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
	//*S.top++ = e;
	Assign_SELemType(*S.top++, e.ord, e.seat, e.direction);
	return OK;
}//Push
PosType NextPos(PosType pos, int direction)
{
	PosType temp;
	switch (direction)
	{
		case 1:
		{
			temp.x = pos.x;
			temp.y = pos.y+1;
			break;
		}
		case 2:
		{
			temp.x = pos.x + 1;
			temp.y = pos.y;
			break;
		}
		case 3:
		{
			temp.x = pos.x;
			temp.y = pos.y - 1;
			break;
		}
		case 4:
		{
			temp.x = pos.x - 1;
			temp.y = pos.y;
			break;
		}
	}
	//加一个越界检查
	return temp;
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
Status Pop(SqStack &S, SElemType &e)
{
	//若栈不空，则删除 S 的栈顶元素，用 e 返回其
	//值，并返回OK；否则返回ERROR
	if (S.top == S.base)
	{
		return ERROR;
	}
	//e = *--S.top;
	S.top--;
	Assign_SELemType(e, (*S.top).ord, (*S.top).seat, (*S.top).direction);
	return OK;
}//Pop
void FootPrint(PosType pos)
{
	Map[pos.x][pos.y] = PASSED;	
}
void MarkPrint(PosType pos)
{
	Map[pos.x][pos.y] = UNREACHABLE;
}
void MakeMap(int size)
{
	memset(Map, 0, sizeof(Map));
	char ch;
	getchar();
	for (int i = 1; i <= size; i++)
	{
		for (int j = 1; j <= size; j++)
		{
			scanf("%c", &ch);
			if ('X' == ch)
			{
				Map[i][j] = UNREACHABLE;
			}
			else
			{
				Map[i][j] = PATH;
			}
		}
		//attention '\n'
		getchar();
	}
	//Print maze
	for (int i = 1; i <= size; i++)
	{
		for (int j = 1; j <= size; j++)
		{
			if (UNREACHABLE == Map[i][j])
			{
				printf("X");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
}
void PrintPath(SqStack S, int size)
{
	SElemType e;
	SqStack temp;
	while (!StackEmpty(S))
	{
		Pop(S, e);
		Map[e.seat.x][e.seat.y] = FINALPATH;
	}
	for (int i = 1; i <= size; i++)
	{
		for (int j = 1; j <= size; j++)
		{
			if (UNREACHABLE == Map[i][j])
			{
				printf("X");
			}
			else if (FINALPATH == Map[i][j])
			{
				printf("O");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
}
Status MazePath(MazeType maze, PosType start, PosType end, int size)
{
	//若迷宫 maze 中存在从入口 start 到出口 end 的通道，
	//则求得一条存放在栈中（从栈底到栈顶），并返回 TRUE，
	//否则返回 FALSE
	SqStack S;
	InitStack(S);
	//curpos = start
	PosType curpos;
	Assign_PosType(curpos, start);//设定当前位置为入口位置
	int curstep = 1;//探索第一步
	SElemType e;
	do{
		if (TRUE == Pass(curpos))
		{//当前位置可以通过
			FootPrint(curpos);//留下足迹
			//e = (curstep, curpos, 1);
			Assign_SELemType(e, curstep, curpos, 1);
			Push(S, e);//加入路径
			if (curpos.x == end.x && curpos.y == end.y)
			{
				//打印路径
				printf("PATH EXIST\n");
				PrintPath(S ,size);
				return TRUE;
			}
			curpos = NextPos(curpos, 1);//下一位置是当前位置的东邻
			curstep++;//探索下一步
		}
		else
		{
			if (!StackEmpty(S))
			{
				Pop(S, e);
				while (4 == e.direction && !StackEmpty(S))
				{
					MarkPrint(e.seat);
					Pop(S, e);//留下不能通过的标记，并退回一步
				}
				if (e.direction < 4)
				{
					e.direction++;
					Push(S, e);
					curpos = NextPos(e.seat, e.direction);
				}
			}
		}
	}while (!StackEmpty(S));

}
#endif