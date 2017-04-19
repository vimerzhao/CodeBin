#include "3_2_maze.h"
int main(int argc, char const *argv[])
{
	freopen("map.txt", "r", stdin);
	printf("Map\n");
	int size;
	scanf("%d", &size);
	MakeMap(size);
	PosType start, end;
	scanf("%d%d", &start.x, &start.y);
	printf("start:\tx:%d\ty:%d\n", start.x, start.y);
	scanf("%d%d", &end.x, &end.y);
	printf("end:\tx:%d\ty:%d\n", end.x, end.y);
	MazeType maze;//un useded for me
	MazePath(maze, start, end, size);
	//PrintPath(size);
	return 0;
}