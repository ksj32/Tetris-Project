#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>


//키보드값들 SetConsoleCursorPosition에 기반한 아스키값
#define LEFT 75 //좌로 이동    
#define RIGHT 77 //우로 이동 
#define UP 72 //회전 
#define DOWN 80 //soft drop
#define SPACE 32 //hard drop
#define p 112 //일시정지 
#define P 80 //일시정지
#define item 105 //아이템생성
#define Item 73 //아이템생성
#define bombitem 98 //폭탄아이템생성 
#define Bombitem 66 //폭탄아이템생성
#define ESC 27 //게임종료 

#define false 0
#define true 1
#define SPECIAL 999 //폭탄아이템

#define ACTIVE_BLOCK -2 // 메인화면배열에 저장될 블럭의 상태들 
#define CEILLING -1     // 블럭이 이동할 수 있는 공간은 0 또는 음의 정수료 표현 
#define EMPTY 0         // 블럭이 이동할 수 없는 공간은 양수로 표현 
#define WALL 1
#define INACTIVE_BLOCK 2 // 이동이 완료된 블럭값 

#define MAIN_X 11 //메인화면 가로크기 
#define MAIN_Y 23 //메인화면 세로크기 
#define MAIN_X_ADJ 3 //메인화면 위치조정 
#define MAIN_Y_ADJ 1 //메인화면 위치조정 

#define NormalBlock 0 // 일반블럭의 우선순위 
#define ItemBlock 1 // 아이템블럭의 우선순위

#define STATUS_X_ADJ MAIN_X_ADJ+MAIN_X+1 //게임정보표시 위치조정 

int STATUS_Y_GOAL; //GOAL 정보표시위치Y 좌표 저장 
int STATUS_Y_LEVEL; //LEVEL 정보표시위치Y 좌표 저장
int STATUS_Y_SCORE; //SCORE 정보표시위치Y 좌표 저장

int blocks[7][4][4][4] = {

	/*type0*/{ { 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 } },
	/*type1*/{ { 0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0 },{ 0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0 },{ 0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0 },{ 0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0 } },
	/*type2*/{ { 0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0 },{ 0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0 } },
	/*type3*/{ { 0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0 },{ 0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0 },{ 0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0 },{ 0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0 } },
	/*type4*/{ { 0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0 },{ 0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0 } },
	/*type5*/{ { 0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0 },{ 0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0 },{ 0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0 } },
	/*type6*/{ { 0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0 },{ 0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0 },{ 0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0 } }

}; //블럭모양 저장 4*4공간에 블럭을 표현 blcoks[b_type][b_rotation][i][j]로 사용 

int b_type; //블럭 종류를 저장 
int b_rotation; //블럭 회전값 저장 
int b_type_next; //다음 블럭값 저장 

int main_stat[MAIN_Y][MAIN_X]; //메인화면의 정보를 저장하는 배열 모니터에 표시후에 sub_main로 복사됨 
int sub_main[MAIN_Y][MAIN_X]; //즉 maincpy는 메인화면이 모니터에 표시되기 전의 정보를 가지고 있음 
							  //main의 전체를 계속 모니터에 표시하지 않고(이렇게 하면 모니터가 깜빡거림) 
							  //sub_main와 배열을 비교해서 값이 달라진 곳만 모니터에 고침 

int bx, by; //이동중인 블럭의 메인화면상의 x,y좌표를 저장 

int key; //키보드로 입력받은 키값을 저장 
int bombkey; //키보드로 입력받은 폭탄아이템 키값 
int bomb_count = 3;

int speed; //게임진행속도 
int level; //현재 level 
int level_goal; //다음레벨로 넘어가기 위한 목표점수 
int cnt; //현재 레벨에서 제거한 줄 수를 저장 
int score; //현재 점수 
int last_score = 0; //마지막게임점수 
int best_score = 0; //랭킹최고점

int change_count = 5;	//블록 변환 아이템 횟수 체크

int new_block_on = 0; //새로운 블럭이 필요함을 알리는 변수 
int crush_on = 0; //현재 이동중인 블럭이 충돌상태인지 알려주는 변수 
int level_up_on = 0; //스테이지완료상태를 알리는 변수 
int space_key_on = 0; //hard drop상태임을 알려주는 변수

