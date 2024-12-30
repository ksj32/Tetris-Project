#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>


//Ű���尪�� SetConsoleCursorPosition�� ����� �ƽ�Ű��
#define LEFT 75 //�·� �̵�    
#define RIGHT 77 //��� �̵� 
#define UP 72 //ȸ�� 
#define DOWN 80 //soft drop
#define SPACE 32 //hard drop
#define p 112 //�Ͻ����� 
#define P 80 //�Ͻ�����
#define item 105 //�����ۻ���
#define Item 73 //�����ۻ���
#define bombitem 98 //��ź�����ۻ��� 
#define Bombitem 66 //��ź�����ۻ���
#define ESC 27 //�������� 

#define false 0
#define true 1
#define SPECIAL 999 //��ź������

#define ACTIVE_BLOCK -2 // ����ȭ��迭�� ����� ���� ���µ� 
#define CEILLING -1     // ���� �̵��� �� �ִ� ������ 0 �Ǵ� ���� ������ ǥ�� 
#define EMPTY 0         // ���� �̵��� �� ���� ������ ����� ǥ�� 
#define WALL 1
#define INACTIVE_BLOCK 2 // �̵��� �Ϸ�� ���� 

#define MAIN_X 11 //����ȭ�� ����ũ�� 
#define MAIN_Y 23 //����ȭ�� ����ũ�� 
#define MAIN_X_ADJ 3 //����ȭ�� ��ġ���� 
#define MAIN_Y_ADJ 1 //����ȭ�� ��ġ���� 

#define NormalBlock 0 // �Ϲݺ��� �켱���� 
#define ItemBlock 1 // �����ۺ��� �켱����

#define STATUS_X_ADJ MAIN_X_ADJ+MAIN_X+1 //��������ǥ�� ��ġ���� 

int STATUS_Y_GOAL; //GOAL ����ǥ����ġY ��ǥ ���� 
int STATUS_Y_LEVEL; //LEVEL ����ǥ����ġY ��ǥ ����
int STATUS_Y_SCORE; //SCORE ����ǥ����ġY ��ǥ ����

int blocks[7][4][4][4] = {

	/*type0*/{ { 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0 } },
	/*type1*/{ { 0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0 },{ 0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0 },{ 0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0 },{ 0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0 } },
	/*type2*/{ { 0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0 },{ 0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0 },{ 0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0 } },
	/*type3*/{ { 0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0 },{ 0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0 },{ 0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0 },{ 0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0 } },
	/*type4*/{ { 0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0 },{ 0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0 } },
	/*type5*/{ { 0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0 },{ 0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0 },{ 0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0 } },
	/*type6*/{ { 0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0 },{ 0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0 },{ 0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0 },{ 0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0 } }

}; //����� ���� 4*4������ ���� ǥ�� blcoks[b_type][b_rotation][i][j]�� ��� 

int b_type; //�� ������ ���� 
int b_rotation; //�� ȸ���� ���� 
int b_type_next; //���� ���� ���� 

int main_stat[MAIN_Y][MAIN_X]; //����ȭ���� ������ �����ϴ� �迭 ����Ϳ� ǥ���Ŀ� sub_main�� ����� 
int sub_main[MAIN_Y][MAIN_X]; //�� maincpy�� ����ȭ���� ����Ϳ� ǥ�õǱ� ���� ������ ������ ���� 
							  //main�� ��ü�� ��� ����Ϳ� ǥ������ �ʰ�(�̷��� �ϸ� ����Ͱ� �����Ÿ�) 
							  //sub_main�� �迭�� ���ؼ� ���� �޶��� ���� ����Ϳ� ��ħ 

int bx, by; //�̵����� ���� ����ȭ����� x,y��ǥ�� ���� 

int key; //Ű����� �Է¹��� Ű���� ���� 
int bombkey; //Ű����� �Է¹��� ��ź������ Ű�� 
int bomb_count = 3;

int speed; //��������ӵ� 
int level; //���� level 
int level_goal; //���������� �Ѿ�� ���� ��ǥ���� 
int cnt; //���� �������� ������ �� ���� ���� 
int score; //���� ���� 
int last_score = 0; //�������������� 
int best_score = 0; //��ŷ�ְ���

int change_count = 5;	//��� ��ȯ ������ Ƚ�� üũ

