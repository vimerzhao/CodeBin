#include "3_4_part2.h"
int main(int argc, char const *argv[])
{
	SqQueue Q;
	InitQueue(Q);
	for (int i = 1; i < 10; ++i)
	{
		EnQueue(Q, i*10);
	}
	PrintQueue(Q);
	int e;
	EnQueue(Q, 100);
	PrintQueue(Q);
	DeQueue(Q, e);
	printf("%d\n", e);
	EnQueue(Q, 100);
	PrintQueue(Q);
	return 0;
}