void set_title(void); //게임시작화면 
void reset(int rand_num); //메인화면 초기화 
void reset_main(void); //메인 메인화면(main_stat[][]를 초기화)
void reset_sub_main(void); //copy 메인화면(sub_main[][]를 초기화)
void draw_map(void); //게임 전체 인터페이스를 표시 
void draw_main(void); //메인화면을 그림 
void new_block(int rand_num); //새로운 블럭을 하나 만듦 
void check_key(void); //키보드로 키를 입력받음 
void drop_block(void); //블럭을 아래로 떨어트림 
int check_crush(int bx, int by, int rotation); //bx, by위치에 rotation회전값을 같는 경우 충돌 판단 
void move_block(int dir); //dir방향으로 블럭을 움직임 
void check_line(void); //줄이 가득찼는지를 판단하고 지움 
void check_level_up(void); //레벨목표가 달성되었는지를 판단하고 levelup시킴 
void check_game_over(int rand_num); //게임오버인지 판단하고 게임오버를 진행 
void pause(void);//게임을 일시정지시킴 
void alert_game(void);

void gotoxy(int x, int y) //gotoxy함수
{
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //커서숨기는 함수에 사용되는 열거형 
void setcursortype(CURSOR_TYPE c) //커서숨기는 함수 
{
	CONSOLE_CURSOR_INFO CurInfo;

	switch (c)
	{
	case NOCURSOR:
		CurInfo.dwSize = 1;
		CurInfo.bVisible = FALSE;
		break;
	case SOLIDCURSOR:
		CurInfo.dwSize = 100;
		CurInfo.bVisible = TRUE;
		break;
	case NORMALCURSOR:
		CurInfo.dwSize = 20;
		CurInfo.bVisible = TRUE;
		break;
	}
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

typedef struct tagNode
{
	int Priority;
	int rand_value;
	struct tagNode* NextNode;
}Node;

typedef struct tagLinkedQueue
{
	Node* Front;
	Node* Rear;
	int count;
}LinkedQueue;

void LQ_CreateQueue(LinkedQueue** Queue);				//큐 생성 함수  
Node* LQ_CreateNode(int NewData);		       			//노드 생성 함수 
void LQ_DestroyNode(Node* _Node);						//노드 삭제 함수 
void LQ_Enqueue(LinkedQueue* Queue, Node* NewNode);		//노드 삽입 함수 
Node* LQ_Dequeue(LinkedQueue* Queue);					//노드 출력 함수 
int LQ_IsEmpty(LinkedQueue* Queue);						//큐 공백검사 함수 
void make_Node(LinkedQueue** Queue);					//큐의 노드 5개 생성 함수  
void check_Queue_count(LinkedQueue* Queue);				//큐의 노드가 없을 경우 make_node함수 호출 함수  

LinkedQueue* Queue;			//큐 선언

int main()
{
	int i;
	int rand_num;

	Node* Popped;				//임시 Node형 포인터 
	LQ_CreateQueue(&Queue);		//큐 생성  

	srand((unsigned)time(NULL)); //난수표생성
	setcursortype(NOCURSOR); //커서 없앰 
	system("mode con:cols=88 lines=27");//콘솔창 화면조정
	system("title 우선순위 큐를 활용한 테트리스");//콘솔창 타이틀지정

	set_title(); //메인타이틀 호출 

	make_Node(&Queue);				// 노드 생성   

	Popped = LQ_Dequeue(Queue);		//Popped 포인터가 큐에서 출력한 노드를 가리키게 함  
	check_Queue_count(Queue);		//큐의 노드 empty확인  
	rand_num = Popped->rand_value;	//노드의 랜덤값을 저장  
	reset(rand_num);				//값에의한 호출로 랜덤값을 함수의 인자로 줌 
	LQ_DestroyNode(Popped);			//노드 삭제  

	while (1)
	{
		for (i = 0; i < 5; i++) //블럭이 한칸떨어지는동안 5번 키입력받을 수 있음 
		{
			check_key(); //키입력확인 
			draw_main(); //화면을 그림 
			Sleep(speed); //게임속도조절 
			if (crush_on && check_crush(bx, by + 1, b_rotation) == false) Sleep(100);
			//블럭이 충돌상태일 경우 추가조작의 시간을 갖게됨
			if (space_key_on == 1) { //스페이스바를 누른경우(hard drop) 추가로 이동및 회전할수 없음 break; 
				space_key_on = 0;
				break;
			}
		}

		check_level_up(); // 레벨업을 체크 
		check_game_over(rand_num); //게임오버를 체크 

		if (new_block_on == 1) // 뉴 블럭 상태가 있는 경우 새로운 블럭 생성
		{
			if (bomb_count == 0)
			{
				bomb_count = -1;
				alert_game();
			}
			//폭탄아이템 블록 생성
			if (bombkey == 7 && bomb_count > 0)  //  B or b를 입력했을 경우에 
			{
				bomb_count--;
				bombkey = 0;  //  노드생성 후에는 폭탄키값을 초기화  
				Popped = LQ_Dequeue(Queue);
				check_Queue_count(Queue);
				for (int i = 1; i < MAIN_X - 1; i++)
				{
					for (int j = 1; j < MAIN_Y - 1; j++)
					{
						main_stat[j][i] = 0;
					}
				}	
			}
			else  //기본 블럭 생성  
			{
				Popped = LQ_Dequeue(Queue);
				check_Queue_count(Queue);
			}

			rand_num = Popped->rand_value;
			new_block(rand_num);
			LQ_DestroyNode(Popped);
		}
		if (new_block_on == 2) { //폭탄 아이템 생성 명령이 발생한 경우
								 //새로운 블록 생성
								 // 10.15 우선순위비교해서 우선순위가 높은 아이템을 먼저꺼냄
								 // 우선순위 비교해서 pop연산 수행하도록 dequeue부분을 수정하면 될 것


		}
		drop_block(); // 블럭을 한칸 내림 
	}
}

void set_title(void) {
	int x = 8; //타이틀화면이 표시되는 x좌표 
	int y = 3; //타이틀화면이 표시되는 y좌표 
	int cnt; //타이틀 프레임을 세는 변수  

			 //박스드로어
	gotoxy(x + 6, y); printf("┌─────────────────────────┐");


	gotoxy(x + 6, y + 4); printf("└─────────────────────────┘");

	gotoxy(x + 8, y + 2); printf("     TETRIS GAME ");

	gotoxy(x + 7, y + 12); printf("PRESS ANY KEY TO PLAY GAME");




	for (cnt = 0;; cnt++) { //cnt를 1씩 증가시키면서 계속 반복   
		gotoxy(x + 7, y + 2); printf("☆");//타이틀깜빡이(앞)
		gotoxy(x + 18, y + 2); printf("☆");//타이틀깜빡이(뒤)



		Sleep(600);
		gotoxy(x + 7, y + 2); printf("★");//타이틀깜빡이(앞)
		gotoxy(x + 18, y + 2); printf("★");//타이틀깜빡이(뒤)
		gotoxy(x + 20 + cnt, y + 12); printf(".");
		Sleep(600);
		if (cnt == 2) {

			gotoxy(x + 18, y + 12); printf("                ");
			cnt = -1;

		}
		if (_kbhit()) {
			break; //키입력이 있으면 무한루프 종료 
		}
	}

	while (_kbhit()) _getch(); //버퍼에 기록된 키값을 버림 
}

void reset(int rand_num) {

	Node* Popped;
	int rand_num_temp;

	FILE* file = fopen("score.dat", "rt"); // score.dat파일을 연결 
	if (file == 0) { best_score = 0; } //파일이 없으면 걍 최고점수에 0을 넣음 
	else {
		fscanf(file, "%d", &best_score); // 파일이 열리면 최고점수를 불러옴 
		fclose(file); //파일 닫음 
	}

	level = 1; //각종변수 초기화 
	score = 0;
	level_goal = 1000;
	key = 0;
	crush_on = 0;
	cnt = 0;
	speed = 100;

	system("cls"); //화면지움 
	reset_main(); // main_stat를 초기화 
	draw_map(); // 게임화면을 그림
	draw_main(); // 메인화면을 그림 

	change_count = 5; //블록변경 찬스 초기화

	Popped = LQ_Dequeue(Queue);			//reset이 발생할 때 예외적으로 노드  Dequeue후 b_tyoe_next에 저장  
	rand_num_temp = Popped->rand_value; //(같은 블럭이 2번 나옴을 방지하기 위함)  
	b_type_next = rand_num_temp; //다음번에 나올 블럭 종류를 랜덤하게 생성 ---------------------------------------------------
	LQ_DestroyNode(Popped);
	new_block(rand_num); //새로운 블럭을 하나 만듦  

}

void reset_main(void) { //메인화면을 초기화  
	int i, j;

	for (i = 0; i < MAIN_Y; i++) { // 메인화면을 0으로 초기화  
		for (j = 0; j < MAIN_X; j++) {
			main_stat[i][j] = 0;
			sub_main[i][j] = 100;
		}
	}
	for (j = 1; j < MAIN_X; j++) { //y값이 3인 위치에 천장을 만듦 
		main_stat[3][j] = CEILLING;
	}
	for (i = 1; i < MAIN_Y - 1; i++) { //좌우 벽을 만듦  
		main_stat[i][0] = WALL;
		main_stat[i][MAIN_X - 1] = WALL;
	}
	for (j = 0; j < MAIN_X; j++) { //바닥벽을 만듦 
		main_stat[MAIN_Y - 1][j] = WALL;
	}
}

void reset_sub_main(void) { //sub_main를 초기화 
	int i, j;

	for (i = 0; i < MAIN_Y; i++) {         //메인화면에 게임에 사용되지 않는 숫자를 넣음 
		for (j = 0; j < MAIN_X; j++) {  //이는 main_stat와 같은 숫자가 없게 하기 위함 
			sub_main[i][j] = 100;
		}
	}
}

void draw_map(void) { //게임 상태 표시를 나타내는 함수  
	int y = 3;             // level, goal, score만 게임중에 값이 바뀔수 도 있음 그 y값을 따로 저장해둠 
						   // 그래서 혹시 게임 상태 표시 위치가 바뀌어도 그 함수에서 안바꿔도 되게.. 
	gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL = y - 1); printf(" 레벨 : %5d", level);
	gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL = y); printf(" 목표  : %5d", 10 - cnt);
	gotoxy(STATUS_X_ADJ, y + 2); printf("+- 다음 블럭 -+ ");
	gotoxy(STATUS_X_ADJ, y + 3); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 4); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 5); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 6); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 7); printf("+-- -  -  - --+ ");
	gotoxy(STATUS_X_ADJ, y + 9); printf(" 현재 점수 :");
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE = y + 10); printf("        %6d", score);
	gotoxy(STATUS_X_ADJ, y + 11); printf(" 이전 점수 :");
	gotoxy(STATUS_X_ADJ, y + 12); printf("        %6d", last_score);
	gotoxy(STATUS_X_ADJ, y + 13); printf(" 최고 점수 :");
	gotoxy(STATUS_X_ADJ, y + 14); printf("        %6d", best_score);
	gotoxy(STATUS_X_ADJ, y + 16); printf("  △   : 블럭변경        SPACE : 즉시낙하");
	gotoxy(STATUS_X_ADJ, y + 17); printf("◁  ▷ : 좌/우 이동      P   : 일시정지");
	gotoxy(STATUS_X_ADJ, y + 18); printf("  ▽   : 약간빠른낙하    ESC  : 종료");
	gotoxy(STATUS_X_ADJ, y + 20); printf("  B   : 전체 폭탄        I  : 블럭 체인지");
}