int new_block_on = 0; //���ο� ���� �ʿ����� �˸��� ���� 
int crush_on = 0; //���� �̵����� ���� �浹�������� �˷��ִ� ���� 
int level_up_on = 0; //���������Ϸ���¸� �˸��� ���� 
int space_key_on = 0; //hard drop�������� �˷��ִ� ����

void set_title(void); //���ӽ���ȭ�� 
void reset(int rand_num); //����ȭ�� �ʱ�ȭ 
void reset_main(void); //���� ����ȭ��(main_stat[][]�� �ʱ�ȭ)
void reset_sub_main(void); //copy ����ȭ��(sub_main[][]�� �ʱ�ȭ)
void draw_map(void); //���� ��ü �������̽��� ǥ�� 
void draw_main(void); //����ȭ���� �׸� 
void new_block(int rand_num); //���ο� ���� �ϳ� ���� 
void check_key(void); //Ű����� Ű�� �Է¹��� 
void drop_block(void); //���� �Ʒ��� ����Ʈ�� 
int check_crush(int bx, int by, int rotation); //bx, by��ġ�� rotationȸ������ ���� ��� �浹 �Ǵ� 
void move_block(int dir); //dir�������� ���� ������ 
void check_line(void); //���� ����á������ �Ǵ��ϰ� ���� 
void check_level_up(void); //������ǥ�� �޼��Ǿ������� �Ǵ��ϰ� levelup��Ŵ 
void check_game_over(int rand_num); //���ӿ������� �Ǵ��ϰ� ���ӿ����� ���� 
void pause(void);//������ �Ͻ�������Ŵ 
void alert_game(void);

