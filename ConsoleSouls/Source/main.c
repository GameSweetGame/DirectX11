#include "console_souls.h"

int main()
{
    system("mode con cols=150 lines=50");//콘솔창 크기 설정
	system("color 0");// 콘솔창 검정색
	
	Menu();
}