void draw_main(void) { //메인화면 그리는 함수 
	int i, j;

	for (j = 1; j < MAIN_X - 1; j++) { //천장은 계속 새로운블럭이 지나가서 지워지면 새로 그려줌 
		if (main_stat[3][j] == EMPTY) main_stat[3][j] = CEILLING;
	}
	for (i = 0; i < MAIN_Y; i++) {
		for (j = 0; j < MAIN_X; j++) {
			if (sub_main[i][j] != main_stat[i][j]) { //cpy랑 비교해서 값이 달라진 부분만 새로 그려줌.

				gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
				switch (main_stat[i][j]) {
				case EMPTY: //빈칸모양 
					printf("  ");
					break;
				case CEILLING: //천장모양 
					printf(". ");
					break;
				case WALL: //벽모양 
					printf("▒");
					break;
				case INACTIVE_BLOCK: //굳은 블럭 모양  
					printf("□");
					break;
				case ACTIVE_BLOCK: //움직이고있는 블럭 모양  
					printf("■");
					break;
				case SPECIAL: //특수아이템모양(폭탄)
					printf("☆");
					break;
				}
			}
		}
	}
	for (i = 0; i < MAIN_Y; i++) { //메인화면을 그린 후 sub_main에 복사  
		for (j = 0; j < MAIN_X; j++) {
			sub_main[i][j] = main_stat[i][j];
		}
	}
}

