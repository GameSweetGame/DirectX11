#include "console_souls.h"

Boss_Info* bossPTR = NULL;
int isBossSkill = 0;
int is2phase = 0;//보스 페이즈 관리
//보스에 좌표와 hp 값을 할당
void InitBoss()
{

	Boss_Info* boss = (Boss_Info*)malloc(sizeof(Boss_Info));
	boss->pos = zeroBossPosition;
	boss->hp = 20;
	boss->cool_time = 2;
	SetCurrentCursorPos(boss->pos.X, boss->pos.Y);
	PrintBoss(boss->pos);

	bossPTR = boss;
	//free(boss);
}
// 보스를 출력하는 함수
void PrintBoss(COORD pos)
{
	int board_array_x = (pos.X - BOARD_GAP_X) / 2;
	int board_array_y = pos.Y - BOARD_GAP_Y;
	char* bossEye = "◎";
	
	//보스 눈 선택 스킬 사용 시 적용
	if (isBossSkill == 1)
	{
		bossEye = "◎";	
	}
	else 
	{
		bossEye = "―";
	}
	 

	for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			SetCurrentCursorPos(pos.X + (x * 2), pos.Y + y);
			if ((y == 2 && x == 2))
			{	
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000e);
				printf("%s", bossEye);
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
			else if (is2phase==1)
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0004);
				printf("11");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
			else
			{
				
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0004);
				printf("00");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
			gameGroundBack[board_array_y + y][board_array_x + x] = ZERO_BOSS;
			
		}
	}
}
 
//보스가 움직일 때 기존 프린트된 보스를 지우는 함수
 void DeletePrintedBoss(COORD pos)
 {
 	int board_array_x = (pos.X - BOARD_GAP_X) / 2;
 	int board_array_y = pos.Y - BOARD_GAP_Y;
 
 	for (int y = 0; y < 5; y++)
 	{
 		for (int x = 0; x < 5; x++)
 		{
 			SetCurrentCursorPos(pos.X + (x * 2), pos.Y + y);
 			printf("  ");
 			gameGroundBack[board_array_y + y][board_array_x + x] = 0;
 		}
 	}
 	SetCurrentCursorPos(pos.X, pos.Y);
 }

 //두 번째 페이즈
 void Phase2Boss(int* is2phase)
 {
	 *is2phase = 1;
	 playing2Phase();
	 Sleep(2000);
	 
 }
 //보스의 스킬

void BossSkill(Boss_Info* boss)
{
	
	playing2Phase();
	Sleep(2000);
	while (1)
	{
		Sleep(50);
		DeleteBoss(boss);
		
		if (myCharacterPosition.X - boss->pos.X < 0)
		{
			boss->pos.X -= 2;
			if (BossCollisionDetect(boss->pos.X - 2, boss->pos.Y))
			{
				break;
			}
		}
		else if(myCharacterPosition.X - boss->pos.X > 0)
		{
			boss->pos.X += 2;
			if (BossCollisionDetect(boss->pos.X + 2, boss->pos.Y))
			{
				break;
			}	
		}

		if (myCharacterPosition.Y - boss->pos.Y < 0)
		{
			boss->pos.Y--;
			if (BossCollisionDetect(boss->pos.X, boss->pos.Y - 2))
			{
				break;
			}
				
		}
		else
		{
			boss->pos.Y++;
			if (BossCollisionDetect(boss->pos.X, boss->pos.Y + 2))
			{
				break;
			}
		}
		PrintBoss(boss->pos);
		
		 //보스 출력
	}
	isBossSkill = 0;
	
	
	
}

// 보스를 움직이는 함수
void MoveBoss(clock_t* starting_timePTR, clock_t* current_timePTR) //시간 지난만큼 보내줌
{
	Boss_Info* boss = bossPTR;


	double bossSpeedUp;
	//보스의 hp가 10이하면 움직임을 빠르게 함
	if (boss_hp <  10)
	{
		if (is2phase == 0)
		{
			Phase2Boss(&is2phase);
		}
		bossSpeedUp = 100.0;
	}
	else 
	{
		bossSpeedUp = 200.0;
	}
	double t_diff = 0; //시간비교변수 1과 2의 차이를 저장할 변수

	t_diff = difftime(*current_timePTR, *starting_timePTR);
	double bossSpeed = bossSpeedUp;     //보스의 움직임 대기 시간

	int direction_x = 0;
	int direction_y = 0;
	
	if (isBossSkill == 1) 
	{
		BossSkill(boss); //보스 스킬
	}



	DeleteBoss(boss);
	//캐릭터의 좌표 값을 이용하여 보스를 움직임 
 	if (t_diff >= bossSpeed) //
 	{
		
		if (myCharacterPosition.X - boss->pos.X < 0)
		{
			if (!BossCollisionDetect(boss->pos.X - 2, boss->pos.Y))
				boss->pos.X -= 2;
		}
		else
		{
			if (!BossCollisionDetect(boss->pos.X + 2, boss->pos.Y))
				boss->pos.X += 2;
		}

		if (myCharacterPosition.Y - boss->pos.Y < 0)
		{
			if (!BossCollisionDetect(boss->pos.X, boss->pos.Y - 1))
				boss->pos.Y--;
		}
		else
		{
			if (!BossCollisionDetect(boss->pos.X, boss->pos.Y + 1))
				boss->pos.Y++;
		}
		*starting_timePTR = *current_timePTR;
 	}
	
	PrintBoss(boss->pos); //보스 출력
}

//보스 좌표에서 보스를 지운다.
void DeleteBoss(Boss_Info* boss)
{
	SetCurrentCursorPos(boss->pos.X, boss->pos.Y);
	DeletePrintedBoss(boss->pos);
}

//보스의 충돌 처리
int BossCollisionDetect(int x, int y)         
{
	int board_array_x = (x - BOARD_GAP_X) / 2;
	int board_array_y = y - BOARD_GAP_Y;

	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			if (gameGroundBack[board_array_y + y][board_array_x + x] == MAP_BORDER)
			{
				return 1;
			}
			else if (gameGroundBack[board_array_y + y][board_array_x + x] == PLAYER|| gameGroundBack[board_array_y + y][board_array_x + x] == SHEILD|| gameGroundBack[board_array_y + y][board_array_x + x] == PLAYER_SWORD)
			{
				double t_diff = 0;
				t_diff = difftime(current_time2, starting_time2);
				if (t_diff >= 1000)
				{
					
					HPDecrease();
					
					starting_time2 = current_time2; //다시 시간계산
				}
				
				return 1;
			}
		}
	}
	return 0;
}
