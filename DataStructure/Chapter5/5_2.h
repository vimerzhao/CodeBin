//5_2.h
/**
author:zhaoyu
date:2016-6-15
*/
//----数组的顺序存储表示----
#include "head.h"
#include <stdarg.h>//表准头文件，提供宏va_start,va_arg和va_end
#define MAX_ARRAY_DIM 8//用于存取变长参数表
#define ElemType int
typedef struct {
	ElemType *base;//数组元素基址，由InitArray分配
	int dim;//数组维数
	int *bounds;//数组维界基址，有InitArray分配
	int *constants;//数组映像函数常量基址，由InitArray分配
}Array;
void test(int x1, ...)
{//测试 stdarg
	va_list ap;
	va_start(ap, x1);
	for (int i = 0; i < x1; i++)
	{
		printf("%d\t", va_arg(ap, int));
	}
	printf("\n");
}

//----基本操作的算法描述----
Status InitArray(Array &A, int dim, ...)
{//若位数 dim 和各维度长度合法，则构造相应的数组A，并返回 OK
	if (dim < 1 || dim > MAX_ARRAY_DIM)
	{
		return ERROR;
	}
	A.dim = dim;
	A.bounds = (int *)malloc(dim * sizeof(int));
	if (!A.bounds)
	{
		exit(OVERFLOW);
	}
	//各维度长度合法，则存入A.bounds,并求出A的元素总数elemtotal
	int elemtotal  = 1;
	va_list ap;
	va_start(ap, dim);//ap 为 va_list 类型，是存放变长参数表信息的数组
	for (int i = 0; i < dim; i++)
	{
		A.bounds[i] = va_arg(ap, int);
		if (A.bounds[i] < 0)
		{
			return ERROR;
		}
		elemtotal *= A.bounds[i];
	}
	va_end(ap);
	A.base = (ElemType *)malloc(elemtotal * sizeof(ElemType));
	if (!A.base)
	{
		exit(OVERFLOW);
	}
	//求映像函数的常数 Ci,存入 A.constant[i-1], i = 1,...,dim
	A.constants = (int *)malloc(dim * sizeof(int));
	A.constants[dim-1] = 1;//L=1
	for (int i = dim-2; i >= 0; --i)
	{
		A.constants[i] = A.bounds[i+1] * A.constants[i+1];
	}
	return OK;
}
Status DestroyArray(Array &A)
{//销毁数组 A
	if (!A.base)
	{
		return ERROR;
	}
	free(A.base);
	A.base = NULL;
	if (!A.bounds)
	{
		return ERROR;
	}
	free(A.bounds);
	A.bounds = NULL;	
	if (!A.constants)
	{
		return ERROR;
	}
	free(A.constants);
	A.constants = NULL;
	return OK;
}
Status Locate(Array A, va_list ap, int &off)
{//若 ap 指示的各下标之合法， 则求出该元素在 A 的相对地址 off
	off = 0;
	for (int i = 0; i < A.dim; ++i)
	{
		int index = va_arg(ap, int);
		if (index < 0 || index > A.bounds[i])
		{
			return OVERFLOW;
		}
		off += A.constants[i]*index;
	}
	return OK;
}
Status Value(Array A, ElemType &e, ...)
{// A 是 n 维数组，e 为元素变量，随后是 n 个下标值
//若各下标不越界，则 e 赋值为所指定的 A 的元素值， 并返回 OK
	va_list ap;
	va_start(ap, e);
	int result = 0, off;
	if ((result = Locate(A, ap, off))<=0)
	{
		return result;
	}
	e = *(A.base + off);
	return OK;
}
Status Assign(Array &A, ElemType e, ...)
{// A 是 n 维数组，e 为元素变量，随后是 n 个下标值
//若各下标不越界，则 e 赋给所指定的 A 的元素， 并返回 OK
	va_list ap;
	va_start(ap, e);
	int result, off;
	if ((result = Locate(A, ap, off)) <= 0)
	{
		return result;
	}
	*(A.base + off) = e;
	return OK;
}