void new_block(int rand_num) { //새로운 블럭 생성  
	int i, j;

	bx = (MAIN_X / 2) - 1; //블럭 생성 위치x좌표(메인화면의 가운데) 
	by = 0;  //블럭 생성위치 y좌표(제일 위) 

	if (rand_num == 7) {
		new_block_on = 0; //new_block 상태를 끔  
		for (i = 0; i < 4; i++) { //메인화면 bx, by위치에 블럭생성  
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_stat[by + i][bx + j] = SPECIAL;
			}
		}
	}
	else {
		b_type = b_type_next; //다음블럭값을 가져옴 
		b_type_next = rand_num; //다음 블럭을 만듦 
		b_rotation = 0;  //회전은 0번으로 가져옴 

		new_block_on = 0; //new_block 상태를 끔  

		for (i = 0; i < 4; i++) { //메인화면 bx, by위치에 블럭생성  
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_stat[by + i][bx + j] = ACTIVE_BLOCK;
			}
		}
	}

	for (i = 1; i <= 3; i++) { //게임상태표시에 다음에 나올블럭을 그림 
		for (j = 0; j < 4; j++) {
			if (blocks[b_type_next][0][i][j] == 1) {
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				printf("■");
			}
			else {
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				printf("  ");
			}
		}
	}
}

