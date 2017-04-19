//3_3.cpp
/**
author:zhaoyu
email:zhaoyu1995.com@gmail.com
date:2016-6-8
note:realize my textbook <<数据结构（C语言版）>>
*/
//Page 54
#include <cstdio>
int cnt = 0;
void move(char a, int n, char b)
{
	printf("%i\tmove disk %d from %c to %c\n", ++cnt, n, a, b);
}
void hanoi(int n, char x, char y, char z)
{
	if (1 == n)
	{
		move(x, 1, z);
	}
	else
	{
		hanoi(n-1, x, z, y);
		move(x, n, z);
		hanoi(n-1, y, x, z);
	}
}
int main(int argc, char const *argv[])
{
	int n;
	scanf("%d", &n);
	hanoi(n, 'X', 'Y', 'Z');
	return 0;
}