#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <windows.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

#define DECK_MAX_CNT 56
#define PLAYER_MAX_CNT 4
#define MAX_NAME_LENGTH 10   // 10���� �������� �������, �ѱ��� �����Ͽ� 20�� ����
#define BUFSIZE 128

char *user[4]; // �̸��� ��� �Լ����� �����ϱ� ������ �������� �����ϴ�.
/* �ʵ忡 �����ִ� ī��迭���� field ���� ���� ������� ���� deck�� �߰���  */

//일단 걍 전역으로 두겠음 
int playerDeck[PLAYER_MAX_CNT][DECK_MAX_CNT]; //플레이어 4명의 덱


/* 인자로 들어온 deck의 카드의 개수 반환 */
int DeckCount(int *deck) {
	int deckCount = 0;
	int decksize = sizeof(deck) / sizeof(deck[0]); // deck 배열의 크기
	int i;

	for (i = 0; i < decksize; i++) {
		if (deck[i] == 0) {
			deckCount = i;
			break;
		}
	}
	return deckCount;
}

/* index에 해당한 카드의 과일개수 리턴 */
int GetFruitCnt(int index) {

	//1개짜리 5개, 2개짜리 3개, 3개짜리 3개, 4개짜리 2개, 5개짜리 1개 존재
	//0 ~ 55 의 index 값을 인자로 받음
	// %14 해서..
	// 0,1,2,3,4 = 1개
	// 5,6,7	 = 2개
	// 8,9,10	 = 3개
	// 11, 12	 = 4개
	// 13		 = 5개

	switch (index % 14) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		return 1;
	case 5:
	case 6:
	case 7:
		return 2;
	case 8:
	case 9:
	case 10:
		return 3;
	case 11:
	case 12:
		return 4;
	case 13:
		return 5;
	default:
		printf("GetFruitCnt error : (index mod 14) value is upper than 13");
		return -1;
	}
}

/* index에 해당한 카드의 과일종류 리턴 */
int GetFruitType(int index) {

	// 14개의 카드가 4개의 종류로 구분됨
	// 0 ~ 55 의 index 값을 인자로 받음
	// 0 ~ 13 : 과일1
	// 14 ~ 27 : 과일2
	// 28 ~ 41 : 과일3
	// 42 ~ 55 : 과일4
	// 반환값 : 0, 1, 2, 3

	return (int)(index / 14);
}
/* 필드에 놓여있는 카드배열들인 field 들을 얻은 사용자의 덱인 deck에 추가함  */

void TakeCardsInField(int *deck, int *field) {
	int i;
	int deckcount = 0, fieldcount = 0; // ��� 0�� üũ�� �Ǵ°�
	int decksize = sizeof(deck) / sizeof(deck[0]); // deck �迭�� ũ��
	int fieldsize = sizeof(field) / sizeof(field[0]); // field �迭�� ũ��

	// deck�迭 ���� ���°���� ī�尡 ������ 
	/*for (i = 0; i < decksize; i++) {
		if (deck[i] == 0) {
			deckcount = i;
			break;
		}
	}*/
	deckcount = DeckCount(deck); // �� ������ �Լ��� ��ü����


	// field�迭 ���� ���°���� ī�尡 ������ 
	for (i = 0; i < fieldsize; i++) { //���⵵ deckCountó�� �Լ��� ��ü ������
		if (field[i] == 0) {
			fieldcount = i;
			break;
		}
	}

	// field�迭�� ������ deck�迭�� �ֱ�
	for (i = 0; i < fieldsize; i++) {
		deck[deckcount + i] = field[i];
	}
}

/* int[4] ������ field�� ���� ���������� 5���� �ִ��� Ȯ�� */
bool IsFiveFruits(int *field) {
	int fieldCpy[4] = { 0 };
	int i;
	for (i = 0; i < 4; i++) {
		fieldCpy[GetFruitType(field[i])] += GetFruitCnt(field[i]);
	}
	//���� 5���� ���� �ִ��� Ȯ��
	for (i = 0; i < 4; i++) {
		if (fieldCpy[i] == 5)
			return true;
	}
	return false;
}

/* ���ڷ� ���� deck�� �ֻ����� push �� */
int Push(int *deck, int cardNum) { //���������� cardNum���� ���� ����..
	int deckCount = DeckCount(deck);
	if (deckCount != DECK_MAX_CNT) //�� �ִ밪�� �ƴ϶��
		deck[deckCount + 1] = cardNum;
}

/* ���ڷ� ���� deck�� �ֻ������� pop �� */
int Pop(int *deck) {
	int i;
	int deckCount = DeckCount(deck);
	int ret = deck[0];
	for (i = 0; i < deckCount - 1; i++) // ���� �տ� �ִ� ���� pop�ϰ� �ڿ� �ִ� ������ ��ĭ�� ������ �����
	{
		deck[i] = deck[i + 1];
	}
	return ret;
}


/* 게임 시작 시 덱을 섞어서 플레이어들에게 나누어주는 함수 */
void Shuffle()
{
    srand((unsigned)time(NULL));
    int deck[DECK_MAX_CNT];
    for(int i = 0; i < DECK_MAX_CNT; i++) // 덱을 만들어줌 
        deck[i] =  i;

    for(int i = 0; i < DECK_MAX_CNT; i++){ // 덱을 섞어줌
        int j = rand() % DECK_MAX_CNT;
        int tmp = 0;
        tmp = deck[i];
        deck[i] = deck[j];
        deck[j] = tmp;
    }

    for(int i = 0; i < PLAYER_MAX_CNT; i++){ // 플레이어들에게 나눠줌
        for(int j = 0; j < DECK_MAX_CNT/PLAYER_MAX_CNT; j++){
            playerDeck[i][j] = deck[DECK_MAX_CNT/PLAYER_MAX_CNT * i + j];
        }
    }
}


