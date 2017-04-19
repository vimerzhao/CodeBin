//4_2_part2.h
/**
author:zhaoyu
*/
//2016-6-12
#include "head.h"
#include <cstdlib>
#include <cstring>
//----串的对分配存储表示----
typedef struct {
	char *ch;//
	int length;	
}HString;

Status StrInsert(HString &S, int pos, HString T)
{//1<= pos <= StrLength(S)+1
//在串的第 pos 个字符之前插入串 T
	if (1 < pos || pos > S.length + 1)
	{
		printf("Illegal pos\n");
		return ERROR;
	}
	if (T.length){//T 非空，则重新分配空间，插入 T
		if (!(S.ch = (char *)realloc(S.ch, (S.length +
		 T.length)*sizeof(char))));
		{
			exit(OVERFLOW);
		}
		for (int i = S.length-1; i >= pos-1; i--)
		{//为插入 T 为腾出位置
			S.ch[i+T.length] = S.ch[i];
		}
		for (int i = pos-1; i <= pos+T.length-2; i++)
		{//插入 T
			S.ch[i] = T.ch[i];
		}
		S.length += T.length;	
	}
	return OK;
}
//page 76-77
//----基本操作的算法描述----
Status StrAssign(HString &T, char *chars)
{//生成一个其值等于串常量 chars的串 T
	if (T.ch)
	{
		T.ch = NULL;
	}
	int L;
	char *c;;
	for (L = 0, c = chars; *c!='\0'; ++L, ++c);//求 chars 的长度 L
	if(!L)
	{
		T.ch = NULL;
		T.length = 0;
	}
	else
	{
		if (!(T.ch = (char *)malloc(L*sizeof(char))))
		{
			exit(OVERFLOW);
		}
		for (int i = 0; i < L; ++i)
		{
			T.ch[i] = chars[i];
		}
		T.length = L;
	}
	return OK;
}
int StrLength(HString S)
{//返回 S 的元素个数， 称为串的长度
	return S.length;
}
int StrCompare(HString S, HString T)
{//若 S >|=|< T，则返回 >|=|< 0
	for (int i = 0; i < S.length && i < T.length; i++)
	{
		if (T.ch[i] != S.ch[i])
		{
			return S.ch[i] - T.ch[i];
		}
	}
	return S.length - T.length;
}
Status ClearString(HString &S)
{
	//将 S 清空为空串
	if (S.ch)
	{
		free(S.ch);
		S.ch = NULL;
	}
	S.length = 0;
	return OK;
}
Status Concat(HString &T, HString S1, HString S2)
{
	if (T.ch)
	{//释放旧空间
		free(T.ch);
	}
	if (!(T.ch = (char *)malloc((S1.length+S2.length)*sizeof(char))))
	{
		exit(OVERFLOW);
	}
	for (int i = 0; i < S1.length; ++i)
	{
		T.ch[i] = S1.ch[i];
	}
	for (int i = 0; i < S2.length; ++i)
	{
		T.ch[i+S1.length] = S2.ch[i];
	}
	T.length = S1.length + S2.length;
	return OK;
}
Status SubString(HString &Sub, HString S, int pos, int len)
{
	//用 Sub 返回串 S 的第 pos 个字符起长度为 len 的子串
	//其中，其中， 1<= pos <<= SreLength(S) 且 0 <= len <= StrLength(S)-pos+1
	if (pos < 1 || pos > S.length || len < 0 || len > S.length-pos+1)
	{
		return ERROR;
	}
	if (Sub.ch)
	{
		free(Sub.ch);
	}
	if (!len)
	{//空子串
		Sub.ch = NULL;
		Sub.length = 0;
	}
	else
	{
		Sub.ch = (char *)malloc(len*sizeof(char));
		for (int i = 0; i < len; ++i)
		{
			Sub.ch[i] = S.ch[pos+i-1];
		}
		Sub.length = len;
	}
	return OK;
}
void PrintHString(HString S)
{
	for (int i = 0; i < S.length; ++i)
	{
		printf("%c", S.ch[i]);
	}
	printf("\n");
}