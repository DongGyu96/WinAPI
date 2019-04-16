#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

typedef struct Board {
	int x1;
	int x2;
	int y1;
	int y2;
	int stat;
}Board;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	static char szAppName[] = "Midterm";
	static char szTitle[] = "2015180030 이동규";		//******* 본인 이름과 학번으로 윈도우 타이틀 설정하기

	MSG	msg;
	HWND hWnd;
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szAppName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wc);

	hWnd = CreateWindow(szAppName, szTitle, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 800, 900, NULL, NULL, hInstance, NULL);

	if (!hWnd)		return (false);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static Board board[10][20];
	static Board block[12], round;
	static int x, y, count = 0;
	static int boardX, boardY;
	static int move = 0;
	static int Exit_Count = 0;
	static RECT rt;
	static HPEN hPen, oldPen;
	static HBRUSH hBrush, oldBrush;
	static BOOL Start, Mouse, Stop;
	switch (uMsg)
	{
		HDC hdc;
		PAINTSTRUCT ps;
	case WM_CREATE:
		GetClientRect(hWnd, &rt);
		round.stat = -1;
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 20; ++j)
			{
				board[i][j].x1 = 40 * i;
				board[i][j].x2 = 40 * (i + 1);
				board[i][j].y1 = 40 * j;
				board[i][j].y2 = 40 * (j + 1);
				board[i][j].stat = 0;
			}
		}
		Start = FALSE;
		Mouse = FALSE;
		Stop = FALSE;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 20; ++j)
			{
				if (board[i][j].stat == 1)
				{
					Exit_Count++;
					if (Exit_Count == 20)
					{
						KillTimer(hWnd, 1);
						KillTimer(hWnd, 2);
						Stop = TRUE;
					}
				}
			}
			Exit_Count = 0;
		}
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < 20; ++j)
			{
				if (board[i][j].stat == 0)
				{
					hPen = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
					oldPen = (HPEN)SelectObject(hdc, hPen);
					Rectangle(hdc, board[i][j].x1, board[i][j].y1, board[i][j].x2, board[i][j].y2);
					SelectObject(hdc, oldPen);
					DeleteObject(hPen);
				}
				else if(board[i][j].stat == 1)
				{
					hBrush = CreateSolidBrush(RGB(255, 0, 0));
					oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
					Rectangle(hdc, board[i][j].x1, board[i][j].y1, board[i][j].x2, board[i][j].y2);
					SelectObject(hdc, oldBrush);
					DeleteObject(hBrush);
				}
			}
		}
		for (int i = 0; i < count; ++i)
		{
			if (round.x1 == block[i].x1 && round.y1 == block[i].y1)
			{
				hBrush = CreateSolidBrush(RGB(0, 255, 255));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				Rectangle(hdc, block[i].x1, block[i].y1, block[i].x2, block[i].y2);
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
			}
			else
			{
				hBrush = CreateSolidBrush(RGB(150, 150, 150));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				Rectangle(hdc, block[i].x1, block[i].y1, block[i].x2, block[i].y2);
				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
			}
		}
		if (round.stat != -1)
		{
			hBrush = CreateSolidBrush(RGB(250, 250, 150));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Ellipse(hdc, round.x1, round.y1, round.x2, round.y2);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		if (wParam == 's' || wParam == 'S')
		{
			if (Mouse)
			{
				Mouse = FALSE;
			}
			else
				Mouse = TRUE;
		}
		else if (wParam == 'p' || wParam == 'P')
		{
			if (!Start)
			{
				Start = TRUE;
				round.x1 = board[0][19].x1;
				round.y1 = board[0][19].y1;
				round.x2 = board[0][19].x2;
				round.y2 = board[0][19].y2;
				round.stat = 1;
				SetTimer(hWnd, 2, 800, NULL);
				SetTimer(hWnd, 1, 800, NULL);
			}
		}
		else if (wParam == 'q' || wParam == 'Q')
		{
			if (Stop)
			{
				Stop = FALSE;
			}
			else
				Stop = TRUE;
		}
		else if (wParam == 'x' || wParam == 'X')
		{
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			PostQuitMessage(0);
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_RIGHT)
		{
			if (round.stat != -1)
			{
				round.stat = 1;
			}
		}
		else if (wParam == VK_LEFT)
		{
			if (round.stat != -1)
			{
				round.stat = 0;
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDOWN:
		if (Mouse)
		{
			y = HIWORD(lParam);
			x = LOWORD(lParam);

			for (int i = 0; i < 10; ++i)
			{
				for (int j = 0; j < 20; ++j)
				{
					if (board[i][j].x1 < x && board[i][j].x2 > x && board[i][j].y1 < y && board[i][j].y2 > y)
					{
						if (count < 12)
						{
							block[count].x1 = board[i][j].x1;
							block[count].x2 = board[i][j].x2;
							block[count].y1 = board[i][j].y1;
							block[count].y2 = board[i][j].y2;
							block[count].stat = 1;
							count++;
						}
					}
				}
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_TIMER:
		switch (wParam) {
		case 1:
			if (!Stop && Start)
			{
				for (int i = 0; i < count; ++i)
				{
					for (int a = 0; a < 10; ++a)
					{
						for (int b = 0; b < 20; ++b)
						{
							if (board[a][b].y1 == block[i].y1 && board[a][b].x1 == block[i].x1)
							{
								boardX = a;
								boardY = b + 1;
							}
						}
					}
					if (block[i].y2 == 800 || board[boardX][boardY].stat == 1)
					{
						block[i].y1 = 0;
						block[i].y2 = 40;
						board[boardX][boardY - 1].stat = 1;
					}
					else
					{
						block[i].y1 += 40;
						block[i].y2 += 40;
					}
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 2:
			if (!Stop && Start)
			{
				if (round.stat == 1)
				{
					for (int i = 19; i >= 0; --i)
					{
						if (board[move + 1][i].stat == 0)
						{
							round.y1 = board[move + 1][i].y1;
							round.y2 = board[move + 1][i].y2;
							break;
						}
					}
					round.x1 += 40;
					round.x2 += 40;
					move++;
					if (round.x2 == 400)
					{
						round.stat = 0;
					}
				}
				else if (round.stat == 0)
				{
					for (int i = 19; i >= 0; --i)
					{
						if (board[move - 1][i].stat == 0)
						{
							round.y1 = board[move - 1][i].y1;
							round.y2 = board[move - 1][i].y2;
							break;
						}
					}
					round.x1 -= 40;
					round.x2 -= 40;
					move--;
					if (round.x1 == 0)
					{
						round.stat = 1;
					}
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}