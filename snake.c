#include "head.h"


void StartGame()
{	
	//Create a snake
	SnakeBody *temp = (SnakeBody *)malloc(sizeof(SnakeBody));
	SnakeHead *Head = (SnakeHead *)malloc(sizeof(SnakeHead));
	Head->Direction = 'L';
	Head->Status = 1;
	Head->x = 10;
	Head->y = 10;
	//assign some functions to snake
	Head->MoveUp = _MoveUp;
	Head->MoveDown = _MoveDown;
	Head->MoveRight = _MoveRight;
	Head->MoveLeft = _MoveLeft;
	Head->GrowUp = _GrowUp;
	Head->IsDie = Die;
	temp->x = 11;
	temp->y = 10;
	temp->next = NULL;
	Head->next = temp;
	// create snake body
	for (int i = 0; i < 5; i++)
	{
		SnakeBody *move = (SnakeBody *)malloc(sizeof(SnakeBody));
		move->x = temp->x + 1;
		move->y = temp->y;
		temp->next = move;
		temp = move;
		move->next = NULL;
	}
	//Init food
	int Foodx = 0, Foody = 0;
	CreateFood(&Foodx, &Foody, Head);
	int Speed = 10;
	while (1 == Head->Status)
	{
		//print the snake
		Print(Head);
		//judge is or not die
		if((*Head->IsDie)(Head))
		{
			break;
		}
		//get direction
		if(GetAsyncKeyState(VK_UP) && 'D' != Head->Direction)
		{
			Head->Direction = 'U';
		}
		else if (GetAsyncKeyState(VK_DOWN) && 'U' != Head->Direction)
		{
			Head->Direction = 'D';
		}
		else if (GetAsyncKeyState(VK_RIGHT) && 'L' != Head->Direction)
		{
			Head->Direction = 'R';
		}
		else if (GetAsyncKeyState(VK_LEFT) && 'R' != Head->Direction)
		{
			Head->Direction = 'L';
		}
		//move
		switch (Head->Direction)
		{
			case 'U':
			{
				//judge is there a food
				if (Head->x == Foodx && Head->y - 1 == Foody)
				{
					(*Head->GrowUp)(Head);
					CreateFood(&Foodx, &Foody, Head);
				}
				else//Just move
				{
					(*Head->MoveUp)(Head);
				}
				break;
			}
			case 'D':
			{
				if (Head->x == Foodx && Head->y + 1 == Foody)
				{
					(*Head->GrowUp)(Head);
					CreateFood(&Foodx, &Foody, Head);
				}
				else
				{
					(*Head->MoveDown)(Head);
				}
				break;
			}
			case 'R':
			{
				if (Head->x + 1 == Foodx && Head->y == Foody)
				{
					(*Head->GrowUp)(Head);
					CreateFood(&Foodx, &Foody, Head);
				}
				else
				{
					(*Head->MoveRight)(Head);	
				}
				
				break;
			}
			case 'L':
			{
				if (Head->x - 1 == Foodx && Head->y == Foody)
				{
					(*Head->GrowUp)(Head);
					CreateFood(&Foodx, &Foody, Head);
				}
				else
				{
					(*Head->MoveLeft)(Head);
				}
				break;
			}
			default:
			{
				break;
			}
		}
		//control 
		if (GetAsyncKeyState(VK_F1) && Speed > 6)
		{
			Speed -= 5;
		}
		else if (GetAsyncKeyState(VK_F2) && Speed < 30)
		{
			Speed += 5;
		}
		else if (GetAsyncKeyState(VK_F3))
		{
			Speed += 10;
		}
		if ('L' == Head->Direction ||'R' == Head->Direction)
		{
			Sleep(1000/Speed);
		}
		else
		{
			Sleep(2000/Speed);
		}
	}
}
int main(int argc, char const *argv[])
{
	//init
	system("cls");
	system("mode con cols=50 lines=30");//no space around equal sign	
	BuildWall();
	HideCursor();
	StartGame();
	SetPosition(36, 20);
	printf("Enter to exit!");
	getchar();
	return 0;
}