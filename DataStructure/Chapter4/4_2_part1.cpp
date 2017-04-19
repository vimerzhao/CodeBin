#include "4_2_part1.h"
int main(int argc, char const *argv[])
{
	SString S1;
	SString S2;
	SString T;
	scanf("%s", S1+1);
	*S1 = StrLength(S1);
	scanf("%s", S2+1);
	*S2 = StrLength(S2);
	Concat(T, S1, S2);
	PrintSString(T);
	int pos = 2, len = 5;
	SubString(T, S1, pos, len);
	PrintSString(T); 
	return 0;
}