#include <windows.h>
#include "resource.h"
#include <atlimage.h>
#include <time.h>

HINSTANCE g_hInst;
LPCTSTR lpszClass = "window program";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

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
		lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 800, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdshow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT IMessage, WPARAM wParam, LPARAM lParam) {
	static RECT rt;
	static CImage img;
	static RECT puzzle[5][5], picture[5][5];
	static int x, y, a, b, n1 = 0, n2 = 0, stage = 3, Sx, Sy, Px, Py;
	static int width, count, direction, Twidth;
	static BOOL stat[5][5] = { false }, example, selection, TIMER;
	static HBITMAP hBit;
	static HDC hdc, mem1dc;
	PAINTSTRUCT ps;
	switch (IMessage) {
	case WM_CREATE:
		example = true;
		selection = false;
		TIMER = false;
		srand((unsigned)time(NULL));
		GetClientRect(hWnd, &rt);
		SetTimer(hWnd, 1, 160, NULL);
		img.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		while (1)
		{
			a = rand() % 3;
			b = rand() % 3;
			if (!stat[a][b])
			{
				stat[a][b] = true;
				puzzle[a][b].left = (rt.bottom / 3) * n1;
				puzzle[a][b].right = (rt.bottom / 3) * (n1 + 1);
				puzzle[a][b].top = (rt.bottom / 3) * n2;
				puzzle[a][b].bottom = (rt.bottom / 3) * (n2 + 1);
				n1++;
				if (n1 == 3)
				{
					n2++;
					n1 = 0;
					if (n2 == 3)
					{
						n1 = 0;
						n2 = 0;
						stat[2][2] = false;
						break;
					}
				}
			}
		}
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				picture[i][j].left = img.GetWidth() / 3 * i;
				picture[i][j].top = img.GetHeight() / 3 * j;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		if (!TIMER)
		{
			y = HIWORD(lParam);
			x = LOWORD(lParam);
			for (int i = 0; i < stage; ++i)
			{
				for (int j = 0; j < stage; ++j)
				{
					if (x >= puzzle[i][j].left && x <= puzzle[i][j].right && y >= puzzle[i][j].top && y <= puzzle[i][j].bottom)
					{
						for (int num1 = 0; num1 < stage; ++num1)
						{
							for (int num2 = 0; num2 < stage; ++num2)
							{
								if (!stat[num1][num2])
								{
									Sx = num1;
									Sy = num2;
								}
							}
						}
						if (puzzle[i][j].left - puzzle[Sx][Sy].left == rt.bottom / stage && puzzle[i][j].left > puzzle[Sx][Sy].left)
						{
							selection = true;
							TIMER = true;
							Px = i;
							Py = j;
							direction = 3;
							SetTimer(hWnd, 2, 160, NULL);
						}
						else if (puzzle[i][j].top - puzzle[Sx][Sy].top == rt.bottom / stage && puzzle[i][j].top > puzzle[Sx][Sy].top)
						{
							selection = true;
							TIMER = true;
							Px = i;
							Py = j;
							direction = 1;
							SetTimer(hWnd, 2, 160, NULL);
						}
						else if (puzzle[i][j].left - puzzle[Sx][Sy].left == rt.bottom / stage && puzzle[i][j].left < puzzle[Sx][Sy].left)
						{
							selection = true;
							TIMER = true;
							Px = i;
							Py = j;
							direction = 2;
							SetTimer(hWnd, 2, 160, NULL);
						}
						else if (puzzle[i][j].top - puzzle[Sx][Sy].top == rt.bottom / stage && puzzle[i][j].top < puzzle[Sx][Sy].top)
						{
							selection = true;
							TIMER = true;
							Px = i;
							Py = j;
							direction = 4;
							SetTimer(hWnd, 2, 160, NULL);
						}
					}
				}
			}
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rt);
		mem1dc = CreateCompatibleDC(hdc);
		SelectObject(mem1dc, hBit);

		BitBlt(hdc, 0, 0, rt.right, rt.bottom, mem1dc, 0, 0, SRCCOPY);

		DeleteDC(mem1dc);
		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		hdc = GetDC(hWnd);
		if (wParam == 1)
		{
			if (hBit == NULL)
			{
				hBit = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
			}
			mem1dc = CreateCompatibleDC(hdc);
			SelectObject(mem1dc, hBit);

			if (example)
			{
				img.StretchBlt(mem1dc, 0, 0, rt.bottom, rt.bottom, 0, 0, img.GetWidth(), img.GetHeight(), SRCCOPY);
			}
			else
			{
				Rectangle(mem1dc, 0, 0, rt.right, rt.bottom);
				for (int i = 0; i < stage; ++i)
				{
					for (int j = 0; j < stage; ++j)
					{
						if (stat[i][j])
						{
							img.StretchBlt(mem1dc, puzzle[i][j].left, puzzle[i][j].top, rt.bottom / stage, rt.bottom / stage, picture[i][j].left, picture[i][j].top, img.GetWidth() / stage, img.GetHeight() / stage, SRCCOPY);
						}
					}
				}
				// Rectangle(mem1dc, puzzle[stage - 1][stage - 1].left, puzzle[stage - 1][stage - 1].top, puzzle[stage - 1][stage - 1].right, puzzle[stage - 1][stage - 1].bottom);
			}

			DeleteDC(mem1dc);
			ReleaseDC(hWnd, hdc);
		}
		else if (wParam == 2)
		{
			if (selection)
			{
				width = rt.bottom / stage;
				Twidth = width / 10;
				if (direction == 1)
				{
					puzzle[Px][Py].top -= Twidth;
					puzzle[Px][Py].bottom -= Twidth;
				}
				else if (direction == 2)
				{
					puzzle[Px][Py].left += Twidth;
					puzzle[Px][Py].right += Twidth;
				}
				else if (direction == 3)
				{
					puzzle[Px][Py].left -= Twidth;
					puzzle[Px][Py].right -= Twidth;
				}
				else if (direction == 4)
				{
					puzzle[Px][Py].top += Twidth;
					puzzle[Px][Py].bottom += Twidth;
				}
				count++;
				if (count == 10)
				{
					stat[Px][Py] = false;
					stat[Sx][Sy] = true;
					puzzle[Sx][Sy].left = puzzle[Px][Py].left;
					puzzle[Sx][Sy].right = puzzle[Px][Py].right;
					puzzle[Sx][Sy].top = puzzle[Px][Py].top;
					puzzle[Sx][Sy].bottom = puzzle[Px][Py].bottom;
					if (direction == 1)
					{
						puzzle[Px][Py].top += width;
						puzzle[Px][Py].bottom += width;
					}
					else if (direction == 2)
					{
						puzzle[Px][Py].left -= width;
						puzzle[Px][Py].right -= width;
					}
					else if (direction == 3)
					{
						puzzle[Px][Py].left += width;
						puzzle[Px][Py].right += width;
					}
					else if (direction == 4)
					{
						puzzle[Px][Py].top -= width;
						puzzle[Px][Py].bottom -= width;
					}
					picture[Sx][Sy].left = picture[Px][Py].left;
					picture[Sx][Sy].top = picture[Px][Py].top;
					TIMER = false;
					count = 0;
					KillTimer(hWnd, 2);
				}
			}
		}
		InvalidateRgn(hWnd, NULL, false);
		break;
	case WM_KEYDOWN:
		if (!TIMER)
		{
			if (wParam == 'f' || wParam == 'F')
			{
				if (example)
				{
					example = false;
				}
				else
				{
					example = true;
				}
			}
			else if (wParam == 's' || wParam == 'S')
			{
				example = false;
			}
			else if (wParam >= 51 && wParam <= 53)
			{
				stage = wParam - 48;
				for (int i = 0; i < stage; ++i)
				{
					for (int j = 0; j < stage; ++j)
					{
						stat[i][j] = false;
					}
				}
				while (1)
				{
					a = rand() % stage;
					b = rand() % stage;
					if (!stat[a][b])
					{
						stat[a][b] = true;
						puzzle[a][b].left = (rt.bottom / stage) * n1;
						puzzle[a][b].right = (rt.bottom / stage) * (n1 + 1);
						puzzle[a][b].top = (rt.bottom / stage) * n2;
						puzzle[a][b].bottom = (rt.bottom / stage) * (n2 + 1);
						n1++;
						if (n1 == stage)
						{
							n2++;
							n1 = 0;
							if (n2 == stage)
							{
								n1 = 0;
								n2 = 0;
								stat[stage - 1][stage - 1] = false;
								break;
							}
						}
					}
				}
				for (int i = 0; i < stage; ++i)
				{
					for (int j = 0; j < stage; ++j)
					{
						picture[i][j].left = img.GetWidth() / stage * i;
						picture[i][j].top = img.GetHeight() / stage * j;
					}
				}
			}
		}
		InvalidateRgn(hWnd, NULL, false);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, IMessage, wParam, lParam);
}