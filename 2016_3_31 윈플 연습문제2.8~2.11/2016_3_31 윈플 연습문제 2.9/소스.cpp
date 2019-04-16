#include <windows.h>
#include <time.h>
#include <stdlib.h>

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
		lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 980, 680, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdshow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT IMessage, WPARAM wParam, LPARAM lParam) {
	static int red = 0, blue = 0, green = 0;
	static int x = 0, y = 600;
	static int num = 0, width = 1;
	static int count = 0, state = 0;
	static int shape = 0;
	static int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	static char scan[30];
	static SIZE size;
	HPEN hPen, oldPen;
	HDC hdc;
	PAINTSTRUCT ps;
	switch (IMessage) {
	case WM_CREATE:
		CreateCaret(hWnd, NULL, 5, 15);
		srand((unsigned)time(NULL));
		ShowCaret(hWnd);
		break;
	case WM_CHAR:
	{
		hdc = GetDC(hWnd);
		if (wParam == VK_RETURN)
		{
			int index = 100;
			int space = 1;
			int n = 0;
			shape = scan[0] - 48;
			for (int i = 1; i < 30; ++i)
			{
				if (scan[i] >= 48 && scan[i] <= 57)
				{
					if (n == 1)
					{
						x1 += (scan[i] - 48) * index;
						index = index / 10;
					}
					else if (n == 2)
					{
						x1 = x1 / space;
						space = 1;
						y1 += (scan[i] - 48) * index;
						index = index / 10;
					}
					else if (n == 3)
					{
						y1 = y1 / space;
						space = 1;
						x2 += (scan[i] - 48) * index;
						index = index / 10;
					}
					else if (n == 4)
					{
						x2 = x2 / space;
						space = 1;
						y2 += (scan[i] - 48) * index;
						index = index / 10;
					}
				}
				else
				{
					n++;
					if (index == 1)
					{
						space = 10;
					}
					index = 100;
					if (n == 5)
					{
						y2 = y2 / space;
						break;
					}
				}
			}
			for (int i = 0; i < 30; ++i)
			{
				scan[i] = '\0';
			}
			num = 0;
			count = 1;
		}
		else if (wParam == '+')
		{
			if (shape != 0)
			{
				width++;
			}
			else
			{
				state = 1;
			}
		}
		else if (wParam == '-')
		{
			if (shape != 0)
			{
				width--;
			}
			else
			{
				state = 1;
			}
		}
		else if (wParam == VK_TAB)
		{
			if (shape != 0)
			{
				red = rand() % 256;
				green = rand() % 256;
				blue = rand() % 256;
			}
			else
			{
				state = 1;
			}
		}
		else
		{
			x1 = 0, y1 = 0, x2 = 0,y2 = 0;
			shape = 0;
			scan[num++] = wParam;
			scan[num] = '\0';
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetTextExtentPoint(hdc, scan, strlen(scan), &size);
		Rectangle(hdc, x, y-5, x + 150, y + 20);
		TextOut(hdc, x, y, scan, strlen(scan));
		hPen = CreatePen(PS_SOLID, width, RGB(red, green, blue));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		SetCaretPos(x + size.cx, y);
		if (state == 0)
		{
			if (shape == 1)
			{
				MoveToEx(hdc, x1, y1, NULL);
				LineTo(hdc, x2, y2);
			}
			else if (shape == 2)
			{
				Ellipse(hdc, x1, y1, x2, y2);
			}
			else if (shape == 3)
			{
				Rectangle(hdc, x1, y1, x2, y2);
			}
		}
		else
		{
			TextOut(hdc, 300, 400, "도형이 존재하지 않습니다!", 25);
			state = 0;
		}
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, IMessage, wParam, lParam);
}