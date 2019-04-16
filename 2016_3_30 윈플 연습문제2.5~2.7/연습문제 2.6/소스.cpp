#include <windows.h>

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
	static char str[10][11];
	static int i = 0;
	static int count = 0;
	static int cnt = 0;
	static SIZE size;
	switch (IMessage) {
		HDC hdc;
		PAINTSTRUCT ps;
	case WM_CREATE:
		CreateCaret(hWnd, NULL, 5, 15);
		ShowCaret(hWnd);
		break;
	case WM_CHAR:
		hdc = GetDC(hWnd);
		if (wParam >= 48 && wParam <= 57)
		{
			if (count == 0)
			{
				str[i][count++] = wParam;
				str[i][count] = '\0';
				i++;
				count = 0;
			}
			else
			{
				count = 0;
				i++;
				str[i][count++] = wParam;
				str[i][count] = '\0';
				i++;
				count = 0;
			}
		}
		else if (wParam == 'r' || wParam == 'R')
		{
			if (cnt == 0)
			{
				cnt = 1;
			}
			else
			{
				cnt = 0;
			}
		}
		else if (wParam == 'q' || wParam == 'Q')
		{
			PostQuitMessage(0);
			break;
		}
		else
		{
			str[i][count++] = wParam;
			str[i][count] = '\0';
		}
		if (count == 10)
		{
			count = 0;
			i++;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (cnt == 0)
		{
			for (int line = 0; line <= i; ++line)
			{
				if (str[line][0] >= 48 && str[line][0] <= 57)
				{
					SetTextColor(hdc, RGB(255, 0, 0));
					TextOut(hdc, 0, line * 20, str[line], strlen(str[line]));
				}
				else
				{
					SetTextColor(hdc, RGB(0, 0, 0));
					TextOut(hdc, 0, line * 20, str[line], strlen(str[line]));
				}
			}
			GetTextExtentPoint(hdc, str[i], strlen(str[i]), &size);
		}
		else
		{
			int y = 0;
			for (int line = i; line >= 0; --line)
			{
				if (str[line][0] >= 48 && str[line][0] <= 57)
				{
					SetTextColor(hdc, RGB(255, 0, 0));
					TextOut(hdc, 0, y * 20, str[line], strlen(str[line]));
				}
				else
				{
					SetTextColor(hdc, RGB(0, 0, 0));
					TextOut(hdc, 0, y * 20, str[line], strlen(str[line]));
				}
				y++;
			}
			GetTextExtentPoint(hdc, str[0], strlen(str[0]), &size);
		}
		SetCaretPos(size.cx, i * 20);
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