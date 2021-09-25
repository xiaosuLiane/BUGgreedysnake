#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <vector>
#include <thread>
using namespace std;
enum Direction { up, down, left, right, Unkown };
struct block
{
	COORD pos;
	Direction direction;
	unsigned long currentime{ 0 };
	int color;
};
struct vector2d
{
	short x;
	short y;
};
#define width 60
#define height 20
vector<block> player;
void toDirection();
void DirectionToPos(int index);
short toRandom(int min, int max);
block Item{ COORD{1,10},Unkown,GetTickCount(),toRandom(0x11,0xEE) };//240-256为白色
block b{ 20,10,right,0L,toRandom(0x11,0xEE) };
vector<vector2d> retAction();
bool gameover = false;//游戏是否结束

int main()
{
	srand((unsigned int)NULL);
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CURSOR_INFO = { 1,0 };
	SetConsoleCursorInfo(hOut, &CURSOR_INFO);
	player.push_back(b);
	thread t1(toDirection);

	while (1)
	{
		for (short hi = 0; hi < width; hi++)
		{
			for (short n = 0; n < height; n++)
			{
				SetConsoleTextAttribute(hOut, 0xFF);
				SetConsoleCursorPosition(hOut, COORD{ hi,n });
				printf(" ");
			}
			SetConsoleTextAttribute(hOut, Item.color);
			SetConsoleCursorPosition(hOut, COORD{ Item.pos.X,Item.pos.Y });
			printf(" ");
			vector<vector2d> vec = retAction();
			for (int i = 0; i < player.size(); i++)
			{
				if (gameover)
					break; 
				if (player[0].pos.X <= 0 || player[0].pos.X >= width || player[0].pos.Y <= 0 || player[0].pos.Y >= height)
				{
					Item = { toRandom(1,width - 1),toRandom(1,height - 1),Unkown,GetTickCount(),toRandom(0x11,0xEE) };//让吃的东西重置一下
					gameover = true; MessageBoxA(0, "请按下空格继续", "gameover", 0);//让消息框堵塞线程，直到消息框消失
					player.clear(); b = { 20,10,right,0L,toRandom(0x11,0xEE) }; player.push_back(b); gameover = false; break;//重置游戏与玩家操作
				}
				if (player[i].pos.X == Item.pos.X && player[i].pos.Y == Item.pos.Y)
				{
					player.push_back(Item);
					Item = { toRandom(1,width-1),toRandom(1,height-1),Unkown,GetTickCount(),toRandom(0x11,0xEE) };
				}
				if ((GetTickCount() - player[i].currentime) > 50)
				{
					if (i > 0)
					{
						player[i].pos.X = vec[i - 1].x;
						player[i].pos.Y = vec[i - 1].y;
					}
					else {
						DirectionToPos(i);
					}
					player[i].currentime = GetTickCount();
				}
				SetConsoleTextAttribute(hOut, player[i].color);
				SetConsoleCursorPosition(hOut, player[i].pos);
				printf(" ");
			}
		}
	}
	return 0;
}
void DirectionToPos(int index)
{
	switch (player[index].direction)
	{
	case up:
		player[index].pos.Y--; break;
	case down:
		player[index].pos.Y++; break;
	case left:
		player[index].pos.X--; break;
	case right:
		player[index].pos.X++; break;
	}
}
void toDirection()
{
	while (1)
	{
		switch (getch())
		{
		case 119:
			player[0].direction = up; break;
		case 115:
			player[0].direction = down; break;
		case 97:
			player[0].direction = left; break;
		case 100:
			player[0].direction = right; break;;
		}
	}
}
vector<vector2d> retAction()
{
	vector<vector2d> vec2d;
	for (int i = 1; i <= player.size(); i++)
	{
		vector2d vec = { player[i - 1].pos.X,player[i - 1].pos.Y };
		vec2d.push_back(vec);
	}
	return vec2d;
}
short toRandom(int min, int max)
{
	short randomShort = (rand() % max) + min;
	while (randomShort >= 240 && 256 >= randomShort)//过滤白色
	{
		randomShort = (rand() % max) + min;
	}
	return randomShort;
}