#include "5_5.h"
int main(int argc, char const *argv[])
{
//	freopen("out.txt", "w", stdout);
	//test function sever

	SString s, t;
	scanf("%s", s+1);
	s[0] = StrLength(s);
	sever(s, t);
	printf("t:%s\n", t+1);
	printf("s:%s\n", s+1);

	GList L = NULL, T = NULL;
	SString S;
	scanf("%s", S+1);
	S[0] = StrLength(S);
	printf("len: %d\n", S[0]);
	CreateGList(L, S);
	printf("\nL--Depth\t%d\n", GListDepth(L));
	CopyGList(T, L);
	printf("\nT--Depth\t%d\n", GListDepth(T));
	printf("T:\t");
	PrintGList(T);
	printf("\n");
	return 0;
}