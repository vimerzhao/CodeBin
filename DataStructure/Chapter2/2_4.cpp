#include "2_4.h"

int main(int argc, char const *argv[])
{
	polynomial Pa, Pb, Pc;
	int n;
	printf("Input n\t");
	scanf("%d", &n);
	CreatPolyn(Pa, n);
	printf("Pa\n");
	PrintPolyn(Pa);
	printf("Input n\t");
	scanf("%d", &n);
	CreatPolyn(Pb, n);
	printf("Lb\n");
	PrintPolyn(Pb);
	AddPolyn(Pa, Pb);
	printf("La + Lb\n");
	PrintPolyn(Pa);
	return 0;
}