void gotoxy(int x, int y) //gotoxy�Լ�
{
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; //Ŀ������� �Լ��� ���Ǵ� ������ 
void setcursortype(CURSOR_TYPE c) //Ŀ������� �Լ� 
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

void LQ_CreateQueue(LinkedQueue** Queue);				//ť ���� �Լ�  
Node* LQ_CreateNode(int NewData);		       			//��� ���� �Լ� 
void LQ_DestroyNode(Node* _Node);						//��� ���� �Լ� 
void LQ_Enqueue(LinkedQueue* Queue, Node* NewNode);		//��� ���� �Լ� 
Node* LQ_Dequeue(LinkedQueue* Queue);					//��� ��� �Լ� 
int LQ_IsEmpty(LinkedQueue* Queue);						//ť ����˻� �Լ� 
void make_Node(LinkedQueue** Queue);					//ť�� ��� 5�� ���� �Լ�  
void check_Queue_count(LinkedQueue* Queue);				//ť�� ��尡 ���� ��� make_node�Լ� ȣ�� �Լ�  

LinkedQueue* Queue;			//ť ����

int main()
{
	int i;
	int rand_num;

	Node* Popped;				//�ӽ� Node�� ������ 
	LQ_CreateQueue(&Queue);		//ť ����  

	srand((unsigned)time(NULL)); //����ǥ����
	setcursortype(NOCURSOR); //Ŀ�� ���� 
	system("mode con:cols=88 lines=27");//�ܼ�â ȭ������
	system("title �켱���� ť�� Ȱ���� ��Ʈ����");//�ܼ�â Ÿ��Ʋ����

	set_title(); //����Ÿ��Ʋ ȣ�� 

	make_Node(&Queue);				// ��� ����   

	Popped = LQ_Dequeue(Queue);		//Popped �����Ͱ� ť���� ����� ��带 ����Ű�� ��  
	check_Queue_count(Queue);		//ť�� ��� emptyȮ��  
	rand_num = Popped->rand_value;	//����� �������� ����  
	reset(rand_num);				//�������� ȣ��� �������� �Լ��� ���ڷ� �� 
	LQ_DestroyNode(Popped);			//��� ����  

	while (1)
	{
		for (i = 0; i < 5; i++) //���� ��ĭ�������µ��� 5�� Ű�Է¹��� �� ���� 
		{
			check_key(); //Ű�Է�Ȯ�� 
			draw_main(); //ȭ���� �׸� 
			Sleep(speed); //���Ӽӵ����� 
			if (crush_on && check_crush(bx, by + 1, b_rotation) == false) Sleep(100);
			//���� �浹������ ��� �߰������� �ð��� ���Ե�
			if (space_key_on == 1) { //�����̽��ٸ� �������(hard drop) �߰��� �̵��� ȸ���Ҽ� ���� break; 
				space_key_on = 0;
				break;
			}
		}

		check_level_up(); // �������� üũ 
		check_game_over(rand_num); //���ӿ����� üũ 

		if (new_block_on == 1) // �� �� ���°� �ִ� ��� ���ο� �� ����
		{
			if (bomb_count == 0)
			{
				bomb_count = -1;
				alert_game();
			}
			//��ź������ ��� ����
			if (bombkey == 7 && bomb_count > 0)  //  B or b�� �Է����� ��쿡 
			{
				bomb_count--;
				bombkey = 0;  //  ������ �Ŀ��� ��źŰ���� �ʱ�ȭ  
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
			else  //�⺻ �� ����  
			{
				Popped = LQ_Dequeue(Queue);
				check_Queue_count(Queue);
			}

			rand_num = Popped->rand_value;
			new_block(rand_num);
			LQ_DestroyNode(Popped);
		}
		if (new_block_on == 2) { //��ź ������ ���� ����� �߻��� ���
								 //���ο� ��� ����
								 // 10.15 �켱�������ؼ� �켱������ ���� �������� ��������
								 // �켱���� ���ؼ� pop���� �����ϵ��� dequeue�κ��� �����ϸ� �� ��


		}
		drop_block(); // ���� ��ĭ ���� 
	}
}

void set_title(void) {
	int x = 8; //Ÿ��Ʋȭ���� ǥ�õǴ� x��ǥ 
	int y = 3; //Ÿ��Ʋȭ���� ǥ�õǴ� y��ǥ 
	int cnt; //Ÿ��Ʋ �������� ���� ����  

			 //�ڽ���ξ�
	gotoxy(x + 6, y); printf("������������������������������������������������������");


	gotoxy(x + 6, y + 4); printf("������������������������������������������������������");

	gotoxy(x + 8, y + 2); printf("     TETRIS GAME ");

	gotoxy(x + 7, y + 12); printf("PRESS ANY KEY TO PLAY GAME");




	for (cnt = 0;; cnt++) { //cnt�� 1�� ������Ű�鼭 ��� �ݺ�   
		gotoxy(x + 7, y + 2); printf("��");//Ÿ��Ʋ������(��)
		gotoxy(x + 18, y + 2); printf("��");//Ÿ��Ʋ������(��)



		Sleep(600);
		gotoxy(x + 7, y + 2); printf("��");//Ÿ��Ʋ������(��)
		gotoxy(x + 18, y + 2); printf("��");//Ÿ��Ʋ������(��)
		gotoxy(x + 20 + cnt, y + 12); printf(".");
		Sleep(600);
		if (cnt == 2) {

			gotoxy(x + 18, y + 12); printf("                ");
			cnt = -1;

		}
		if (_kbhit()) {
			break; //Ű�Է��� ������ ���ѷ��� ���� 
		}
	}

	while (_kbhit()) _getch(); //���ۿ� ��ϵ� Ű���� ���� 
}

void reset(int rand_num) {

	Node* Popped;
	int rand_num_temp;

	FILE* file = fopen("score.dat", "rt"); // score.dat������ ���� 
	if (file == 0) { best_score = 0; } //������ ������ �� �ְ������� 0�� ���� 
	else {
		fscanf(file, "%d", &best_score); // ������ ������ �ְ������� �ҷ��� 
		fclose(file); //���� ���� 
	}

	level = 1; //�������� �ʱ�ȭ 
	score = 0;
	level_goal = 1000;
	key = 0;
	crush_on = 0;
	cnt = 0;
	speed = 100;

	system("cls"); //ȭ������ 
	reset_main(); // main_stat�� �ʱ�ȭ 
	draw_map(); // ����ȭ���� �׸�
	draw_main(); // ����ȭ���� �׸� 

	change_count = 5; //��Ϻ��� ���� �ʱ�ȭ

	Popped = LQ_Dequeue(Queue);			//reset�� �߻��� �� ���������� ���  Dequeue�� b_tyoe_next�� ����  
	rand_num_temp = Popped->rand_value; //(���� ���� 2�� ������ �����ϱ� ����)  
	b_type_next = rand_num_temp; //�������� ���� �� ������ �����ϰ� ���� ---------------------------------------------------
	LQ_DestroyNode(Popped);
	new_block(rand_num); //���ο� ���� �ϳ� ����  

}

void reset_main(void) { //����ȭ���� �ʱ�ȭ  
	int i, j;

	for (i = 0; i < MAIN_Y; i++) { // ����ȭ���� 0���� �ʱ�ȭ  
		for (j = 0; j < MAIN_X; j++) {
			main_stat[i][j] = 0;
			sub_main[i][j] = 100;
		}
	}
	for (j = 1; j < MAIN_X; j++) { //y���� 3�� ��ġ�� õ���� ���� 
		main_stat[3][j] = CEILLING;
	}
	for (i = 1; i < MAIN_Y - 1; i++) { //�¿� ���� ����  
		main_stat[i][0] = WALL;
		main_stat[i][MAIN_X - 1] = WALL;
	}
	for (j = 0; j < MAIN_X; j++) { //�ٴں��� ���� 
		main_stat[MAIN_Y - 1][j] = WALL;
	}
}

void reset_sub_main(void) { //sub_main�� �ʱ�ȭ 
	int i, j;

	for (i = 0; i < MAIN_Y; i++) {         //����ȭ�鿡 ���ӿ� ������ �ʴ� ���ڸ� ���� 
		for (j = 0; j < MAIN_X; j++) {  //�̴� main_stat�� ���� ���ڰ� ���� �ϱ� ���� 
			sub_main[i][j] = 100;
		}
	}
}

void draw_map(void) { //���� ���� ǥ�ø� ��Ÿ���� �Լ�  
	int y = 3;             // level, goal, score�� �����߿� ���� �ٲ�� �� ���� �� y���� ���� �����ص� 
						   // �׷��� Ȥ�� ���� ���� ǥ�� ��ġ�� �ٲ� �� �Լ����� �ȹٲ㵵 �ǰ�.. 
	gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL = y - 1); printf(" ���� : %5d", level);
	gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL = y); printf(" ��ǥ  : %5d", 10 - cnt);
	gotoxy(STATUS_X_ADJ, y + 2); printf("+- ���� �� -+ ");
	gotoxy(STATUS_X_ADJ, y + 3); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 4); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 5); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 6); printf("|             | ");
	gotoxy(STATUS_X_ADJ, y + 7); printf("+-- -  -  - --+ ");
	gotoxy(STATUS_X_ADJ, y + 9); printf(" ���� ���� :");
	gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE = y + 10); printf("        %6d", score);
	gotoxy(STATUS_X_ADJ, y + 11); printf(" ���� ���� :");
	gotoxy(STATUS_X_ADJ, y + 12); printf("        %6d", last_score);
	gotoxy(STATUS_X_ADJ, y + 13); printf(" �ְ� ���� :");
	gotoxy(STATUS_X_ADJ, y + 14); printf("        %6d", best_score);
	gotoxy(STATUS_X_ADJ, y + 16); printf("  ��   : ������        SPACE : ��ó���");
	gotoxy(STATUS_X_ADJ, y + 17); printf("��  �� : ��/�� �̵�      P   : �Ͻ�����");
	gotoxy(STATUS_X_ADJ, y + 18); printf("  ��   : �ణ��������    ESC  : ����");
	gotoxy(STATUS_X_ADJ, y + 20); printf("  B   : ��ü ��ź        I  : �� ü����");
}

