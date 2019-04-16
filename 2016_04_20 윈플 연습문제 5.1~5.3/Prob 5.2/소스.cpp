#include <windows.h>
#include "resource.h"
#define speed 5

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
	static HBITMAP hBitmap, oldBitmap;
	static BITMAP bmp;
	static int mWidth, mHeight, width, height;
	static int stat = 0;
	static int startX = 0, startY = 0, endX = 0, endY = 0;
	static double X, Y, X1, Y1;
	static HPEN hPen, oldPen;
	static HBRUSH hBrush;
	static BOOL drag, start, copy, copy_start;
	HDC hdc, memdc;
	PAINTSTRUCT ps;
	switch (IMessage) {
	case WM_CREATE:
		start = TRUE;
		drag = FALSE;
		copy = FALSE;
		copy_start = FALSE;
		break;
	case WM_LBUTTONDOWN:
		if (start)
		{
			startX = LOWORD(lParam);
			startY = HIWORD(lParam);
			drag = TRUE;
		}
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);
		if (drag)
		{
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
			hBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
			Rectangle(hdc, startX, startY, endX, endY);
			DeleteObject(hBrush);
		}
		break;
	case WM_LBUTTONUP:
		if (start)
		{
			drag = FALSE;
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
			start = FALSE;
			InvalidateRect(hWnd, NULL, TRUE);
		}
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

		width = endX - startX;
		height = endY - startY;
		X = (double)startX / (double)rt.right;
		Y = (double)startY / (double)rt.bottom;
		X1 = (double)width / (double)rt.right;
		Y1 = (double)height / (double)rt.bottom;
		width = mWidth * X1;
		height = mHeight * Y1;
		X = mWidth * X;
		Y = mHeight * Y;

		StretchBlt(hdc, 0, 0, rt.right, rt.bottom, memdc, 0, 0, mWidth, mHeight, SRCCOPY);

		hPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		hBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
		Rectangle(hdc, startX, startY, endX, endY);
		DeleteObject(hBrush);
		if (stat == 0)
		{
			StretchBlt(hdc, startX, startY, endX - startX, endY - startY, memdc, X, Y, width, height, SRCCOPY);
		}
		else if (stat == 1)
		{
			width = width / 10 * 8;
			height = height / 10 * 8;
			StretchBlt(hdc, startX, startY, endX - startX, endY - startY, memdc, X, Y, width, height, SRCCOPY);
		}
		else if (stat == 2)
		{
			width = width / 10 * 6;
			height = height / 10 * 6;
			StretchBlt(hdc, startX, startY, endX - startX, endY - startY, memdc, X, Y, width, height, SRCCOPY);
		}
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);

		if (copy)
		{
			StretchBlt(hdc, 0, 0, endX - startX, endY - startY, memdc, X, Y, width, height, SRCCOPY);
		}

		SelectObject(memdc, hBitmap);
		DeleteObject(hBitmap);
		DeleteDC(memdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_UP)
		{
			startY -= speed;
			endY  -= speed;
		}
		else if (wParam == VK_DOWN)
		{
			startY += speed;
			endY += speed;
		}
		else if (wParam == VK_RIGHT)
		{
			startX += speed;
			endX += speed;
		}
		else if (wParam == VK_LEFT)
		{
			startX -= speed;
			endX -= speed;
		}
		else if (wParam == 48)
		{
			stat = 0;
		}
		else if (wParam == 49)
		{
			stat = 1;
		}
		else if (wParam == 50)
		{
			stat = 2;
		}
		else if (wParam == 'C' || wParam == 'c')
		{
			if (!start)
			{
				copy_start = TRUE;
			}
		}
		else if (wParam == 'P' || wParam == 'p')
		{
			if (copy_start)
			{
				copy = TRUE;
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, IMessage, wParam, lParam);
}
