#include <windows.h>
#include "resource.h"
#include <atlimage.h>
#define speed 20

HINSTANCE g_hInst;
LPCTSTR lpszClass = "window program";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

void Mouse_Animation(int, int, HDC);
void Cat_Animation(int, int, HDC);

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
	static RECT rt, Rect[4];
	static CImage img;
	static int x, y, catX = 100, catY = 50, count;
	static BOOL Click;
	static HBITMAP hBit;
	static HDC hdc, mem1dc;
	static HBRUSH hBrush, oldBrush;
	PAINTSTRUCT ps;
	switch (IMessage) {
	case WM_CREATE:
		GetClientRect(hWnd, &rt);
		SetTimer(hWnd, 1, 100, NULL);
		img.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		Rect[0].left = 200;
		Rect[0].top = 100;
		Rect[0].right = 300;
		Rect[0].bottom = 500;
		Rect[1].left = 900;
		Rect[1].top = 100;
		Rect[1].right = 1000;
		Rect[1].bottom = 500;
		Rect[2].left = 200;
		Rect[2].top = 600;
		Rect[2].right = 1000;
		Rect[2].bottom = 700;
		Rect[3].left = 500;
		Rect[3].top = 150;
		Rect[3].right = 700;
		Rect[3].bottom = 400;
		Click = false;
		break;
	case WM_LBUTTONDOWN:
		y = HIWORD(lParam);
		x = LOWORD(lParam);
		for (int i = 0; i < 4; ++i)
		{
			if (x >= Rect[i].left && x <= Rect[i].right && y >= Rect[i].top && y <= Rect[i].bottom)
			{
				count++;
			}
		}
		if (count == 0)
		{
			Click = true;
		}
		break;
	case WM_MOUSEMOVE:
		if (Click)
		{
			y = HIWORD(lParam);
			x = LOWORD(lParam);
		}
		InvalidateRgn(hWnd, NULL, false);
		break;
	case WM_LBUTTONUP:
		count = 0;
		Click = false;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rt);
		mem1dc = CreateCompatibleDC(hdc);
		SelectObject(mem1dc, hBit);

		BitBlt(hdc, 0, 0, rt.right, rt.bottom, mem1dc, 0, 0, SRCCOPY);

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

		img.StretchBlt(mem1dc, 0, 0, rt.right, rt.bottom, 0, 0, img.GetWidth(), img.GetHeight(), SRCCOPY);

		hBrush = CreateSolidBrush(RGB(220, 190, 130));
		oldBrush = (HBRUSH)SelectObject(mem1dc, hBrush);
		for (int i = 0; i < 4; ++i)
		{
			Rectangle(mem1dc, Rect[i].left, Rect[i].top, Rect[i].right, Rect[i].bottom);
		}
		SelectObject(mem1dc, oldBrush);
		DeleteObject(hBrush);

		if (Click)
		{
			Mouse_Animation(x, y, mem1dc);
			Cat_Animation(catX, catY, mem1dc);
			if (x > catX)
			{
				if (catX + speed > Rect[0].left && catY >= Rect[0].top && catY <= Rect[0].bottom && catX + speed < Rect[0].right)
				{
					if (catY - Rect[0].top > Rect[0].bottom - catY)
					{
						catY += speed;
					}
					else
					{
						catY -= speed;
					}
				}
				else if (catX + speed > Rect[1].left && catY >= Rect[1].top && catY <= Rect[1].bottom && catX + speed < Rect[1].right)
				{
					if (catY - Rect[1].top > Rect[1].bottom - catY)
					{
						catY += speed;
					}
					else
					{
						catY -= speed;
					}
				}
				else if (catX + speed > Rect[2].left && catY >= Rect[2].top && catY <= Rect[2].bottom && catX + speed < Rect[2].right)
				{
					if (catY - Rect[2].top > Rect[2].bottom - catY)
					{
						catY += speed;
					}
					else
					{
						catY -= speed;
					}
				}
				else if (catX + speed > Rect[3].left && catY >= Rect[3].top && catY <= Rect[3].bottom && catX + speed < Rect[3].right)
				{
					if (catY - Rect[3].top > Rect[3].bottom - catY)
					{
						catY += speed;
					}
					else
					{
						catY -= speed;
					}
				}
				else
				{
					catX += speed;
					if (y > catY)
					{
						catY += speed;
					}
					if (y < catY)
					{
						catY -= speed;
					}
				}
			}
			if (x < catX)
			{
				if (catX - speed > Rect[0].left && catY >= Rect[0].top && catY <= Rect[0].bottom && catX - speed < Rect[0].right)
				{
					if (catY - Rect[0].top > Rect[0].bottom - catY)
					{
						catY += speed;
					}
					else
					{
						catY -= speed;
					}
				}
				else if (catX - speed > Rect[1].left && catY >= Rect[1].top && catY <= Rect[1].bottom && catX - speed < Rect[1].right)
				{
					if (catY - Rect[1].top > Rect[1].bottom - catY)
					{
						catY += speed;
					}
					else
					{
						catY -= speed;
					}
				}
				else if (catX - speed > Rect[2].left && catY >= Rect[2].top && catY <= Rect[2].bottom && catX - speed < Rect[2].right)
				{
					if (catY - Rect[2].top > Rect[2].bottom - catY)
					{
						catY += speed;
					}
					else
					{
						catY -= speed;
					}
				}
				else if (catX - speed > Rect[3].left && catY >= Rect[3].top && catY <= Rect[3].bottom && catX - speed < Rect[3].right)
				{
					if (catY - Rect[3].top > Rect[3].bottom - catY)
					{
						catY += speed;
					}
					else
					{
						catY -= speed;
					}
				}
				else
				{
					catX -= speed;
					if (y > catY)
					{
						catY += speed;
					}
					if (y < catY)
					{
						catY -= speed;
					}
				}
			}
		}

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


void Mouse_Animation(int x, int y, HDC mem1dc)
{
	CImage Run[3];
	static int move_count;

	move_count++;
	move_count = move_count % 3;
	Run[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));
	Run[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));
	Run[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));

	Run[move_count].TransparentBlt(mem1dc, x-15, y-15, 30, 30, 0, 0, Run[move_count].GetWidth(), Run[move_count].GetHeight(), RGB(255, 0, 0));
}

void Cat_Animation(int x, int y, HDC mem1dc)
{
	CImage Run[4];
	static int cat_count;

	cat_count++;
	cat_count = cat_count % 4;
	Run[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
	Run[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
	Run[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
	Run[3].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));

	Run[cat_count].TransparentBlt(mem1dc, x - 40, y - 30, 80, 60, 0, 0, Run[cat_count].GetWidth(), Run[cat_count].GetHeight(), RGB(255, 0, 0));
}