void draw_main(void) { //����ȭ�� �׸��� �Լ� 
	int i, j;

	for (j = 1; j < MAIN_X - 1; j++) { //õ���� ��� ���ο���� �������� �������� ���� �׷��� 
		if (main_stat[3][j] == EMPTY) main_stat[3][j] = CEILLING;
	}
	for (i = 0; i < MAIN_Y; i++) {
		for (j = 0; j < MAIN_X; j++) {
			if (sub_main[i][j] != main_stat[i][j]) { //cpy�� ���ؼ� ���� �޶��� �κи� ���� �׷���.

				gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
				switch (main_stat[i][j]) {
				case EMPTY: //��ĭ��� 
					printf("  ");
					break;
				case CEILLING: //õ���� 
					printf(". ");
					break;
				case WALL: //����� 
					printf("��");
					break;
				case INACTIVE_BLOCK: //���� �� ���  
					printf("��");
					break;
				case ACTIVE_BLOCK: //�����̰��ִ� �� ���  
					printf("��");
					break;
				case SPECIAL: //Ư�������۸��(��ź)
					printf("��");
					break;
				}
			}
		}
	}
	for (i = 0; i < MAIN_Y; i++) { //����ȭ���� �׸� �� sub_main�� ����  
		for (j = 0; j < MAIN_X; j++) {
			sub_main[i][j] = main_stat[i][j];
		}
	}
}

