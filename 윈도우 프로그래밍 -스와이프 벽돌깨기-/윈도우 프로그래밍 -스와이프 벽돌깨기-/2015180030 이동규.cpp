#include <windows.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define width 1;
#define PI 3.141592

HINSTANCE g_hInst;
LPCTSTR lpszClass = "window program";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

typedef struct BALL {
	int x1, x2, y1, y2;
	int stat;
}BALL;
double abc(double x)
{
	if (x > 0)
	{
		return x;
	}
	else if (x < 0)
	{
		return -x;
	}
	else return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevinstance, LPSTR lpszCmdParam, int nCmdshow) {
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(
		lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, 500, 150, 800, 800, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdshow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT IMessage, WPARAM wParam, LPARAM lParam) {
	static int x, y, lineX, lineY, endX, endY, oldX, oldY;
	static int ballx, bally;
	static int ball_speedX, ball_speedY, num, num1;
	static int count, ball_count;
	static int ball_num = 1, exit;
	static int block_num, stage = 1;
	static double radian;
	static BOOL Drag, Start, Game;
	static char text[10];
	static BALL ball[200];
	static RECT rt;
	static RECT block[10][6];
	static char game_text[100];
	static int item[10][6];
	static BOOL itemmove[10][6];
	static int block_life[10][6];
	static HPEN hPen, oldPen;
	static HBRUSH hBrush, oldBrush;
	HDC hdc;
	PAINTSTRUCT ps;
	switch (IMessage) {
	case WM_CREATE:
		Game = TRUE;
		srand((unsigned)time(NULL));
		GetClientRect(hWnd, &rt);
		ball[0].x1 = rt.right / 2 - 15;
		ball[0].x2 = rt.right / 2 + 15;
		ball[0].y1 = rt.bottom - 115;
		ball[0].y2 = rt.bottom - 85;
		x = (ball[0].x1 + ball[0].x2) / 2;
		y = (ball[0].y1 + ball[0].y2) / 2;
		for (int i = 0; i < 6; ++i)
		{
			block_num = rand() % 10;
			if (block_num < 6)
			{
				block[0][i].left = (rt.right / 6) * i;
				block[0][i].top = 0;
				block[0][i].right = (rt.right / 6) * (i+1);
				block[0][i].bottom = ((rt.bottom - 100) / 10);
				block_life[0][i] = stage;
			}
			else if (block_num > 5 && block_num < 8)
			{
				block[0][i].left = ((rt.right / 6) * i) + 45;
				block[0][i].top = 15;
				block[0][i].right = (rt.right / 6) * (i + 1) - 45;
				block[0][i].bottom = ((rt.bottom - 100) / 10) - 15;
				item[0][i] = 0;
				block_life[0][i] = -1;
			}
		}
		count = 1;
		SetTimer(hWnd, 2, 160, NULL);
		Drag = FALSE;
		Start = FALSE;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		wsprintf(text, "x %d", count);
		hPen = CreatePen(PS_DASH, 1, RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		MoveToEx(hdc, 0, rt.bottom - 100, NULL);
		LineTo(hdc, rt.right, rt.bottom - 100);
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);

		hBrush = CreateSolidBrush(RGB(50, 50, 255));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		if (Start)
		{
			for (int i = 0; i < count ; ++i)
			{
				Ellipse(hdc, ball[i].x1, ball[i].y1, ball[i].x2, ball[i].y2);
			}
		}
		else if (!Start)
		{
			Ellipse(hdc, ball[0].x1, ball[0].y1, ball[0].x2, ball[0].y2);
			TextOut(hdc, ball[0].x1, ball[0].y2, text, strlen(text));
		}
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);

		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 6; ++j)
			{
				if (block_life[i][j] > 0)
				{
					wsprintf(text, "%d", block_life[i][j]);
					hBrush = CreateSolidBrush(RGB(250, 30, 30));
					oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
					Rectangle(hdc, block[i][j].left, block[i][j].top, block[i][j].right, block[i][j].bottom);
					SelectObject(hdc, oldBrush);
					DeleteObject(hBrush);
					SetBkColor(hdc, RGB(250, 30, 30));
					DrawText(hdc, text, strlen(text), &block[i][j], DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				}
				else if (block_life[i][j] == -1)
				{
					hBrush = CreateSolidBrush(RGB(50, 250, 50));
					oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
					Ellipse(hdc, block[i][j].left, block[i][j].top, block[i][j].right, block[i][j].bottom);
					SelectObject(hdc, oldBrush);
					DeleteObject(hBrush);
				}
			}
		}
		SetBkColor(hdc, RGB(255, 255, 255));
		wsprintf(game_text, "%d/100 스테이지", stage);
		TextOut(hdc, 5, rt.bottom - 60, game_text, strlen(game_text));
		if (!Game)
		{
			TextOut(hdc, 300, 350, "패배!", 5);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		if (!Start)
		{
			Drag = TRUE;
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
		}
		break;
	case WM_LBUTTONUP:
		Start = TRUE;
		Drag = FALSE;
		ball_speedX = abs(x - endX) / 13;
		ball_speedY = abs(y - endY) / 13;
		SetTimer(hWnd, 1, 80, NULL);
		InvalidateRect(hWnd, NULL, TRUE);
		for (int i = 1; i < count; ++i)
		{
			ball[i].stat = ball[0].stat;
		}
		break;
	case WM_TIMER:
		hdc = GetDC(hWnd);
		switch (wParam) {
		case 1:
			if (Start)
			{
				num1++;
				if (count != ball_count)
				{
					num++;
					if (num == 2)
					{
						ball_count++;
						num = 0;
					}
				}
				for (int i = 0; i < ball_count; ++i)
				{
					if (ball[i].stat == 1) // 왼쪽 위
					{
						ball[i].x1 -= ball_speedX;
						ball[i].y1 -= ball_speedY;
						ball[i].x2 -= ball_speedX;
						ball[i].y2 -= ball_speedY;
						ballx = (ball[i].x1 + ball[i].x2) / 2;
						bally = (ball[i].y1 + ball[i].y2) / 2;
						if (ball[i].x1 <= 0)
						{
							ball[i].stat = 2;
						}
						else if (ball[i].y1 <= 0)
						{
							ball[i].stat = 3;
						}
						for (int a = 0; a < 10; ++a)
						{
							for (int b = 0; b < 6; ++b)
							{
								if (block_life[a][b] > 0)
								{
									if (block[a][b].bottom > ball[i].y1 && block[a][b].left < ballx && ballx < block[a][b].right && ball[i].y1 > block[a][b].bottom - 50)
									{
										block_life[a][b]--;
										ball[i].stat = 3;
										InvalidateRect(hWnd, NULL, TRUE);
									}
									else if (block[a][b].right > ball[i].x1 && block[a][b].top < bally && bally < block[a][b].bottom && ball[i].x1 > block[a][b].right - 50)
									{
										block_life[a][b]--;
										ball[i].stat = 2;
										InvalidateRect(hWnd, NULL, TRUE);
									}
								}
								else if (block_life[a][b] == -1)
								{
									if (ballx > block[a][b].left && ballx < block[a][b].right && bally > block[a][b].top && bally < block[a][b].bottom)
									{
										count++;
										block_life[a][b] = 0;
									}
								}
							}
						}
						InvalidateRect(hWnd, NULL, TRUE);
					}
					else if (ball[i].stat == 2) // 오른쪽 위
					{
						ball[i].x1 += ball_speedX;
						ball[i].y1 -= ball_speedY;
						ball[i].x2 += ball_speedX;
						ball[i].y2 -= ball_speedY;
						ballx = (ball[i].x1 + ball[i].x2) / 2;
						bally = (ball[i].y1 + ball[i].y2) / 2;
						if (ball[i].x2 >= rt.right)
						{
							ball[i].stat = 1;
						}
						else if (ball[i].y1 <= 0)
						{
							ball[i].stat = 4;
						}
						for (int a = 0; a < 10; ++a)
						{
							for (int b = 0; b < 6; ++b)
							{
								if (block_life[a][b] > 0)
								{
									if (block[a][b].bottom > ball[i].y1 && block[a][b].left < ballx && ballx < block[a][b].right && ball[i].y1 > block[a][b].bottom - 50)
									{
										block_life[a][b]--;
										ball[i].stat = 4;
										InvalidateRect(hWnd, NULL, TRUE);
									}
									else if (block[a][b].left < ball[i].x2 && block[a][b].top < bally && bally < block[a][b].bottom && ball[i].x2 < block[a][b].left + 50)
									{
										block_life[a][b]--;
										ball[i].stat = 1;
										InvalidateRect(hWnd, NULL, TRUE);
									}
								}
								else if (block_life[a][b] == -1)
								{
									if (ballx > block[a][b].left && ballx < block[a][b].right && bally > block[a][b].top && bally < block[a][b].bottom)
									{
										count++;
										block_life[a][b] = 0;
									}
								}
							}
						}
						InvalidateRect(hWnd, NULL, TRUE);
					}
					else if (ball[i].stat == 3) // 왼쪽 아래
					{
						ball[i].x1 -= ball_speedX;
						ball[i].y1 += ball_speedY;
						ball[i].x2 -= ball_speedX;
						ball[i].y2 += ball_speedY;
						ballx = (ball[i].x1 + ball[i].x2) / 2;
						bally = (ball[i].y1 + ball[i].y2) / 2;
						if (ball[i].x1 <= 0)
						{
							ball[i].stat = 4;
						}
						for (int a = 0; a < 10; ++a)
						{
							for (int b = 0; b < 6; ++b)
							{
								if (block_life[a][b] > 0)
								{
									if (block[a][b].top < ball[i].y2 && block[a][b].left < ballx && ballx < block[a][b].right && ball[i].y2 < block[a][b].top + 50)
									{
										block_life[a][b]--;
										ball[i].stat = 1;
										InvalidateRect(hWnd, NULL, TRUE);
									}
									else if (block[a][b].right > ball[i].x1 && block[a][b].top < bally && bally < block[a][b].bottom && ball[i].x1 > block[a][b].right - 50)
									{
										block_life[a][b]--;
										ball[i].stat = 4;
										InvalidateRect(hWnd, NULL, TRUE);
									}
								}
								else if (block_life[a][b] == -1)
								{
									if (ballx > block[a][b].left && ballx < block[a][b].right && bally > block[a][b].top && bally < block[a][b].bottom)
									{
										count++;
										block_life[a][b] = 0;
									}
								}
							}
						}
						InvalidateRect(hWnd, NULL, TRUE);
					}
					else if (ball[i].stat == 4) // 오른쪽 아래
					{
						ball[i].x1 += ball_speedX;
						ball[i].y1 += ball_speedY;
						ball[i].x2 += ball_speedX;
						ball[i].y2 += ball_speedY;
						ballx = (ball[i].x1 + ball[i].x2) / 2;
						bally = (ball[i].y1 + ball[i].y2) / 2;
						if (ball[i].x2 >= rt.right)
						{
							ball[i].stat = 3;
						}
						for (int a = 0; a < 10; ++a)
						{
							for (int b = 0; b < 6; ++b)
							{
								if (block_life[a][b] > 0)
								{
									if (block[a][b].top < ball[i].y2 && block[a][b].left < ballx && ballx < block[a][b].right && ball[i].y2 < block[a][b].top + 50)
									{
										block_life[a][b]--;
										ball[i].stat = 2;
										InvalidateRect(hWnd, NULL, TRUE);
									}
									else if (block[a][b].left < ball[i].x2 && block[a][b].top < bally && bally < block[a][b].bottom && ball[i].x2 < block[a][b].left + 50)
									{
										block_life[a][b]--;
										ball[i].stat = 3;
										InvalidateRect(hWnd, NULL, TRUE);
									}
								}
								else if (block_life[a][b] == -1)
								{
									if (ballx > block[a][b].left && ballx < block[a][b].right && bally > block[a][b].top && bally < block[a][b].bottom)
									{
										count++;
										block_life[a][b] = 0;
									}
								}
							}
						}
						InvalidateRect(hWnd, NULL, TRUE);
					}
				}
				// 바닥에 도착
				for (int i = 0; i < count; ++i)
				{
					if (ball[i].y2 > rt.bottom - 100 && num1 > 2)
					{
						ball[i].stat = -1;
						if (ball[i].stat == -1)
						{
							exit++;
							if (exit == count)
							{
								Drag = FALSE;
								Start = FALSE;
								stage++;
								x = (ball[0].x1 + ball[0].x2) / 2;
								y = (ball[0].y1 + ball[0].y2) / 2;
								for (int a = 0; a < count; ++a)
								{
									ball[a].x1 = ball[0].x1;
									ball[a].x2 = ball[0].x2;
									ball[a].y1 = ball[0].y1;
									ball[a].y2 = ball[0].y2;
									ball[a].stat = 0;
								}
								ball_count = 0;
								if (stage == 101)
								{
									Game = FALSE;
									TextOut(hdc, 400, 350, "승리!", 5);
								}
								exit = 0;
								for (int block_line = 0; block_line < 6; ++block_line)
								{
									if (block_life[8][block_line] > 0)
									{
										Game = FALSE;
										TextOut(hdc, 400, 350, "패배!", 5);
										KillTimer(hWnd, 1);
									}
								}
								for (int block_move = 9; block_move > 0; --block_move)
								{
									for (int block_line = 0; block_line < 6; ++block_line)
									{
										if (block_life[block_move - 1][block_line] > 0)
										{
											block[block_move][block_line].left = block[block_move - 1][block_line].left;
											block[block_move][block_line].top = block[block_move - 1][block_line].top + ((rt.bottom - 100) / 10);
											block[block_move][block_line].right = block[block_move - 1][block_line].right;
											block[block_move][block_line].bottom = block[block_move - 1][block_line].bottom + ((rt.bottom - 100) / 10);
											block[block_move - 1][block_line].left = 0;
											block[block_move - 1][block_line].top = 0;
											block[block_move - 1][block_line].right = 0;
											block[block_move - 1][block_line].bottom = 0;
											block_life[block_move][block_line] = block_life[block_move - 1][block_line];
											block_life[block_move - 1][block_line] = 0;
										}
										else if (block_life[block_move - 1][block_line] == -1)
										{
											block[block_move][block_line].left = ((rt.right / 6) * block_line) + 45;
											block[block_move][block_line].top = block[block_move - 1][block_line].top + ((rt.bottom - 100) / 10);
											block[block_move][block_line].right = ((rt.right / 6) * (block_line + 1)) - 45;
											block[block_move][block_line].bottom = block[block_move - 1][block_line].bottom + ((rt.bottom - 100) / 10);
											item[block_move][block_line] = TRUE;
											item[block_move - 1][block_line] = FALSE;
											block_life[block_move][block_line] = block_life[block_move - 1][block_line];
											block_life[block_move - 1][block_line] = 0;
										}
									}
								}
								for (int block_line = 0; block_line < 6; ++block_line)
								{
									block_num = rand() % 10;
									if (block_num < 5)
									{
										block[0][block_line].left = (rt.right / 6) * block_line;
										block[0][block_line].top = 0;
										block[0][block_line].right = (rt.right / 6) * (block_line + 1);
										block[0][block_line].bottom = ((rt.bottom - 100) / 10);
										block_life[0][block_line] = stage;
									}
									else if (block_num == 6)
									{
										block[0][block_line].left = ((rt.right / 6) * block_line) + 45;
										block[0][block_line].top = 15;
										block[0][block_line].right = (rt.right / 6) * (block_line + 1) - 45;
										block[0][block_line].bottom = ((rt.bottom - 100) / 10) - 15;
										item[0][block_line] = TRUE;
										block_life[0][block_line] = -1;
									}
								}
								num1 = 0;
								KillTimer(hWnd, 1);
							}
						}
					}
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
			case 2:
				for (int i = 0; i < 10; ++i)
				{
					for (int j = 0; j < 6; ++j)
					{
						if (block_life[i][j] == -1)
						{
							if (itemmove[i][j])
							{
								block[i][j].left += width;
								block[i][j].top += width;
								block[i][j].right -= width;
								block[i][j].bottom -= width;
								item[i][j]--;
								if (item[i][j] == 0)
								{
									itemmove[i][j] = FALSE;
								}
							}
							else
							{
								block[i][j].left -= width;
								block[i][j].top -= width;
								block[i][j].right += width;
								block[i][j].bottom += width;
								item[i][j]++;
								if (item[i][j] == 5)
								{
									itemmove[i][j] = TRUE;
								}
							}
						}
					}
				}
				InvalidateRect(hWnd, NULL, TRUE);
				break;
		}
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);
		if (Drag && !Start && Game)
		{
			for (int i = 0; i < 10; ++i)
			{
				for (int j = 0; j < 6; ++j)
				{
					if (block_life[i][j] > 0)
					{
						wsprintf(text, "%d", block_life[i][j]);
						hBrush = CreateSolidBrush(RGB(250, 30, 30));
						oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
						Rectangle(hdc, block[i][j].left, block[i][j].top, block[i][j].right, block[i][j].bottom);
						SelectObject(hdc, oldBrush);
						DeleteObject(hBrush);
						SetBkColor(hdc, RGB(250, 30, 30));
						DrawText(hdc, text, strlen(text), &block[i][j], DT_SINGLELINE | DT_CENTER | DT_VCENTER);
					}
					else if (block_life[i][j] == -1)
					{
						hBrush = CreateSolidBrush(RGB(50, 250, 50));
						oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
						Ellipse(hdc, block[i][j].left, block[i][j].top, block[i][j].right, block[i][j].bottom);
						SelectObject(hdc, oldBrush);
						DeleteObject(hBrush);
					}
				}
			}
			if (abc(y - HIWORD(lParam)) / abc(x - LOWORD(lParam)) > tan(PI/18) && HIWORD(lParam) < rt.bottom - 100)
			{
				if (x > LOWORD(lParam))
				{
					if (abc(y - HIWORD(lParam)) / abc(x - LOWORD(lParam)) > abc(650.0 / x))
					{
						hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
						oldPen = (HPEN)SelectObject(hdc, hPen);
						MoveToEx(hdc, x, y, NULL);
						LineTo(hdc, oldX, oldY);
						SelectObject(hdc, oldPen);
						DeleteObject(hPen);
						hBrush = CreateSolidBrush(RGB(50, 50, 255));
						oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
						Ellipse(hdc, ball[0].x1, ball[0].y1, ball[0].x2, ball[0].y2);
						SelectObject(hdc, oldBrush);
						DeleteObject(hBrush);

						oldX = endX;
						oldY = endY;
						endX = LOWORD(lParam);
						endY = HIWORD(lParam);

						hPen = CreatePen(PS_DASHDOTDOT, 1, RGB(0, 0, 255));
						oldPen = (HPEN)SelectObject(hdc, hPen);
						MoveToEx(hdc, x, y, NULL);
						LineTo(hdc, endX, endY);
						SelectObject(hdc, oldPen);
						DeleteObject(hPen);
					}
					else
					{
						hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
						oldPen = (HPEN)SelectObject(hdc, hPen);
						MoveToEx(hdc, x, y, NULL);
						LineTo(hdc, oldX, oldY);
						SelectObject(hdc, oldPen);
						DeleteObject(hPen);

						hBrush = CreateSolidBrush(RGB(50, 50, 255));
						oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
						Ellipse(hdc, ball[0].x1, ball[0].y1, ball[0].x2, ball[0].y2);
						SelectObject(hdc, oldBrush);
						DeleteObject(hBrush);

						oldX = endX;
						oldY = endY;
						endX = LOWORD(lParam);
						endY = HIWORD(lParam);

						hPen = CreatePen(PS_DASHDOTDOT, 1, RGB(0, 0, 255));
						oldPen = (HPEN)SelectObject(hdc, hPen);
						MoveToEx(hdc, x, y, NULL);
						LineTo(hdc, endX, endY);
						SelectObject(hdc, oldPen);
						DeleteObject(hPen);
					}
					ball[0].stat = 1;
				}
				else if (x < LOWORD(lParam))
				{
					if (abc(y - HIWORD(lParam)) / abc(x - LOWORD(lParam)) > abc(650.0 / (rt.right - x)))
					{
						hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
						oldPen = (HPEN)SelectObject(hdc, hPen);
						MoveToEx(hdc, x, y, NULL);
						LineTo(hdc, oldX, oldY);
						SelectObject(hdc, oldPen);
						DeleteObject(hPen);

						hBrush = CreateSolidBrush(RGB(50, 50, 255));
						oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
						Ellipse(hdc, ball[0].x1, ball[0].y1, ball[0].x2, ball[0].y2);
						SelectObject(hdc, oldBrush);
						DeleteObject(hBrush);

						oldX = endX;
						oldY = endY;
						endX = LOWORD(lParam);
						endY = HIWORD(lParam);

						hPen = CreatePen(PS_DASHDOTDOT, 1, RGB(0, 0, 255));
						oldPen = (HPEN)SelectObject(hdc, hPen);
						MoveToEx(hdc, x, y, NULL);
						LineTo(hdc, endX, endY);
						SelectObject(hdc, oldPen);
						DeleteObject(hPen);
					}
					else
					{
						hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
						oldPen = (HPEN)SelectObject(hdc, hPen);
						MoveToEx(hdc, x, y, NULL);
						LineTo(hdc, oldX, oldY);
						SelectObject(hdc, oldPen);
						DeleteObject(hPen);

						hBrush = CreateSolidBrush(RGB(50, 50, 255));
						oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
						Ellipse(hdc, ball[0].x1, ball[0].y1, ball[0].x2, ball[0].y2);
						SelectObject(hdc, oldBrush);
						DeleteObject(hBrush);

						oldX = endX;
						oldY = endY;
						endX = LOWORD(lParam);
						endY = HIWORD(lParam);

						hPen = CreatePen(PS_DASHDOTDOT, 1, RGB(0, 0, 255));
						oldPen = (HPEN)SelectObject(hdc, hPen);
						MoveToEx(hdc, x, y, NULL);
						LineTo(hdc, endX, endY);
						SelectObject(hdc, oldPen);
						DeleteObject(hPen);
					}
					ball[0].stat = 2;
				}
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, IMessage, wParam, lParam);
}
