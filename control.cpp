#include <Windows.h>
#include <stdlib.h>
#include <string>
#include <conio.h>
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

#define BOARD_SIZE 12 //Kích thước ma trận bàn cờ
#define LEFT 3 //Tọa độ trái màn hình bàn cờ
#define TOP 1 //Tọa độ trên màn hình bàn cờ
#define OPTION_HIGH 2
#define OPTION_WIDTH  20


extern struct _POINT { int x, y, c; }; //x: tọa độ dòng, y: tọa độ cột, c: đánh dấu
extern _POINT _A[BOARD_SIZE][BOARD_SIZE]; //Ma trận bàn cờ
extern bool _TURN; //true là lượt người thứ nhất và false là lượt người thứ hai
extern int _COMMAND; //Biến nhận giá trị phím người dùng nhập
extern int _X, _Y; //Tọa độ hiện hành trên màn hình bàn cờ
extern struct MENU
{
	string opt1;
	string opt2;
	string opt3;
	string opt4;
};
extern MENU menu;
extern int _OPTION;
extern int Total_game;
extern int Score1;
extern int Score2;
extern string Player1_name;
extern string Player2_name;

void ResetGame();

//Hàm View
int ProcessFinish(int pWhoWin);
void AskContinue();
void GotoXY(int x, int y);
void SetColor(int backgound_color, int text_color);
void HighLight(int x, int y, int w, int h, int color);
void DrawBox(int x, int y, int w, int h);
void DrawOption(int x, int y, int w, int h, int b_color, int t_color, string s);
void DrawMenu(int x, int y, int w, int h, MENU m);
void Draw_newgame_opt(int x, int y, int w, int h);
void Hightlight_Play_turn(int x, int y, int w, int h, int color, int player);
void DrawBoard(int pSize);
void DrawTurn(int x, int y, int w, int h);
void PrintText(string text, int color, int x, int y);
void DrawLoaded(_POINT _A[][BOARD_SIZE]);
//Hàm Control
void ExitGame();
void MoveRight();
void MoveLeft();
void MoveUp();
void MoveDown();
void MenuUp();
void MenuDown();
//Hàm Model
int CheckTick(int pX, int pY);
int CheckBoard(int pX, int pY);
int TestBoard();
void ResetData();
int Bot(int _X, int _Y, int& pX, int& pY);
void SaveData(string filename);
void LoadData(string filename);
vector<string> LoadFiles();
bool CheckFileExistence(string filename);
/*Hàm dọn dẹp tài nguyên*/
void StartGame()
{
	system("cls");
	ResetGame();
	DrawBox(55, 19, 60, 8);
	DrawTurn(55, _A[0][BOARD_SIZE - 1].y, 60, 12);
	DrawBoard(BOARD_SIZE);
	DrawOption(_A[0][0].x - 2, _A[BOARD_SIZE - 1][BOARD_SIZE - 1].y + 2, 10, 2, 15, 0, "M:MENU");
	DrawOption(_A[0][BOARD_SIZE - 1].x - 12, _A[BOARD_SIZE - 1][BOARD_SIZE - 1].y + 2, 14, 2, 15, 0, "L:SAVE GAME");
}

/*Hàm thoát game*/
void ExitGame()
{
	system("cls");
}

/*Hàm di chuyển sang phải*/
void MoveRight()
{
	if (_X < _A[BOARD_SIZE - 1][BOARD_SIZE - 1].x)
	{
		_X += 4;
		GotoXY(_X, _Y);
	}
}

/*Hàm di chuyển sang trái*/
void MoveLeft()
{
	if (_X > _A[0][0].x)
	{
		_X -= 4;
		GotoXY(_X, _Y);
	}
}

/*Hàm di chuyển xuống*/
void MoveDown()
{
	if (_Y < _A[BOARD_SIZE - 1][BOARD_SIZE - 1].y)
	{
		_Y += 2;
		GotoXY(_X, _Y);
	}
}

/*Hàm di chuyển lên*/
void MoveUp()
{
	if (_Y > _A[0][0].y)
	{
		_Y -= 2;
		GotoXY(_X, _Y);
	}
}

/*Hàm di chuyển lên trong menu*/
void MenuUp(int& o)
{
	if (o != 1)
	{
		_Y -= (OPTION_HIGH + 1);
		GotoXY(_X, _Y);
		o--;
	}
}

