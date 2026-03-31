#include "console_souls.h"

COORD myCharacterPosition = { CHARACTER_POS_X ,CHARACTER_POS_Y };	//처음 포지션
char myCharacter[][3][3] =                     // 메인 캐릭터 배열
{
	{
		//main_character_id=0 일 때 위를 바라봄
		{3, 4, 7},
		{0, 2, 0},
		{0, 0, 0}
	},
	{ 
		//main_character_id=1 일 때 왼쪽을 바라봄
		{7, 0, 0},
		{4, 2, 0},
		{3, 0, 0}
	},
	{
		//main_character_id=2 일 때 아래쪽을 바라봄
		{0, 0, 0},
		{0, 2, 0},
		{7, 4, 3}
	},
	{
		//main_character_id=3 일 때 오른쪽을 바라봄
		{0, 0, 3},
		{0, 2, 4},
		{0, 0, 7}
	}
};
int SwordDirecton = UP;    //무기 방향
int myCharacterId = 0; //main_character_id로 캐릭터 방향
int isInvincibleState = 0; // 캐릭터 무적상태

void DeleteCharacter(char main_character[][3])       // 출력된 블록을 삭제하는 함수
{
	int y, x;
	COORD current_position = myCharacterPosition;				//게임보드가 (10,5)부터 그려지는데 현재 캐릭터 포지션은 떨어져있는 부분이 적용 안되어 있음
	int board_array_x = (current_position.X - BOARD_GAP_X) / 2; //때문에 현재 포지션 X에서 GBOARD_ORIGIN_X를 빼주고 x는 2칸씩 차지하기 때문에 2를 나눔
	int board_array_y = current_position.Y - BOARD_GAP_Y;		//Y도 GBOARD_ORIGIN_Y를 빼주고 

	for (y = 0; y < 3; y++)
	{
		for (x = 0; x < 3; x++)
		{
			SetCurrentCursorPos(current_position.X + (x * 2), current_position.Y + y);
			if (main_character[y][x] == 2 || main_character[y][x] == 7 || main_character[y][x] == 3|| main_character[y][x] == 4)              // 메인 캐릭터에 적용
			{
				printf("  ");
				gameGroundBack[board_array_y + y][board_array_x + x] = 0;
			}
		}
	}
	SetCurrentCursorPos(current_position.X, current_position.Y);
}

void PrintCharacter(char main_character[][3])                // 블록을 출력하는 함수
{
	int y, x;
	COORD current_position = myCharacterPosition;
	int board_array_x = (current_position.X - BOARD_GAP_X) / 2;
	int board_array_y = current_position.Y - BOARD_GAP_Y;

	for (y = 0; y < 3; y++)
	{
		for (x = 0; x < 3; x++)
		{
			SetCurrentCursorPos(current_position.X + (x * 2), current_position.Y + y);
			if (main_character[y][x] == 2)                          // 메인 캐릭터에 적용    
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);//텍스트 캐릭터 색
				printf("◆");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				gameGroundBack[board_array_y + y][board_array_x + x] = PLAYER;
			}
			if (main_character[y][x] == 3)                          // SHILED
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);// 방패 색

				printf("#");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				gameGroundBack[board_array_y + y][board_array_x + x] = SHEILD;
			}
			if (main_character[y][x] == 3)                          // fire
			{

				gameGroundBack[board_array_y + y][board_array_x + x] = SPACE;
			}
			if (main_character[y][x] == 7)                          // SWORD   
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000c); //텍스트 무기 색
				if (myCharacterId == 0)
					printf("/ ");
				else if (myCharacterId == 1)
					printf(" \\");
				else if (myCharacterId == 2)
					printf(" /");
				else
					printf("\\");

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				gameGroundBack[board_array_y + y][board_array_x + x] = PLAYER_SWORD;
			}
		}
	}
	SetCurrentCursorPos(current_position.X, current_position.Y);
}


void ShiftUp()                                  // 상하좌우 움직임
{
	if (!MyCharacterCollisionDetect(myCharacterPosition.X, myCharacterPosition.Y - 1, myCharacter[myCharacterId]))
	{
		return;
	}
	SwordDirecton = UP;
	DeleteCharacter(myCharacter[myCharacterId]);
	SetCurrentCursorPos(myCharacterPosition.X, myCharacterPosition.Y -= 1);
	myCharacterId = 0;
	PrintCharacter(myCharacter[myCharacterId]);
}