void check_key(void) {
	key = 0; //키값 초기화  

	if (bomb_count == -1) {
		gotoxy(STATUS_X_ADJ, 24); printf("         %d회                   %d회", 0, change_count);		//연속적으로 현 횟수를 알기위해 이 함수 내에 작성
	}
	else {
		gotoxy(STATUS_X_ADJ, 24); printf("         %d회                    %d회", bomb_count, change_count);		//연속적으로 현 횟수를 알기위해 이 함수 내에 작성
	}
	if (_kbhit()) { //키입력이 있는 경우  
		key = _getch(); //키값을 받음
		if (key == 224) { //방향키인경우 
			do { key = _getch(); } while (key == 224);//방향키지시값을 버림 
			switch (key) {
			case LEFT: //왼쪽키 눌렀을때  
				if (check_crush(bx - 1, by, b_rotation) == true) move_block(LEFT);
				break;                            //왼쪽으로 갈 수 있는지 체크 후 가능하면 이동
			case RIGHT: //오른쪽 방향키 눌렀을때- 위와 동일하게 처리됨 
				if (check_crush(bx + 1, by, b_rotation) == true) move_block(RIGHT);
				break;
			case DOWN: //아래쪽 방향키 눌렀을때-위와 동일하게 처리됨 
				if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
				break;
			case UP: //위쪽 방향키 눌렀을때 
				if (check_crush(bx, by, (b_rotation + 1) % 4) == true) move_block(UP);
				//회전할 수 있는지 체크 후 가능하면 회전
				else if (crush_on == 1 && check_crush(bx, by - 1, (b_rotation + 1) % 4) == true) move_block(100);
			}                    //바닥에 닿은 경우 위쪽으로 한칸띄워서 회전이 가능하면 그렇게 함(특수동작)
		}
		else { //방향키가 아닌경우 
			switch (key) {
			case SPACE: //스페이스키 눌렀을때 
				space_key_on = 1; //스페이스키 상태를 띄움 
				while (crush_on == 0) { //바닥에 닿을때까지 이동시킴 
					drop_block();
					score += level; // hard drop 보너스
					gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score); //점수 표시  
				}
				break;
			case P: //P(대문자) 눌렀을때 
			case p: //p(소문자) 눌렀을때 
				pause(); //일시정지 
				break;
			case item://아이템생성(대문자 I)이 눌렸을 때
			case Item://아이템생성(소문자 i)이 눌렸을 때
				if (change_count >= 1)		//블록 변환아이템 5번 사용가능
				{
					new_block_on = 1;
					//화면 내 필드비우기.
					for (int i = 0; i < 10; i++)
					{
						for (int j = 0; j < 150; j++)
						{
							if (main_stat[i][j] == ACTIVE_BLOCK)
							{
								main_stat[i][j] = EMPTY;
							}
						}
					}
					change_count--;
					gotoxy(MAIN_X_ADJ + 1, MAIN_Y_ADJ + 2); printf("블럭을 변경합니다!\n");		//아이템 사용문구
					Sleep(500);
					reset_sub_main();
				}
				else
				{
					gotoxy(MAIN_X_ADJ + 1, MAIN_Y_ADJ + 2); printf("아이템이 없습니다!\n");		//아이템 모두 소진한 경우 경고
					Sleep(500);
					reset_sub_main();
				}
				break;
			case bombitem://폭탄 아이템생성(소문자 b)이 눌렸을 때
			case Bombitem://폭탄 아이템생성(대문자 B)이 눌렸을 때
				bombkey = 7;
				break;
			case ESC: //ESC눌렀을때 
				system("cls"); //화면을 지우고 
				exit(0); //게임종료 
			}
		}
	}
	while (_kbhit())
		_getch(); //키버퍼를 비움 
}

