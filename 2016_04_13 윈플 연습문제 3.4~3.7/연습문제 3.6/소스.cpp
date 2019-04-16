#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define num 25
#define SIZE 60

HINSTANCE g_hInst;
LPCTSTR lpszClass = "window program";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

bool collisionEllipseCheck(RECT rc, RECT rc2);
double Lengthpts(int x1, int y1, int x2, int y2)
{
	return (sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1)));
}
BOOL InCircle(int x, int y, int mx, int my)
{
	if (Lengthpts(x, y, mx, my) < SIZE)
	{
		return TRUE;
	}
	else
		return FALSE;
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
		lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 700, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdshow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT IMessage, WPARAM wParam, LPARAM lParam) {
	static RECT one, oldone, rt;
	static RECT obstacle[num];
	static int sign[num] = { 0 };
	static int dist = 0;
	static int count = 0;
	static int mx, my, x, y;
	static char text[30];
	static BOOL Selection;
	HBRUSH hBrush, oldBrush;
	HPEN hPen, oldPen;
	HDC hdc;
	PAINTSTRUCT ps;
	switch (IMessage) {
	case WM_CREATE:
		srand((unsigned)time(NULL));
		GetClientRect(hWnd, &rt);
		one.top = 0;
		one.left = 0;
		one.bottom = SIZE;
		one.right = SIZE;
		oldone.top = 0;
		oldone.left = 0;
		oldone.bottom = SIZE;
		oldone.right = SIZE;

		for (int i = 0; i < num; ++i)
		{
			dist = rand() % 100 + 100;
			obstacle[i].top = rand() % 550;
			obstacle[i].left = rand() % 850;
			obstacle[i].bottom = obstacle[i].top + dist;
			obstacle[i].right = obstacle[i].left + dist;
		}

		Selection = FALSE;
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (InCircle(one.left, one.top, mx, my))
		{
			Selection = TRUE;
		}
		break;
	case WM_LBUTTONUP:
		Selection = FALSE;
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);
		mx = LOWORD(lParam);
		my = HIWORD(lParam);
		if (Selection)
		{
			one.left = mx - (SIZE / 2);
			one.top = my - (SIZE / 2);
			one.bottom = my + (SIZE / 2);
			one.right = mx + (SIZE / 2);
			hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			Ellipse(hdc, oldone.left, oldone.top, oldone.right, oldone.bottom);
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);
		
			Ellipse(hdc, one.left, one.top, one.right, one.bottom);

			for (int i = 0; i < num; ++i)
			{
				if (sign[i] == 0)
				{
					if (collisionEllipseCheck(one, obstacle[i]))
					{
						sign[i] = 1;
						count++;
					}
				}
			}

			for (int i = 0; i < num; ++i)
			{
				if (sign[i] == 0)
				{
					hBrush = CreateSolidBrush(RGB(0, 255, 0));
					oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
					Ellipse(hdc, obstacle[i].left, obstacle[i].top, obstacle[i].right, obstacle[i].bottom);
					SelectObject(hdc, oldBrush);
					DeleteObject(hBrush);
				}
				else
				{
					hBrush = CreateSolidBrush(RGB(255, 0, 0));
					oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
					Ellipse(hdc, obstacle[i].left, obstacle[i].top, obstacle[i].right, obstacle[i].bottom);
					SelectObject(hdc, oldBrush);
					DeleteObject(hBrush);
				}
			}
			oldone.left = one.left;
			oldone.top = one.top;
			oldone.right = one.right;
			oldone.bottom = one.bottom;
			if (one.right >= rt.right && one.left >= rt.bottom)
			{
				Selection = FALSE;
				wsprintf(text, "°ÔÀÓ Á¾·á! ÃÑ Ãæµ¹ È½¼ö´Â = %d", count);
				TextOut(hdc, rt.right / 2 - 100, rt.bottom / 2- 50, text, strlen(text));
			}
		}
		ReleaseDC(hWnd, hdc);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		Ellipse(hdc, one.left, one.top, one.right, one.bottom);

		hBrush = CreateSolidBrush(RGB(0, 255, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		for (int i = 0; i < num; ++i)
		{
			Ellipse(hdc, obstacle[i].left, obstacle[i].top, obstacle[i].right, obstacle[i].bottom);
		}
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

bool collisionEllipseCheck(RECT rc, RECT rc2)
{
	double rc_R = (rc.right - rc.left) / 2;
	double rc2_R = (rc2.right - rc2.left) / 2;
	double rX = (rc.left + (rc.right - rc.left) / 2) - (rc2.left + (rc2.right - rc2.left) / 2);
	double rY = (rc.top + (rc.bottom - rc.top) / 2) - (rc2.top + (rc2.bottom - rc2.top) / 2);
	double length = (rc_R + rc2_R)  * (rc_R + rc2_R);

	if (length >= (rX * rX + rY * rY))
	{
		return true;
	}
	return false;
}