/*Hàm di chuyển xuống trong menu*/
void MenuDown(int& o, int n)
{
	if (o != n)
	{
		_Y += (OPTION_HIGH + 1);
		GotoXY(_X, _Y);
		o++;
	}
}

void SaveGame() {
	string filename;

	system("cls");
	system("color F0");
	do {
		PrintText("Nhap file name ban muon luu: ", 15, 40, 15);
		getline(cin, filename);
		filename += ".txt";
		if (!CheckFileExistence(filename)) {
			break;
		}
		else {
			PrintText("Nhap lai ten khac: ", 15, 40, 17);
		}
	} while (1);
	
	SaveData(filename);

	ofstream savedfile;
	
		savedfile.open("gamelist.txt", ios::app);
		savedfile << filename << endl;
		savedfile.close();
}

void LoadGame(string filename) {
	system("cls");
	system("color F0");

	LoadData(filename);
	DrawBoard(BOARD_SIZE);
	DrawBox(55, 19, 60, 8);
	DrawTurn(55, _A[0][BOARD_SIZE - 1].y, 60, 12);
	DrawBoard(BOARD_SIZE);
	DrawOption(_A[0][0].x - 2, _A[BOARD_SIZE - 1][BOARD_SIZE - 1].y + 2, 10, 2, 15, 0, "M:MENU");
	DrawOption(_A[0][BOARD_SIZE - 1].x - 12, _A[BOARD_SIZE - 1][BOARD_SIZE - 1].y + 2, 14, 2, 15, 0, "L:SAVE GAME");
	GotoXY(_X, _Y);
}

//GamePlay của PvP
void PlayPvP()
{
	bool validEnter = true;
	while (1)
	{
		_COMMAND = toupper(_getch());
		if (_COMMAND == 'M')
		{
			ResetData();
			return;
		}
		else
		{
			if (_COMMAND == 'A')
			{
				HighLight(_X - 1, _Y, 3, 1, 15);
				GotoXY(_X, _Y);
				if (CheckTick(_X, _Y) == -1)
				{
					SetColor(15, 0);
					cout << "X";
				}
				else if (CheckTick(_X, _Y) == 1)
				{
					SetColor(15, 0);
					cout << "O";
				}
				GotoXY(_X, _Y);
				MoveLeft();
				if (CheckTick(_X, _Y) == 0)
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
				}
				else if (CheckTick(_X, _Y) == -1)
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
					SetColor(14, 0);
					cout << "X";
					GotoXY(_X, _Y);
				}
				else
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
					SetColor(14, 0);
					cout << "O";
					GotoXY(_X, _Y);
				}
			}
			else if (_COMMAND == 'W')
			{
				HighLight(_X - 1, _Y, 3, 1, 15);
				GotoXY(_X, _Y);
				if (CheckTick(_X, _Y) == -1)
				{
					SetColor(15, 0);
					cout << "X";
				}
				else if (CheckTick(_X, _Y) == 1)
				{
					SetColor(15, 0);
					cout << "O";
				}
				GotoXY(_X, _Y);
				MoveUp();
				if (CheckTick(_X, _Y) == 0)
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
				}
				else if (CheckTick(_X, _Y) == -1)
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
					SetColor(14, 0);
					cout << "X";
					GotoXY(_X, _Y);
				}
				else
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
					SetColor(14, 0);
					cout << "O";
					GotoXY(_X, _Y);
				}
			}
			else if (_COMMAND == 'S')
			{
				HighLight(_X - 1, _Y, 3, 1, 15);
				GotoXY(_X, _Y);
				if (CheckTick(_X, _Y) == -1)
				{
					SetColor(15, 0);
					cout << "X";
				}
				else if (CheckTick(_X, _Y) == 1)
				{
					SetColor(15, 0);
					cout << "O";
				}
				GotoXY(_X, _Y);
				MoveDown();
				if (CheckTick(_X, _Y) == 0)
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
				}
				else if (CheckTick(_X, _Y) == -1)
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
					SetColor(14, 0);
					cout << "X";
					GotoXY(_X, _Y);
				}
				else
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
					SetColor(14, 0);
					cout << "O";
					GotoXY(_X, _Y);
				}
			}
			else if (_COMMAND == 'D')
			{
				HighLight(_X - 1, _Y, 3, 1, 15);
				GotoXY(_X, _Y);
				if (CheckTick(_X, _Y) == -1)
				{
					SetColor(15, 0);
					cout << "X";
				}
				else if (CheckTick(_X, _Y) == 1)
				{
					SetColor(15, 0);
					cout << "O";
				}
				GotoXY(_X, _Y);
				MoveRight();
				if (CheckTick(_X, _Y) == 0)
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
				}
				else if (CheckTick(_X, _Y) == -1)
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
					SetColor(14, 0);
					cout << "X";
					GotoXY(_X, _Y);
				}
				else
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
					SetColor(14, 0);
					cout << "O";
					GotoXY(_X, _Y);
				}
			}
			else if (_COMMAND == 13) //Nhấn Enter
			{
				switch (CheckBoard(_X, _Y))
				{
				case -1:
					HighLight(_X - 1, _Y, 3, 1, 15);
					GotoXY(_X, _Y);
					SetColor(14, 0);
					cout << "X";
					break;
				case 1:
					HighLight(_X - 1, _Y, 3, 1, 15);
					GotoXY(_X, _Y);
					SetColor(14, 0);
					cout << "O";
					break;
				case 0:
					validEnter = false; //Khi đánh vào ô đã đánh rồi
				}
				//Kiểm tra và xử lý thắng thua
				if (validEnter == true)
				{
					switch (ProcessFinish(TestBoard()))
					{
					case 2:
					{
						if (_TURN == true)
						{
							Hightlight_Play_turn(55, _A[0][BOARD_SIZE - 1].y, 60, 12, 15, 2);
							Hightlight_Play_turn(55, _A[0][BOARD_SIZE - 1].y, 60, 12, 14, 1);
						}
						else
						{
							Hightlight_Play_turn(55, _A[0][BOARD_SIZE - 1].y, 60, 12, 15, 1);
							Hightlight_Play_turn(55, _A[0][BOARD_SIZE - 1].y, 60, 12, 14, 2);
						}
						break;
					}
					case -1:
					case 1:
					case 0:
						AskContinue();
						while (1)
						{
							int temp = toupper(_getch());
							if (temp == 'N')
							{
								return;
							}
							else if (temp == 'Y')
							{
								SetColor(15, 0);
								StartGame();
								break;
							}
						}
					}
				}
				validEnter = true; //Mở khóa
			}
			else if (_COMMAND = 'L') {
				SaveGame();
				return;
			}
		}
	}
}

