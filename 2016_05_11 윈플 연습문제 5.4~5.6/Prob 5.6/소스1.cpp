#include <windows.h>
#include "resource.h"
#include <atlimage.h>

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
		lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 800, NULL, (HMENU)NULL, hInstance, NULL);

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
	static CImage img;
	static RECT puzzle[5][5];
	static int x, y, a, b, n1, n2;
	static HBRUSH h;
	static BOOL stat[5][5] = { FALSE };
	static HBITMAP hBit;
	static HDC hdc, mem1dc;
	PAINTSTRUCT ps;
	switch (IMessage) {
	case WM_CREATE:
		GetClientRect(hWnd, &rt);
		SetTimer(hWnd, 1, 160, NULL);
		img.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				puzzle[i][j].left = (rt.right / 3) * j;
				puzzle[i][j].right = (rt.right / 3) * (j + 1);
				puzzle[i][j].top = (rt.bottom / 3) * i;
				puzzle[i][j].bottom = (rt.bottom / 3) * (i + 1);
			}
		}

		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rt);
		mem1dc = CreateCompatibleDC(hdc);
		SelectObject(mem1dc, hBit);
		// BitBlt(hdc, 0, 0, rt.right, rt.bottom, mem1dc, 0, 0, SRCCOPY);
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				img.StretchBlt(hdc, puzzle[i][j].left, puzzle[i][j].top, rt.right / 3, rt.bottom / 3, puzzle[i][j].left, puzzle[i][j].top, rt.right / 3, rt.bottom / 3, SRCCOPY);
				FrameRect(hdc, &puzzle[i][j], h);
			}
		}
		

		DeleteDC(mem1dc);
		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		hdc = GetDC(hWnd);
		if (hBit == NULL)
		{
			hBit = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
		}
		mem1dc = CreateCompatibleDC(hdc);
		SelectObject(mem1dc, hBit);

		// img.StretchBlt(mem1dc, 0, 0, rt.bottom, rt.bottom, 0, 0, img.GetWidth(), img.GetHeight(), SRCCOPY);

		DeleteDC(mem1dc);
		ReleaseDC(hWnd, hdc);
		InvalidateRgn(hWnd, NULL, false);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, IMessage, wParam, lParam);
}