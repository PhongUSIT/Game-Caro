#include <windows.h>
#include <iostream>
#include <conio.h>
#include <string>
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
extern int Score1;
extern int Score2;
extern string Player1_name;
extern string Player2_name;

/*Hàm cố định màn hình*/
void FixConsoleWindow()
{
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}

/*Hàm di chuyển con trỏ màn hình tới tọa độ (x,y)*/
void GotoXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/*0 = Black      8 = Gray
1 = Blue       9 = Light Blue
2 = Green      10 = Light Green
3 = Aqua       11 = Light Aqua
4 = Red        12 = Light Red
5 = Purple     13 = Light Purple
6 = Yellow     14 = Light Yellow
7 = White   15 = Bright White
*/
void SetColor(int backgound_color, int text_color)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	int color_code = backgound_color * 16 + text_color;
	SetConsoleTextAttribute(hStdout, color_code);
}

/*Hàm nhận phím quyết định có tiếp tục hay không*/
void AskContinue()
{
	GotoXY(60, 16);
	cout << "Nhan phim \"Y\" de tiep tuc hoac \"N\" de dung tro choi";
}

/*Hàm vẽ 1 cái box*/
void DrawBox(int x, int y, int w, int h)
{
	if (w <= 1 || h <= 1 || w % 2 != 0 || h % 2 != 0)
		return;
	GotoXY(x, y);
	cout << char(201);
	GotoXY(x, y + h);
	cout << char(200);
	for (int i = 1; i < w; i++)
	{
		GotoXY(x + i, y);
		cout << char(205);
		GotoXY(x + i, y + h);
		cout << char(205);
	}
	GotoXY(x + w, y);
	cout << char(187);
	GotoXY(x + w, y + h);
	cout << char(188);
	for (int j = 1; j < h; j++)
	{
		GotoXY(x, y + j);
		cout << char(186);
		GotoXY(x + w, y + j);
		cout << char(186);
	}
}

void TextBox(int x, int y, int w, int h, string s)
{
	int length = s.size();
	GotoXY(x + (w - length) / 2 + 1, y + h / 2);
	cout << s;
}

void HighLight(int x, int y, int w, int h, int color)
{
	SetColor(color, color);
	for (int i = x; i < x + w; i++)
		for (int j = y; j < y + h; j++)
		{
			GotoXY(i, j);
			cout << " ";
		}
	SetColor(15, 0);
}

void DrawOption(int x, int y, int w, int h, int b_color, int t_color, string s)
{
	SetColor(b_color, t_color);
	DrawBox(x, y, w, h);
	TextBox(x, y, w, h, s);
}

void printLogo(int x)
{
	SetColor(15, x);
	unsigned char logo[] = {
		32,219,219,219,219,219,219,187,32,219,219,219,219,219,219,187,32,32,32,32,32,32,32,219,219,219,219,219,219,187,32,219,219,219,219,219,187,32,219,219,219,219,219,219,187,32,32,219,219,219,219,219,219,187,32,
219,219,201,205,205,205,205,188,219,219,201,205,205,205,219,219,187,32,32,32,32,32,219,219,201,205,205,205,205,188,219,219,201,205,205,219,219,187,219,219,201,205,205,219,219,187,219,219,201,205,205,205,219,219,187,
219,219,186,32,32,32,32,32,219,219,186,32,32,32,219,219,186,32,32,32,32,32,219,219,186,32,32,32,32,32,219,219,219,219,219,219,219,186,219,219,219,219,219,219,201,188,219,219,186,32,32,32,219,219,186,
219,219,186,32,32,32,32,32,219,219,186,32,32,32,219,219,186,32,32,32,32,32,219,219,186,32,32,32,32,32,219,219,201,205,205,219,219,186,219,219,201,205,205,219,219,187,219,219,186,32,32,32,219,219,186,
200,219,219,219,219,219,219,187,200,219,219,219,219,219,219,201,188,32,32,32,32,32,200,219,219,219,219,219,219,187,219,219,186,32,32,219,219,186,219,219,186,32,32,219,219,186,200,219,219,219,219,219,219,201,188,
32,200,205,205,205,205,205,188,32,200,205,205,205,205,205,188,32,32,32,32,32,32,32,200,205,205,205,205,205,188,200,205,188,32,32,200,205,188,200,205,188,32,32,200,205,188,32,200,205,205,205,205,205,188,32 };
	int top = 4, left = 35;
	int num_lines = 6, num_chars = 55;
	for (int i = 0; i < num_lines; i++)
	{
		GotoXY(left, i + top);
		for (int j = 0; j < num_chars; j++)
			putchar(logo[i * num_chars + j]);
	}
}

