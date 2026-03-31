#include "console_souls.h"
//bgm, 효과음 개수 만큼 구조체를 생성

MCI_OPEN_PARMS openBgm;
MCI_PLAY_PARMS playBgm;
MCI_OPEN_PARMS openRolling;
MCI_PLAY_PARMS playRolling;
MCI_OPEN_PARMS openPlayerDamage;
MCI_PLAY_PARMS playPlayerDamage;

int dwID;

//openBgm.lpstrDeviceType = WaveAudio를 사용하면 wav파일
void playingBgm() 
{
	openBgm.lpstrElementName = TEXT("sound\\background.mp3"); //파일 오픈
	openBgm.lpstrDeviceType = TEXT("mpegvideo"); //mp3 형식
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&openBgm);
	dwID = openBgm.wDeviceID;
	mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&playBgm); //음악 반복 재생
}

void playingAttack() 
{
	PlaySound(TEXT("sound\\attack2.wav"), NULL, SND_FILENAME | SND_ASYNC );
}
//
void bossHit()
{
	PlaySound(TEXT("sound\\blood2.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void playingRolling()
{
	dwID = openRolling.wDeviceID;
	mciSendCommand(dwID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL); //음원 재생 위치를 처음으로 초기화
	
	openRolling.lpstrElementName = TEXT("sound\\movement.wav"); //파일 오픈
	openRolling.lpstrDeviceType = TEXT("mpegvideo"); //wav
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&openRolling);
	
	mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&playRolling); //
}

void playing2Phase()
{
	PlaySound(TEXT("sound\\dragon.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void playingDamage()
{
	dwID = openPlayerDamage.wDeviceID;
	mciSendCommand(dwID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL); //음원 재생 위치를 처음으로 초기화
	
	openPlayerDamage.lpstrElementName = TEXT("sound\\voicedamage.wav"); //파일 오픈
	openPlayerDamage.lpstrDeviceType = TEXT("mpegvideo"); //wav
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&openPlayerDamage);
	
	mciSendCommand(dwID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&playPlayerDamage); //
}

void playingMenuSelect()
{
	PlaySound(TEXT("sound\\CURSOL_SELECT.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void playingMenuOK()
{
	PlaySound(TEXT("sound\\CURSOL_OK.wav"), NULL, SND_FILENAME | SND_ASYNC);
}

void playYouDied()
{
	PlaySound(TEXT("sound\\youdied.wav"), NULL, SND_FILENAME | SND_ASYNC);
}
