#ifndef _HEAD_H_
#define _HEAD_H_
#include <stdio.h>
#include <windows.h>//
int Score = -1;
typedef struct node1
{
	int x;
	int y;
	struct node1 *next;
}SnakeBody;
typedef struct node2
{
	int x;
	int y;//location
	char Status;
	char Direction;
	void (*MoveUp)(struct node2 *);
	void (*MoveDown)(struct node2 *);
	void (*MoveRight)(struct node2 *);
	void (*MoveLeft)(struct node2 *);
	void (*GrowUp)(struct node2 *);
	int (*IsDie)(struct node2 *);
	SnakeBody *next;
}SnakeHead;
void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
void SetPosition(int x, int y)
{
    COORD pos;
    HANDLE hOutput;
    pos.X = x;
    pos.Y = y;
    hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOutput, pos);
}
void Print(SnakeHead *H)
{
	SetPosition(H->x, H->y);
	printf("@");//mean snake head
	SnakeBody *temp = H->next;
	while(NULL != temp)
	{
		SetPosition(temp->x, temp->y);
		printf("+");//means snake body
		temp = temp->next;
	}
}
void Move(SnakeHead *H)
{
	SnakeBody *temp = H->next;
	while (NULL != temp->next->next)
	{
		temp = temp->next;
	}
	temp->next->next = H->next;
	H->next = temp->next;
	SetPosition(temp->next->x, temp->next->y);
	printf(" ");
	temp->next = NULL;
	H->next->x = H->x;
	H->next->y = H->y;
}
void _MoveUp(SnakeHead *H)
{
	Move(H);
	H->y--;
}
void _MoveDown(SnakeHead *H)
{
	Move(H);
	H->y++;
}
void _MoveRight(SnakeHead *H)
{
	Move(H);
	H->x++;	
}
void _MoveLeft(SnakeHead *H)
{
	Move(H);
	H->x--;
}
void _GrowUp(SnakeHead *H)
{
	SnakeBody *temp = (SnakeBody *)malloc(sizeof(SnakeBody));
	temp->x = H->x;
	temp->y = H->y;
	temp->next = H->next;
	H->next =  temp;
	switch (H->Direction)
	{
		case 'U':
		{
			H->y--;
			break;
		}
		case 'D':
		{
			H->y++;
			break;
		}
		case 'L':
		{
			H->x--;
			break;
		}
		case 'R':
		{
			H->x++;
			break;
		}
		default:
		{
			break;
		}
	}
	Print(H);
}
const int Left = 3;
const int Right = 34;
const int Top = 3;
const int Bottom = 27;
void BuildWall()
{
	for (int x = Left; x <= Right; x++)
	{
		SetPosition(x, Top);
		printf("-");
		SetPosition(x, Bottom);
		printf("-");
	}
	for (int y = Top; y <= Bottom; y++)
	{
		SetPosition(Left, y);
		printf("|");
		SetPosition(Right, y);
		printf("|");
	}
	//some hint
	SetPosition(15, 1);
	printf("Snake");
	SetPosition(20, 2);
	printf("zhaoyu");
	SetPosition(37, 7);
	printf("F1:Slow Down");
	SetPosition(37, 9);
	printf("F2:Speed Up");
	SetPosition(37, 11);
	printf("F3:Speed Up++");
	SetPosition(37, 15);
	printf("Score:");
}
void CreateFood(int *x, int *y, SnakeHead *H)
{
	int flag = 1;
	int i = 0, j = 0; 
	//make sure in the wall && not on snake
	SnakeBody *temp = H->next;
	while (1 == flag)
	{
		i = rand()%30;
		i += 4;
		j = rand()%24;
		j += 4;
		flag = 0;
		if (i == H->x && j == H->y)
		{
			flag = 1;
			continue;
		}
		temp = H->next;
		while (NULL != temp)
		{
			if (i == temp->x && j == temp->y)
			{
				flag = 1;
				break;
			}
			temp = temp->next;
		}
	}

	*x = i;
	*y = j;
	SetPosition(i, j);
	printf("$");//dollar mead food!
	SetPosition(45, 15);
	//once food created ,score plus 1
	printf("%d", ++Score);
}
int Die(SnakeHead *H)
{
	//out the wall will die
	if (H->x <= Left || H->x >= Right || H->y >= Bottom || H->y <= Top)
	{
		return 1;//die
	}
	// crush into itself will die
	SnakeBody *temp = H->next;
	while (NULL !=temp)
	{
		if (H->x == temp->x && H->y == temp->y)
		{
			return 1;
		}
		temp = temp->next;
	}
	// 0 mean not die
	return 0;
}
#endif