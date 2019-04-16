#include <windows.h>
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = "window program";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

typedef struct Board {
	int x1, x2, y1, y2, stat;
}Board;

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
		lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 700, NULL, (HMENU)NULL, hInstance, NULL);

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
	static HBITMAP hBitmap, oldBitmap;
	static BITMAP bmp;
	static int mWidth, mHeight, width, height, stat;
	static int startX = 0, startY = 0, endX = 0, endY = 0, x, y, drawX, drawY, oldX, oldY;
	static double wx, wy, wx1, wy1;
	static Board board[3][3];
	static Board draw[5][5];
	static HPEN hPen, oldPen;
	static HBRUSH hBrush;
	static BOOL drag, start;
	HDC hdc, memdc;
	PAINTSTRUCT ps;
	switch (IMessage) {
	case WM_CREATE:
		start = FALSE;
		drag = FALSE;
		stat = 3;
		GetClientRect(hWnd, &rt);
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				board[i][j].x1 = rt.right/6 * i;
				board[i][j].y1 = rt.bottom/3 * j;
				board[i][j].x2 = rt.right / 6 * (i+1);
				board[i][j].y2 = rt.bottom / 3 * (j+1);
				board[i][j].stat = 0;
				draw[i][j].x1 = board[i][j].x1 + rt.right/2;
				draw[i][j].y1 = board[i][j].y1;
				draw[i][j].x2 = board[i][j].x2 + rt.right/2;
				draw[i][j].y2 = board[i][j].y2;
				draw[i][j].stat = 0;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		if (start)
		{
			startX = LOWORD(lParam);
			startY = HIWORD(lParam);
			oldX = startX;
			oldY = startY;
			if (startX > rt.right / 2)
			{
				drag = TRUE;
			}
		}
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);
		if (drag)
		{
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
			if (endX > rt.right / 2)
			{
				hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
				oldPen = (HPEN)SelectObject(hdc, hPen);
				Rectangle(hdc, startX, startY, oldX, oldY);
				SelectObject(hdc, oldPen);
				DeleteObject(hPen);
				Rectangle(hdc, startX, startY, endX, endY);
				oldX = endX;
				oldY = endY;
			}
		}
		break;
	case WM_LBUTTONUP:
		if (start)
		{
			drag = FALSE;
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
			start = FALSE;
			draw[0][0].x1 = startX;
			draw[0][0].y1 = startY;
			draw[0][0].x2 = endX;
			draw[0][0].y2 = endY;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		else
		{
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					if (x > board[i][j].x1 && x<board[i][j].x2 && y > board[i][j].y1 && y < board[i][j].y2)
					{
						for (int a = 0; a < 3; ++a)
						{
							for (int b = 0; b < 3; ++b)
							{
								board[a][b].stat = 0;
							}
						}
						board[i][j].stat = 1;

						width = board[i][j].x2 - board[i][j].x1;
						height = board[i][j].y2 - board[i][j].y1;
						wx = (double)board[i][j].x1 / (double)(rt.right / 2);
						wy = (double)board[i][j].y1 / (double)rt.bottom;
						wx1 = (double)width / (double)(rt.right/2);
						wy1 = (double)height / (double)rt.bottom;
						width = mWidth * wx1;
						height = mHeight * wy1;
						wx = mWidth * wx;
						wy = mHeight * wy;
					}
				}
			}

			for (int i = 0; i < stat; ++i)
			{
				for (int j = 0; j < stat; ++j)
				{
					if (x > draw[i][j].x1 && x<draw[i][j].x2 && y > draw[i][j].y1 && y < draw[i][j].y2)
					{
						for (int a = 0; a < stat; ++a)
						{
							for (int b = 0; b < stat; ++b)
							{
								draw[a][b].stat = 0;
							}
						}
						draw[i][j].stat = 1;
					}
				}
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rt);
		memdc = CreateCompatibleDC(hdc);
		hBitmap = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		oldBitmap = (HBITMAP)SelectObject(memdc, hBitmap);
		GetObject(hBitmap, sizeof(BITMAP), &bmp);
		mWidth = bmp.bmWidth;
		mHeight = bmp.bmHeight;

		StretchBlt(hdc, 0, 0, rt.right/2, rt.bottom, memdc, 0, 0, mWidth, mHeight, SRCCOPY);

		hBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (board[i][j].stat == 0)
				{
					hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
					oldPen = (HPEN)SelectObject(hdc, hPen);
					Rectangle(hdc, board[i][j].x1, board[i][j].y1, board[i][j].x2, board[i][j].y2);
					SelectObject(hdc, oldPen);
					DeleteObject(hPen);
				}
				else if (board[i][j].stat == 1)
				{
					hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
					oldPen = (HPEN)SelectObject(hdc, hPen);
					Rectangle(hdc, board[i][j].x1, board[i][j].y1, board[i][j].x2, board[i][j].y2);
					SelectObject(hdc, oldPen);
					DeleteObject(hPen);
				}
			}
		}

		if (stat == 0)
		{
			StretchBlt(hdc, draw[0][0].x1, draw[0][0].y1, draw[0][0].x2 - draw[0][0].x1, draw[0][0].y2 - draw[0][0].y1, memdc, wx, wy, width, height, SRCCOPY);
			hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			Rectangle(hdc, draw[0][0].x1, draw[0][0].y1, draw[0][0].x2, draw[0][0].y2);
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);
		}
		for (int i = 0; i < stat; ++i)
		{
			for (int j = 0; j < stat; ++j)
			{
				if (draw[i][j].stat == 0)
				{
					hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
					oldPen = (HPEN)SelectObject(hdc, hPen);
					Rectangle(hdc, draw[i][j].x1, draw[i][j].y1, draw[i][j].x2, draw[i][j].y2);
					SelectObject(hdc, oldPen);
					DeleteObject(hPen);
				}
				else if (draw[i][j].stat == 1)
				{
					StretchBlt(hdc, draw[i][j].x1, draw[i][j].y1, draw[i][j].x2 - draw[i][j].x1, draw[i][j].y2 - draw[i][j].y1, memdc, wx, wy, width, height, SRCCOPY);
					hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
					oldPen = (HPEN)SelectObject(hdc, hPen);
					Rectangle(hdc, draw[i][j].x1, draw[i][j].y1, draw[i][j].x2, draw[i][j].y2);
					SelectObject(hdc, oldPen);
					DeleteObject(hPen);
				}
			}
		}
		DeleteObject(hBrush);

		SelectObject(memdc, hBitmap);
		DeleteObject(hBitmap);
		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		if (wParam >= 49 && wParam <= 53)
		{
			stat = wParam - 48;
			for (int i = 0; i < stat; ++i)
			{
				for (int j = 0; j < stat; ++j)
				{
					draw[i][j].x1 = (((rt.right / 2) / stat) * i) + rt.right/2;
					draw[i][j].y1 = rt.bottom / stat * j;
					draw[i][j].x2 = (((rt.right / 2) / stat) * (i + 1)) + rt.right/2;
					draw[i][j].y2 = rt.bottom / stat * (j + 1);
					draw[i][j].stat = 0;
				}
			}
		}
		else if (wParam == 48)
		{
			stat = 0;
			draw[0][0].x1 = 0;
			draw[0][0].y1 = 0;
			draw[0][0].x2 = 0;
			draw[0][0].y2 = 0;
			start = TRUE;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, IMessage, wParam, lParam);
}