void new_block(int rand_num) { //���ο� �� ����  
	int i, j;

	bx = (MAIN_X / 2) - 1; //�� ���� ��ġx��ǥ(����ȭ���� ���) 
	by = 0;  //�� ������ġ y��ǥ(���� ��) 

	if (rand_num == 7) {
		new_block_on = 0; //new_block ���¸� ��  
		for (i = 0; i < 4; i++) { //����ȭ�� bx, by��ġ�� ������  
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_stat[by + i][bx + j] = SPECIAL;
			}
		}
	}
	else {
		b_type = b_type_next; //���������� ������ 
		b_type_next = rand_num; //���� ���� ���� 
		b_rotation = 0;  //ȸ���� 0������ ������ 

		new_block_on = 0; //new_block ���¸� ��  

		for (i = 0; i < 4; i++) { //����ȭ�� bx, by��ġ�� ������  
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_stat[by + i][bx + j] = ACTIVE_BLOCK;
			}
		}
	}

	for (i = 1; i <= 3; i++) { //���ӻ���ǥ�ÿ� ������ ���ú��� �׸� 
		for (j = 0; j < 4; j++) {
			if (blocks[b_type_next][0][i][j] == 1) {
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				printf("��");
			}
			else {
				gotoxy(STATUS_X_ADJ + 2 + j, i + 6);
				printf("  ");
			}
		}
	}
}

void check_key(void) {
	key = 0; //Ű�� �ʱ�ȭ  

	if (bomb_count == -1) {
		gotoxy(STATUS_X_ADJ, 24); printf("         %dȸ                   %dȸ", 0, change_count);		//���������� �� Ƚ���� �˱����� �� �Լ� ���� �ۼ�
	}
	else {
		gotoxy(STATUS_X_ADJ, 24); printf("         %dȸ                    %dȸ", bomb_count, change_count);		//���������� �� Ƚ���� �˱����� �� �Լ� ���� �ۼ�
	}
	if (_kbhit()) { //Ű�Է��� �ִ� ���  
		key = _getch(); //Ű���� ����
		if (key == 224) { //����Ű�ΰ�� 
			do { key = _getch(); } while (key == 224);//����Ű���ð��� ���� 
			switch (key) {
			case LEFT: //����Ű ��������  
				if (check_crush(bx - 1, by, b_rotation) == true) move_block(LEFT);
				break;                            //�������� �� �� �ִ��� üũ �� �����ϸ� �̵�
			case RIGHT: //������ ����Ű ��������- ���� �����ϰ� ó���� 
				if (check_crush(bx + 1, by, b_rotation) == true) move_block(RIGHT);
				break;
			case DOWN: //�Ʒ��� ����Ű ��������-���� �����ϰ� ó���� 
				if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN);
				break;
			case UP: //���� ����Ű �������� 
				if (check_crush(bx, by, (b_rotation + 1) % 4) == true) move_block(UP);
				//ȸ���� �� �ִ��� üũ �� �����ϸ� ȸ��
				else if (crush_on == 1 && check_crush(bx, by - 1, (b_rotation + 1) % 4) == true) move_block(100);
			}                    //�ٴڿ� ���� ��� �������� ��ĭ����� ȸ���� �����ϸ� �׷��� ��(Ư������)
		}
		else { //����Ű�� �ƴѰ�� 
			switch (key) {
			case SPACE: //�����̽�Ű �������� 
				space_key_on = 1; //�����̽�Ű ���¸� ��� 
				while (crush_on == 0) { //�ٴڿ� ���������� �̵���Ŵ 
					drop_block();
					score += level; // hard drop ���ʽ�
					gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score); //���� ǥ��  
				}
				break;
			case P: //P(�빮��) �������� 
			case p: //p(�ҹ���) �������� 
				pause(); //�Ͻ����� 
				break;
			case item://�����ۻ���(�빮�� I)�� ������ ��
			case Item://�����ۻ���(�ҹ��� i)�� ������ ��
				if (change_count >= 1)		//��� ��ȯ������ 5�� ��밡��
				{
					new_block_on = 1;
					//ȭ�� �� �ʵ����.
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
					gotoxy(MAIN_X_ADJ + 1, MAIN_Y_ADJ + 2); printf("���� �����մϴ�!\n");		//������ ��빮��
					Sleep(500);
					reset_sub_main();
				}
				else
				{
					gotoxy(MAIN_X_ADJ + 1, MAIN_Y_ADJ + 2); printf("�������� �����ϴ�!\n");		//������ ��� ������ ��� ���
					Sleep(500);
					reset_sub_main();
				}
				break;
			case bombitem://��ź �����ۻ���(�ҹ��� b)�� ������ ��
			case Bombitem://��ź �����ۻ���(�빮�� B)�� ������ ��
				bombkey = 7;
				break;
			case ESC: //ESC�������� 
				system("cls"); //ȭ���� ����� 
				exit(0); //�������� 
			}
		}
	}
	while (_kbhit())
		_getch(); //Ű���۸� ��� 
}

