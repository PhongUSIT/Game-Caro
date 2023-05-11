#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <string>
using namespace std;

#define BOARD_SIZE 12 //Kích thước ma trận bàn cờ
#define LEFT 3 //Tọa độ trái màn hình bàn cờ
#define TOP 1 //Tọa độ trên màn hình bàn cờ
#define OPTION_HIGH 2
#define OPTION_WIDTH  20

//Khai báo kiểu dữ liệu
struct _POINT { int x, y, c; }; //x: tọa độ dòng, y: tọa độ cột, c: đánh dấu
_POINT _A[BOARD_SIZE][BOARD_SIZE]; //Ma trận bàn cờ
bool _TURN; //true là lượt người thứ nhất và false là lượt người thứ hai
int _COMMAND; //Biến nhận giá trị phím người dùng nhập
int _X, _Y; //Tọa độ hiện hành trên màn hình bàn cờ
struct MENU
{
	string opt1;
	string opt2;
	string opt3;
	string opt4;
};
MENU menu = { "NEW GAME","LOAD GAME","ABOUT","EXIT" };
int _OPTION; //Thứ tự hộp menu
int Score1;
int Score2;
string Player1_name;
string Player2_name;

//Hàm View
void FixConsoleWindow();
//Hàm model
void Play();

void main()
{
	FixConsoleWindow();
	while (1)
	{
		Play();
	}
}
