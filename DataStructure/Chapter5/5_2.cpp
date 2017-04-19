//5_2.cpp
/**
author:zhaoyu
date:2016-6-15
*/
#include "5_2.h"
int main(int argc, char const *argv[])
{
	test(3, 1, 2, 3);
	Array A;
	InitArray(A, 3, 4, 2, 2);
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			for (int k = 0; k < 2; ++k)
			{
				Assign(A, 100*i+j*10+k, i, j, k);
			}
		}
	}
	for (int i = 0; i < 4; ++i)
	{
		printf("\n");
		for (int j = 0; j < 2; ++j)
		{
			for (int k = 0; k < 2; ++k)
			{
				int e;
				Value(A, e, i, j, k);
				printf("%d-%d-%d\t%d\n", i, j, k, e);
			}
		}
	}	
	return 0;
}