void drop_block(void) {
	int i, j;

	if (crush_on && check_crush(bx, by + 1, b_rotation) == true) crush_on = 0; //���� ��������� crush ���� �� 
	if (crush_on && check_crush(bx, by + 1, b_rotation) == false) { //���� ��������ʰ� crush ���°� ���������� 
		for (i = 0; i < MAIN_Y; i++) { //���� �������� ���� ���� 
			for (j = 0; j < MAIN_X; j++) {
				if (main_stat[i][j] == ACTIVE_BLOCK) main_stat[i][j] = INACTIVE_BLOCK;
			}
		}
		crush_on = 0; //���¸� �� 
		check_line(); //����üũ�� �� 
		new_block_on = 1; //���ο� ������ ���¸� ��    
		return; //�Լ� ���� 
	}
	if (check_crush(bx, by + 1, b_rotation) == true) move_block(DOWN); //���� ��������� ������ ��ĭ �̵� 
	if (check_crush(bx, by + 1, b_rotation) == false) crush_on++; //������ �̵��� �ȵǸ�  crush ���¸� ��
}

int check_crush(int bx, int by, int b_rotation) { //������ ��ǥ�� ȸ�������� �浹�� �ִ��� �˻� 
	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) { //������ ��ġ�� ����ȭ��� ������� ���ؼ� ��ġ�� false�� ���� 
			if (blocks[b_type][b_rotation][i][j] == 1 && main_stat[by + i][bx + j] > 0) return false;
		}
	}
	return true; //�ϳ��� �Ȱ�ġ�� true���� 
};

