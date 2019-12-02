#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <termios.h>

#define DECK_MAX_CNT 56
#define PLAYER_MAX_CNT 4
#define MAX_NAME_LENGTH 10   // 10글자 제한으로 잡았으나, 한글을 고려하여 20을 잡음
#define BUFSIZE 128
#define MAX 256

char *user[4]; // 10글자 제한으로 잡았으나, 한글을 고려하여 20을 잡음

//일단 걍 전역으로 두겠음 
int playerDeck[PLAYER_MAX_CNT][DECK_MAX_CNT]; //플레이어 4명의 덱
int countcard[4] = { 0 }; // 플레이어들이 낸 맨 앞 카드
int collectcard[56] = { 0 }; // 쌓이는 카드들
int collectnum = 0; // 쌓이는 횟수
bool playerGameOvered[PLAYER_MAX_CNT] = {false, false, false, false}; //플레이어 4명이 게임오버인지 아닌지

// 함수 선언(C언어...)
int DeckCount(int *deck);
int GetFruitCnt(int index);
int GetFruitType(int index);
void TakeCardsInField(int *deck, int *field);
bool IsFiveFruits(int *field);
int Push(int *deck, int cardNum);
int Pop(int *deck);
void Shuffle();
void DrawPlayerCard(int playerNum);
void GameStart();
int getch(void);
void* Gamescreen(void *data);
void* InputGameKey(void *data);

