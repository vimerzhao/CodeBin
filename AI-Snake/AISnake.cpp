#include "Function.h"
int main(void) {
	srand((unsigned int)time(NULL));

	createInterface();						//创建游戏界面
	getchar();								//回车开始
	while (Snake.size() < WIDTH*HEIGHT) {	//未占满全屏
		snakeMove();						//移动一步
		printSnake();						//打印新的蛇
		Sleep(20);							//控制移动速度
		//getchar();
		checkSnake();						//检查移动是否合法，主要调试使用
	}
	getchar();								//回车结束
	return 0;
}