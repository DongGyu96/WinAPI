#include <windows.h>
#include <stdlib.h>
#include <time.h>
#define speed1 6
#define speed2 4
#define speed3 8

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
	static RECT rt;
	static int x1[3], x2[3], y1[3], y2[3];
	static int width[3] = { 1 };
	static int ax1[3], ax2[3], ay1[3], ay2[3];
	static bool sign = false;
	HBRUSH hBrush, oldBrush;
	HPEN hPen, oldPen;
	HDC hdc;
	PAINTSTRUCT ps;
	switch (IMessage) {
	case WM_CREATE:
		srand((unsigned)time(NULL));
		GetClientRect(hWnd, &rt);
		x1[0] = 0;
		y1[0] = 200;
		x2[0] = 150;
		y2[0] = 300;
		x1[1] = rt.right - 150;
		y1[1] = 350;
		x2[1] = rt.right;
		y2[1] = 450;
		x1[2] = 0;
		y1[2] = 500;
		x2[2] = 150;
		y2[2] = 600;

		ax1[0] = 0;
		ay1[0] = 200;
		ax2[0] = 0;
		ay2[0] = 300;
		ax1[1] = rt.right;
		ay1[1] = 350;
		ax2[1] = rt.right;
		ay2[1] = 450;
		ax1[2] = 0;
		ay1[2] = 500;
		ax2[2] = 0;
		ay2[2] = 600;
		SetTimer(hWnd, 1, 16, NULL);
		break;
	case WM_CHAR:
		if (wParam == 'b' || wParam == 'B')
		{
			width[2] = 5;
			width[1] = 1;
			width[0] = 1;
			sign = true;
		}
		else if (wParam == 'y' || wParam == 'Y')
		{
			width[1] = 5;
			width[0] = 1;
			width[2] = 1;
			SetTimer(hWnd, 2, 3000, NULL);
		}
		else if (wParam == 'r' || wParam == 'R')
		{
			width[0] = 5;
			width[1] = 1;
			width[2] = 1;
			sign = false;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_TIMER:
		if (sign)
		{
			switch (wParam) {
			case 1:
				x1[0] += speed1;
				x2[0] += speed1;
				if (x2[0] > rt.right)
				{
					ax2[0] = x2[0] - rt.right;
					if (x1[0] > rt.right)
					{
						ax2[0] = 0;
						x1[0] = 0;
						x2[0] = 150;
					}
				}

				x1[1] -= speed2;
				x2[1] -= speed2;
				if (x1[1] < 0)
				{
					ax1[1] = rt.right + x1[1];
					if (x2[1] < 0)
					{
						ax1[1] = rt.right;
						x1[1] = rt.right - 150;
						x2[1] = rt.right;
					}
				}

				x1[2] += speed3;
				x2[2] += speed3;
				if (x2[2] > rt.right)
				{
					ax2[2] = x2[2] - rt.right;
					if (x1[2] > rt.right)
					{
						ax2[2] = 0;
						x1[2] = 0;
						x2[2] = 150;
					}
				}
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			case 2:
				sign = false;
				width[1] = 1;
				KillTimer(hWnd, 2);
				break;
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		else
		{
			width[2] = 1;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		Rectangle(hdc, rt.right - 300, 0, rt.right, 100);

		hPen = CreatePen(PS_SOLID, width[0], RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		hBrush = CreateSolidBrush(RGB(220, 0, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Ellipse(hdc, rt.right - 300, 0, rt.right - 200, 100);
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);

		hPen = CreatePen(PS_SOLID, width[1], RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		hBrush = CreateSolidBrush(RGB(255, 255, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Ellipse(hdc, rt.right - 200, 0, rt.right - 100, 100);
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);

		hPen = CreatePen(PS_SOLID, width[2], RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		hBrush = CreateSolidBrush(RGB(50, 250, 150));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Ellipse(hdc, rt.right - 100, 0, rt.right, 100);
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);

		hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		hBrush = CreateSolidBrush(RGB(150, 150, 250));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		for (int i = 0; i < 3; ++i)
		{
			Rectangle(hdc, x1[i], y1[i], x2[i], y2[i]);
		}

		for (int i = 0; i < 3; ++i)
		{
			Rectangle(hdc, ax1[i], ay1[i], ax2[i], ay2[i]);
		}
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, IMessage, wParam, lParam);
}