void ShiftDown()
{
	if (!MyCharacterCollisionDetect(myCharacterPosition.X, myCharacterPosition.Y + 1, myCharacter[myCharacterId]))
	{
		return;
	}
	SwordDirecton = DOWN;
	DeleteCharacter(myCharacter[myCharacterId]);
	SetCurrentCursorPos(myCharacterPosition.X, myCharacterPosition.Y += 1);
	myCharacterId = 2;
	PrintCharacter(myCharacter[myCharacterId]);
}

void ShiftRight()
{
	if (!MyCharacterCollisionDetect(myCharacterPosition.X + 2, myCharacterPosition.Y, myCharacter[myCharacterId]))
	{
		return;
	}
	SwordDirecton = RIGHT;
	DeleteCharacter(myCharacter[myCharacterId]);
	SetCurrentCursorPos(myCharacterPosition.X += 2, myCharacterPosition.Y);
	myCharacterId = 3;
	PrintCharacter(myCharacter[myCharacterId]);
}

void ShiftLeft()
{
	if (!MyCharacterCollisionDetect(myCharacterPosition.X - 2, myCharacterPosition.Y, myCharacter[myCharacterId]))
	{
		return;
	}
	SwordDirecton = LEFT;
	DeleteCharacter(myCharacter[myCharacterId]);
	SetCurrentCursorPos(myCharacterPosition.X -= 2, myCharacterPosition.Y);
	myCharacterId = 1;
	PrintCharacter(myCharacter[myCharacterId]);

}


void ProcessKeyInput(int time)              // 방향키를 입력받아 움직이는 함수 적용 (time 기본값 30)
{
	if (GetAsyncKeyState(LEFT) & 0x8000)
	{ 
		//왼쪽 방향
		ShiftLeft();
		//SwordDirecton = LEFT;
	}

	if (GetAsyncKeyState(RIGHT) & 0x8000)
	{ 
		//오른쪽 방향
		ShiftRight();
		//SwordDirecton = RIGHT;
	}

	if (GetAsyncKeyState(UP) & 0x8000)
	{ 
		//위 방향
		ShiftUp();
		//SwordDirecton = UP;
	}

	if (GetAsyncKeyState(DOWN) & 0x8000)
	{ 
		//아래 방향
		ShiftDown();
		//SwordDirecton = DOWN;
	}

	if (GetAsyncKeyState(ATTACT) & 0x8000)
	{
		double t_diff = 0; //시간비교변수 1과 2의 차이를 저장할 변수

		t_diff = difftime(current_time2, starting_time2);
		if (t_diff >=500)
		{
			playingAttack();
			MyCharacterAttack(myCharacterPosition);
			starting_time2 = current_time2; //다시 시간계산
		}
	}

	if (GetAsyncKeyState(ROLLING) & 0x0001)
	{
		
		Rolling(); //구르기
	}

	Sleep(time);
}
void HPDecrease() // 캐릭터 HP감소 함수
{
	isInvincibleState = 1;
	playingDamage();
	hp--;
	PrintMyCharacterHP();
	//for (int i = 0; i < 2; i++)
	//{
	//	DeleteCharacter(main_character[main_character_id]);
	//	ProcessKeyInput(1);
	//	ShowCharacter(main_character[main_character_id]);
	//	ProcessKeyInput(1);
	//}
	if (hp <= 0)
	{
		playYouDied();
		YouDied();
		
	}

	isInvincibleState = 0;
}

void Rolling() //구르기 함수
{
	playingRolling(); //구르기 사운드
	isInvincibleState = 1;
	for (int i = 0; i < 2; i++)
	{
		DeleteCharacter(myCharacter[myCharacterId]);
		ProcessKeyInput(1);
		PrintCharacter(myCharacter[myCharacterId]);
		ProcessKeyInput(1);
	}
	isInvincibleState = 0;
}