void DrawMenu(int x, int y, int w, int h, MENU m)
{
	system("cls");
	/*printLogo(10);
	Sleep(1000);
	printLogo(11);
	Sleep(1000);
	printLogo(12);
	Sleep(1000);
	printLogo(13);
	Sleep(1000);
	printLogo(14);
	Sleep(1000);*/
	printLogo(0);
	HighLight(x, y, w, h, 14);
	DrawOption(x, y, w, h, 14, 0, m.opt1);
	DrawOption(x, y + (1 + h), w, h, 15, 0, m.opt2);
	DrawOption(x, y + 2 * (1 + h), w, h, 15, 0, m.opt3);
	DrawOption(x, y + 3 * (1 + h), w, h, 15, 0, m.opt4);
	_X = x; _Y = y;
	GotoXY(_X, _Y);
}

void Draw_newgame_opt(int x, int y, int w, int h)
{
	MENU m = { "PvP","PVC" };
	SetColor(15, 0);
	system("cls");
	HighLight(x, y, w, h, 14);
	DrawOption(x, y, w, h, 14, 0, m.opt1);
	DrawOption(x, y + (1 + h), w, h, 15, 0, m.opt2);
	DrawOption(8, 25, 10, 2, 15, 0, "B:BACK");
}

void DrawBoard(int pSize)
{
	for (int i = 0; i <= pSize; i++)
	{
		for (int j = 0; j <= pSize; j++)
		{
			if ((i == 0 && j == 0))
			{
				GotoXY(LEFT + 4 * i, TOP + 2 * j);
				cout << char(218) << char(196) << char(196) << char(196);
				GotoXY(LEFT + 4 * i, TOP + (2 * j) + 1);
				cout << char(179);
			}
			// duong duoi
			else if ((i == pSize && j == 0))
			{
				GotoXY(LEFT + 4 * i, TOP + 2 * j);
				cout << char(191);
				GotoXY(LEFT + 4 * i, TOP + (2 * j) + 1);
				cout << char(179);
			}
			// goc trai duoi
			else if ((i == 0 && j == pSize))
			{
				GotoXY(LEFT + 4 * i, TOP + 2 * j);
				cout << char(192) << char(196) << char(196) << char(196);
			}
			// goc phai duoi
			else if ((i == pSize && j == pSize))
			{
				GotoXY(LEFT + 4 * i, TOP + 2 * j);
				cout << char(217);
			}
			//duong tren
			else if ((i != 0 && j == 0) && (i != pSize && j == 0))
			{
				GotoXY(LEFT + 4 * i, TOP + 2 * j);
				cout << char(194) << char(196) << char(196) << char(196);
				GotoXY(LEFT + 4 * i, TOP + (2 * j) + 1);
				cout << char(179);
			}
			// duong duoi
			else if ((i != pSize && j == pSize) && (i != 0 && j == pSize))
			{
				GotoXY(LEFT + 4 * i, TOP + 2 * j);
				cout << char(193) << char(196) << char(196) << char(196);
			}
			// ben trai
			else if (i == 0 && j != pSize)
			{
				GotoXY(LEFT + 4 * i, TOP + 2 * j);
				cout << char(195) << char(196) << char(196) << char(196);
				GotoXY(LEFT + 4 * i, TOP + (2 * j) + 1);
				cout << char(179);
			}
			else if (i == pSize && j != pSize)
			{
				GotoXY(LEFT + 4 * i, TOP + 2 * j);
				cout << char(180);
				GotoXY(LEFT + 4 * i, TOP + (2 * j) + 1);
				cout << char(179);
			}
			else
			{
				GotoXY(LEFT + 4 * i, TOP + 2 * j);
				cout << char(197) << char(196) << char(196) << char(196);
				GotoXY(LEFT + 4 * i, TOP + (2 * j) + 1);
				cout << char(179);
			}
		}
	}
	GotoXY(_X + 0, _Y + 0);
	HighLight(_X - 1, _Y, 3, 1, 14);
	GotoXY(_X + 0, _Y + 0);
}