void move_block(int dir) { //���� �̵���Ŵ 
	int i, j;

	switch (dir) {
	case LEFT: //���ʹ��� 
		for (i = 0; i < 4; i++) { //������ǥ�� ���� ���� 
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_stat[by + i][bx + j] = EMPTY;
			}
		}
		for (i = 0; i < 4; i++) { //�������� ��ĭ���� active block�� ���� 
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_stat[by + i][bx + j - 1] = ACTIVE_BLOCK;
			}
		}
		bx--; //��ǥ�� �̵� 
		break;

	case RIGHT:    //������ ����. ���ʹ����̶� ���� ������ ���� 
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

	case DOWN:    //�Ʒ��� ����. ���ʹ����̶� ���� ������ ����
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

	case UP: //Ű���� ���� �������� ȸ����Ŵ. 
		for (i = 0; i < 4; i++) { //������ǥ�� ���� ����  
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_stat[by + i][bx + j] = EMPTY;
			}
		}
		b_rotation = (b_rotation + 1) % 4; //ȸ������ 1������Ŵ(3���� 4�� �Ǵ� ���� 0���� �ǵ���) 
		for (i = 0; i < 4; i++) { //ȸ���� ���� ���� 
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) main_stat[by + i][bx + j] = ACTIVE_BLOCK;
			}
		}
		break;

	case 100: //���� �ٴ�, Ȥ�� �ٸ� ���� ���� ���¿��� ��ĭ���� �÷� ȸ���� ������ ��� 
			  //�̸� ���۽�Ű�� Ư������ 
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

	int    block_amount; //������ �������� �����ϴ� ���� 
	int combo = 0; //�޺����� �����ϴ� ���� ������ �ʱ�ȭ 

	for (i = MAIN_Y - 2; i > 3;) { //i=MAIN_Y-2 : ���ʺ��� ��ĭ����,  i>3 : õ��(3)�Ʒ����� �˻� 
		block_amount = 0; //������ ���� ���� �ʱ�ȭ 
		for (j = 1; j < MAIN_X - 1; j++) { //���� �������� �����縦 �� 
			if (main_stat[i][j] > 0) block_amount++;
		}
		if (block_amount == MAIN_X - 2) { //���� ���� �� ��� 
			if (level_up_on == 0) { //���������°� �ƴ� ��쿡
				score += 100 * level; //�����߰� 
				cnt++; //���� �� ���� ī��Ʈ ���� 
				combo++; //�޺��� ����  
			}
			for (k = i; k > 1; k--) { //������ ��ĭ�� ��� ����(������ õ���� �ƴ� ��쿡��) 
				for (l = 1; l < MAIN_X - 1; l++) {
					if (main_stat[k - 1][l] != CEILLING) main_stat[k][l] = main_stat[k - 1][l];
					if (main_stat[k - 1][l] == CEILLING) main_stat[k][l] = EMPTY;
					//������ õ���� ��쿡�� õ���� ��ĭ ������ �ȵǴϱ� ��ĭ�� ���� 
				}
			}
		}
		else i--;
	}
	if (combo) { //�� ������ ���� ���� �� �������� ���� 
		if (combo > 1) { //�޺��޼� �� �߰����� �� ���ʽ� ���� ȹ�濩�� ����ȭ�鿡 �Ѹ�
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 1, MAIN_Y_ADJ + by - 2); printf("%d COMBO!", combo);
			Sleep(500);
			score += (combo * level * 100);
			reset_sub_main(); //�ؽ�Ʈ�� ����� ���� sub_main�� �ʱ�ȭ.
							  //(sub_main�� main_stat�� ���� �ٸ��Ƿ� ������ draw()ȣ��� ����ȭ�� ��ü�� ���� �׸��� ��) 
		}
		gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", (cnt <= 10) ? 10 - cnt : 0);
		gotoxy(STATUS_X_ADJ, STATUS_Y_SCORE); printf("        %6d", score);
	}
}


void alert_game()
{
	int x = 5; int y = 5;
	for (int i = 1; i < MAIN_X - 2; i++) { //���� �Ͻ����� �޼��� 
		gotoxy(x, y + 0); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
		gotoxy(x, y + 1); printf("��   +-----------------------+  ��");
		gotoxy(x, y + 2); printf("��  ��ź�� ���� �Ҹ��Ͽ����ϴ�. ��");
		gotoxy(x, y + 3); printf("��     ���� ��ź�� �����ϴ�.    ��");
		gotoxy(x, y + 4); printf("��   +-----------------------+  ��");
		gotoxy(x, y + 5); printf("��       �����: Ű���� �Է�    ��");
		gotoxy(x, y + 6); printf("��                              ��");
		gotoxy(x, y + 7); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
	}
	_getch(); //Ű�Է½ñ��� ��� 

	system("cls"); //ȭ�� ����� ���� �׸� 
	reset_sub_main();
	draw_main();
	draw_map();

}