int MyCharacterCollisionDetect(int position_x, int position_y, char main_character[3][3]) //캐릭터 충돌 감지
{
	int board_array_x = (position_x - BOARD_GAP_X) / 2;
	int board_array_y = position_y - BOARD_GAP_Y;
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			if ((main_character[y][x] == PLAYER && gameGroundBack[board_array_y + y][board_array_x + x] == MAP_BORDER) || (main_character[y][x] == PLAYER_SWORD && gameGroundBack[board_array_y + y][board_array_x + x] == MAP_BORDER)|| (main_character[y][x] == SPACE && gameGroundBack[board_array_y + y][board_array_x + x] == MAP_BORDER))     // 메인 캐릭터와 맵이 충돌할 때
			{
				return 0;
			}
			if ((main_character[y][x] == PLAYER && gameGroundBack[board_array_y + y][board_array_x + x] == ZERO_BOSS)|| (main_character[y][x] == PLAYER_SWORD && gameGroundBack[board_array_y + y][board_array_x + x] == ZERO_BOSS)|| (main_character[y][x] == SPACE && gameGroundBack[board_array_y + y][board_array_x + x] == ZERO_BOSS))     // 메인 캐릭터와 보스가 충돌할 때
			{
				
// 				if (isInvincibleState == 0)
// 				{
// 					HPDecrease();
// 				}
				return 0;
			}
			if ((main_character[y][x] == PLAYER && gameGroundBack[board_array_y + y][board_array_x + x] == TRAP) || (main_character[y][x] == PLAYER_SWORD && gameGroundBack[board_array_y + y][board_array_x + x] == TRAP)|| (main_character[y][x] == SHEILD && gameGroundBack[board_array_y + y][board_array_x + x] == TRAP)|| (main_character[y][x] == SPACE && gameGroundBack[board_array_y + y][board_array_x + x] == TRAP))   // 메인 캐릭터와 트랩이 충돌할 때
			{
				isBossSkill = 1;
				return 0;
			}
		}
	}
	return 1;
}


void MyCharacterAttack(COORD pos) {


	int x = pos.X, y = pos.Y;
	int board_array_x = (pos.X - BOARD_GAP_X) / 2;
	int board_array_y = pos.Y - BOARD_GAP_Y;
	const char* swordUpArr[4] = { "┳" ,"■","■","▲" };
	const char* swordDownArr[4] = { "┻" ,"■","■","▼" };
	
	switch (SwordDirecton) {
	case LEFT:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000c);

		SetCurrentCursorPos(x - 6, y);
		printf("◀■■┣ ");

		

		if (gameGroundBack[board_array_y][board_array_x - 3] == ZERO_BOSS) // 칼 끝에 보스가 닿았을 때
		{
			boss_hp = boss_hp - 1;
			bossHit();
			PrintBossHP(); //HP 깎고 HP바 다시 출력
		}
		Sleep(150);
		SetCurrentCursorPos(x - 6, y);
		printf("          ");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		break;

	case RIGHT:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000c);

		SetCurrentCursorPos(x + 4, y + 2);
		printf(" ┫■■▶");

		

		if (gameGroundBack[board_array_y + 2][board_array_x + 6] == ZERO_BOSS) // 칼 끝에 보스가 닿았을 때
		{
			boss_hp = boss_hp - 1;
			bossHit();
			PrintBossHP(); //HP 깎고 HP바 다시 출력
		}
		Sleep(150);
		SetCurrentCursorPos(x + 4, y + 2); //칼 삭제
		printf("          ");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		break;

	case UP:

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000c);
		for (int i = 0; i < 4; i++) //칼 출력
		{
			SetCurrentCursorPos(x + 4, y - i);
			printf("%s", swordUpArr[i]);
		}

		

		if (gameGroundBack[board_array_y - 3][board_array_x + 3] == ZERO_BOSS || gameGroundBack[board_array_y - 3][board_array_x + 2] == ZERO_BOSS) // 칼 끝에 보스가 닿았을 때
		{
			boss_hp = boss_hp - 1;
			bossHit();
			PrintBossHP(); //HP 깎고 HP바 다시 출력
		}
		Sleep(150);
		for (int i = 0; i < 4; i++) //칼 삭제
		{
			SetCurrentCursorPos(x + 4, y - i);
			printf("  ");

		}

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		break;
	case DOWN:

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000c);
		for (int i = 0; i < 4; i++) //칼 출력
		{
			SetCurrentCursorPos(x + 1, (y + 2));
			printf(" ");

			SetCurrentCursorPos(x, (y + 2) + i);
			printf("%s", swordDownArr[i]);
		}

		

		if (gameGroundBack[board_array_y + 5][board_array_x] == ZERO_BOSS || gameGroundBack[board_array_y + 5][board_array_x + 1] == ZERO_BOSS) // 칼 끝에 보스가 닿았을 때
		{
			boss_hp = boss_hp - 1;
			bossHit();
			PrintBossHP(); //HP 깎고 HP바 다시 출력
		}
		Sleep(150);
		for (int i = 0; i < 4; i++) //칼 삭제
		{
			SetCurrentCursorPos(x, (y + 2) + i);
			printf("  ");
		}

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		break;

	}
	
}
