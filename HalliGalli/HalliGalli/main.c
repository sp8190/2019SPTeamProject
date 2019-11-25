#include <stdio.h>
#include <windows.h>
#include <stdbool.h>

#define DECK_MAX_CNT 56

/* 필드에 놓여있는 카드배열들인 field 들을 얻은 사용자의 덱인 deck에 추가함  */
void TakeCardsInField(int *deck, int *field) {
	int i;
	int deckcount = 0, fieldcount = 0; // 어디서 0이 체크가 되는가
	int decksize = sizeof(deck) / sizeof(deck[0]); // deck 배열의 크기
	int fieldsize = sizeof(field) / sizeof(field[0]); // field 배열의 크기

	// deck배열 에서 몇번째부터 카드가 없는지 
	/*for (i = 0; i < decksize; i++) {
		if (deck[i] == 0) {
			deckcount = i;
			break;
		}
	}*/
	deckcount = DeckCount(deck); // 위 구문을 함수로 대체했음


	// field배열 에서 몇번째부터 카드가 없는지 
	for (i = 0; i < fieldsize; i++) { //여기도 deckCount처럼 함수로 대체 가능함
		if (field[i] == 0) {
			fieldcount = i;
			break;
		}
	}

	// field배열안 변수들 deck배열에 넣기
	for (i = 0; i < fieldsize; i++) {
		deck[deckcount + i] = field[i];
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

/* 인자로 들어온 deck의 최상위에 push 함 */
int Push(int *deck, int cardNum) { //보고서에는 cardNum같은 변수 없음..
	int deckCount = DeckCount(deck);
	if(deckCount != DECK_MAX_CNT) //덱 최대값이 아니라면
		deck[deckCount + 1] = cardNum;
}

/* 인자로 들어온 deck의 최상위값을 pop 함 */
int Pop(int *deck) {
	int i;
	int deckCount = DeckCount(deck);
	int ret = deck[0];
	for (i = 0; i < deckCount - 1; i++) // 제일 앞에 있는 값을 pop하고 뒤에 있는 값들을 한칸씩 앞으로 댕겨줌
	{
		deck[i] = deck[i + 1];
	}
	return ret;
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
	/* 사용자의 디폴트 네임 */
	char defaultName1[] = "사용자1";
	char defaultName2[] = "사용자2";
	char defaultName3[] = "사용자3";
	char defaultName4[] = "사용자4";
	int num;
	
	printf("사용자1: %s\n", &defaultName1);
	printf("사용자2: %s\n", &defaultName2);
	printf("사용자3: %s\n", &defaultName3);
	printf("사용자4: %s\n", &defaultName4);
	do {
			
			printf("수정할 사용자의 번호를 입력해주세요(1~4)\n");
			printf("해당 항목에서 벗어나시려면 0을 눌러주세요.\n");
			scanf("%d", &num);
			switch (num) {
			case 0 :
				break;
			case 1 :
				defaultName1[0] = '\0'; //배열 초기화
				printf("사용자%d의 정보를 수정하겠습니다.\n", &num);
				printf("변경할 이름을 적어주십시오. : ");
				scanf("%s", &defaultName1); //사용자가 입력한 정보로 변경
				break;
			case 2 :
				defaultName2[0] = '\0';
				printf("사용자%d의 정보를 수정하겠습니다.\n", &num);
				printf("변경할 이름을 적어주십시오. : ");
				scanf("%s", &defaultName2);
				break;
			case 3 :
				defaultName3[0] = '\0';
				printf("사용자%d의 정보를 수정하겠습니다.\n", &num);
				printf("변경할 이름을 적어주십시오. : ");
				scanf("%s", &defaultName3);
				break;
			case 4 :
				defaultName4[0] = '\0';
				printf("사용자%d의 정보를 수정하겠습니다.\n", &num);
				printf("변경할 이름을 적어주십시오. : ");
				scanf("%s", &defaultName4);
				break;
			default:
				printf("사용자는 1부터 4까지 존재합니다.\n");
			}
			if (num == 0)//0입력시 종료
				break;
		
	} while (num<0||num>4);
	
}

/* 게임 방법 설명 출력(txt 파일 읽어옴) */
void GameDescription() {

}

/* halli galli 게임 시작하는함수 */
void GameStart() {

}

/*  메인함수 ^^ */
void main(void) {
	int select;

	printf("할리갈리\n\n");
	printf("1.게임 시작\n");
	printf("2.사용자 이름 수정\n");
	printf("3.게임 방법 설명\n");
	printf("원하시는 항목을 선택해주세요.:");
	scanf("%d", &select);

	switch (select) {
	case 1:
		GameStart();
		break;
	case 2:
		ModifyName();
		break;
	case 3:
		GameDescription();
		break;
	default:
		printf("1에서 3까지의 번호를 선택해주세요.\n");
	}
}