int ProcessFinish(int pWhoWin)
{
	int x = 60, y = 21;

	switch (pWhoWin)
	{
	case 1:
		HighLight(x - 5, y - 2, 60, 8, 14);
		SetColor(14, 0);
		DrawBox(x - 5, y - 2, 60, 8);
		GotoXY(x - 3, y); //Nhảy tới vị trí thích hợp để in chuỗi thắng/thua/hòa
		cout << "    OOOOOOOOO       OOOO       OOOO OOOOO OOOOOO     OOOO";
		GotoXY(x - 3, y + 1); cout << "  OOOO     OOOO     OOOO       OOOO  OOO  OOOO OO    OOOO";
		GotoXY(x - 3, y + 2); cout << "OOOO         OOOO   OOOO       OOOO  OOO  OOOO  OO   OOOO";
		GotoXY(x - 3, y + 3); cout << "OOOO         OOOO   OOOO  OOO  OOOO  OOO  OOOO   OO  OOOO";
		GotoXY(x - 3, y + 4); cout << "  OOOO     OOOO     OOOO OO OO OOOO  OOO  OOOO    OO OOOO";
		GotoXY(x - 3, y + 5); cout << "    OOOOOOOOO       OOOOOO   OOOOOO OOOOO OOOO     OOOOOO";
		Score2++;
		break;
	case -1:
		HighLight(x - 5, y - 2, 60, 8, 14);
		SetColor(14, 0);
		DrawBox(x - 5, y - 2, 60, 8);
		GotoXY(x, y); //Nhảy tới vị trí thích hợp để in chuỗi thắng/thua/hòa
		cout << "XXXX    XXXX    XXXX       XXXX XXXXX XXXXXX     XXXX";
		GotoXY(x, y + 1); cout << "  XXX  XXX      XXXX       XXXX  XXX  XXXX XX    XXXX";
		GotoXY(x, y + 2); cout << "    XXXXX       XXXX       XXXX  XXX  XXXX  XX   XXXX";
		GotoXY(x, y + 3); cout << "    XXXXX       XXXX  XXX  XXXX  XXX  XXXX   XX  XXXX";
		GotoXY(x, y + 4); cout << "  XXXX XXXX     XXXX XX XX XXXX  XXX  XXXX    XX XXXX";
		GotoXY(x, y + 5); cout << "XXXX     XXXX   XXXXXX   XXXXXX XXXXX XXXX     XXXXXX";
		Score1++;
		break;
	case 0:
		HighLight(x - 5, y - 2, 60, 8, 14);
		SetColor(14, 0);
		DrawBox(x - 5, y - 2, 60, 8);
		GotoXY(x - 4, y); //Nhảy tới vị trí thích hợp để in chuỗi thắng/thua/hòa
		cout << " ########      ########          ####       ###       ###";
		GotoXY(x - 4, y + 1);  cout << " ###   ###     ###   ###       ###  ###     ###       ###";
		GotoXY(x - 4, y + 2);  cout << " ###     ###   ###  ###       ###    ###    ###       ###";
		GotoXY(x - 4, y + 3); cout << " ###     ###   #######       ############   ###  ###  ###";
		GotoXY(x - 4, y + 4); cout << " ###   ###     ###  ###     ###        ###  ### ## ## ###";
		GotoXY(x - 4, y + 5); cout << " ########      ###   ###   ###          ### #####   #####";
		break;
	case 2:
		_TURN = !_TURN; //Đổi lượt nếu không có gì xảy ra
	}
	GotoXY(_X, _Y); //Trả về vị trí hiện hành của con trỏ màn hình bàn cờ
	return pWhoWin;
}

