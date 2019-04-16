#include <windows.h>
#define width 80

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
	static int x1 = width * 2, x2= width * 3, y1 = width * 2, y2 = width * 3;
	static int change = 0;
	static int red = 0, blue = 255, green = 255;
	static bool flag;
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	switch (IMessage) {
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
	{
		hdc = BeginPaint(hWnd, &ps);
		if (wParam == VK_UP)
		{
			y1 -= width;
			y2 -= width;
			if (y1 == width * 0)
			{
				y1 = width * 4;
				y2 = width * 5;
			}
		}
		else if (wParam == VK_LEFT)
		{
			x1 -= width;
			x2 -= width;
			if (x1 == width * 0)
			{
				x1 = width * 4;
				x2 = width * 5;
			}
		}
		else if (wParam == VK_DOWN)
		{
			y1 += width;
			y2 += width;
			if (y1 == width * 5)
			{
				y1 = width * 1;
				y2 = width * 2;
			}
		}
		else if (wParam == VK_RIGHT)
		{
			x1 += width;
			x2 += width;
			if (x1 == width * 5)
			{
				x1 = width * 1;
				x2 = width * 2;
			}
		}
		else if (wParam == 'R' || wParam == 'r')
		{
			if (change == 0)
			{
				change = 1;
			}
			else
			{
				change = 0;
			}
		}
		flag = true;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	}
	case WM_KEYUP:
		flag = false;
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				Rectangle(hdc, width*(j+1), width*(i+1), width*(j+2), width*(i+2));
			}
		}
		if (flag)
		{
			red = 255;
			blue = 0;
			green = 0;
		}
		else
		{
			red = 0;
			blue = 255;
			green = 255;
		}
		hBrush = CreateSolidBrush(RGB(red,green,blue));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

		if (change == 0)
		{
			Rectangle(hdc, x1, y1, x2, y2);
		}
		else
		{
			Ellipse(hdc, x1, y1, x2, y2);
		}
		SelectObject(hdc, oldBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, IMessage, wParam, lParam);
}