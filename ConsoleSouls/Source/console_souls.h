#pragma once
#pragma comment (lib, "winmm.lib")    //음악

#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "mmsystem.h"                //음악
#include <digitalv.h>;                //음악

void DrawToBackBuffer(int i, int j, char* image); // 더블버퍼링 시도
// 메인 캐릭터
COORD myCharacterPosition;	//처음 포지션
int myCharacterId;
char myCharacter[][3][3]; // 캐릭터 배열
int invincibleState; //무적상태 체크


COORD zeroBossPosition;	//처음 포지션
int zeroBossId;
char zeroBoss[][5][5]; // 캐릭터 배열
void PrintBossHP();

typedef struct Boss_Info {
	COORD pos;
	int cool_time;
	int hp;

}Boss_Info;

extern Boss_Info* bossPTR;


void ShowBoss(COORD* boss);       
void MoveBoss();                      
void DeleteBoss();                    
void InitBoss();
int BossCollisionDetect(int x, int y);


void RemoveCursor(void); // 커서를 안보이게 지운다
void SetCurrentCursorPos(int x, int y);    // 커서의 위치를 설정

void SetTrap();
void DrawMap(); 
void DeleteCharacter(char main_character[][3]); // 기존에 출력 된 캐릭터 삭제
void PrintCharacter(char main_character[][3]);   // 움직이는 곳에 캐릭터를 생성
void HPDecrease(); //캐릭터 HP감소 함수

void MyCharacterAttack(COORD pos);//캐릭터의 공격
void Rolling();//캐릭터 구르기

int MyCharacterCollisionDetect(int position_x, int position_y, char main_character[][3]); //캐릭터의 충돌 감지 콜리젼


#define UP 0x57 //W   GetAsyncKeyState 가상 키코드 값
#define DOWN 0x53 //S
#define LEFT 0x41 //A
#define RIGHT 0x44 //D
#define ATTACT 0x4A	//J 키
#define ROLLING 0x4B //K 키

#define SPACE VK_SPACE
#define ENTER VK_RETURN
#define CHARACTER_POS_X 65
#define CHARACTER_POS_Y 20
#define SWORD_RANGE 5

#define BOARD_GAP_X 10 //공백
#define BOARD_GAP_Y 5//공백

#define GROUND_WIDTH 60    //게임보드
#define GROUND_HEIGHT 40   //게임보드

int gameGroundBack[GROUND_HEIGHT + 1][GROUND_WIDTH + 2]; //충돌 처리    경계선 부분을 세로+1 가로+2를 해줌

void PrintMyCharacterHP();
void PrintBoss(COORD pos);
void BossSkill(Boss_Info* boss);

#define MAP_BORDER 1
#define PLAYER 2
#define SHEILD 3
#define SPACE 4
#define TRAP 6
#define PLAYER_SWORD 7
#define ZERO_BOSS 8
#define PLAYER_ATTACK 9
#define PLAYER_POWER_ATTACK 10
#define HP_X 10
#define HP_Y 47
#define BOSS_HP_X 21
#define BOSS_HP_Y 2
int SwordDirecton;//무기 방향

extern int hp, boss_hp, isBossSkill, is2phase, isPlaying; //캐릭터 HP, 보스HP, 보스 2페이즈

// 시간 이용하여 보스 움직임 제어, 캐릭터 공격 제어
clock_t starting_time;   //시간비교변수 보스의 움직임 제어
clock_t current_time;    //시간비교변수1

clock_t starting_time2;   //캐릭터 공격 딜레이
clock_t current_time2;    
clock_t starting_time3;   //시간비교변수 보스의 움직임 제어
clock_t current_time3;


// 게임 메뉴 함수
void Menu();
//void TitleDraw();
void PlayGame();
void GameClear();
void YouDied();
void ControlExpln();


//사운드
extern int dwID;
void playingSword(); //공격사운드
void playingBgm();	 //BGM
void bossHit();      //보스가 맞았을 때 사운드
void playingRolling();// 구르기
void playing2Phase(); //보스 2페이즈
void playingDamage(); //플레이어가 맞았을 때
void playingMenuSelect(); // 메뉴창 커서 이동
void playingMenuOK();//메뉴창 엔터키
void playYouDied(); //캐릭터가 죽을 때