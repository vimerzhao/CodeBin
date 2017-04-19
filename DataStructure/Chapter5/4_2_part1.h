//4_2_part1.h
/**
author:zhaoyu
*/
//2016-6-10
//----串的定长顺序存储表示----
#ifndef _4_2_PART1_H_
#define _4_2_PART1_H_
#include "head.h"
#define MAXSTRLEN 255//用户可以在255以内定义最大串长
//这语法还不是很熟悉
typedef unsigned char SString[MAXSTRLEN+1];//0 号单元存放串的长度
int StrLength(SString T)
{
	for (int i = 1; i <= MAXSTRLEN; ++i)
	{
		if ('\0' == T[i])
		{
			return i-1;
		}
	}
	return MAXSTRLEN;
}

/**
algorithm 4.2
*/
Status Concat(SString &T, SString S1, SString S2)
{
	//用 T 返回由 S1 和 S2 连接而成的新串。
	//若未截断，则返回 TRUE，否则返回 FALSE
	Status uncut;
	if (S1[0] + S2[0] < MAXSTRLEN)
	{//未截断
		int i = 1;
		for (i = 1; i <= S1[0]; ++i)
		{
			T[i] = S1[i];
		}
		for (i = 1; i <= S2[0]; ++i)
		{
			T[S1[0]+i] = S2[i];
		}
		T[0] = S1[0] + S2[0];
		uncut = TRUE;
	}
	else if (S1[0] < MAXSTRLEN)
	{
		int i = 1;
		for (i = 1; i <= S1[0]; i++)
		{
			T[i] = S1[i];
		}
		for (i = S1[0]+1; i <= MAXSTRLEN; i++)
		{
			T[i] = S2[i-S1[0]];
		}
		T[0] = MAXSTRLEN;
		uncut = FALSE;
	}
	else
	{
		for (int i = 1; i <= MAXSTRLEN; i++)
		{
			T[i] = S1[i];
		}
		T[0] = S1[0] = MAXSTRLEN;
		uncut = FALSE;
	}
	return uncut;
}
/**
algorithm 4.3
*/
Status SubString(SString &Sub, SString S, int pos, int len)
{
	//用 Sub 返回串 S 的第 pos 个字符起长度为 len 的字串
	//其中， 1<= pos <<= SreLength(S) 且 0 <= len <= StrLength(S)-pos+1
	if (pos < 1 || pos > S[0] || len < 0 || len > S[0]-pos+1)
	{
		return ERROR;
	}
	for (int i = 1; i <= len; i++)
	{
		Sub[i] = S[i+pos-1];
	}
	Sub[len+1] = '\0';
	Sub[0] = len;
	return OK;
}
/**
add for chapter 5 / page 116.117
*/
Status StrCompare(SString S, SString T)
{
	for (int i = 1; i <= S[0] && i <=T[0]; i++)
	{
		if (S[i]-T[i] > 0){
			return 1;
		}
		if (S[i]-T[i] < 0)
		{
			return -1;
		}
	}
	if (S[0] == T[0])
	{
		return 0;
	}
	return S[0]>T[0]?1:-1;
}
Status StrEmpty(SString S)
{
	if (0 == StrLength(S))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
Status StrCopy(SString &T, SString S)
{//健壮性并不够
	for (int i = 0; i <= S[0]+1; i++)
	{
		T[i] = S[i];
	}
	return OK;
}
Status ClearString(SString S)
{
	S[0] = 0;
	S[1] = '\0';
	return OK;
}
void PrintSString(SString T)
{
	//if(T[])
	for (int i = 1; i <= T[0]; ++i)
	{
		printf("%c", *(T+i));
	}
	printf("\n");
}
#endif