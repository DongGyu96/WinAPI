#include <windows.h>
#include "resource.h"
#include <atlimage.h>
#define Gspeed 1
#define Sspeed 15

HINSTANCE g_hInst;
LPCTSTR lpszClass = "window program";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

void Move_Animation(int, int, HDC);
void Jump_Animation(int, int, HDC);
void Monster_Animation(int, int, HDC);
void Explosion_Animation(int, int, HDC);

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
	static int Gx1, Gx2, Sx1, Sx2, x, y, stat, count, Mx, My, explosion, Ex;
	static CImage ground, sky;
	static HBITMAP hBit;
	static HDC hdc, mem1dc, mem2dc;
	static BOOL Boom;
	PAINTSTRUCT ps;
	switch (IMessage) {
	case WM_CREATE:
		Gx2 = 60;
		Gx1 = 0;
		Sx1 = 0;
		Sx2 = 500;
		Boom = FALSE;
		GetClientRect(hWnd, &rt);
		x = 50;
		y = rt.bottom - 250;
		Mx = rt.right - 160;
		My = rt.bottom - 280;
		ground.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		sky.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		SetTimer(hWnd, 1, 100, NULL);
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
		if (wParam == 1)
		{
			Gx1 += Gspeed;
			Gx2 += Gspeed;
			Sx1 += Sspeed;
			Sx2 += Sspeed;
			Mx -= 10;
			if (Gx2 > ground.GetWidth())
			{
				Gx1 = 0;
				Gx2 = 60;
			}
			if (Sx2 > sky.GetWidth())
			{
				Sx1 = 0;
				Sx2 = 500;
			}
			if (Mx < 0)
			{
				Mx = rt.right - 160;
			}
		}
		mem1dc = CreateCompatibleDC(hdc);

		SelectObject(mem1dc, hBit);

		sky.StretchBlt(mem1dc, 0, 0, rt.right, rt.bottom, Sx1, 0, Sx2 - Sx1, sky.GetHeight(), SRCCOPY);
		ground.TransparentBlt(mem1dc, 0, 0, rt.right, rt.bottom, Gx1, 0, Gx2 - Gx1, ground.GetHeight(), RGB(255,255,255));
		Monster_Animation(Mx, My, mem1dc);		
		if (((x + 80 > Mx && x + 80 < Mx + 140) || (x > Mx && x < Mx + 140)) && (y > My && y < My + 110))
		{
			
			explosion = 0;
			Ex = Mx - 40;
			Boom = TRUE;
		}
		if (Boom)
		{
			explosion++;
			if (explosion == 8)
			{
				Boom = FALSE;
			}
			Explosion_Animation(Ex, My, mem1dc);
		}
		if (stat == 1)
		{
			if (count < 4)
			{
				y -= 20;
				count++;
			}
			else if (count >= 4)
			{
				y += 20;
				count++;
				if (count == 8)
				{
					stat = 0;
					count = 0;
				}
			}
			Jump_Animation(x, y, mem1dc);
		}
		else 
		{
			Move_Animation(x, y, mem1dc);
		}

		DeleteDC(mem1dc);
		ReleaseDC(hWnd, hdc);
		InvalidateRgn(hWnd, NULL, false);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_DOWN || wParam == VK_UP || wParam == VK_LEFT || wParam == VK_RIGHT)
		{
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
				if (y > rt.bottom - 350)
					y -= 5;
			}
			else if (wParam == VK_DOWN)
			{
				if (y < rt.bottom - 200)
				{
					y += 5;
				}
			}
		}
		else if (wParam == VK_SPACE)
		{
			stat = 1;
		}
		InvalidateRgn(hWnd, NULL, false);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, IMessage, wParam, lParam);
}

void Move_Animation(int x, int y, HDC mem1dc)
{
	CImage Run[8];
	static int move_count;

	move_count++;
	move_count = move_count % 8;
	Run[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
	Run[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
	Run[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
	Run[3].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));
	Run[4].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));
	Run[5].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));
	Run[6].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP9));
	Run[7].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP10));
	
	Run[move_count].TransparentBlt(mem1dc, x, y, 80, 80, 0, 0, Run[move_count].GetWidth(), Run[move_count].GetHeight(), RGB(10, 0, 0));
	//	StretchBlt(mem1dc, x, y, 80, 80, mem2dc, 0, 0, width, height, SRCCOPY);
}

void Jump_Animation(int x, int y, HDC mem1dc)
{
	CImage Jump[8];
	static int jump_count;

	jump_count++;
	jump_count = jump_count % 8;
	Jump[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));
	Jump[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12));
	Jump[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13));
	Jump[3].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP14));
	Jump[4].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP15));
	Jump[5].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP16));
	Jump[6].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP17));
	Jump[7].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP18));

	Jump[jump_count].TransparentBlt(mem1dc, x, y, 80, 80, 0, 0, Jump[jump_count].GetWidth(), Jump[jump_count].GetHeight(), RGB(10, 0, 0));
	//	StretchBlt(mem1dc, x, y, 80, 80, mem2dc, 0, 0, width, height, SRCCOPY);
}

void Monster_Animation(int x, int y, HDC mem1dc)
{
	CImage Monster[3];
	static int monster_count;

	monster_count++;
	monster_count = monster_count % 3;
	Monster[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP19));
	Monster[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP20));
	Monster[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP21));

	Monster[monster_count].TransparentBlt(mem1dc, x, y, 140, 110, 0, 0, Monster[monster_count].GetWidth(), Monster[monster_count].GetHeight(), RGB(250,235,215));
}

void Explosion_Animation(int x, int y, HDC mem1dc)
{
	CImage Explosion[8];
	static int explosion_count;

	explosion_count++;
	explosion_count = explosion_count % 8;
	Explosion[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP22));
	Explosion[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP23));
	Explosion[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP24));
	Explosion[3].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP25));
	Explosion[4].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP26));
	Explosion[5].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP27));
	Explosion[6].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP28));
	Explosion[7].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP29));

	Explosion[explosion_count].TransparentBlt(mem1dc, x, y, 240, 120, 0, 0, Explosion[explosion_count].GetWidth(), Explosion[explosion_count].GetHeight(), RGB(255,255,255));
	//	StretchBlt(mem1dc, x, y, 80, 80, mem2dc, 0, 0, width, height, SRCCOPY);
}