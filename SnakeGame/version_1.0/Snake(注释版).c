//2016-2-12
//zhaoyu
//Gmail:zhaoyu1995.com@gmail.com
//Language: C
//Platform:Code::Blocks
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

typedef struct snake
{
    int x;
    int y;
    struct snake *next;
}Snake;//一节蛇身的坐标，并借助链表连接蛇身
int X, Y;//食物的坐标
enum STATUS{Up = 1, Down, Left, Right};
//定义一个枚举类型，表示蛇头移动方向，用枚举类型提高了代码可读性
Snake *pHead, *pBody;//定义两个指针，记录蛇头位置以及遍历蛇身时用
enum STATUS Direction;//定义一个枚举变量，记录蛇头当前方向
int score=0, scorePerFood=10;//总分数以及每个食物的分数
int gameStatus = 0;//游戏状态：0正常 1、撞墙死亡2、咬到自己3、选择退出
int timeInterval = 200;//间隔时间，数值越小则蛇身移动越快
void gameEnd(void);
void setPosition(int x, int y)
{
    COORD pos;
/*
    COORD是Windows API中定义的一种结构，表示一个字符在控制台屏幕上的坐标。其定义为：
    typedef struct _COORD {
    SHORT X; // horizontal coordinate
    SHORT Y; // vertical coordinate
    } COORD;
*/
    HANDLE hOutput;
/*
    HANDLE：句柄，是Windows用来表示对象的（不是C++的对象），HANDLE是一个通用句柄表示。
*/
    pos.X = x;
    pos.Y = y;
    hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
/*
    GetStdHandle是一个Windows API函数。
    它用于从一个特定的标准设备（标准输入、标准输出或标准错误）中取得一个句柄（用来标识不同设备的数值）。
*/
    SetConsoleCursorPosition(hOutput, pos);
/*
    SetConsoleCursorPosition是API中定位光标位置的函数。
*/
}
void hideCursor()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
    //这个结构包含的是控制台光标的信息
    /*
    * typedef struct _CONSOLE_CURSOR_INFO {
    * DWORD dwSize;// 光标百分比厚度（1~100） 
    *  BOOL  bVisible;// 是否可见
    * } CONSOLE_CURSOR_INFO, *PCONSOLE_CURSOR_INFO;
    */
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
    /*
    GetConsoleCursorInfo 函数
    检索有关指定的控制台屏幕缓冲区的光标的可见性和大小信息。
    语法
    BOOL WINAPI GetConsoleCursorInfo(
    　　__in HANDLE hConsoleOutput,
    　　__out PCONSOLE_CURSOR_INFO lpConsoleCursorInfo
    　　);
    hConsoleOutput:控制台屏幕缓冲区的句柄。该句柄必须具有的 GENERIC_READ 的访问权限。
    lpConsoleCursorInfo:指向接收有关该控制台的光标的信息的CONSOLE_CURSOR_INFO结构的指针。
    */
}
void creatInterface(void)
{
    int i;
    for(i=0;i<58;i+=2)
    {
        setPosition(i,0);
        printf("¡ö");//a ¡ö occupy two character space
        setPosition(i,26);
        printf("¡ö");
    }
    for(i=1;i<26;i++)
    {
        setPosition(0,i);
        printf("¡ö");
        setPosition(56,i);
        printf("¡ö");
    }
    setPosition(65, 10);
    printf("Introduction");
    setPosition(63,12);
    printf("¡ü\tMove up.");
    setPosition(63,14);
    printf("¡ý\tMove Down.");
    setPosition(63,16);
    printf("¡û\tMove left.");
    setPosition(63,18);
    printf("¡ú\tMove right.");
    setPosition(63,20);
    printf("F1\tSpeed up.");
    setPosition(63, 22);
    printf("F2\tSlow down.");
    setPosition(63, 24);
    printf("Space\tGame pause.");
    setPosition(63,26);
    printf("ESC\tQuit the game.");
    setPosition(63, 4);
    printf("Score:");
    setPosition(63, 6);
    printf("Score Per Food:");
}
void initializeSnake(void)
{
    Snake *pTail;
    int i;
    pTail = (Snake *)malloc(sizeof(Snake));
    pTail->x = 24;
    pTail->y = 5;
    pTail->next = NULL;
    for (i = 1; i < 4; i++)
    {
        pHead = (Snake *)malloc(sizeof(Snake));
        pHead->next = pTail;
        pHead->x = 24 + 2*i;
        pHead->y = 5;
        pTail = pHead;//Important
    }
    while (pTail != NULL)
    {
        setPosition(pTail->x, pTail->y);
        printf("¡ö");
        pTail = pTail->next;
    }
}
int biteSelf(void)
{
    Snake *pSelf = pHead->next;
    while (pSelf != NULL)
    {
        if (pHead->x==pSelf->x && pHead->y==pSelf->y)
        {
            return 1;
        }
        pSelf = pSelf->next;
    }
    return 0;
}
void creatFood(void)
{
    int flag = 0;
    X = Y = 4;
    srand((unsigned)time(NULL));
    do{
        X = rand()%52 + 2;
        Y = rand()%24 + 1;

        if (X%2 != 0)
        {
            flag = 1;
        }
        else
        {
            flag = 0;
        }//Important
        pBody = pHead;
        while (pBody->next != NULL)
        {
            if(pBody->x==X && pBody->y==Y)
            {
                flag = 1;
            }
            pBody = pBody->next;
        }
    }while(flag==1);
    setPosition(X, Y);
    printf("¡ö");
}

