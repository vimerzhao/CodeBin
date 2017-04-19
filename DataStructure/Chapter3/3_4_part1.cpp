#include "3_4_part1.h"
int main(int argc, char const *argv[])
{
	LinkQueue Q;
	InitQueue(Q);
	for (int i = 1; i < 10; i++)
	{
		EnQueue(Q, i);
	}
	PrintQueue(Q);
	int e;
	for (int i = 0; i < 5; ++i)
	{
		DeQueue(Q, e);
		printf("%d\t", e);
	}
	return 0;
}