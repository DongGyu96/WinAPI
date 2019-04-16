#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

typedef struct PIC {
	int x1, x2, y1, y2;
	int red, blue, green;
	int stat, shape;
}PIC;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	HINSTANCE g_hInst;//
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = "API Programming";
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	RECT rcWindow = { 0, 0, 900, 700 };
	AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, false);
	hWnd = CreateWindow("API Programming", "API Programming", WS_OVERLAPPEDWINDOW, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	HPEN hPen, oldPen;
	static RECT rt;
	static PIC pic[20];
	static int red, blue, green, oldX, oldY, endX, endY, startX, startY, count, stat;
	static BOOL Line, Rect, Elli, Drag, Help;
	switch (iMsg) {
	case WM_CREATE:
		GetClientRect(hWnd, &rt);
		srand((unsigned)time(NULL));
		Line = false;
		Rect = false;
		Elli = false;
		Drag = false;
		Help = false;
		red = 255;
		blue = 255;
		green = 255;
		stat = 1;
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_SHAPE_LINE:
			Line = true;
			Rect = false;
			Elli = false;
			break;
		case ID_SHAPE_RECTANGLE:
			Rect = true;
			Line = false;
			Elli = false;
			break;
		case ID_SHAPE_ELLIPSE:
			Elli = true;
			Line = false;
			Rect = false;
			break;
		case ID_COLOR_RED:
			red = 255;
			green = 0;
			blue = 0;
			break;
		case ID_COLOR_GREEN:
			red = 0;
			green = 255;
			blue = 0;
			break;
		case ID_COLOR_YELLOW:
			red = 255;
			green = 255;
			blue = 0;
			break;
		case ID_COLOR_RANDOM:
			red = rand() % 256;
			green = rand() % 256;
			blue = rand() % 256;
			break;
		case ID_BORDER_ON:
			stat = 2;
			break;
		case ID_BORDER_OFF:
			stat = 1;
			break;
		case ID_CHANGE_SHRINK:
			for (int i = 0; i < count; ++i)
			{
				pic[i].x1 += pic[i].x1 / 100 * 5;
				pic[i].y1 += pic[i].y1 / 100 * 5;
				pic[i].x2 -= pic[i].x2 / 100 * 5;
				pic[i].y2 -= pic[i].y2 / 100 * 5;
			}
			InvalidateRect(hWnd, NULL, true);
			break;
		case ID_CHANGE_EXPAND:
			for (int i = 0; i < count; ++i)
			{
				pic[i].x1 -= pic[i].x1 / 100 * 5;
				pic[i].y1 -= pic[i].y1 / 100 * 5;
				pic[i].x2 += pic[i].x2 / 100 * 5;
				pic[i].y2 += pic[i].y2 / 100 * 5;
			}
			InvalidateRect(hWnd, NULL, true);
			break;
		case ID_HELP_HELP:
			if (Help)
			{
				Help = false;
			}
			else
			{
				Help = true;
			}
			InvalidateRect(hWnd, NULL, true);
			break;
		case ID_HELP_EXIT:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		if (Rect || Elli)
		{
			oldX = startX = LOWORD(lParam);
			oldY = startY = HIWORD(lParam);
			Drag = true;
		}
		else if (Line)
		{
			oldX = LOWORD(lParam);
			oldY = HIWORD(lParam);
			Drag = true;
		}
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);
		if (Line && Drag)
		{
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
			MoveToEx(hdc, oldX, oldY, NULL);
			LineTo(hdc, endX, endY);
			oldX = endX;
			oldY = endY;
		}
		else if (Rect && Drag)
		{
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);

			hBrush = CreateSolidBrush(RGB(255,255,255));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			hPen = CreatePen(PS_SOLID, stat, RGB(255, 255, 255));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			Rectangle(hdc, startX, startY, oldX, oldY);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);

			hBrush = CreateSolidBrush(RGB(red, green, blue));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			hPen = CreatePen(PS_SOLID, stat, RGB(0, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			Rectangle(hdc, startX, startY, endX, endY);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);
			oldX = endX;
			oldY = endY;
		}
		else if (Elli && Drag)
		{
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);

			hBrush = CreateSolidBrush(RGB(255, 255, 255));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			hPen = CreatePen(PS_SOLID, stat, RGB(255, 255, 255));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			Ellipse(hdc, startX, startY, oldX, oldY);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);

			hBrush = CreateSolidBrush(RGB(red, green, blue));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			hPen = CreatePen(PS_SOLID, stat, RGB(0, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			Ellipse(hdc, startX, startY, endX, endY);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);
			oldX = endX;
			oldY = endY;
		}
		break;
	case WM_LBUTTONUP:
		if (Rect || Elli)
		{
			if (Rect)
			{
				pic[count].shape = 1;
			}
			else if(Elli)
			{
				pic[count].shape = 2;
			}
			pic[count].x1 = startX;
			pic[count].x2 = endX;
			pic[count].y1 = startY;
			pic[count].y2 = endY;
			pic[count].red = red;
			pic[count].green = green;
			pic[count].blue = blue;
			count++;
		}
		Line = false;
		Rect = false;
		Elli = false;
		Drag = false;
		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		for (int i = 0; i < count; ++i)
		{
			if (pic[i].shape == 1)
			{
				hBrush = CreateSolidBrush(RGB(pic[i].red, pic[i].green, pic[i].blue));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				hPen = CreatePen(PS_SOLID, pic[i].stat, RGB(0, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, hPen);

				Rectangle(hdc, pic[i].x1, pic[i].y1, pic[i].x2, pic[i].y2);

				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
				SelectObject(hdc, oldPen);
				DeleteObject(hPen);
			}
			else if(pic[i].shape == 2)
			{
				hBrush = CreateSolidBrush(RGB(pic[i].red, pic[i].green, pic[i].blue));
				oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
				hPen = CreatePen(PS_SOLID, pic[i].stat, RGB(0, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, hPen);

				Ellipse(hdc, pic[i].x1, pic[i].y1, pic[i].x2, pic[i].y2);

				SelectObject(hdc, oldBrush);
				DeleteObject(hBrush);
				SelectObject(hdc, oldPen);
				DeleteObject(hPen);
			}
		}

		if (Help)
		{
			TextOut(hdc, rt.right / 3, 0, "윈플 연습문제 4.3", 17);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}