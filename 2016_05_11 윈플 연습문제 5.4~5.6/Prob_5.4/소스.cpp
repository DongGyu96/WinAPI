#include <windows.h>
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = "window program";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

void Move_Animation(int, int, HDC, HDC, BOOL);
void Jump_Animation(int, int, HDC, HDC);
void Click_Animation(int, int, HDC, HDC);

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
	static int x, y, mWidth, mHeight, x1, y1;
	static int stat = 0;
	static int count;
	static int count1;
	static BITMAP bmp;
	static HBITMAP hBitmap1, hBitmap2, oldBitmap1, oldBitmap2;
	static BOOL MOVE, JUMP, CLICK;
	static HDC hdc, mem1dc, mem2dc;
	PAINTSTRUCT ps;
	switch (IMessage) {
	case WM_CREATE:
		GetClientRect(hWnd, &rt);
		MOVE = CLICK = JUMP = FALSE;
		SetTimer(hWnd, 1, 100, NULL);
		hBitmap2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		GetObject(hBitmap2, sizeof(BITMAP), &bmp);
		mWidth = bmp.bmWidth;
		mHeight = bmp.bmHeight;
		x = 100;
		y = mHeight - 150;
		break;
	case WM_LBUTTONDOWN:
		x1 = LOWORD(lParam);
		y1 = HIWORD(lParam);

		if (x1 > x && x1 < x+80 && y1 > y && y1 < y + 80)
		{
			stat = 2;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rt);
		mem1dc = CreateCompatibleDC(hdc);
		oldBitmap1 = (HBITMAP)SelectObject(mem1dc, hBitmap1);

		BitBlt(hdc, 0, 0, rt.right, rt.bottom, mem1dc, 0, 0, SRCCOPY);


		SelectObject(mem1dc, oldBitmap1);
		DeleteDC(mem2dc);
		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		hdc = GetDC(hWnd);
		if (hBitmap1 == NULL)
		{
			hBitmap1 = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
		}
		mem1dc = CreateCompatibleDC(hdc);
		mem2dc = CreateCompatibleDC(mem1dc);

		oldBitmap1 = (HBITMAP)SelectObject(mem1dc, hBitmap1);
		oldBitmap2 = (HBITMAP)SelectObject(mem2dc, hBitmap2);

		StretchBlt(mem1dc, 0, 0, rt.right, rt.bottom, mem2dc, 0, 0, mWidth, mHeight, SRCCOPY);
		if (stat == 0)
		{
			Move_Animation(x, y, mem1dc, hdc, MOVE);
		}
		else if (stat == 2)
		{
			count++;
			if (count == 5)
			{
				stat = 0;
				count = 0;
			}
			x -= 5;
			y -= 5;
			Click_Animation(x, y, mem1dc, hdc);
		}
		else if (stat == 1)
		{
			if (count < 4)
			{
				y -= 10;
				count++;
			}
			else if (count == 4)
			{
				y += 10;
				count1++;
				if (count1 == 4)
				{
					stat = 0;
					count = 0;
					count1 = 0;
				}
			}
			Jump_Animation(x, y, mem1dc, hdc);
		}

		SelectObject(mem2dc, oldBitmap2);
		DeleteDC(mem2dc);
		SelectObject(mem1dc, oldBitmap1);
		DeleteDC(mem1dc);
		ReleaseDC(hWnd, hdc);
		InvalidateRgn(hWnd, NULL, false);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_DOWN || wParam == VK_UP || wParam == VK_LEFT || wParam == VK_RIGHT)
		{
			MOVE = TRUE;
			if (wParam == VK_LEFT)
			{
				if (x > 0)
				{
					x -= 5;
				}
			}
			else if (wParam == VK_RIGHT)
			{
				x += 5;
			}
			else if (wParam == VK_UP)
			{
				if (y > 0)
					y -= 5;
			}
			else if (wParam == VK_DOWN)
			{
				y += 5;
			}
		}
		else if (wParam == 'j' || wParam == 'J')
		{
			stat = 1;
		}
		InvalidateRgn(hWnd, NULL, false);
		break;
	case WM_KEYUP:
		MOVE = FALSE;
		InvalidateRgn(hWnd, NULL, false);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, IMessage, wParam, lParam);
}

void Move_Animation(int x, int y, HDC mem1dc, HDC hdc, BOOL MOVE)
{
	HDC mem2dc;
	HBITMAP RunBit[8];
	BITMAP bmp;
	static int move_count;
	int i, width, height;

	move_count++;
	move_count = move_count % 8;
	RunBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
	RunBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
	RunBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
	RunBit[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
	RunBit[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));
	RunBit[5] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));
	RunBit[6] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));
	RunBit[7] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP9));

	GetObject(RunBit[move_count], sizeof(BITMAP), &bmp);
	width = bmp.bmWidth;
	height = bmp.bmHeight;

	mem2dc = CreateCompatibleDC(mem1dc);

	if (MOVE)
	{
		SelectObject(mem2dc, RunBit[move_count]);
	}
	else SelectObject(mem2dc, RunBit[0]);
	TransparentBlt(mem1dc, x, y, 80, 80, mem2dc, 0, 0, width, height, RGB(10, 0, 0));
	//	StretchBlt(mem1dc, x, y, 80, 80, mem2dc, 0, 0, width, height, SRCCOPY);
	 
	for (i = 0; i < 8; i++)  
		DeleteObject(RunBit[i]);
	DeleteDC(mem2dc);
}

void Jump_Animation(int x, int y, HDC mem1dc, HDC hdc)
{
	HDC mem2dc;
	HBITMAP JumpBit[8];
	BITMAP bmp;
	static int jump_count;
	int i, width, height;

	jump_count++;
	jump_count = jump_count % 8;
	JumpBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP10));
	JumpBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));
	JumpBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12));
	JumpBit[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13));
	JumpBit[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP14));
	JumpBit[5] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP15));
	JumpBit[6] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP16));
	JumpBit[7] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP17));

	GetObject(JumpBit[0], sizeof(BITMAP), &bmp);
	width = bmp.bmWidth;
	height = bmp.bmHeight;

	mem2dc = CreateCompatibleDC(mem1dc);

	SelectObject(mem2dc, JumpBit[jump_count]);
	TransparentBlt(mem1dc, x, y, 80, 80, mem2dc, 0, 0, width, height, RGB(10, 0, 0));

	for (i = 0; i < 8; i++)
		DeleteObject(JumpBit[i]);
	DeleteDC(mem2dc);
}

void Click_Animation(int x, int y, HDC mem1dc, HDC hdc)
{
	HDC mem2dc;
	HBITMAP ClickBit[5];
	BITMAP bmp;
	static int click_count;
	int i, width, height;

	click_count++;
	click_count = click_count % 5;
	ClickBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP18));
	ClickBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP19));
	ClickBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP20));
	ClickBit[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP21));
	ClickBit[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP22));

	GetObject(ClickBit[click_count], sizeof(BITMAP), &bmp);
	width = bmp.bmWidth;
	height = bmp.bmHeight;

	mem2dc = CreateCompatibleDC(mem1dc);

	SelectObject(mem2dc, ClickBit[click_count]);
	TransparentBlt(mem1dc, x, y, 80, 80, mem2dc, 0, 0, width, height, RGB(10, 0, 0));

	for (i = 0; i < 5; i++)
		DeleteObject(ClickBit[i]);
	DeleteDC(mem2dc);
}