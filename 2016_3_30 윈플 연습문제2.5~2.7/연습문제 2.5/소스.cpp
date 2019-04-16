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
	static char str[10][100];
	static int i = 0;
	static int count = 0;
	static SIZE size;
	static int home = 0;
	switch (IMessage) {
		HDC hdc;
		PAINTSTRUCT ps;
	case WM_CREATE:
		CreateCaret(hWnd, NULL, 5, 15);
		ShowCaret(hWnd);
		break;
	case WM_CHAR:
		hdc = GetDC(hWnd);
		if (wParam == VK_BACK)
		{
			count--;
			if (count == 0)
			{
				i--;
				count = strlen(str[i]);
			}
		}
		else if (wParam == VK_RETURN)
		{
			i++;
			count = 0;
		}
		else if (wParam == VK_ESCAPE)
		{
			for (int j = 0; j < i; ++j)
			{
				for (int cnt = 0; cnt < count; ++cnt)
				{
					str[j][cnt] = '\0';
				}
			}
			count = 0;
			i = 0;
		}
		else if (wParam == VK_TAB)
		{
			for (int j = 0; j < 4; ++j)
			{
				str[i][count++] = ' ';
			}
			str[i][count] = '\0';
		}
		else if (wParam == '.')
		{
			home++;
		}
		else
		{
			str[i][count++] = wParam;
		}
		str[i][count] = '\0';
		InvalidateRect(hWnd, NULL, TRUE);
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetTextExtentPoint(hdc, str[i], strlen(str[i]), &size);
		for (int line = 0; line <= i; ++line)
		{
			TextOut(hdc, 0, line*20, str[line], strlen(str[line]));
		}
		if (home == 0)
		{
			SetCaretPos(size.cx, i * 20);
		}
		else
		{
			SetCaretPos(0, i * 20);
			home = 0;
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