void hitWall(void)
{
    if (pHead->x==0 || pHead->x>=56 || pHead->y==0 || pHead->y>=26)
    {
        gameStatus = 1;
        gameEnd();
    }
}
void snakeMove(void)
{
    Snake *pNextHead;
    hitWall();

    pNextHead = (Snake *)malloc(sizeof(Snake));
    pNextHead->next = pHead;
    switch(Direction)
    {
    case Up:
        pNextHead->x = pHead->x;
        pNextHead->y = pHead->y - 1;
        break;
    case Down:
        pNextHead->x = pHead->x;
        pNextHead->y = pHead->y + 1;
        break;
    case Right:
        pNextHead->x = pHead->x + 2;
        pNextHead->y = pHead->y;
        break;
    case Left:
        pNextHead->x = pHead->x - 2;
        pNextHead->y = pHead->y;
        break;
    default:
        break;
    }
    pHead = pNextHead;
    pBody = pHead;
    if (pNextHead->x == X && pNextHead->y == Y)
    {
        while (pBody != NULL)
        {
            setPosition(pBody->x, pBody->y);
            printf("¡ö");
            pBody = pBody->next;
        }
        score += scorePerFood;
        creatFood();
    }
    else
    {
        setPosition(pBody->x, pBody->y);
        printf("¡ö");
        while (pBody->next->next != NULL)
        {
            pBody = pBody->next;
        }
        setPosition(pBody->next->x, pBody->next->y);
        printf("  ");
        free(pBody->next);
        pBody->next = NULL;
    }
    if (biteSelf() == 1)
    {
        gameStatus = 2;
        gameEnd();
    }
}
void pause(void)
{
    while(1)
    {
        Sleep(300);
        if(GetAsyncKeyState(VK_SPACE))
        {
            break;
        }
    }
}
void gameCircle(void)
{
    Direction = Right;
    while (1)
    {
        setPosition(72, 4);
        printf("%d", score);
        setPosition(80, 6);
        printf("%d ", scorePerFood);//Attention space is needed
        if (GetAsyncKeyState(VK_UP) && Direction!=Down)
        {
            Direction = Up;
        }
        else if (GetAsyncKeyState(VK_DOWN) && Direction!=Up)
        {
            Direction = Down;
        }
        else if (GetAsyncKeyState(VK_LEFT) && Direction!=Right)
        {
            Direction = Left;
        }
        else if (GetAsyncKeyState(VK_RIGHT) && Direction!=Left)
        {
            Direction = Right;
        }
        else if (GetAsyncKeyState(VK_SPACE))
        {
            pause();
        }
        else if (GetAsyncKeyState(VK_ESCAPE))
        {
            gameStatus = 3;
            break;
        }
        else if (GetAsyncKeyState(VK_F1))
        {
            if(timeInterval >= 70)
            {
                timeInterval -= 60;
            }
        }
        else if (GetAsyncKeyState(VK_F2))
        {
            if (timeInterval <= 400)
            {
                timeInterval += 60;
            }

        }
        switch (timeInterval)
        {
            case 20: scorePerFood = 20;break;
            case 80: scorePerFood = 15;break;
            case 140: scorePerFood = 12;break;
            case 200: scorePerFood = 10;break;
            case 260: scorePerFood = 8;break;
            case 320: scorePerFood = 6;break;
            case 380: scorePerFood = 4;break;
            case 440: scorePerFood = 2;break;
            default: scorePerFood = 0;break;
        }
        Sleep(timeInterval);
        snakeMove();
    }
}
void welcomePage(void)
{
    setPosition(35,2);
	printf("Welcome to Snake");
	setPosition(15, 5);
	printf("Rules:");
	setPosition(15, 7);
	printf("1.  Use ¡ü.¡ý.¡û.¡ú to control the movement of the Snake.");
	setPosition(15, 9);
	printf("2.  Biting the snake itself is forbidden.");
	setPosition(15, 11);
	printf("3.  Hit the wall is forbidden.");
	setPosition(15, 13);
	printf("Developeder:  zhaoyu.");
    setPosition(15, 15);
    printf("Blog:  http://blog.csdn.net/sinat_30046339");
    setPosition(28, 23);
    printf("Press any key to continue...");
	setPosition(0, 28);
	getchar();
	system("cls");
}
void gameEnd(void)
{
    system("cls");
    setPosition(32, 10);
    switch(gameStatus)
    {
    case 1:
        printf("You hit the wall!");
        break;
    case 2:
        printf("You bit yourself!");
        break;
    case 3:
        printf("You chose to end the game.");
        break;
    default:
        break;
    }
    setPosition(32, 14);
    printf("Your final score is   %d", score);
    getchar();
    setPosition(0, 25);
    exit(0);
}
void gameStart(void)
{
	system("mode con cols=100 lines=30");//注意等号两边不能有空格
	welcomePage();//输出一个简单的欢迎界面
	creatInterface();//打印游戏界面
	initializeSnake();//初始化蛇身
	creatFood();//打印食物位置
}



//Main Function
int main(void)
{
    hideCursor();//隐藏鼠标指针
	gameStart();//开始游戏
	gameCircle();//进行游戏
	gameEnd();//结束游戏
	return 0;
}
