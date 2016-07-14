#include "Prepare.h"
void setPosition(int x, int y) {
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);
}

void hideCursor() {
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void checkSnake() {
	int x = Snake.front().x;
	int y = Snake.front().y;
	list<SnakeNode>::iterator it = Snake.begin();
	for (it++; it != Snake.end(); it++) {
		if (x == (*it).x && y == (*it).y) {
			setPosition(2, 20);printf("check ERROR 1");
			getchar();
		}
		if ((*it).x > WIDTH || (*it).x < 2 || (*it).y > HEIGHT || (*it).y < 1) {
			setPosition(2, 20);printf("check ERROR 2");
			getchar();
		}
	}
}

void isHeadequalTail() {
	SnakeNode Head = Snake.front();
	SnakeNode Tail = Snake.back();
	if (Head.x == Tail.x && Head.y == Tail.y) {
		setPosition(10, 10);
		printf("isHeadequalTail ERROR");
		for (;true;) {
			getchar();
		}
	}
}

void createFood() {
	int x, y;
	bool flag = true;
	while (flag) {
		x = rand() % (WIDTH / 2);
		x = 2 * (x + 1);
		y = rand() % HEIGHT + 1;
		flag = false;
		for (list<SnakeNode>::iterator it = Snake.begin(); it != Snake.end(); it++) {
			if (x == (*it).x && y == (*it).y) {
				flag = true;
				break;
			}
		}
	}
	Food.x = x;
	Food.y = y;
	setPosition(Food.x, Food.y);
	printf("◆");
}

void printSnake() {
	SnakeNode Head = Snake.front();

	if (Head.x == Food.x && Head.y == Food.y) {
		createFood();
		setPosition(WIDTH / 2 + 2, 0);
		printf("%3d", Snake.size());
	}
	else {
		setPosition(theTail.x, theTail.y);
		printf("  ");
	}
	setPosition(Head.x, Head.y);
	printf("■");
}

bool isOpposite(int a, int b) {
	if (0 == a && 1 == b) {
		return true;
	}
	else if (1 == a && 0 == b) {
		return true;
	}
	else if (2 == a && 3 == b) {
		return true;
	}
	else if (3 == a && 2 == b) {
		return true;
	}
	else {
		return false;
	}

}

void createInterface() {
	system("mode con cols=44 lines=24");									//设置控制台
	hideCursor();															//
	for (int i = 0; i <= WIDTH; i += 2) {
		setPosition(i, 0);
		printf("●");
		setPosition(i, HEIGHT + ROWUNIT);
		printf("●");
	}
	for (int i = 0; i < HEIGHT + 2; i++) {
		setPosition(0, i);
		printf("●");
		setPosition(WIDTH + COLUMNUNIT, i);
		printf("●");//
	}
	setPosition(WIDTH / 2 - 2, 21);
	printf("AI Snake");
	SnakeNode temp;
	for (int i = 0; i < 4; i++) {
		temp.x = 2 + i * 2;
		temp.y = 1;
		setPosition(temp.x, temp.y);
		printf("■");
		Snake.push_front(temp);
	}
	SnakeDirection = 2;
	setPosition(WIDTH / 2 - 2, 0);
	printf("Score:");
	setPosition(WIDTH / 2 + 2, 0);
	printf("%3d", Snake.size());
	createFood();
}

void getFoodDistance() {

	
	for (int x = 2; x <= WIDTH; x += COLUMNUNIT) {
		for (int y = 1; y <= HEIGHT; y += ROWUNIT) {
			bfsDistance[x][y] = INF;														//初始化
		}
	}
	for (list<SnakeNode>::iterator it = Snake.begin(); it != Snake.end(); it++) {
		if ((*it).x < 2 || (*it).x > WIDTH || (*it).y < 1 || (*it).y >HEIGHT) {				//调试使用，异常情况
			setPosition(44, 20);printf("110:%d--d%", (*it).x, (*it).y);
			getchar();
		}
		bfsDistance[(*it).x][(*it).y] = SNAKEBODY;											//蛇身用常量SNAKEBODY标记
	}
	for (int x = 0; x <= WIDTH + COLUMNUNIT; x += COLUMNUNIT) {
		bfsDistance[x][0] = bfsDistance[x][HEIGHT + ROWUNIT] = WALL;
	}
	for (int y = 0; y <= HEIGHT + ROWUNIT; y += ROWUNIT) {
		bfsDistance[0][y] = bfsDistance[WIDTH + COLUMNUNIT][y] = WALL;
	}																						//围墙用常量WALL标记
	bfsDistance[Food.x][Food.y] = 0;														//食物位置记为 0

	/************************************************************************/
	/*依据bfsDistance通过BFS搜索每个节点，计算与食物的距离，蛇身与墙视为无穷大	*/
	/************************************************************************/
	queue<SnakeNode> Point;
	Point.push(Food);
	SnakeNode temp, adj;
	while (!Point.empty()) {
		temp = Point.front();												
		Point.pop();												
		for (int i = 0; i < 4; i++) {
			adj.x = temp.x + Direction[i][0];
			adj.y = temp.y + Direction[i][1];
			if (INF == bfsDistance[adj.x][adj.y]) {											//INF说明是尚未遍历的位置
				bfsDistance[adj.x][adj.y] = bfsDistance[temp.x][temp.y] + 1;
				Point.push(adj);
			}
		}
	}
	/************************************************************************/
	/* 注:函数运行结束应该求出每个位置与食物的距离		                        */
	/************************************************************************/
}

bool isReachable() {
	SnakeNode temp;
	SnakeNode Head = Snake.front();
	for (int i = 0; i < 4; i++) {
		if (isOpposite(i, SnakeDirection)) {									//相反方向排除
			continue;
		}
		temp.x = Head.x + Direction[i][0];
		temp.y = Head.y + Direction[i][1];
		if (bfsDistance[temp.x][temp.y] < INF) {								//检查是否有可达到的方向
			return true;
		}
	}
	return false;
}

bool isVirtualTailReachable_Regular() {
	int tempDirection = -1, virtualDirection = SnakeDirection;
	/************************************************************************/
	/* 为了判断吃了食物后能否找到蛇尾，建立一条虚拟蛇前去吃食物，并判断           */
	/************************************************************************/
	VirtualSnake.clear();															//初始化
	for (list<SnakeNode>::iterator it = Snake.begin(); it != Snake.end(); it++) {
		VirtualSnake.push_back(*(it));
	}																				//复制蛇身
	SnakeNode MoveHead = Snake.front();												//获取蛇头
	SnakeNode adj;																	//保存临时使用的邻居变量
	SnakeNode Result;																//保存最终选择的邻居位置
	while (MoveHead.x != Food.x || MoveHead.y != Food.y) {							//如果蛇头不在食物位置
		/************************************************************************/
		/* 寻找蛇头附近距离食物最近的位置                                          */
		/************************************************************************/
		int MinDistance = INF;
		for (int i = 0; i < 4; i++) {
			if (isOpposite(i, virtualDirection)) {
				continue;
			}
			adj.x = MoveHead.x + Direction[i][0];
			adj.y = MoveHead.y + Direction[i][1];
			if (bfsDistance[adj.x][adj.y] < MinDistance) {
				MinDistance = bfsDistance[adj.x][adj.y];
				Result.x = adj.x;
				Result.y = adj.y;
				tempDirection = i;
			}
		}
		if (MinDistance >= INF) {												//异常情况
			setPosition(46, 10);
			printf("isVirtualTailReachable_Regular ERROR");
			getchar();
			exit(-101);															//设置一个错误代码
		}
		VirtualSnake.push_front(Result);
		if (Result.x != Food.x || Result.y != Food.y) {							//如果没吃到食物
			VirtualSnake.pop_back();												//虚拟蛇移动
		}
		virtualDirection = tempDirection;										//更新虚拟蛇的方向
		if (MoveHead.x == Snake.front().x && MoveHead.y == Snake.front().y) {	//第一步记录下来，以备使用
			NextStep.x = Result.x;
			NextStep.y = Result.y;
			NextDirection = virtualDirection;
		}																		//逻辑正确的话这个语句块只执行一次
		MoveHead = VirtualSnake.front();										//更新蛇头位置
	}
	//VirtualSnake.pop_back();注意蛇尾不用减去，虚拟蛇吃蛇舞后蛇厂家一
	/************************************************************************/
	/* 运行到此，虚拟蛇应该吃了食物                                            */
	/************************************************************************/


	/************************************************************************/
	/* 这里检查一下虚拟蛇的移动是否正确，如果正确至少蛇长度+1                   */
	/************************************************************************/
	if (VirtualSnake.size() != Snake.size()+1) {
		setPosition(46, 11);printf("VirtualSnake != Snake, enter to exit");
		getchar();getchar();
		exit(-1);
	}

	/************************************************************************/
	/* 通过虚拟蛇计算虚拟蛇头是否能到达虚拟蛇尾                                 */
	/*计算方法同样是 BFS，蛇头可达位置与蛇尾可达即能到达							*/
	/*首先初始化计算矩阵 bfsJudgeVirtualTail									*/
	/************************************************************************/
	for (int x = 2; x <= WIDTH; x += COLUMNUNIT) {
		for (int y = 1; y <= HEIGHT; y += ROWUNIT) {
			bfsJudgeVirtualTail[x][y] = INF;
		}
	}
	for (list<SnakeNode>::iterator it = VirtualSnake.begin(); it != VirtualSnake.end(); it++) {
		bfsJudgeVirtualTail[(*it).x][(*it).y] = SNAKEBODY;
	}
	for (int x = 0; x <= WIDTH + COLUMNUNIT; x += COLUMNUNIT) {
		bfsJudgeVirtualTail[x][0] = bfsJudgeVirtualTail[x][HEIGHT + ROWUNIT] = WALL;
	}
	for (int y = 0; y <= HEIGHT + ROWUNIT; y += ROWUNIT) {
		bfsJudgeVirtualTail[0][y] = bfsJudgeVirtualTail[WIDTH + COLUMNUNIT][y] = WALL;
	}
	SnakeNode VirtualSnakeTail = VirtualSnake.back();
	bfsJudgeVirtualTail[VirtualSnakeTail.x][VirtualSnakeTail.y] = 0;					//蛇尾是出发点，标记为0
	/************************************************************************/
	/* 利用队列 BFS 求出与虚拟蛇尾的曼哈顿距离                                 */
	/************************************************************************/
	queue<SnakeNode> Point;
	Point.push(VirtualSnakeTail);
	SnakeNode temp;
	while (!Point.empty()) {
		temp = Point.front();												//取队首
		Point.pop();
		for (int i = 0; i < 4; i++) {
			adj.x = temp.x + Direction[i][0];
			adj.y = temp.y + Direction[i][1];
			if (INF == bfsJudgeVirtualTail[adj.x][adj.y]) {							//未搜索点
				bfsJudgeVirtualTail[adj.x][adj.y] = bfsJudgeVirtualTail[temp.x][temp.y] + 1;
				Point.push(adj);
			}
		}
	}
	/************************************************************************/
	/*运行到此处应该正确输出虚拟蛇尾到每个位置的距离	                            */
	/************************************************************************/
	bfsJudgeVirtualTail[VirtualSnakeTail.x][VirtualSnakeTail.y] = SNAKEBODY;//遗忘此句会出现蛇头吃掉蛇尾的可能

	/************************************************************************/
	/*现在判断虚拟蛇头是否可达                                                */
	/************************************************************************/
	int TailDistance = INF;
	SnakeNode VirtualSnakeHead = VirtualSnake.front();
	for (int i = 0; i < 4; i++) {
		adj.x = VirtualSnakeHead.x + Direction[i][0];
		adj.y = VirtualSnakeHead.y + Direction[i][1];
		if (bfsJudgeVirtualTail[adj.x][adj.y] < INF) {
			return true;
		}
	}
	return false;
}

bool isVirtualTailReachable_Random() {
	int tempDirection = -1, virtualDirection = SnakeDirection;
	/************************************************************************/
	/* 为了判断吃了食物后能否找到蛇尾，建立一条虚拟蛇前去吃食物，并判断           */
	/************************************************************************/
	VirtualSnake.clear();															//初始化
	for (list<SnakeNode>::iterator it = Snake.begin(); it != Snake.end(); it++) {
		VirtualSnake.push_back(*(it));
	}																				//复制蛇身
	SnakeNode MoveHead = Snake.front();												//获取蛇头
	SnakeNode adj;																	//保存临时使用的邻居变量
	SnakeNode Result;																//保存最终选择的邻居位置
	while (MoveHead.x != Food.x || MoveHead.y != Food.y) {							//如果蛇头不在食物位置
		/************************************************************************/
		/* 寻找蛇头附近距离食物最近的位置                                          */
		/************************************************************************/
		int MinDistance = INF;
		int random_i = rand() % 4;
		for (int t = 0; t < 4; random_i = (random_i + 1) % 4,t++) {
			if (isOpposite(random_i, virtualDirection)) {
				continue;
			}
			adj.x = MoveHead.x + Direction[random_i][0];
			adj.y = MoveHead.y + Direction[random_i][1];
			if (bfsDistance[adj.x][adj.y] < MinDistance) {
				MinDistance = bfsDistance[adj.x][adj.y];
				Result.x = adj.x;
				Result.y = adj.y;
				tempDirection = random_i;
			}
		}
		if (MinDistance >= INF) {												//异常情况
			setPosition(46, 10);
			printf("isVirtualTailReachable_Regular ERROR");
			getchar();
			exit(-1);
		}
		VirtualSnake.push_front(Result);
		if (Result.x != Food.x || Result.y != Food.y) {							//如果没吃到食物
			VirtualSnake.pop_back();												//虚拟蛇移动
		}
		virtualDirection = tempDirection;										//更新虚拟蛇的方向
		if (MoveHead.x == Snake.front().x && MoveHead.y == Snake.front().y) {	//第一步记录下来，以备使用
			NextStep.x = Result.x;
			NextStep.y = Result.y;
			NextDirection = virtualDirection;
		}																		//逻辑正确的话这个语句块只执行一次
		MoveHead = VirtualSnake.front();										//更新蛇头位置
	}
	/************************************************************************/
	/* 运行到此，虚拟蛇应该吃了食物                                            */
	/************************************************************************/

	/************************************************************************/
	/* 这里检查一下虚拟蛇的移动是否正确，如果正确至少蛇长度+1                    */
	/************************************************************************/
	if (VirtualSnake.size() != Snake.size()+1) {
		setPosition(46, 11);printf("VirtualSnake != Snake, enter to exit");
		getchar();getchar();
		exit(-1);
	}

	/************************************************************************/
	/* 通过虚拟蛇计算虚拟蛇头是否能到达虚拟蛇尾                                 */
	/*计算方法同样是 BFS，蛇头可达位置与蛇尾可达即能到达							*/
	/*首先初始化计算矩阵 bfsJudgeVirtualTail									*/
	/************************************************************************/
	for (int x = 2; x <= WIDTH; x += COLUMNUNIT) {
		for (int y = 1; y <= HEIGHT; y += ROWUNIT) {
			bfsJudgeVirtualTail[x][y] = INF;
		}
	}
	for (list<SnakeNode>::iterator it = VirtualSnake.begin(); it != VirtualSnake.end(); it++) {
		bfsJudgeVirtualTail[(*it).x][(*it).y] = SNAKEBODY;
	}
	for (int x = 0; x <= WIDTH + COLUMNUNIT; x += COLUMNUNIT) {
		bfsJudgeVirtualTail[x][0] = bfsJudgeVirtualTail[x][HEIGHT + ROWUNIT] = WALL;
	}
	for (int y = 0; y <= HEIGHT + ROWUNIT; y += ROWUNIT) {
		bfsJudgeVirtualTail[0][y] = bfsJudgeVirtualTail[WIDTH + COLUMNUNIT][y] = WALL;
	}
	SnakeNode VirtualSnakeTail = VirtualSnake.back();
	bfsJudgeVirtualTail[VirtualSnakeTail.x][VirtualSnakeTail.y] = 0;					//蛇尾是出发点，标记为0
	/************************************************************************/
	/* 利用队列 BFS 求出与虚拟蛇尾的曼哈顿距离                                 */
	/************************************************************************/
	queue<SnakeNode> Point;
	Point.push(VirtualSnakeTail);
	SnakeNode temp;
	while (!Point.empty()) {
		temp = Point.front();												//取队首
		Point.pop();
		for (int i = 0; i < 4; i++) {
			adj.x = temp.x + Direction[i][0];
			adj.y = temp.y + Direction[i][1];
			if (INF == bfsJudgeVirtualTail[adj.x][adj.y]) {							//未搜索点
				bfsJudgeVirtualTail[adj.x][adj.y] = bfsJudgeVirtualTail[temp.x][temp.y] + 1;
				Point.push(adj);
			}
		}
	}
	/************************************************************************/
	/*运行到此处应该正确输出虚拟蛇尾到每个位置的距离                             */
	/************************************************************************/
	bfsJudgeVirtualTail[VirtualSnakeTail.x][VirtualSnakeTail.y] = SNAKEBODY;			//遗忘此句会出现蛇头吃掉蛇尾的可能

	/************************************************************************/
	/*现在判断虚拟蛇头是否可达                                                */
	/************************************************************************/
	int TailDistance = INF;
	SnakeNode VirtualSnakeHead = VirtualSnake.front();
	for (int i = 0; i < 4; i++) {
		adj.x = VirtualSnakeHead.x + Direction[i][0];
		adj.y = VirtualSnakeHead.y + Direction[i][1];
		if (bfsJudgeVirtualTail[adj.x][adj.y] < INF) {
			return true;
		}
	}
	return false;

}

void eatFoodMove() {
	Snake.push_front(NextStep);													//删除尾巴的操作在打印时进行
	SnakeDirection = NextDirection;												//注意要更新方向
	theTail = Snake.back();
	if (NextStep.x != Food.x || NextStep.y != Food.y) {
		Snake.pop_back();														//没吃到食物蛇尾就删除
	}
}

bool eatFood() {
	getFoodDistance();
	if (isReachable()) {
		if (isVirtualTailReachable_Regular() || isVirtualTailReachable_Random()) {//注：这里用了短路运算
			eatFoodMove();
			return true;
		} else {
			return false;
		}
	}
	return false;																   //如果不可达
}

bool isSafe(SnakeNode NewHead) {
	VirtualSnake.clear();
	for (list<SnakeNode>::iterator it = Snake.begin(); it != Snake.end(); it++) {
		VirtualSnake.push_back(*(it));
	}																			//复制真实蛇
	VirtualSnake.push_front(NewHead);
	VirtualSnake.pop_back();

	for (int x = 2; x <= WIDTH; x += COLUMNUNIT) {								//广度优先求出每个位置与虚拟蛇尾的距离
		for (int y = 1; y <= HEIGHT; y += ROWUNIT) {
			bfsJudgeVirtualTail[x][y] = INF;
		}
	}
	for (list<SnakeNode>::iterator it = VirtualSnake.begin(); it != VirtualSnake.end(); it++) {
		bfsJudgeVirtualTail[(*it).x][(*it).y] = SNAKEBODY;
	}
	for (int x = 0; x <= WIDTH + COLUMNUNIT; x += COLUMNUNIT) {
		bfsJudgeVirtualTail[x][0] = bfsJudgeVirtualTail[x][HEIGHT + ROWUNIT] = WALL;
	}
	for (int y = 0; y <= HEIGHT + ROWUNIT; y += ROWUNIT) {
		bfsJudgeVirtualTail[0][y] = bfsJudgeVirtualTail[WIDTH + COLUMNUNIT][y] = WALL;
	}
	SnakeNode VirtualSnakeTail = VirtualSnake.back();
	bfsJudgeVirtualTail[VirtualSnakeTail.x][VirtualSnakeTail.y] = 0;
	queue<SnakeNode> Point;
	Point.push(VirtualSnakeTail);												//利用队列 BFS 求出与虚拟蛇尾的曼哈顿距离
	SnakeNode temp, adj;
	while (!Point.empty()) {
		temp = Point.front();													//取队首
		Point.pop();
		for (int i = 0; i < 4; i++) {
			adj.x = temp.x + Direction[i][0];
			adj.y = temp.y + Direction[i][1];
			if (INF == bfsJudgeVirtualTail[adj.x][adj.y]) {						//未搜索点
				bfsJudgeVirtualTail[adj.x][adj.y] = bfsJudgeVirtualTail[temp.x][temp.y] + 1;
				Point.push(adj);
			}
		}
	}
	bfsJudgeVirtualTail[VirtualSnakeTail.x][VirtualSnakeTail.y] = SNAKEBODY;	//易忘

	SnakeNode VirtualSnakeHead = VirtualSnake.front();
	if (VirtualSnakeHead.x != NewHead.x || VirtualSnakeHead.y != NewHead.y) {
		setPosition(46, 16);printf("VirtualSnakeHead != NewHead");
		getchar();
		exit(-105);																//异常情况，调试使用
	}
	for (int i = 0; i < 4; i++) {
		adj.x = VirtualSnakeHead.x + Direction[i][0];
		adj.y = VirtualSnakeHead.y + Direction[i][1];
		if (bfsJudgeVirtualTail[adj.x][adj.y] < INF) {
			return true;
		}
	}
	return false;
}

void followTailMove() {

	Snake.push_front(NextStep);//删除尾巴的操作在打印时进行
	SnakeDirection = NextDirection;//注意要更新方向
	theTail = Snake.back();
	if (NextStep.x != Food.x || NextStep.y != Food.y) {
		Snake.pop_back();
	}
}

int FoodDistance(SnakeNode adj) {
	if (bfsDistance[adj.x][adj.y] < INF) {										//如何可达就用这个距离
		return bfsDistance[adj.x][adj.y];								
	} else {
		return abs(adj.x - Food.x) + abs(adj.y - Food.y);						//不可达就用曼哈顿距离
	}
}

bool followTail() {
	/************************************************************************/
	/* 如果不能去吃食物，则执行方案二：远离食物，但是必须保证移动后能找到蛇      */
	/************************************************************************/
	for (int x = 2; x <= WIDTH; x += COLUMNUNIT) {
		for (int y = 1; y <= HEIGHT; y += ROWUNIT) {
			bfsJudgeRealTail[x][y] = INF;
		}
	}
	for (list<SnakeNode>::iterator it = Snake.begin(); it != Snake.end(); it++) {
		bfsJudgeRealTail[(*it).x][(*it).y] = SNAKEBODY;
	}
	for (int x = 0; x <= WIDTH + COLUMNUNIT; x += COLUMNUNIT) {
		bfsJudgeRealTail[x][0] = bfsJudgeRealTail[x][HEIGHT + ROWUNIT] = WALL;
	}
	for (int y = 0; y <= HEIGHT + ROWUNIT; y += ROWUNIT) {
		bfsJudgeRealTail[0][y] = bfsJudgeRealTail[WIDTH + COLUMNUNIT][y] = WALL;
	}

	SnakeNode RealSnakeTail = Snake.back();
	bfsJudgeRealTail[RealSnakeTail.x][RealSnakeTail.y] = 0;
	//利用队列 BFS 求出与蛇尾的曼哈顿距离
	queue<SnakeNode> Point;
	Point.push(RealSnakeTail);
	SnakeNode temp, adj;
	while (!Point.empty()) {
		temp = Point.front();												//取队首
		Point.pop();
		for (int i = 0; i < 4; i++) {
			adj.x = temp.x + Direction[i][0];
			adj.y = temp.y + Direction[i][1];
			if (INF == bfsJudgeRealTail[adj.x][adj.y]) {					//未搜索点
				bfsJudgeRealTail[adj.x][adj.y] = bfsJudgeRealTail[temp.x][temp.y] + 1;
				Point.push(adj);
			}
		}
	}
	//运行到这里应该计算出了蛇尾到所有位置的距离，下面判定蛇头是否可以到达蛇尾
	//尤其需要注意的是，蛇尾位置要重置为 SNAKEBODY，否则蛇头会与蛇尾重合
	bfsJudgeRealTail[RealSnakeTail.x][RealSnakeTail.y] = SNAKEBODY;
	/************************************************************************/
	/* 先判断蛇尾是否可达			                                            */
	/************************************************************************/
	SnakeNode tempAdj;
	SnakeNode tempHead = Snake.front();
	bool tempReachable = false;
	for (int i = 0; i < 4; i++) {
		tempAdj.x = tempHead.x + Direction[i][0];
		tempAdj.y = tempHead.y + Direction[i][1];
		if (bfsJudgeRealTail[tempAdj.x][tempAdj.y] < INF) {
			tempReachable = true;
			break;
		}
	}
	/************************************************************************/
	/*如果可以则移动一步，应满足移动之后仍能找到蛇尾                            */
	/*移动与食物尽可能远离，以腾出空间等待食物可以到达                          */
	/************************************************************************/
	int SafeDirection[4];
	int count = 0;
	SnakeNode RealHead = Snake.front();
	for (int i = 0; i < 4; i++) {											//找出可以移动的方向
		if (isOpposite(i, SnakeDirection)) {
			continue;
		}
		adj.x = RealHead.x + Direction[i][0];
		adj.y = RealHead.y + Direction[i][1];
		if (bfsJudgeRealTail[adj.x][adj.y] < INF) {
			if (isSafe(adj)) {
				SafeDirection[count] = i;
				count++;
			}
		}
	}

	int MaxDistance = -1;
	SnakeNode Result;
	if (0 == count) {														//如果没有安全的方向
		return false;
	}
	else {
		for (int k = 0; k < count; k++) {									//找出最远的方向
			adj.x = RealHead.x + Direction[SafeDirection[k]][0];
			adj.y = RealHead.y + Direction[SafeDirection[k]][1];
			int tempDistance = FoodDistance(adj);
			if (tempDistance > MaxDistance) {
				MaxDistance = tempDistance;
				Result.x = adj.x;
				Result.y = adj.y;
				NextDirection = SafeDirection[k];
			}
		}
		NextStep.x = Result.x;
		NextStep.y = Result.y;
		followTailMove();
		checkSnake();														//检查
		isHeadequalTail();
		return true;
	}
	return false;
}

void DFS(int x, int y)
{
	if (WALL == dfsDistance[x][y]) {
		return;
	}
	dfsDistance[x][y] = WALL;											//这一句要加深理解
	if (currentDepth > deepest) {
		deepest = currentDepth;
	}
	currentDepth++;
	SnakeNode Temp;
	for (int i = 0; i < 4; i++) {
		Temp.x = x + Direction[i][0];
		Temp.y = y + Direction[i][1];
		if (Temp.x < 2 || Temp.x > WIDTH || Temp.y < 1 || Temp.y > HEIGHT) {
			continue;
		}
		if (WALL == dfsDistance[Temp.x][Temp.y]) {
			continue;
		}
		DFS(Temp.x, Temp.y);
	}
	currentDepth--;
}

int getDepth(SnakeNode temp) {
	for (int x = 2; x <= WIDTH; x += COLUMNUNIT) {						//初始化;为了不至于递归导致始终横跨，所以边沿不计入递归
		for (int y = 1; y <= HEIGHT; y += ROWUNIT) {
			dfsDistance[x][y] = 0;
		}
	}
	//这里留个边界
	for (int x = 0; x <= WIDTH + COLUMNUNIT; x += COLUMNUNIT) {
		dfsDistance[x][0] = dfsDistance[x][HEIGHT + ROWUNIT] = WALL;
	}
	for (int y = 1; y <= HEIGHT + ROWUNIT; y += ROWUNIT) {
		dfsDistance[0][y] = dfsDistance[WIDTH + COLUMNUNIT][y] = WALL;
	}

	for (list<SnakeNode>::iterator it = Snake.begin(); it != Snake.end(); it++) {
		if ((*it).x < 2 || (*it).x> WIDTH || (*it).y > HEIGHT || (*it).y < 1) {		//调试使用
			setPosition(44, 4);printf("iterator Error\t%d\t%d", (*it).x, (*it).y);
			getchar();
			exit(-1);
		}
		dfsDistance[(*it).x][(*it).y] = WALL;
	}

	currentDepth = 0, deepest = -1;											//初始化
	DFS(temp.x, temp.y);													//递归深度优先遍历
	return deepest;
}

void snakeWander() {
	SnakeNode NextHead;														//为了让蛇在困境中尽可能坚持，采用 DFS 
	SnakeNode temp;
	SnakeNode CurrentHead = Snake.front();

	for (int x = 4; x <= WIDTH - COLUMNUNIT; x += COLUMNUNIT) {				//辅助矩阵，标记不可移动到的点
		for (int y = 2; y <= HEIGHT - ROWUNIT; y += ROWUNIT) {
			Mark[x][y] = INF;
		}
	}																		//蛇以及墙的位置再特殊标记
	for (list<SnakeNode>::iterator it = VirtualSnake.begin(); it != VirtualSnake.end(); it++) {
		Mark[(*it).x][(*it).y] = SNAKEBODY;
	}
	for (int x = 0; x <= WIDTH + COLUMNUNIT; x += COLUMNUNIT) {
		Mark[x][0] = Mark[x][HEIGHT + ROWUNIT] = WALL;
		Mark[x][1] = Mark[x][HEIGHT] = WALL;
	}
	for (int y = 0; y <= HEIGHT + ROWUNIT; y += ROWUNIT) {
		Mark[0][y] = Mark[WIDTH + COLUMNUNIT][y] = WALL;
		Mark[2][y] = Mark[WIDTH][y] = WALL;
	}

	int MaxDepth = -100;
	int tempDirection = -1;
	for (int i = 0; i < 4; i++) {
		if (isOpposite(i, SnakeDirection)){
			continue;
		}
		temp.x = CurrentHead.x + Direction[i][0];
		temp.y = CurrentHead.y + Direction[i][1];
		if (INF == Mark[temp.x][temp.y]) {									//判断是否符合要求
			int depth = getDepth(temp);
			if (depth > MaxDepth) {
				MaxDepth = depth;
				tempDirection = i;
				NextHead.x = temp.x;
				NextHead.y = temp.y;
			}
		}
	}
	if (-1 == tempDirection) {
		getchar();
		exit(-3);
	} else {
		SnakeDirection = tempDirection;
		Snake.push_front(NextHead);
		theTail = Snake.back();
		if (NextStep.x != Food.x || NextStep.y != Food.y) {
			Snake.pop_back();
		}
	}
}

void snakeMove() {
	if (!eatFood()) {														//满足状态1：去吃食物
		if (!followTail()) {												//满足状态2：追尾巴
			snakeWander();													//随机选择
		}
	}
}