void drop_block(void) {
	int i, j;

	if (crush_on && check_crush(bx, by + 1, b_rotation) == true) crush_on = 0; //밑이 비어있으면 crush 상태 끔 
	if (crush_on && check_crush(bx, by + 1, b_rotation) == false) { //밑이 비어있지않고 crush 상태가 켜저있으면 
		for (i = 0; i < MAIN_Y; i++) { //현재 조작중인 블럭을 굳힘 
			for (j = 0; j < MAIN_X; j++) {
				if (main_stat[i][j] == ACTIVE_BLOCK) main_stat[i][j] = INACTIVE_BLOCK;
			}
		}
		crush_on = 0; //상태를 끔 
		check_line(); //라인체크를 함 
		new_block_on = 1; //새로운 블럭생성 상태를 켬    
		return; //함수 종료 
	}
	if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN); //밑이 비어있으면 밑으로 한칸 이동 
	if (check_crush(bx, by + 1, b_rotation) == false) crush_on++; //밑으로 이동이 안되면  crush 상태를 켬
}

int check_crush(int bx, int by, int b_rotation) { //지정된 좌표와 회전값으로 충돌이 있는지 검사 
	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) { //지정된 위치의 메인화면과 블럭모양을 비교해서 겹치면 false를 리턴 
			if (blocks[b_type][b_rotation][i][j] == 1 && main_stat[by + i][bx + j] > 0) return false;
		}
	}
	return true; //하나도 안겹치면 true리턴 
};

void move_block(int dir) { //블럭을 이동시킴 
	int i, j;

	switch (dir) {
	case LEFT: //왼쪽방향 
		for (i = 0; i < 4; i++) { //현재좌표의 블럭을 지움 
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_stat[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i < 4; i++) { //왼쪽으로 한칸가서 active block을 찍음 
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_stat[by + i][bx + j - 1] = ACTIVE_BLOCK;
			}
		}
		bx--; //좌표값 이동 
		break;

	case RIGHT:    //오른쪽 방향. 왼쪽방향이랑 같은 원리로 동작 
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_stat[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_stat[by + i][bx + j + 1] = ACTIVE_BLOCK;
			}
		}
		bx++;
		break;

	case DOWN:    //아래쪽 방향. 왼쪽방향이랑 같은 원리로 동작
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_stat[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_stat[by + i + 1][bx + j] = ACTIVE_BLOCK;
			}
		}
		by++;
		break;

	case UP: //키보드 위쪽 눌렀을때 회전시킴. 
		for (i = 0; i < 4; i++) { //현재좌표의 블럭을 지움  
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_stat[by + i][bx + j] = EMPTY;
			}
		}
		b_rotation = (b_rotation + 1) % 4; //회전값을 1증가시킴(3에서 4가 되는 경우는 0으로 되돌림) 
		for (i = 0; i < 4; i++) { //회전된 블럭을 찍음 
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_stat[by + i][bx + j] = ACTIVE_BLOCK;
			}
		}
		break;

	case 100: //블럭이 바닥, 혹은 다른 블럭과 닿은 상태에서 한칸위로 올려 회전이 가능한 경우 
			  //이를 동작시키는 특수동작 
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_stat[by + i][bx + j] = EMPTY;
			}
		}
		b_rotation = (b_rotation + 1) % 4;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_stat[by + i - 1][bx + j] = ACTIVE_BLOCK;
			}
		}
		by--;
		break;
	}
}

void check_line(void) {
	int i, j, k, l;

	int    block_amount; //한줄의 블럭갯수를 저장하는 변수 
	int combo = 0; //콤보갯수 저장하는 변수 지정및 초기화 

	for (i = MAIN_Y - 2; i > 3;) { //i=MAIN_Y-2 : 밑쪽벽의 윗칸부터,  i>3 : 천장(3)아래까지 검사 
		block_amount = 0; //블럭갯수 저장 변수 초기화 
		for (j = 1; j < MAIN_X - 1; j++) { //벽과 벽사이의 블럭갯루를 셈 
			if (main_stat[i][j] > 0) block_amount++;
		}
		if (block_amount == MAIN_X - 2) { //블럭이 가득 찬 경우 
			if (level_up_on == 0) { //레벨업상태가 아닌 경우에
				score += 100 * level; //점수추가 
				cnt++; //지운 줄 갯수 카운트 증가 
				combo++; //콤보수 증가  
			}
			for (k = i; k > 1; k--) { //윗줄을 한칸씩 모두 내림(윗줄이 천장이 아닌 경우에만) 
				for (l = 1; l < MAIN_X - 1; l++) {
					if (main_stat[k - 1][l] != CEILLING) main_stat[k][l] = main_stat[k - 1][l];
					if (main_stat[k - 1][l] == CEILLING) main_stat[k][l] = EMPTY;
					//윗줄이 천장인 경우에는 천장을 한칸 내리면 안되니까 빈칸을 넣음 
				}
			}
		}
		else i--;
	}
	if (combo) { //줄 삭제시 점수 갱신 및 남은점수 갱신 
		if (combo > 1) { //콤보달성 시 추가점수 및 보너스 점수 획득여부 메인화면에 뿌림
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 1, MAIN_Y_ADJ + by - 2); printf("%d COMBO!", combo);
			Sleep(500);
			score += (combo * level * 100);
			reset_sub_main(); //텍스트를 지우기 위해 sub_main을 초기화.
							  //(sub_main와 main_stat가 전부 다르므로 다음번 draw()호출시 메인화면 전체를 새로 그리게 됨) 
		}
		gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", (cnt <= 10) ? 10 - cnt : 0);
		gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score);
	}
}


