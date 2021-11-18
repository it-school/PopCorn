#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <ctime>

using namespace std;

const double SPEED = 0.1;
const int MAX_X = 36, MAX_Y = 15, WIDTH = 7;
static int pX = (MAX_X - WIDTH / 2) / 2, pY = MAX_Y - 1, bX = pX, bY = pY, blX = bX, blY = bY;
double x = 5, y = 2, xSpeed = SPEED, ySpeed = SPEED;

void ChangeConsoleCursorVisibility()
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = !cursorInfo.bVisible;
	SetConsoleCursorInfo(out, &cursorInfo);
}

inline void setcur(int x, int y, int color)
{
	COORD coord{ x, y };
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, color);
	SetConsoleCursorPosition(hConsole, coord);
};

void drawPlatform(int x, int y, int WIDTH, int color)
{
	setcur(x, y, color);
	cout << "<";
	for (int p = 1; p <= WIDTH - 2; p++)
	{
		cout << "=";
	}
	cout << ">";
}

void gameOver()
{
	setcur(0, MAX_Y + 1, 4);
	cout << "Game over   ";
	exit(_getch());
}

void ballMove(void* arg)
{
	do
	{
		x += xSpeed;
		y += ySpeed;
		bX = (int)x;
		bY = (int)y;

		if ((bY == MAX_Y - 2 && (bX == pX - 1 || bX == pX + WIDTH)))
		{
			xSpeed = -xSpeed;
			ySpeed = -ySpeed;
		}
		else

			if ((bY == MAX_Y - 2 && bX >= pX + 1 && bX < pX + WIDTH - 1) || (y <= 0)) // upper border / platform
			{
				ySpeed = -ySpeed;				
			}
			else

				if (bX == 0 || bX == MAX_X) // left/right border
				{
					xSpeed = -xSpeed;
				}
				else

					if (bY == MAX_Y) // down border
					{
						gameOver();
					}

		if (fabs(fabs(bX - x) - fabs(xSpeed)) < SPEED / 2 || fabs(fabs(bY - y) - fabs(ySpeed)) < SPEED / 2)
		{
			setcur(blX, blY, 0);
			cout << "@";
			setcur(bX, bY, 11);
			cout << "@";
			blX = bX;
			blY = bY;
		}
		Sleep(10);
	} while (true);
}


int main()
{
	ChangeConsoleCursorVisibility();

	char key = 0;

	//	double x = 0, y = 0, xSpeed = SPEED, ySpeed = SPEED;

	for (char y = 0; y < MAX_Y; y++)
	{
		setcur(MAX_X, y, 15);
		cout << "|";
	}

	setcur(0, MAX_Y, 15);
	for (char x = 0; x <= MAX_X; x++)
	{
		cout << "-";
	}
	cout << endl;

	HANDLE hBall = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ballMove, 0, 0, 0);
	//	HANDLE hPlatform = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)platformMove, 0, 0, 0);
	//	WaitForSingleObject(hBall, 0);

	cout << "Game started\n";

	do {
		if (_kbhit())
		{
			key = _getch();

			switch (key)
			{
			case 75: // LEFT
				if (pX > 0)
				{
					drawPlatform(pX--, pY, WIDTH, 0);
					drawPlatform(pX, pY, WIDTH, 14);
				}
				break;
			case 77: // RIGHT
				if (pX < MAX_X - WIDTH)
				{
					drawPlatform(pX++, pY, WIDTH, 0);
					drawPlatform(pX, pY, WIDTH, 14);
				}
				break;
			case 27: // ESCAPE:
				TerminateThread(hBall, 0); // Dangerous source of errors!
				CloseHandle(hBall);
				gameOver();
			}
		}
	} while (true);


	cout << "Game finished\n";
}