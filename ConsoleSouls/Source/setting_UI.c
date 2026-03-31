#include "console_souls.h"

int hp = 5; //캐릭터의 HP
int boss_hp = 12;

//////////////////////커서
void RemoveCursor(void)   // 깜박이는 커서를 제거
{
	CONSOLE_CURSOR_INFO current_info;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &current_info);
	current_info.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &current_info);
}

void SetCurrentCursorPos(int x, int y)    // 커서의 위치를 설정
{
	COORD position = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}
/////////////////////커서

// 맵 그리기
void DrawMap()
{
	int x, y;
	for (y = 0; y <= GROUND_HEIGHT; y++) //세로 줄 왼쪽
	{
		SetCurrentCursorPos(BOARD_GAP_X, BOARD_GAP_Y + y);
		if (y == 0)
			printf("##");
		else if (y == GROUND_HEIGHT)
			printf("##");
		else
			printf("##");
	}

	for (y = 0; y <= GROUND_HEIGHT; y++) //세로 줄 오른쪽
	{
		SetCurrentCursorPos(BOARD_GAP_X + (GROUND_WIDTH + 1) * 2, BOARD_GAP_Y + y);
		if (y == 0)
			printf("##");
		else if (y == GROUND_HEIGHT)
			printf("##");
		else
			printf("##");
	}

	for (x = 1; x <= (GROUND_WIDTH + 1) * 2; x++) // 가로 줄, 위 아래
	{
		SetCurrentCursorPos(BOARD_GAP_X + x, BOARD_GAP_Y);
		printf("#");
		SetCurrentCursorPos(BOARD_GAP_X + x, BOARD_GAP_Y + GROUND_HEIGHT+1);
		printf("#");
	}

	for (int y = 0; y < GROUND_HEIGHT + 1; y++) // 충돌 체크를 위한
	{
		for (int x = 0; x < GROUND_WIDTH + 2; x++)
		{
			if (x == 0 || y == 0 || x == GROUND_WIDTH || y == GROUND_HEIGHT)
			{
				gameGroundBack[y][x] = 1;
			}
			else
			{
				gameGroundBack[y][x] = 0;
			}
		}
	}
}


// 트랩 설치
void SetTrap()
{
	int trap[6][2] = { {30,15},{50,20},{90,10},{120,40},{40,35},{90,30} };
	int board_array_x;
	int board_array_y;
	for (int i=0; i<6;i++)
	{

		board_array_x = (trap[i][0] - BOARD_GAP_X) / 2;
		board_array_y = trap[i][1] - BOARD_GAP_Y;
		SetCurrentCursorPos(trap[i][0], trap[i][1]);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
		printf("◎");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		gameGroundBack[board_array_y][board_array_x] = TRAP;
		
	}
}


//캐릭터 HP 표시
void PrintMyCharacterHP()
{

	SetCurrentCursorPos(HP_X, HP_Y);
	printf("HP : ");
	for (int i = 0; i < 3; i++) //지우기 작업
	{
		printf("             ");
	}

	SetCurrentCursorPos(HP_X, HP_Y);

	printf("HP : ");
	for (int i = 0; i < hp; i++) //바뀐 HP 다시 출력 작업
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		printf("♥ ");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000c);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
void PrintBossHP()
{

	SetCurrentCursorPos(BOSS_HP_X + 1, BOSS_HP_Y + 1);
	if (boss_hp<=0)
	{
		printf("          ");
	}
	SetCurrentCursorPos(BOSS_HP_X + 50, BOSS_HP_Y);
	printf("ZERO");
	SetCurrentCursorPos(BOSS_HP_X + 1, BOSS_HP_Y + 1);
	for (int i = 0; i < boss_hp; i++) //지우기 작업
	{
		printf("          ");
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);

	SetCurrentCursorPos(BOSS_HP_X + 1, BOSS_HP_Y + 1);
	for (int i = 0; i < boss_hp; i++)
	{
		printf("▒▒▒▒▒");
	}
	

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	
}