// void gotoxy(int x,int y) // 필요하면 주석 풀고 쓰면 됨
// {
//     printf("%c[%d;%df",0x1B,y,x);
// }

void DrawPlayerCard(int playerNum)
{
    printf("플레이어 %d\n",playerNum+1);
    int fruitCnt = GetFruitCnt(playerDeck[playerNum][0]); // 덱의 가장 앞에 있는 카드 출력
    char* fruitType;
    switch(GetFruitType(playerDeck[playerNum][0])){ 
        case 0:
            fruitType = "♤";
            break;
        case 1:
            fruitType = "♡";
            break;
        case 2:
            fruitType = "♧";
            break;
        case 3:
            fruitType = "◇";
            break;
    }
    printf("과일\n");
    for(int i = 0; i < fruitCnt; i++){
        printf("%s",fruitType);
    }
    printf("\n\n\n");
}

/* ������� �̸� ���� */
void ModifyName() {
	/* ������� ����Ʈ ���� */
	int num;
	do {
		do {
			printf("�����1: %s\n", user[0]);
			printf("�����2: %s\n", user[1]);
			printf("�����3: %s\n", user[2]);
			printf("�����4: %s\n", user[3]);

			printf("������ ������� ��ȣ�� �Է����ּ���(1~4)\n");
			printf("�ش� �׸񿡼� ����÷��� 0�� �����ּ���.\n");
			scanf_s("%d", &num);
		} while (num < 0 || num > 4);

		if (num == 0)
			break;

		char *modName;
		do {
			int i, myInputLen = 0;
			modName = "\0";
			modName = malloc(sizeof(char) * BUFSIZE); // +�� ���� NULL ������ ��
			user[num - 1] = NULL; //�迭 �ʱ�ȭ
			printf("�����%d�� ������ �����ϰڽ��ϴ�.\n", num);
			printf("������ �̸��� �����ֽʽÿ�. : ");
			scanf_s("%s", modName, sizeof(char)*BUFSIZE); //����ڰ� �Է��� ������ ����
			myInputLen = strlen(modName);
			for (i = 0; i < strlen(modName); i++) {
				if (((int)modName[i]) <= 32 || ((int)modName[i]) >= 127) { // ��°��� �ƽ�Ű�ڵ� �ƴ� ����
					i++;
					myInputLen--;
				}
			}
			if (myInputLen <= MAX_NAME_LENGTH) {
				break;
			}
			else {
				printf("������� �̸��� %d���ڰ� �ִ��Դϴ�. �ٽ��Է����ּ���.\n\n", MAX_NAME_LENGTH);
			}
		} while (true);
		user[num-1] = modName;
	} while (true);

}

/* ���� ��� ���� ���(txt ���� �о��) */
void GameDescription() {

}

/* halli galli ���� �����ϴ��Լ� */
void GameStart() {
    pid_t childPid = fork();
    int* childStat;
    if(childPid == -1) { // fork error
        perror("failed to fork");
    }
    else if(childPid == 0){ // 자식 코드. 게임이 실행되는 프로세스
        //게임 화면 출력하고
        printf("--------------------------------------------------------------------------------------------------------------\n");
        printf("\n");
        printf("\n");
        printf("\n");

        Shuffle(); // 패 섞어서 4명에게 나누어줌

        /* 패가 어떻게 섞였는지 확인하고 싶을 때 이 for문 주석 풀기 
        for(int i = 0; i < 4; i++){ 
            printf("플레이어 %d의 패 ",i);
            for(int j=0; j<DECK_MAX_CNT/PLAYER_MAX_CNT; j++)
                printf("%d  ", playerDeck[i][j]);
            printf("\n");
        } */

        for(int i = 0; i < PLAYER_MAX_CNT; i++) // 각 플레이어의 카드패 공개
            DrawPlayerCard(i);

        printf("\n");
        printf("\n");
        printf("\n");
        printf("--------------------------------------------------------------------------------------------------------------\n");

        //쓰레드 두개 돌려서 게임이 진행되는 부분과 입력을 받는 부분으로 나눈다

    }
    else{ // 부모 코드
        wait(childStat); // 부모 프로세스는 자식이 종료될 때 까지 기다림
        printf("부모의 종료");
    }
}

/*  �����Լ� ^^ */
void main(void) {
	//�ʱ� �̸� �Ҵ�
	char *defaultUserName[4] = { "�����1", "�����2", "�����3", "�����4"};
	for (int i = 0; i < 4; i++) {
		user[i] = (char*)malloc(sizeof(char)*BUFSIZE);
		strcpy_s(user[i], sizeof(char)*BUFSIZE, defaultUserName[i]);
	}

	//���� �����÷ο�
	do {
		int select;
		printf("�Ҹ�����\n\n");
		printf("1.���� ����\n");
		printf("2.����� �̸� ����\n");
		printf("3.���� ��� ����\n");
		printf("4.���� ����\n");
		printf("���Ͻô� �׸��� �������ּ���.:");
		scanf_s("%d", &select);

		if (select == 1)
			GameStart();
		else if (select == 2)
			ModifyName();
		else if (select == 3)
			GameDescription();
		else if (select == 4) {
			printf("������ �����մϴ�.\n");
			break;
		}
		else
			printf("1���� 3������ ��ȣ�� �������ּ���.\n");
	} while (1);

	//�޸� ����
	for (int i = 0; i < 4; i++) {
		free(user[i]);
	}
}
