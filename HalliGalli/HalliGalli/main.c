#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>

#define DECK_MAX_CNT 56
#define MAX_NAME_LENGTH 10   // 10���� �������� �������, �ѱ��� ����Ͽ� 20�� ����
#define BUFSIZE 128

char *user[4] = { "�����1", "�����2", "�����3", "�����4" }; // �̸��� ��� �Լ����� �����ϱ� ������ �������� �����ϴ�.

/* �ʵ忡 �����ִ� ī��迭���� field ���� ���� ������� ���� deck�� �߰���  */
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

/* ���ڷ� ���� deck�� ī���� ���� ��ȯ */
int DeckCount(int *deck) {
	int deckCount = 0;
	int decksize = sizeof(deck) / sizeof(deck[0]); // deck �迭�� ũ��
	int i;

	for (i = 0; i < decksize; i++) {
		if (deck[i] == 0) {
			deckCount = i;
			break;
		}
	}
	return deckCount;
}

/* ���ڷ� ���� deck�� �ֻ����� push �� */
int Push(int *deck, int cardNum) { //�������� cardNum���� ���� ����..
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

/* index�� �ش��� ī���� ���ϰ��� ���� */
int GetFruitCnt(int index) {

	//1��¥�� 5��, 2��¥�� 3��, 3��¥�� 3��, 4��¥�� 2��, 5��¥�� 1�� ����
	//0 ~ 55 �� index ���� ���ڷ� ����
	// %14 �ؼ�..
	// 0,1,2,3,4 = 1��
	// 5,6,7	 = 2��
	// 8,9,10	 = 3��
	// 11, 12	 = 4��
	// 13		 = 5��

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
		printf("GetFruitCnt error : index % 14 value is upper than 13");
		return -1;
	}
}

/* index�� �ش��� ī���� �������� ���� */
int GetFruitType(int index) {

	// 14���� ī�尡 4���� ������ ���е�
	// 0 ~ 55 �� index ���� ���ڷ� ����
	// 0 ~ 13 : ����1
	// 14 ~ 27 : ����2
	// 28 ~ 41 : ����3
	// 42 ~ 55 : ����4
	// ��ȯ�� : 0, 1, 2, 3

	return (int)(index / 14);
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
			modName = malloc(sizeof(char) * BUFSIZE); // +1�� �� ���� NULL ����� ��
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

}

/*  �����Լ� ^^ */
void main(void) {
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
			return 0;
		}
		else
			printf("1���� 3������ ��ȣ�� �������ּ���.\n");
	} while (1);
}