/* 인자로 들어온 deck의 카드의 개수 반환 */
int DeckCount(int *deck) {
	int deckCount = 0;
	int i;

	for (i = 0; i < DECK_MAX_CNT; i++) {
		// printf("deck[i] : %d\n",deck[i]);
		if (deck[i] == -1) { //비어있는 경우
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

/* 인자로 들어온 deck의 최상위에 push 함 */
int Push(int *deck, int cardNum) { //보고서에는 cardNum같은 변수 없음..
	int deckCount = DeckCount(deck);
	if (deckCount != DECK_MAX_CNT) //덱 최대값이 아니라면
		deck[deckCount] = cardNum;
}

/* 인자로 들어온 deck의 최상위값을 pop 함 */
int Pop(int *deck) {
	int i;
	int deckCount = DeckCount(deck);
	int ret = deck[0];
	for (i = 0; i < deckCount; i++) // 제일 앞에 있는 값을 pop하고 뒤에 있는 값들을 한칸씩 앞으로 댕겨줌
	{
		deck[i] = deck[i + 1];
	}
	return ret;
}


/* 게임 시작 시 덱을 섞어서 플레이어들에게 나누어주는 함수 */
void Shuffle()
{
	int i, j, k;
	
    srand((unsigned)time(NULL));
    int deck[DECK_MAX_CNT];
    for(i = 0; i < DECK_MAX_CNT; i++) // 덱을 만들어줌 
        deck[i] =  i;

    for(i = 0; i < DECK_MAX_CNT; i++){ // 덱을 섞어줌
        j = rand() % DECK_MAX_CNT;
        int tmp = 0;
        tmp = deck[i];
        deck[i] = deck[j];
        deck[j] = tmp;
    }

    for(i = 0; i < PLAYER_MAX_CNT; i++){ // 플레이어들에게 나눠줌
        for(j = 0; j < DECK_MAX_CNT/PLAYER_MAX_CNT; j++){
            playerDeck[i][j] = deck[DECK_MAX_CNT/PLAYER_MAX_CNT * i + j];
        }
		for(k = DECK_MAX_CNT/PLAYER_MAX_CNT; k < DECK_MAX_CNT; k++){ // 비어있는 경우는 -1로 초기화
			playerDeck[i][k] = -1;
		}
    }
}


// void gotoxy(int x,int y) // 필요하면 주석 풀고 쓰면 됨
// {
//     printf("%c[%d;%df",0x1B,y,x);
// }

/* 인자로 들어온 플레이어 번호에 해당하는 플레이어의 덱에서 제일 윗장의 카드를 보여줌 */
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

// 한 사용자라도 카드가 0장인지 체크해서 게임이 끝났는지 확인
bool CheckIfGameOver(){
	bool allGameOver = false;
	int gameOveredPlayers = 0; // 게임 오버된 플레이어의 수
	int i;
	for(i = 0; i < PLAYER_MAX_CNT; i++){		
		if(playerDeck[i][0] == -1){ // 맨 윗장이 비어있다면
			playerGameOvered[i] = true;
		}
	}	
	for(i = 0; i < PLAYER_MAX_CNT; i++){		
		if(playerGameOvered[i] == true){ 
			gameOveredPlayers ++;
		}
		if(gameOveredPlayers == PLAYER_MAX_CNT - 1){
			printf("게임 종료!\n");
			exit(0);
		}
	}
	return false;
}

/* 사용자의 이름 수정 */
void ModifyName() {
	/* 사용자의 디폴트 네임 */
	int num;
	char *modName;
	int i, myInputLen = 0, myInputLenCpy=0;
	do {
		do {
			printf("사용자1: %s\n", user[0]);
			printf("사용자2: %s\n", user[1]);
			printf("사용자3: %s\n", user[2]);
			printf("사용자4: %s\n", user[3]);

			printf("수정할 사용자의 번호를 입력해주세요(1~4)\n");
			printf("해당 항목에서 벗어나시려면 0을 눌러주세요.\n");
			scanf("%d", &num);
		} while (num < 0 || num > 4);

		if (num == 0)
			break;

		do {
			modName = "\0";
			modName = malloc(sizeof(char) * BUFSIZE); // +1은 맨 뒤의 NULL 고려한 것
			user[num - 1] = NULL; //배열 초기화
			printf("사용자%d의 정보를 수정하겠습니다.\n", num);
			printf("변경할 이름을 적어주십시오. : ");
			scanf("%s", modName); //사용자가 입력한 정보로 변경
			myInputLen = strlen(modName);
			myInputLenCpy = myInputLen;
			for (i = 0; i < myInputLenCpy; i++) {
				if (((int)modName[i]) <= 32 || ((int)modName[i]) >= 127) { // 출력가능 아스키코드 아닌 문자
					i+=2;
					myInputLen-=2;
				}
			}
			if (myInputLen <= MAX_NAME_LENGTH) {
				break;
			}
			else {
				printf("사용자의 이름은 %d글자가 최대입니다. 다시입력해주세요.\n\n", MAX_NAME_LENGTH);
			}
		} while (true);
		user[num-1] = modName;
	} while (true);
}

/* 게임 방법 설명 출력(txt 파일 읽어옴) */
void GameDescription() {
	char buffer[MAX];
	int readn = 0;
	int fp;

	fp = open("description.txt", O_RDONLY);//읽기 전용으로 파일을 읽는다.

	if (fp < 0) {//에러 메시지 호출
		perror("파일 에러입니다.");
		return 0;
	}

	readn = read(fp, buffer, MAX - 1);//fp의 내용을 buffer에 MAX-1만큼 읽는다.
	printf("%s\n", buffer);//buffer의 내용 출력

	close(fp);
}


/* halli galli 게임 시작하는함수 */
void GameStart() {
	pid_t childPid = fork();
	int* childStat;
	if (childPid == -1) { // fork error
		perror("failed to fork");
	}
	else if (childPid == 0) { // 자식 코드. 게임이 실행되는 프로세스
	   //게임 화면 출력하고
		printf("--------------------------------------------------------------------------------------------------------------\n");
		printf("\n");
		printf("\n");
		printf("\n");

		Shuffle(); // 패 섞어서 4명에게 나누어줌

		//  패가 어떻게 섞였는지 확인하고 싶을 때 이 for문 주석 풀기
		// for(int i = 0; i < 4; i++){
		//    printf("플레이어 %d의 패 ",i);
		//    for(int j=0; j<DECK_MAX_CNT/PLAYER_MAX_CNT; j++)
		// 	 printf("%d  ", playerDeck[i][j]);
		//    printf("\n");
		// } 

		// for (int i = 0; i < PLAYER_MAX_CNT; i++) // 각 플레이어의 카드패 공개
		// 	DrawPlayerCard(i);

		printf("\n");
		printf("\n");
		printf("\n");
		printf("--------------------------------------------------------------------------------------------------------------\n");

		//쓰레드 두개 돌려서 게임이 진행되는 부분과 입력을 받는 부분으로 나눈다

		pthread_t p_thread[2]; // pthread 2개 생성
		int i;
		int j;
		int start = rand() % PLAYER_MAX_CNT;
		int key = 0; // 버튼 누른값
		int count = 0; // 게임 끝나는 조건

		// 사용자 덱 처음에 보여줌 (확인용)
		// for (i = 0; i < PLAYER_MAX_CNT; i++) {
		//   printf("\n [%d]player deck :", i + 1);
		//   for (j = 0; j < DECK_MAX_CNT; j++) {
		//      printf("[%d]", playerDeck[i][j]);
		//   }
		// }

		printf("%d player turn", start + 1);

		while (1) {

			for (i = 0; i < PLAYER_MAX_CNT; i++) {
				if (playerDeck[i][0] == 0) {
					count++;
				}
			}
			//(확인용)
			if (count == 100) {
				count = 0;
				break;
			}

			// 게임이 진행되는 부분
			pthread_create(&p_thread[1], NULL, Gamescreen, (void *)&key);
			pthread_create(&p_thread[0], NULL, InputGameKey, (void *)&start);

			pthread_join(p_thread[1], NULL);
			pthread_join(p_thread[0], (void **)&key);

			count++; // 초기화(확인용)

			start++; // next player~
			if (key == 119 || key == 120 || key == 47 || key == 93) start--; // hit the ring!

			while(playerGameOvered[start] == true){ // 게임 오버인 사람은 빼놓고 턴이 돌아가게
				start++;
				if(start == PLAYER_MAX_CNT)
					start = 0;
			}
			if(start == PLAYER_MAX_CNT)
				start = 0;
		}

	}
	else { // 부모 코드
		wait(childStat); // 부모 프로세스는 자식이 종료될 때 까지 기다림
		printf("부모의 종료");
	}
}

// 실시간으로 키 입력 받는 함수
int getch(void)
{
	int ch;
	struct termios buf;
	struct termios save;

	tcgetattr(0, &save);
	buf = save;
	buf.c_lflag &= ~(ICANON | ECHO);
	buf.c_cc[VMIN] = 1;
	buf.c_cc[VTIME] = 0;
	tcsetattr(0, TCSAFLUSH, &buf);
	ch = getchar();
	tcsetattr(0, TCSAFLUSH, &save);
	return ch;
}

void* Gamescreen(void *data)
{
	int open = *((int *)data); // 누가 카드를 냈나?
	int printcard = 0;
	int i,j;

	// 카드 넘겼을 때 화면 출력
	switch (open) {
	case 113: // 1player
		if(playerGameOvered[0] == false){
			DrawPlayerCard(0);
			printcard = Pop(playerDeck[0]);
			countcard[0] = printcard; // 1player 앞에 놓여진 카드
			collectcard[collectnum] = printcard;
		}
		break;
	case 122: // 2player
		if(playerGameOvered[1] == false){
			DrawPlayerCard(1);
			printcard = Pop(playerDeck[1]);
			countcard[1] = printcard; // 2player 앞에 놓여진 카드
			collectcard[collectnum] = printcard;
		}
		break;
	case 46: // 3player
		if(playerGameOvered[2] == false){
			DrawPlayerCard(2);
			printcard = Pop(playerDeck[2]);
			countcard[2] = printcard; // 3player 앞에 놓여진 카드
			collectcard[collectnum] = printcard;
		}
		break;
	case 91: // 4player
		if(playerGameOvered[3] == false){
			DrawPlayerCard(3);
			printcard = Pop(playerDeck[3]);
			countcard[3] = printcard; // 4player 앞에 놓여진 카드
			collectcard[collectnum] = printcard;
		}
		break;
	}

	// 종을 눌렀을 때 화면 출력
	switch (open) {
	case 119: // 1player
	   	if(playerGameOvered[0] == false){
			if (IsFiveFruits(countcard) == true) {	   // 알맞게 종을 눌렀을 경우
				TakeCardsInField(playerDeck[0], collectcard);
				printf("정답! 플레이어 0 득점\n");
			}
			// 종이 틀렸을 경우
			if (IsFiveFruits(countcard) == false) {
				printf("오답! 플레이어 0 감점\n");
				for(i = 0; i < PLAYER_MAX_CNT; i++){
					printcard = Pop(playerDeck[0]);
					if(printcard != -1){ 
						if(i != 0 && playerGameOvered[i] == false){ // 본인의 번호가 아니고, 게임오버 되지 않은 플레이어한테
							Push(playerDeck[i], printcard);	// 내 카드 팝해서 추가시켜줌
						}
					} else{ // 덱이 전부 비었다면
						if(CheckIfGameOver()){ // 이 플레이어의 패배를 기록하고, 모든 게임이 끝났는지 확인한다.
							exit(0);
						}
					}
				}
			}
		}
		break;
	case 120: // 2player
	   	if(playerGameOvered[1] == false){
			if (IsFiveFruits(countcard) == true) {
				TakeCardsInField(playerDeck[1], collectcard);
				printf("정답! 플레이어 1 득점\n");
			}
			if (IsFiveFruits(countcard) == false) {
				printf("오답! 플레이어 1 감점\n");
				for(i = 0; i < PLAYER_MAX_CNT; i++){
					printcard = Pop(playerDeck[1]);
					if(printcard != -1){ 
						if(i != 1  && playerGameOvered[i] == false){
							Push(playerDeck[i], printcard);		
						}
					} else{
						if(CheckIfGameOver()){
							exit(0);
						}
					}
				}
			}
		}
		break;
	case 47: // 3player
	   	if(playerGameOvered[2] == false){
			if (IsFiveFruits(countcard) == true) {
				TakeCardsInField(playerDeck[2], collectcard);
				printf("정답! 플레이어 2 득점\n");
			}
			if (IsFiveFruits(countcard) == false) {
				printf("오답! 플레이어 2 감점\n");

				for(i = 0; i < PLAYER_MAX_CNT; i++){
					printcard = Pop(playerDeck[2]);
					if(printcard != -1){ 
						if(i != 2  && playerGameOvered[i] == false){
							Push(playerDeck[i], printcard);		
						}
					} else{
						if(CheckIfGameOver()){
							exit(0);
						}
					}
				}
			}
		}
		break;
	case 93: // 4player
	   	if(playerGameOvered[3] == false){
			if (IsFiveFruits(countcard) == true) {
				TakeCardsInField(playerDeck[3], collectcard);
				printf("정답! 플레이어 3 득점\n");
			}
			if (IsFiveFruits(countcard) == false) {
				printf("오답! 플레이어 3 감점\n");

				for(i = 0; i < PLAYER_MAX_CNT; i++){
					printcard = Pop(playerDeck[3]);
					if(printcard != -1){ 
						if(i != 3  && playerGameOvered[i] == false){
							Push(playerDeck[i], printcard);		
						}
					} else{
						if(CheckIfGameOver()){
							exit(0);
						}
					}
				}
			}
		}
		break;
	}

	for (i = 0; i < PLAYER_MAX_CNT; i++) {
		printf("\n [%d]player deck의 상위 세 장 :", i + 1);
		for (j = 0; j < 3; j++) {
			printf("[%d]", playerDeck[i][j]);
		}
	}
	printf("\n");

	if(CheckIfGameOver()){
		exit(0);
	}
}

void* InputGameKey(void *data)
{
	int ch = 0;
	int count = *((int *)data); // 어떤 플레이어가 하는지

	//1 player cardopen : 113 / ring : 119
	//2 player cardopen : 122 / ring : 120
	//3 player cardopen : 46 / ring : 47
	//4 player cardopen : 91 / ring : 93
	while (1) {
		ch = getch();
		//printf(" [%d this hit!!] ", ch); (확인용)

		// 종 누르는 거
		if (ch == 119) { // W
			// printf("[1player] ring ");
			return (void *)119;
		}
		else if (ch == 120) { // X
			// printf("[2player] ring ");
			return (void *)120;
		}
		else if (ch == 47) { // /
			// printf("[3player] ring ");
			return (void *)47;
		}
		else if (ch == 93) { // ]
			// printf("[4player] ring ");
			return (void *)93;
		}

		// 카드 넘기는거
		switch (count) {
		case 0:
			if (ch == 113) { // Q
				printf("[1player] open ");
				return (void *)113;
			}
			break;
		case 1:
			if (ch == 122) { // Z
				printf("[2player] open ");
				return (void *)122;
			}
			break;
		case 2:
			if (ch == 46) { // . 
				printf("[3player] open ");
				return (void *)46;
			}
			break;
		case 3:
			if (ch == 91) { // [
				printf("[4player] open ");
				return (void *)91;
			}
			break;
		}
	}
}

/*  메인 함수 ^^ */
void main(void) {
	//초기 이름 할당
	char *defaultUserName[4] = { "사용자1", "사용자2", "사용자3", "사용자4"};
	int i;
	for (i = 0; i < 4; i++) {
		user[i] = (char*)malloc(sizeof(char)*BUFSIZE);
		strcpy(user[i], defaultUserName[i]);
	}


	//메인 실행플로우
	do {
		int select;
		printf("할리갈리\n\n");
		printf("1.게임 시작\n");
		printf("2.사용자 이름 수정\n");
		printf("3.게임 방법 설명\n");
		printf("4.게임 종료\n");
		printf("원하시는 항목을 선택해주세요.:");
		scanf("%d", &select);

		if (select == 1)
			GameStart();
		else if (select == 2)
			ModifyName();
		else if (select == 3)
			GameDescription();
		else if (select == 4) {
			printf("게임을 종료합니다.\n");
			break;
		}
		else
			printf("1에서 3까지의 번호를 선택해주세요.\n");
	} while (1);

	//메모리 정리
	for (int i = 0; i < 4; i++) {
		free(user[i]);
	}
}