void check_game_over(int rand_num) {

	int x = 5; int y = 5;

	for (int i = 1; i < MAIN_X - 2; i++) {
		if (main_stat[3][i] > 0) { //õ��(������ ����° ��)�� inactive�� �����Ǹ� ���� ���� 
			gotoxy(x, y + 0); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�"); //���ӿ��� �޼��� 
			gotoxy(x, y + 1); printf("��                              ��");
			gotoxy(x, y + 2); printf("��   +-----------------------+  ��");
			gotoxy(x, y + 3); printf("��   |  G A M E  O V E R..   |  ��");
			gotoxy(x, y + 4); printf("��   +-----------------------+  ��");
			gotoxy(x, y + 5); printf("��   ����� ����: %6d        ��", score);
			gotoxy(x, y + 6); printf("��                              ��");
			gotoxy(x, y + 7); printf("��   Ű���� �Է�: ���� �����   ��");
			gotoxy(x, y + 8); printf("��                              ��");
			gotoxy(x, y + 9); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
			last_score = score; //���������� �ű� 

			if (score > best_score) { //�ְ��� ���Ž� 
				FILE* file = fopen("score.dat", "wt"); //score.dat�� ���� ����                

				gotoxy(x, y + 6); printf("��  �ڡڡ� �ְ����޼�! �ڡڡ�   ��  ");


				if (file == 0) { //���� �����޼���  
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

void pause(void) { //���� �Ͻ����� �Լ� 
	int i, j;

	int x = 5;
	int y = 5;

	for (i = 1; i < MAIN_X - 2; i++) { //���� �Ͻ����� �޼��� 
		gotoxy(x, y + 0); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
		gotoxy(x, y + 1); printf("��                              ��");
		gotoxy(x, y + 2); printf("��   +-----------------------+  ��");
		gotoxy(x, y + 3); printf("��   |      �� �� �� ��      |  ��");
		gotoxy(x, y + 4); printf("��   +-----------------------+  ��");
		gotoxy(x, y + 5); printf("��       �����: Ű���� �Է�    ��");
		gotoxy(x, y + 6); printf("��                              ��");
		gotoxy(x, y + 7); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
	}
	_getch(); //Ű�Է½ñ��� ��� 

	system("cls"); //ȭ�� ����� ���� �׸� 
	reset_sub_main();
	draw_main();
	draw_map();

	for (i = 1; i < 3; i++) { // ������ �׸� 
		for (j = 0; j < 4; j++) {
			if (blocks[b_type_next][0][i][j] == 1) {
				gotoxy(MAIN_X + MAIN_X_ADJ + 3 + j, i + 6);
				printf("��");
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

	if (cnt >= 10) { //�������� 10�پ� ���־���. 10���̻� ���� ��� 
		draw_main();
		level_up_on = 1; //������ ���¸� ��� 
		level += 1; //������ 1 �ø� 
		cnt = 0; //���� �� �� �ʱ�ȭ   

		for (i = 0; i < 4; i++) {
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
			printf("             ");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
			printf("             ");
			Sleep(200);

			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 3, MAIN_Y_ADJ + 4);
			printf("��LEVEL UP!��");
			gotoxy(MAIN_X_ADJ + (MAIN_X / 2) - 2, MAIN_Y_ADJ + 6);
			printf("��SPEED UP!��");
			Sleep(200);
		}
		change_count += 3; //��Ϻ������� ����
		reset_sub_main(); //�ؽ�Ʈ�� ����� ���� sub_main�� �ʱ�ȭ.
						  //(sub_main�� main_stat�� ���� �ٸ��Ƿ� ������ draw()ȣ��� ����ȭ�� ��ü�� ���� �׸��� ��) 

		for (i = MAIN_Y - 2; i > MAIN_Y - 2 - (level - 1); i--) { //�������������� �� ����-1�� ����ŭ �Ʒ��� ���� ������ 
			for (j = 1; j < MAIN_X - 1; j++) {
				main_stat[i][j] = INACTIVE_BLOCK; // ���� ������ ��� ä��� 
				gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i); // ���� �����... �̻����̰� 
				printf("��");
				Sleep(20);
			}
		}
		Sleep(100); //�������� �����ֱ� ���� delay 
		check_line(); //������ ��� ä��� �����
					  //.check_line()�Լ� ���ο��� level up ���°� �����ִ� ��� ������ ����.         
		switch (level) { //�������� �ӵ��� ��������. 
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
		level_up_on = 0; //������ ���²���

		gotoxy(STATUS_X_ADJ, STATUS_Y_LEVEL); printf(" LEVEL : %5d", level); //����ǥ�� 
		gotoxy(STATUS_X_ADJ, STATUS_Y_GOAL); printf(" GOAL  : %5d", 10 - cnt); // ������ǥ ǥ�� 
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

	/*== �켱���� ==*/
	if (NewData == 7)  // ���ڰ��� 7���� �޾Ҵٸ� �����ۺ����� ����Ͽ� �켱������ ItemBlock(1)������ ����  
		NewNode->Priority = ItemBlock;
	else  //  ���ڰ��� 7�� ���� �ʾҴٸ� �����ۺ����� ����Ͽ� �켱������ NormalBlock(0)������ ����
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
		LQ_Enqueue(*Queue, LQ_CreateNode(rand() % 7));		//������ ����� data������ ���� �� ť�� ����
	}
}

void check_Queue_count(LinkedQueue* Queue)
{
	if (Queue->Rear->NextNode == NULL)
	{
		make_Node(&Queue);
	}
}
