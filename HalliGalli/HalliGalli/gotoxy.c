#include <stdio.h>
#include <stdlib.h>
//#include <curses.h>
#include <Windows.h>//������������ �� ����

//#define gotoxy(x,y) wmove(stdscr,y-1,x-1)
void gotoxy(int x, int y) {
	COORD Pos;
	Pos.X = x;
	Pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
void mPicture() {//����ȭ��
	int x = 1, y = 1;
	gotoxy(x, y);
	printf(" ");
	for (int i = 0; i < 75; i++) {
		printf("-");
	}
	printf(" ");

	for (int i = 1; i < 23; i++) {
		y++;
		gotoxy(x, y);
		printf("|");
	}
	//gotoxy(x, y);
	printf(" ");
	for (int i = 0; i < 75; i++)
		printf("-");
	printf(" ");//������� ������ �׸�
	for (int i = 1; i < 23; i++) {
		y--;
		gotoxy(x + 76, y);
		printf("|");
	}
	printf(" ");
}
void Picture1() {//���� ����ڵ��� �� �� ��
	int x = 8, y = 4;
	int m = 12, n = 14;
	gotoxy(x, y);
	printf("-");
	for (int i = 0; i < 13; i++) {
		printf("-");
	}
	for (int i = 1; i < 10; i++) {
		y++;
		gotoxy(x, y - 1);
		printf("|");
	}
	//gotoxy(x, y);

	for (int i = 0; i < 13; i++)
		printf("-");
	printf("-");
	for (int i = 1; i < 10; i++) {
		y--;
		gotoxy(x + 14, y + 0.5);
		printf("|");
	}
	gotoxy(m, n);
	for (int i = 0; i < 6; i++) {
		printf("-");
	}
	for (int i = 0; i < 3; i++) {
		n++;
		gotoxy(m, n - 1);
		printf("|");
	}
	//gotoxy(x, y);

	for (int i = 0; i < 5; i++)
		printf("-");
	printf("-");
	for (int i = 0; i < 3; i++) {
		n--;
		gotoxy(m + 6, n);
		printf("|");
	}
}
void Picture2() {
	int x = 23, y = 4;
	int m = 27, n = 14;
	gotoxy(x, y);
	printf("-");
	for (int i = 0; i < 13; i++) {
		printf("-");
	}
	for (int i = 1; i < 10; i++) {
		y++;
		gotoxy(x, y - 1);
		printf("|");
	}
	//gotoxy(x, y);

	for (int i = 0; i < 13; i++)
		printf("-");
	printf("-");
	for (int i = 1; i < 10; i++) {
		y--;
		gotoxy(x + 14, y + 0.5);
		printf("|");
	}
	gotoxy(m, n);
	for (int i = 0; i < 6; i++) {
		printf("-");
	}
	for (int i = 0; i < 3; i++) {
		n++;
		gotoxy(m, n - 1);
		printf("|");
	}
	//gotoxy(x, y);

	for (int i = 0; i < 5; i++)
		printf("-");
	printf("-");
	for (int i = 0; i < 3; i++) {
		n--;
		gotoxy(m + 6, n);
		printf("|");
	}
}
void Picture3() {
	int x = 38, y = 4;
	int m = 42, n = 14;
	gotoxy(x, y);
	printf("-");
	for (int i = 0; i < 13; i++) {
		printf("-");
	}
	for (int i = 1; i < 10; i++) {
		y++;
		gotoxy(x, y - 1);
		printf("|");
	}
	//gotoxy(x, y);

	for (int i = 0; i < 13; i++)
		printf("-");
	printf("-");
	for (int i = 1; i < 10; i++) {
		y--;
		gotoxy(x + 14, y + 0.5);
		printf("|");
	}
	gotoxy(m, n);
	for (int i = 0; i < 6; i++) {
		printf("-");
	}
	for (int i = 0; i < 3; i++) {
		n++;
		gotoxy(m, n - 1);
		printf("|");
	}
	//gotoxy(x, y);

	for (int i = 0; i < 5; i++)
		printf("-");
	printf("-");
	for (int i = 0; i < 3; i++) {
		n--;
		gotoxy(m + 6, n);
		printf("|");
	}
}
void Picture4() {
	int x = 53, y = 4;
	int m = 57, n = 14;
	gotoxy(x, y);
	printf("-");
	for (int i = 0; i < 13; i++) {
		printf("-");
	}
	for (int i = 1; i < 10; i++) {
		y++;
		gotoxy(x, y - 1);
		printf("|");
	}
	//gotoxy(x, y);

	for (int i = 0; i < 13; i++)
		printf("-");
	printf("-");
	for (int i = 1; i < 10; i++) {
		y--;
		gotoxy(x + 14, y + 0.5);
		printf("|");
	}
	gotoxy(m, n);
	for (int i = 0; i < 6; i++) {
		printf("-");
	}
	for (int i = 0; i < 3; i++) {
		n++;
		gotoxy(m, n - 1);
		printf("|");
	}
	//gotoxy(x, y);

	for (int i = 0; i < 5; i++)
		printf("-");
	printf("-");
	for (int i = 0; i < 3; i++) {
		n--;
		gotoxy(m + 6, n);
		printf("|");
	}
}
int main() {
	system("mode con cols=80 lines=25");//ȭ�� ���� ,������������ stdlib.h�� ����� ��
	mPicture();
	Picture1();
	Picture2();
	Picture3();
	Picture4();
	_getch();
}