//GamePlay của PvC
void PlayPvC()
{
	int count = -1;
	bool validEnter = true;
	while (1)
	{
		_COMMAND = toupper(_getch());
		if (_COMMAND == 'M')
		{
			ResetData();
			return;
		}
		else
		{
			if (_COMMAND == 27)
			{
				SetColor(15, 0);
				ExitGame();
				return;
			}
			else if (_COMMAND == 'A')
			{
				HighLight(_X - 1, _Y, 3, 1, 15);
				GotoXY(_X, _Y);
				if (CheckTick(_X, _Y) == -1)
				{
					SetColor(15, 0);
					cout << "X";
				}
				else if (CheckTick(_X, _Y) == 1)
				{
					SetColor(15, 0);
					cout << "O";
				}
				GotoXY(_X, _Y);
				MoveLeft();
				if (CheckTick(_X, _Y) == 0)
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
				}
				else if (CheckTick(_X, _Y) == -1)
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
					SetColor(14, 0);
					cout << "X";
					GotoXY(_X, _Y);
				}
				else
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
					SetColor(14, 0);
					cout << "O";
					GotoXY(_X, _Y);
				}
			}
			else if (_COMMAND == 'W')
			{
				HighLight(_X - 1, _Y, 3, 1, 15);
				GotoXY(_X, _Y);
				if (CheckTick(_X, _Y) == -1)
				{
					SetColor(15, 0);
					cout << "X";
				}
				else if (CheckTick(_X, _Y) == 1)
				{
					SetColor(15, 0);
					cout << "O";
				}
				GotoXY(_X, _Y);
				MoveUp();
				if (CheckTick(_X, _Y) == 0)
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
				}
				else if (CheckTick(_X, _Y) == -1)
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
					SetColor(14, 0);
					cout << "X";
					GotoXY(_X, _Y);
				}
				else
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
					SetColor(14, 0);
					cout << "O";
					GotoXY(_X, _Y);
				}
			}
			else if (_COMMAND == 'S')
			{
				HighLight(_X - 1, _Y, 3, 1, 15);
				GotoXY(_X, _Y);
				if (CheckTick(_X, _Y) == -1)
				{
					SetColor(15, 0);
					cout << "X";
				}
				else if (CheckTick(_X, _Y) == 1)
				{
					SetColor(15, 0);
					cout << "O";
				}
				GotoXY(_X, _Y);
				MoveDown();
				if (CheckTick(_X, _Y) == 0)
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
				}
				else if (CheckTick(_X, _Y) == -1)
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
					SetColor(14, 0);
					cout << "X";
					GotoXY(_X, _Y);
				}
				else
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
					SetColor(14, 0);
					cout << "O";
					GotoXY(_X, _Y);
				}
			}
			else if (_COMMAND == 'D')
			{
				HighLight(_X - 1, _Y, 3, 1, 15);
				GotoXY(_X, _Y);
				if (CheckTick(_X, _Y) == -1)
				{
					SetColor(15, 0);
					cout << "X";
				}
				else if (CheckTick(_X, _Y) == 1)
				{
					SetColor(15, 0);
					cout << "O";
				}
				GotoXY(_X, _Y);
				MoveRight();
				if (CheckTick(_X, _Y) == 0)
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
				}
				else if (CheckTick(_X, _Y) == -1)
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
					SetColor(14, 0);
					cout << "X";
					GotoXY(_X, _Y);
				}
				else
				{
					HighLight(_X - 1, _Y, 3, 1, 14);
					GotoXY(_X, _Y);
					SetColor(14, 0);
					cout << "O";
					GotoXY(_X, _Y);
				}
			}
			else if (_COMMAND == 13) //Nhấn Enter
			{
				switch (CheckBoard(_X, _Y))
				{
				case -1:
					HighLight(_X - 1, _Y, 3, 1, 15);
					GotoXY(_X, _Y);
					SetColor(14, 0);
					cout << "X";
					break;
				case 0:
					validEnter = false; //Khi đánh vào ô đã đánh rồi
				}
				//Kiểm tra và xử lý thắng thua
				if (validEnter == true)
				{
					switch (ProcessFinish(TestBoard()))
					{
					case 2:
					{
						if (_TURN == true)
						{
							Hightlight_Play_turn(55, _A[0][BOARD_SIZE - 1].y, 60, 12, 15, 2);
							Hightlight_Play_turn(55, _A[0][BOARD_SIZE - 1].y, 60, 12, 14, 1);
						}
						else
						{
							Hightlight_Play_turn(55, _A[0][BOARD_SIZE - 1].y, 60, 12, 15, 1);
							Hightlight_Play_turn(55, _A[0][BOARD_SIZE - 1].y, 60, 12, 14, 2);
						}
						break;
					}
					case -1:
					case 1:
					case 0:
						AskContinue();
						while (1)
						{
							int temp = toupper(_getch());
							if (temp == 'N')
							{
								return;
							}
							else if (temp == 'Y')
							{
								SetColor(15, 0);
								StartGame();
								count = 1; //Gán giá trị xác nhận đã chơi lại
								break;
							}
						}
					}
					if (count != 1)
					{

						//Lượt của bot
						int pX, pY;
						Bot(_X, _Y, pX, pY);
						Sleep(500);
						Hightlight_Play_turn(55, _A[0][BOARD_SIZE - 1].y, 60, 12, 14, 1);
						Hightlight_Play_turn(55, _A[0][BOARD_SIZE - 1].y, 60, 12, 15, 2);
						GotoXY(_X, _Y);
						SetColor(15, 0);
						cout << "X";
						GotoXY(_A[pX][pY].x, _A[pX][pY].y);
						SetColor(14, 0);
						cout << "O";
						Sleep(600);
						GotoXY(_A[pX][pY].x, _A[pX][pY].y);
						SetColor(15, 0);
						cout << "O";
						GotoXY(_X, _Y);
						SetColor(14, 0);
						cout << "X";

						if (count == -2)
							count = 0;

						//Kiểm tra bot có thắng không
						switch (ProcessFinish(TestBoard()))
						{
						case -1:
						case 1:
						case 0:
							AskContinue();
							while (1)
							{
								int temp = toupper(_getch());
								if (temp == 'N')
								{
									return;
								}
								else if (temp == 'Y')
								{
									SetColor(15, 0);
									StartGame();
									count = -2; //Gán giá trị xác nhận máy win và đã chơi lại
									break;
								}
							}
						}
					}
					else
						count = 0;
				}
				validEnter = true; //Mở khóa
			}
		}
	}
}


