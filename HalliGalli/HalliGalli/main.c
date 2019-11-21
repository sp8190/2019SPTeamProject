#include <stdio.h>
#include <stdbool.h>

/* 필드에 놓여있는 카드배열들인 field 들을 얻은 사용자의 덱인 deck에 추가함  */
void TakeCardsInField(int *deck, int *field) {
	int i;
	int deckcount = 0, fieldcount = 0; // 어디서 0이 체크가 되는가
	int decksize = sizeof(deck) / sizeof(deck[0]); // deck 배열의 크기
	int fieldsize = sizeof(field) / sizeof(field[0]); // field 배열의 크기

	// deck배열 에서 몇번째부터 카드가 없는지 
	for (i = 0; i < decksize; i++) {
		if (deck[i] == 0) {
			deckcount = i;
			break;
		}
	}
	// field배열 에서 몇번째부터 카드가 없는지 
	for (i = 0; i < fieldsize; i++) {
		if (field[i] == 0) {
			fieldcount = i;
			break;
		}
	}
	// field배열안 변수들 deck배열에 넣기
	for (i = 0; i < fieldsize; i++) {
		deck[deckcount + i] = field[i]
	}
}

/* int[4] 형태인 field에 같은 과일종류가 5개가 있는지 확인 */
bool IsFiveFruits(int *field) {
	int fieldCpy[4] = { 0 };
	int i;
	for (i = 0; i < 4; i++) {
		fieldCpy[GetFruitType(field[i])] += GetFruitCnt(field[i]);
	}
	//총합 5개인 과일 있는지 확인
	for (i = 0; i < 4; i++) {
		if (fieldCpy[i] == 5)
			return true;
	}
	return false;
}

/* 인자로 들어온 deck의 카드의 개수 반환 */
int DeckCount(int *deck) {

}

/* 인자로 들어온 deck의 최상위에 push 함 */
int Push(int *deck) {

}

/* 인자로 들어온 deck의 최상위값을 pop 함 */
int Pop(int *deck) {

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
		printf("GetFruitCnt error : index % 14 value is upper than 13");
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

/* 사용자의 이름 수정 */
void ModifyName() {

}

/* 게임 방법 설명 출력(txt 파일 읽어옴) */
void GameDescription() {

}

/* halli galli 게임 시작하는함수 */
void GameStart() {

}

/*  메인함수 ^^ */
void main(void) {
}
