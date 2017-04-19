//2_3_part1.cpp
/**
author:zhaoyu
email:zhaoyu1995.com@gmail.com
date:2016-6-4
note:realize my textbook <<数据结构（C语言版）>>
*/
#include "head.h"
#include "2_3_part1.h"
int main(int argc, char const *argv[])
{
	LinkList La, Lb, Lc;
	//逆序建表
	printf("Create ListA\n");
	CreateList_L(La, 4);
	printf("Create ListB\n");
	CreateList_L(Lb, 6);
	printf("Print ListA\n");
	PrintList(La);
	printf("Print ListB\n");
	PrintList(Lb);
	printf("Insert 7 at location 4 In ListA\n");
	ListInsert_L(La, 4, 7);
	PrintList(La);
	printf("Delete 7 at location 3 In ListB\n");
	int temp;
	ListDelete_L(Lb, 3, temp);
	PrintList(Lb);
	printf("%d is deleted\n", temp);
	printf("Merge ListA and ListB\n");
	MergeList_L(La, Lb, Lc);
	PrintList(Lc);
	printf("Get element at location 1 2 3 in ListC\n");
	int e1, e2, e3;
	GetElem_L(Lc,1,e1);
	GetElem_L(Lc,2,e2);
	GetElem_L(Lc,3,e3);
	printf("%d\t%d\t%d\n", e1, e2, e3);
	return 0;
}