#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <conio.h>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int enemyY[3];
int enemyX[3];
int enemyFlag[3];
char car[4][4] = { ' ','±','±',' ',
                   '±','±','±','±',
                   ' ','±','±',' ',
                   '±','±','±','±' };

int carPos = WIN_WIDTH / 2;
int score = 0;

void gotoxy(int x, int y) {
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size) {
    if (size == 0) {
        size = 20;
    }

    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console, &lpCursor);
}

void drawBorder() {
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < 17; j++) {
            gotoxy(0 + j, i);
            cout << "±";
            gotoxy(WIN_WIDTH - j, i);
            cout << "±";
        }
    }

    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        gotoxy(SCREEN_WIDTH, i);
        cout << "±";
    }
}

void genEnemy(int ind) {
    enemyX[ind] = 17 + rand() % 33;
}

void drawEnemy(int ind) {
    if (enemyFlag[ind] == true) {
        gotoxy(enemyX[ind], enemyY[ind]);
        cout << "****";
        gotoxy(enemyX[ind], enemyY[ind] + 1);
        cout << " ** ";
        gotoxy(enemyX[ind], enemyY[ind] + 2);
        cout << "****";
        gotoxy(enemyX[ind], enemyY[ind] + 3);
        cout << " ** ";
    }
}

void eraseEnemy(int ind) {
    if (enemyFlag[ind] == true) {
        gotoxy(enemyX[ind], enemyY[ind]);
        cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind] + 1);
        cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind] + 2);
        cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind] + 3);
        cout << "    ";
    }
}

void resetEnemy(int ind) {
    eraseEnemy(ind);
    enemyY[ind] = 1;
    genEnemy(ind);
}

void drawCar() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            gotoxy(j + carPos, i + 22);
            cout << car[i][j];
        }
    }
}

void eraseCar() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            gotoxy(j + carPos, i + 22);
            cout << " ";
        }
    }
}

int collision() {
    if (enemyY[0] + 4 >= 23) {
        if (enemyX[0] + 4 - carPos >= 0 && enemyX[0] + 4 - carPos < 9) {
            return 1;
        }
    }

    return 0;
}

int main() {
    srand((unsigned)time(NULL));
    setcursor(0, 0);
    char ch = ' ';
    int gameOver = 0;

    drawBorder();
    drawCar();
    genEnemy(0);
    genEnemy(1);
    genEnemy(2);

    while (ch != 'q' && ch != 'Q' && gameOver == 0) {
        if (_kbhit()) {
            ch = _getch();
            if (ch == 'a' || ch == 'A') {
                if (carPos > 18) {
                    carPos -= 4;
                    eraseCar();
                    drawCar();
                }
            }
            if (ch == 'd' || ch == 'D') {
                if (carPos < 50) {
                    carPos += 4;
                    eraseCar();
                    drawCar();
                }
            }
            if (ch == 'p' || ch == 'P') {
                ch = ' ';
                while (ch != 'p' && ch != 'P') {
                    ch = _getch();
                }
            }
        }

        for (int i = 0; i < 3; i++) {
            eraseEnemy(i);

            if (enemyFlag[i] == true) {
                enemyY[i] += 1;

                if (collision()) {
                    score++;
                    resetEnemy(i);
                }

                if (enemyY[i] > SCREEN_HEIGHT - 4) {
                    resetEnemy(i);
                    score--;
                }

                drawEnemy(i);
            }
        }

        if (score < 0) {
            gameOver = 1;
        }

        gotoxy(SCREEN_WIDTH + 7, SCREEN_HEIGHT / 2);
        cout << "Score: " << score;

        Sleep(50);
    }

    system("cls");
    cout << "Game Over!" << endl;
    cout << "Your score: " << score << endl;

    return 0;
}

