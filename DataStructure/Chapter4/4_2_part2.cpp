#include "4_2_part2.h"
int main(int argc, char const *argv[])
{
	char S[100];
	HString T = {NULL, 0};
	HString S1 = {NULL, 0};
	HString S2 = {NULL, 0};
	scanf("%s", S);
	StrAssign(S1, S);
	printf("S1:\t");
	PrintHString(S1);
	printf("StrLength\t%d\n", StrLength(S1));
	scanf("%s", S);
	StrAssign(S2, S);
	printf("S2\t");
	PrintHString(S2);
	printf("StrLength\t%d\n", StrLength(S1));
	printf("Compare S1 vs S2\t%d\n", StrCompare(S1, S2));
	Concat(T, S1, S2);
	printf("T = S1 + S2\t");
	PrintHString(T);
	SubString(S1, T, 2, 3);
	printf("Sub :T[2]-T[4]\t");
	PrintHString(S1);
	return 0;
}