void Draw_infor(int x, int y, int w, int h, int player)
{
	string s = "";
	if (player == 1)
	{
		s = "Player 1(X)";
		GotoXY(x + (w / 2 - s.size()) / 2, y + 1);
		cout << s;
		s = Player1_name;
		GotoXY(x + (w / 2 - s.size()) / 2, y + 2);
		cout << s;
		s = "Score:";
		GotoXY(x + (w / 2 - s.size()) / 2, y + 3);
		cout << s;
		s = to_string(Score1);
		GotoXY(x + (w / 2 - s.size()) / 2, y + 4);
		cout << s;
	}
	else
	{
		s = "Player 2(O)";
		GotoXY(x + (3 * w / 2 - s.size()) / 2, y + 1);
		cout << s;
		s = Player2_name;
		GotoXY(x + (3 * w / 2 - s.size()) / 2, y + 2);
		cout << s;
		s = "Score:";
		GotoXY(x + (3 * w / 2 - s.size()) / 2, y + 3);
		cout << s;
		s = to_string(Score2);
		GotoXY(x + (3 * w / 2 - s.size()) / 2, y + 4);
		cout << s;
	}
}

void Hightlight_Play_turn(int x, int y, int w, int h, int color, int player)
{
	if (player == 1)
	{
		HighLight(x, y, w / 2, h, color);
		DrawBox(x, y, w / 2, h);
		GotoXY(x + w / 2, y);
		cout << char(203);
		GotoXY(x + w / 2, y + h);
		cout << char(202);
	}
	else
	{
		HighLight(x + w / 2, y, w / 2, h, color);
		DrawBox(x + w / 2, y, w / 2, h);
		GotoXY(x + w / 2, y);
		cout << char(203);
		GotoXY(x + w / 2, y + h);
		cout << char(202);
	}
	SetColor(color, 0);
	Draw_infor(x, y + 1, w, h, player);
	SetColor(15, 0);
}

void DrawTurn(int x, int y, int w, int h)
{
	DrawBox(x, y, w / 2, h);
	DrawBox(x + (w / 2), y, w / 2, h);
	GotoXY(x + w / 2, y);
	cout << char(203);
	GotoXY(x + w / 2, y + h);
	cout << char(202);
	Hightlight_Play_turn(x, y, w, h, 14, 1);
	Draw_infor(x, y + 1, w, h, 2);
}

void DrawAbout()
{
	SetColor(15, 0);
	system("cls");
	DrawOption(8, 25, 10, 2, 15, 0, "B:BACK");
	GotoXY(30, 6);
	cout << "This is the Caro Game project of group 4. This assignment is our";
	GotoXY(30, 7);
	cout << "own work, except where I have acknowledged the use of the works";
	GotoXY(30, 8);
	cout << "of other people. While you play the game, mistakes may inevitably";
	GotoXY(30, 9);
	cout << "occur. We appreciate your understanding and feedback.";
	GotoXY(30, 11);
	cout << "Lecturer: Truong Toan Thinh";
	GotoXY(30, 12);
	cout << "Members: + Nguyen Minh Nha - 22120242";
	GotoXY(39, 13);
	cout << "+ Tran Ngoc Nhan - 22120249";
	GotoXY(39, 14);
	cout << "+ Tran Thanh Phong - 22120265";
	GotoXY(39, 15);
	cout << "+ Cao Tien Minh - 22120207";
	while (1)
	{
		_COMMAND = toupper(_getch());
		if (_COMMAND == 'B')
			return;
	}
}

void PrintText(string text, int color, int x, int y)
{
	GotoXY(x, y);
	SetColor(color,0);
	cout << text;
	SetColor(15,0);
}
void DrawLoaded(_POINT _A[][BOARD_SIZE])
{
	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
			if (_A[i][j].c == -1)
			{
				PrintText("X", 252, _A[i][j].x, _A[i][j].y);
			}
			else if (_A[i][j].c == 1)
			{
				PrintText("O", 250, _A[i][j].x, _A[i][j].y);
			}
	}
}

