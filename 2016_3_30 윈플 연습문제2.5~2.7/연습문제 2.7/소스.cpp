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
	static int x = 0, y = 0;
	static char str[10][30];
	static char sort[30][10];
	static char star[30];
	static int i = 0;
	static int count = 0, star_count = 0;
	static SIZE size;
	static int change = 0;
	static int sort_count = 0;
	static int sort_line = 0;
	switch (IMessage) {
		HDC hdc;
		PAINTSTRUCT ps;
	case WM_CREATE:
		srand((unsigned)time(NULL));
		CreateCaret(hWnd, NULL, 5, 15);
		ShowCaret(hWnd);
		break;
	case WM_CHAR:
		hdc = GetDC(hWnd);
		if (wParam == VK_BACK)
		{
			for (int a = 0; a<count; ++a)
			{
				str[i][a] = str[i][a + 1];
			}
			count--;
		}
		else if (wParam == VK_RETURN)
		{
			i++;
			count = 0;
		}
		else if (wParam == '1')
		{
			x = rand() % 600;
			y = rand() % 400;
		}
		else if (wParam == '2')
		{
			for (int a = 0; a < 10; ++a)
			{
				for (int b = 0; b < 30; ++b)
				{
					if (str[a][b] >= 65 && str[a][b] <= 90)
					{
						str[a][b] += 32;
					}
					else if (str[a][b] >= 97 && str[a][b] <= 122)
					{
						str[a][b] -= 32;
					}
				}
			}
		}
		else if (wParam == '3')
		{
			if (change == 0)
			{
				change = 1;
			}
			else
			{
				int b = 0;
				for (int line = 0; line <= i; ++line)
				{
					for (int a = 0; a < strlen(str[line]) ; ++a)
					{
						if (str[line][a] == '*')
						{
							str[line][a] = star[b];
							b++;
						}
					}
				}
				for (int a = 0; a < star_count; ++a)
				{
					star[a] = '\0';
				}
				star_count = 0;
				change = 0;
			}
		}
		else if (wParam == '4')
		{
			if (sort_count == 0)
			{
				sort_count = 1;
				int num = 0;
				for (int line = 0; line < 10; ++line)
				{
					for (int a = 0; a < 30; ++a)
					{
						if (str[line][a] != '\0')
						{
							sort[sort_line][num] = str[line][a];
							num++;
							if (num == 9)
							{
								num = 0;
								sort_line++;
							}
						}
						else
						{
							break;
						}
					}
				}
			}
			else
			{
				sort_count = 0;
				for (int line = 0; line < sort_line; ++line)
				{
					for (int a = 0; a < 10; ++a)
					{
						sort[line][a] = '\0';
					}
				}
				sort_line = 0;
			}
		}
		else
		{
			if (change == 1)
			{
				star[star_count++] = wParam;
				str[i][count++] = '*';
				star[star_count] = '\0';
				sort_count = 0;
			}
			else
			{
				str[i][count++] = wParam;
				sort_count = 0;
			}

			if (count == 29)
			{
				count = 0;
				i++;
			}
		}
		str[i][count] = '\0';
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (sort_count == 0)
		{
			GetTextExtentPoint(hdc, str[i], strlen(str[i]), &size);
			for (int line = 0; line <= i; ++line)
			{
				TextOut(hdc, x, y + (line * 20), str[line], strlen(str[line]));
			}
			SetCaretPos(x + size.cx, y + (i * 20));
		}
		else
		{
			GetTextExtentPoint(hdc, sort[sort_line], strlen(sort[sort_line]), &size);
			for (int line = 0; line <= sort_line; ++line)
			{
				TextOut(hdc, x, y + (line * 20), sort[line], strlen(sort[line]));
			}
			SetCaretPos(x + size.cx, y + (sort_line * 20));
		}
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