void alert_game()
{
	int x = 5; int y = 5;
	for (int i = 1; i < MAIN_X - 2; i++) { //게임 일시정지 메세지 
		gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
		gotoxy(x, y + 1); printf("▤   +-----------------------+  ▤");
		gotoxy(x, y + 2); printf("▤  폭탄을 전부 소모하였습니다. ▤");
		gotoxy(x, y + 3); printf("▤     남은 폭탄이 없습니다.    ▤");
		gotoxy(x, y + 4); printf("▤   +-----------------------+  ▤");
		gotoxy(x, y + 5); printf("▤       재시작: 키보드 입력    ▤");
		gotoxy(x, y + 6); printf("▤                              ▤");
		gotoxy(x, y + 7); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
	}
	_getch(); //키입력시까지 대기 

	system("cls"); //화면 지우고 새로 그림 
	reset_sub_main();
	draw_main();
	draw_map();

}

void check_game_over(int rand_num) {

	int x = 5; int y = 5;

	for (int i = 1; i < MAIN_X - 2; i++) {
		if (main_stat[3][i] > 0) { //천장(위에서 세번째 줄)에 inactive가 생성되면 게임 오버 
			gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤"); //게임오버 메세지 
			gotoxy(x, y + 1); printf("▤                              ▤");
			gotoxy(x, y + 2); printf("▤   +-----------------------+  ▤");
			gotoxy(x, y + 3); printf("▤   |  G A M E  O V E R..   |  ▤");
			gotoxy(x, y + 4); printf("▤   +-----------------------+  ▤");
			gotoxy(x, y + 5); printf("▤   당신의 점수: %6d        ▤", score);
			gotoxy(x, y + 6); printf("▤                              ▤");
			gotoxy(x, y + 7); printf("▤   키보드 입력: 게임 재시작   ▤");
			gotoxy(x, y + 8); printf("▤                              ▤");
			gotoxy(x, y + 9); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
			last_score = score; //게임점수를 옮김 

			if (score > best_score) { //최고기록 갱신시 
				FILE* file = fopen("score.dat", "wt"); //score.dat에 점수 저장                

				gotoxy(x, y + 6); printf("▤  ★★★ 최고점달성! ★★★   ▤  ");


				if (file == 0) { //파일 에러메세지  
					gotoxy(0, 0);
					printf("FILE ERROR: SYSTEM CANNOT WRITE BEST SCORE ON \"SCORE.DAT\"");
				}
				else {
					fprintf(file, "%d", score);
					fclose(file);
				}
			}
			Sleep(1000);
			while (_kbhit()) _getch();
			key = _getch();
			reset(rand_num);
		}
	}
}

