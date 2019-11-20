#include <stdio.h>
#include <stdbool.h>

/* �ʵ忡 �����ִ� ī��迭���� field ���� ���� ������� ���� deck�� �߰���  */
void TakeCardsInField(int *deck, int *field) {

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

}

/* ���ڷ� ���� deck�� �ֻ����� push �� */
int Push(int *deck) {

}

/* ���ڷ� ���� deck�� �ֻ������� pop �� */
int Pop(int *deck) {

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

}

/* ���� ��� ���� ���(txt ���� �о��) */
void GameDescription() {

}

/* halli galli ���� �����ϴ��Լ� */
void GameStart() {

}

/*  �����Լ� ^^ */
void main(void) {
}