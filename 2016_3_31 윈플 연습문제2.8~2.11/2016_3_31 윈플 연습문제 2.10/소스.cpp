#include <windows.h>
#include <time.h>
#include <stdlib.h>
#define ARRAY_SIZE 5

typedef struct Shape {
	int stat;
	int x1;
	int y1;
	int x2;
	int y2;
	int red, green, blue;
}Shape;

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
	static int red = 0;
	static int select = 0;
	static Shape shape[ARRAY_SIZE] = { 0,0,0,0,0,0,0,0 };
	static POINT point[ARRAY_SIZE][6] = {0};
	static int count = 0;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	HDC hdc;
	PAINTSTRUCT ps;
	switch (IMessage) {
	case WM_CREATE:
		srand((unsigned)time(NULL));
		ShowCaret(hWnd);
		break;
	case WM_CHAR:
		if (wParam == 'e')
		{
			shape[count].stat = 1;
			shape[count].x1 = rand() % 900;
			shape[count].y1 = rand() % 600;
			shape[count].x2 = rand() % 900;
			shape[count].y2 = rand() % 600;
			shape[count].red = rand() % 256;
			shape[count].green = rand() % 256;
			shape[count].blue = rand() % 256;
			count++;
		}
		else if (wParam == 't')
		{
			shape[count].stat = 2;
			for (int n = 0; n < 3; ++n)
			{
				point[count][n] = { rand() % 900, rand() % 600 };
			}
			shape[count].red = rand() % 256;
			shape[count].green = rand() % 256;
			shape[count].blue = rand() % 256;
			count++;
		}
		else if (wParam == 'r')
		{
			shape[count].stat = 3;
			shape[count].x1 = rand() % 900;
			shape[count].y1 = rand() % 600;
			shape[count].x2 = rand() % 900;
			shape[count].y2 = rand() % 600;
			shape[count].red = rand() % 256;
			shape[count].green = rand() % 256;
			shape[count].blue = rand() % 256;
			count++;
		}
		else if (wParam >= 49 && wParam <= 53)
		{
			select = wParam - 48;
		}
		else if (wParam == VK_BACK)
		{
			if (select >= 1 && select <= 5)
			{
				shape[select - 1].stat = 0;
				count = select - 1;
			}
		}
		if (count == 5)
		{
			count = 0;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (int i = 0; i < ARRAY_SIZE; ++i)
		{
			int width = 1;
			hBrush = CreateSolidBrush(RGB(shape[i].red, shape[i].green, shape[i].blue));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			if (i == select - 1)
			{
				red = 255;
				width = 5;
			}
			hPen = CreatePen(PS_SOLID, width, RGB(red,0,0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			if (shape[i].stat != 0)
			{
				if (shape[i].stat == 1)
				{
					Ellipse(hdc, shape[i].x1, shape[i].y1, shape[i].x2, shape[i].y2);
				}
				else if (shape[i].stat == 2)
				{
					Polygon(hdc, point[i], 3);
				}
				else if (shape[i].stat == 3)
				{
					Rectangle(hdc, shape[i].x1, shape[i].y1, shape[i].x2, shape[i].y2);
				}
				red = 0;
				width = 0;
			}
			else
			{
				continue;
			}
		}
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, IMessage, wParam, lParam);
}