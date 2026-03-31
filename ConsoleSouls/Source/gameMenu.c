#include "console_souls.h"
#include "ImageLayer.h"

// 게임 플레이 시작 지점
isPlaying = 1;
void Menu()
{
	RemoveCursor();
	system("cls");
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer); //초기화

	Image images[1] = {
		{"image\\consolesouls.bmp", 200, 300, 1}
	/*	,{"darksouls.bmp", 0, 0, 2}
		,{"darksouls.bmp", 0, 0, 2}*///{이미지 이름, 시작 x좌표, 시작 y좌표, 크기 배율(쓰지 않으면 기본값인 16이 들어감)} 
	}; //배열의 첫 원소가 가장 아래 그려진다.

	imageLayer.imageCount = 1; //images 배열의 크기보다 작거나 같아야 한다.
	imageLayer.images = images;
	//TitleDraw();
	int menuSelect = 35;

	while (1)
	{
		Sleep(100);
		SetCurrentCursorPos(70, 30);
		printf("게임 시작");

		SetCurrentCursorPos(70, 35);
		printf("조작키 설명");

		SetCurrentCursorPos(65, menuSelect);
		printf(">>>");
		SetCurrentCursorPos(0, 90);

		imageLayer.renderAll(&imageLayer);

		if (GetAsyncKeyState(VK_UP) & 0x0001)
		{
			SetCurrentCursorPos(65, menuSelect);
			printf("    ");
			menuSelect = 30;
			playingMenuSelect();
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			playingMenuSelect();
			SetCurrentCursorPos(65, menuSelect);
			printf("    ");
			menuSelect = 35;
			
		}
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			if (menuSelect == 30)//게임 시작
			{
				playingMenuOK();
				break;
			}
			else if (menuSelect == 35)
			{
				playingMenuOK();
				ControlExpln();
				continue;
			}
		}
	}
	
	system("cls");
	PlayGame();
	/*getchar();*/
}

COORD zeroBossPosition = { CHARACTER_POS_X + 30 ,CHARACTER_POS_Y };
void PlayGame()
{

	playingBgm(); //배경음악
	RemoveCursor();
	InitBoss();// 보스 생성

	DrawMap();                        //맵 그리기
	PrintBossHP();							//보스의 HP
	PrintMyCharacterHP();					//캐릭터의 HP
	starting_time = clock();
	starting_time2 = clock();

	while (isPlaying)
	{
		//게임시작시간기록
		current_time = clock();
		current_time2 = clock();
		SetTrap();
		PrintCharacter(myCharacter[myCharacterId]);
		MoveBoss(&starting_time, &current_time);//보스 움직임 컨트롤
		ProcessKeyInput(100);
		if (boss_hp <= 0)
		{
			isPlaying = 0;//게임 while문 정지
		}
	}
	PlaySound(NULL, 0, 0);    // 노래 종료
	GameClear();
}

void GameClear()
{
	system("cls");
	printf("CLEAR!!! 다시 하려면 엔터키를 입력하세요.");
	
	while (1)
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			break;
		}
	}
	// 클리어 음악
	
	boss_hp = 12;
	is2phase = 0;
	isPlaying = 1;
	hp = 5;
	system("cls");
	Menu();
	//초기화
}

void YouDied()
{
	isPlaying = 0;
	system("cls");
	ImageLayer imageLayer = DEFAULT_IMAGE_LAYER;
	imageLayer.initialize(&imageLayer); //초기화

	Image images[1] = {
		{"image\\youdied.bmp", 200, 400, 1}
		/*	,{"darksouls.bmp", 0, 0, 2}
			,{"darksouls.bmp", 0, 0, 2}*///{이미지 이름, 시작 x좌표, 시작 y좌표, 크기 배율(쓰지 않으면 기본값인 16이 들어감)} 
	}; //배열의 첫 원소가 가장 아래 그려진다.
	imageLayer.imageCount = 1; //images 배열의 크기보다 작거나 같아야 한다.
	imageLayer.images = images;
	SetCurrentCursorPos(60, 30);
	printf("다시 하려면 엔터키를 누르세요.");
	while (1)
	{
		imageLayer.renderAll(&imageLayer);
		
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			break;
		}
		Sleep(100);
	}

	boss_hp = 10;
	is2phase = 0;
	isPlaying = 1;
	hp = 5;
	system("cls");
	Menu();
	//초기화
}

void ControlExpln()
{
	system("cls");
	while (1)
	{
		SetCurrentCursorPos(72, 15);
		printf("- 조작법 -");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		SetCurrentCursorPos(45, 19);
		printf("△(W)");
		SetCurrentCursorPos(40, 21);
		printf("◁(A)");
		SetCurrentCursorPos(50, 21);
		printf("▷(D)");
		SetCurrentCursorPos(45, 21);
		printf("▽(S)");

		SetCurrentCursorPos(88, 20);
		printf("공격: J");
		SetCurrentCursorPos(102, 20);
		printf("회피: K");
		SetCurrentCursorPos(60, 25);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x000e);
		printf("덫을 밟으면 제로의 눈이 떠진다.");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		if (GetAsyncKeyState(VK_RETURN) & 0x0001)
		{
			system("cls");
			playingMenuOK();
			break;
		}
		Sleep(100);
	}
}