void pause(void) { //게임 일시정지 함수 
	int i, j;

	int x = 5;
	int y = 5;

	for (i = 1; i < MAIN_X - 2; i++) { //게임 일시정지 메세지 
		gotoxy(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
		gotoxy(x, y + 1); printf("▤                              ▤");
		gotoxy(x, y + 2); printf("▤   +-----------------------+  ▤");
		gotoxy(x, y + 3); printf("▤   |      일 시 정 지      |  ▤");
		gotoxy(x, y + 4); printf("▤   +-----------------------+  ▤");
		gotoxy(x, y + 5); printf("▤       재시작: 키보드 입력    ▤");
		gotoxy(x, y + 6); printf("▤                              ▤");
		gotoxy(x, y + 7); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
	}
	_getch(); //키입력시까지 대기 

	system("cls"); //화면 지우고 새로 그림 
	reset_sub_main();
	draw_main();
	draw_map();

	for (i = 1; i < 3; i++) { // 다음블럭 그림 
		for (j = 0; j < 4; j++) {
			if (blocks[b_type_next][0][i][j] == 1) {
				gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
				printf("■");
			}
			else {
				gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
				printf("  ");
			}
		}
	}
}

void check_level_up(void) {
	int i, j;

	if (cnt >= 10) { //레벨별로 10줄씩 없애야함. 10줄이상 없앤 경우 
		draw_main();
		level_up_on = 1; //레벨업 상태를 띄움 
		level += 1; //레벨을 1 올림 
		cnt = 0; //지운 줄 수 초기화   

		for (i = 0; i < 4; i++) {
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
			printf("             ");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
			printf("             ");
			Sleep(200);

			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
			printf("☆LEVEL UP!☆");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
			printf("☆SPEED UP!☆");
			Sleep(200);
		}
		change_count += 3; //블록변경찬스 보충
		reset_sub_main(); //텍스트를 지우기 위해 sub_main을 초기화.
						  //(sub_main와 main_stat가 전부 다르므로 다음번 draw()호출시 메인화면 전체를 새로 그리게 됨) 

		for (i = MAIN_Y - 2; i > MAIN_Y - 2 - (level - 1); i--) { //레벨업보상으로 각 레벨-1의 수만큼 아랫쪽 줄을 지워줌 
			for (j = 1; j < MAIN_X - 1; j++) {
				main_stat[i][j] = INACTIVE_BLOCK; // 줄을 블럭으로 모두 채우고 
				gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i); // 별을 찍어줌... 이뻐보이게 
				printf("★");
				Sleep(20);
			}
		}
		Sleep(100); //별찍은거 보여주기 위해 delay 
		check_line(); //블럭으로 모두 채운것 지우기
					  //.check_line()함수 내부에서 level up 상태가 켜져있는 경우 점수는 없음.         
		switch (level) { //레벨별로 속도를 조절해줌. 
		case 2:
			speed = 50;
			break;
		case 3:
			speed = 25;
			break;
		case 4:
			speed = 10;
			break;
		case 5:
			speed = 5;
			break;
		case 6:
			speed = 4;
			break;
		case 7:
			speed = 3;
			break;
		case 8:
			speed = 2;
			break;
		case 9:
			speed = 1;
			break;
		case 10:
			speed = 0;
			break;
		}
		level_up_on = 0; //레벨업 상태꺼줌

		gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL); printf(" LEVEL : %5d", level); //레벨표시 
		gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", 10 - cnt); // 레벨목표 표시 
	}
}

void LQ_CreateQueue(LinkedQueue** Queue)
{
	(*Queue) = (LinkedQueue*)malloc(sizeof(LinkedQueue));
	(*Queue)->Front = NULL;
	(*Queue)->Rear = NULL;
	(*Queue)->count = 0;
}

Node* LQ_CreateNode(int NewData)
{
	Node* NewNode = (Node*)malloc(sizeof(Node));

	/*== 우선순위 ==*/
	if (NewData == 7)  // 인자값을 7으로 받았다면 아이템블럭으로 취급하여 우선순위를 ItemBlock(1)값으로 설정  
		NewNode->Priority = ItemBlock;
	else  //  인자값을 7로 받지 않았다면 아이템블럭으로 취급하여 우선순위를 NormalBlock(0)값으로 설정
		NewNode->Priority = NormalBlock;

	NewNode->rand_value = NewData;
	NewNode->NextNode = NULL;

	return NewNode;
}

void LQ_DestroyNode(Node* _Node)
{
	free(_Node);
}

void LQ_Enqueue(LinkedQueue* Queue, Node* NewNode)
{
	if (Queue->Front == NULL)
	{
		Queue->Front = NewNode;
		Queue->Rear = NewNode;
		Queue->count++;
	}
	else
	{
		Queue->Rear->NextNode = NewNode;
		Queue->Rear = NewNode;
		Queue->count++;
	}
}

Node* LQ_Dequeue(LinkedQueue* Queue)
{
	Node* Front = Queue->Front;

	if (Queue->Front->NextNode == NULL)
	{
		Queue->Front = NULL;
		Queue->Rear = NULL;
	}
	else
	{
		Queue->Front = Queue->Front->NextNode;
	}

	Queue->count--;

	return Front;
}

int LQ_IsEmpty(LinkedQueue* Queue)
{
	return (Queue->Front == NULL);
}

void make_Node(LinkedQueue** Queue)
{
	int i;

	for (i = 0; i < 5; i++)
	{
		LQ_Enqueue(*Queue, LQ_CreateNode(rand() % 7));		//난수를 노드의 data값으로 저장 및 큐에 삽입
	}
}

void check_Queue_count(LinkedQueue* Queue)
{
	if (Queue->Rear->NextNode == NULL)
	{
		make_Node(&Queue);
	}
}
