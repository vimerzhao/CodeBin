#include "3_2_expression.h"
int main(int argc, char const *argv[])
{
	float ans;
	for (int i = 0; i < 5; ++i)
	{
		ans = EvaluateExpression();
		printf("%.3f\n", ans);
	}
	return 0;
}