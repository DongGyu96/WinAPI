#include <windows.h>
#include <atlimage.h>
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = "윈플 문제 6-2번";

int move, speed, jump, charater, size, MoveStr, Check;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Dlg6_1Proc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

void Move_Animation(int, int, HDC);
void Jump_Animation(int, int, HDC);
void Monster_Animation(int, int, HDC);
void Explosion_Animation(int, int, HDC);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
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
	WndClass.lpszMenuName = NULL;  WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, 0, 0, 1000, 600,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	PAINTSTRUCT ps;
	static HDC hDC, mem1DC;
	static CImage ground, sky;
	static RECT rect;
	static BOOL Boom;
	static HBITMAP hBit;
	static HINSTANCE hlnstance;
	static int x, y, mx, my, Sx1, Sx2, Gx1, Gx2, explosion, Ex, count, flag, strX;
	static char str[100];

	switch (iMessage) {
	case WM_CREATE:
		SetTimer(hWnd, 1, 100, NULL);
		Gx2 = 60;
		Gx1 = 0;
		Sx1 = 0;
		Sx2 = 500;
		GetClientRect(hWnd, &rect);
		x = 50;
		y = rect.bottom - 250;
		mx = rect.right - 160;
		my = rect.bottom - 280;
		flag = 1;
		speed = 10;
		size = 80;
		MoveStr = 0;
		Check = 0;
		ground.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		sky.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		mem1DC = CreateCompatibleDC(hDC);
		SelectObject(mem1DC, hBit);

		BitBlt(hDC, 0, 0, rect.right, rect.bottom, mem1DC, 0, 0, SRCCOPY);

		wsprintf(str, "윈플 연습문제 6.2");
		TextOut(hDC, strX, 0, str, strlen(str));
		DeleteDC(mem1DC);
		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		DialogBox(hlnstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dlg6_1Proc);
		break;

	case WM_TIMER:
		hDC = GetDC(hWnd);
		if (hBit == NULL) {
			hBit = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
		}
		if (wParam == 1) {
			Gx1 += 5;
			Gx2 += 5;
			Sx1 += 30;
			Sx2 += 30;
			mx -= 5;
			if (Gx2 > ground.GetWidth()) {
				Gx1 = 0;
				Gx2 = 60;
			}
			if (Sx2 > sky.GetWidth()) {
				Sx1 = 0;
				Sx2 = 500;
			}
			if (mx < 0) {
				mx = rect.right - 160;
			}
		}
		if (MoveStr == 1) {
			strX += 10;
			if (strX >= 800)
				strX = 0;
		}
		mem1DC = CreateCompatibleDC(hDC);

		SelectObject(mem1DC, hBit);

		sky.StretchBlt(mem1DC, 0, 0, rect.right, rect.bottom, Sx1, 0, Sx2 - Sx1, sky.GetHeight(), SRCCOPY);
		ground.TransparentBlt(mem1DC, 0, 0, rect.right, rect.bottom, Gx1, 0, Gx2 - Gx1, ground.GetHeight(), RGB(255, 255, 255));
		Monster_Animation(mx, my, mem1DC);

		if (((x + 80 > mx && x + 80 < mx + 140) || (x > mx && x < mx + 140)) && (y > my && y < my + 110)) {
			explosion = 0;
			Ex = mx - 40;
			Boom = TRUE;
		}
		if (Boom)
		{
			explosion++;
			if (explosion == 8)
			{
				Boom = FALSE;
			}
			Explosion_Animation(Ex, my, mem1DC);
		}
		if (move == 1) {
			x += speed;
		}
		else if (move == 2) {
			x -= speed;
		}
		if (speed <= 0) {
			move = 0;
			speed = 0;
		}

		if (jump == 1) {
			if (count < 4) {
				y -= 30;
				++count;
			}
			else if (count >= 4) {
				y += 30;
				++count;
				if (count == 8) {
					count = 0;
					jump = 0;
				}
			}
			Jump_Animation(x, y, mem1DC);
		}
		else {
			Move_Animation(x, y, mem1DC);
		}

		DeleteDC(mem1DC);
		ReleaseDC(hWnd, hDC);
		InvalidateRgn(hWnd, NULL, false);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

BOOL CALLBACK Dlg6_1Proc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	HDC hDC;
	PAINTSTRUCT ps;

	switch (iMessage) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			jump = 1;
			InvalidateRect(hDlg, NULL, TRUE);
			break;

		case IDOK2:
			move = 1;
			InvalidateRect(hDlg, NULL, TRUE);
			break;

		case IDOK3:
			move = 2;
			InvalidateRect(hDlg, NULL, TRUE);
			break;

		case IDOK5:
			speed += 5;
			break;

		case IDOK4:
			speed -= 5;
			break;

		case IDC_CHECK1:
			++MoveStr;
			if (MoveStr == 2)
				MoveStr = 0;
			InvalidateRect(hDlg, NULL, TRUE);
			break;

		case IDC_CHECK2:
			++Check;
			if (Check == 2)
				Check = 0;
			InvalidateRect(hDlg, NULL, TRUE);
			break;


		case IDC_RADIO1:
			charater = 1;
			InvalidateRect(hDlg, NULL, TRUE);
			break;

		case IDC_RADIO2:
			charater = 2;
			InvalidateRect(hDlg, NULL, TRUE);
			break;

		case IDC_RADIO3:
			size = 100;
			InvalidateRect(hDlg, NULL, TRUE);
			break;

		case IDC_RADIO4:
			size = 60;
			InvalidateRect(hDlg, NULL, TRUE);
			break;

		case IDC_RADIO5:
			size = 80;
			InvalidateRect(hDlg, NULL, TRUE);
			break;

		case IDCANCEL:
			PostQuitMessage(0);
			break;
		}
		break;
	}
	return 0;
}

void Move_Animation(int x, int y, HDC mem1DC)
{
	CImage Run1[8];
	static int move1_count;
	CImage Run2[7];
	static int move2_count;


	if (charater == 1) {
		++move1_count;
		move1_count = move1_count % 8;
		Run1[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
		Run1[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
		Run1[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
		Run1[3].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));
		Run1[4].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));
		Run1[5].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));
		Run1[6].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP9));
		Run1[7].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP10));

		Run1[move1_count].TransparentBlt(mem1DC, x, y, size, size, 0, 0, Run1[move1_count].GetWidth(), Run1[move1_count].GetHeight(), RGB(10, 0, 0));
	}
	else if (charater == 2) {
		++move2_count;
		move2_count = move2_count % 7;
		Run2[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP30));
		Run2[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP31));
		Run2[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP32));
		Run2[3].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP33));
		Run2[4].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP34));
		Run2[5].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP35));
		Run2[6].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP36));

		Run2[move2_count].TransparentBlt(mem1DC, x, y, size, size, 0, 0, Run2[move2_count].GetWidth(), Run2[move2_count].GetHeight(), RGB(0, 0, 0));
	}
}

void Jump_Animation(int x, int y, HDC mem1DC)
{
	CImage Jump1[8];
	static int jump1_count;
	CImage Jump2[7];
	static int jump2_count;

	if (charater == 1) {
		++jump1_count;
		jump1_count = jump1_count % 8;
		Jump1[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));
		Jump1[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12));
		Jump1[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13));
		Jump1[3].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP14));
		Jump1[4].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP15));
		Jump1[5].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP16));
		Jump1[6].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP17));
		Jump1[7].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP18));

		Jump1[jump1_count].TransparentBlt(mem1DC, x, y, size, size, 0, 0, Jump1[jump1_count].GetWidth(), Jump1[jump1_count].GetHeight(), RGB(10, 0, 0));
	}
	else if (charater == 2) {
		++jump2_count;
		jump2_count = jump2_count % 7;
		Jump2[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP30));
		Jump2[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP31));
		Jump2[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP32));
		Jump2[3].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP33));
		Jump2[4].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP34));
		Jump2[5].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP35));
		Jump2[6].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP36));

		Jump2[jump2_count].TransparentBlt(mem1DC, x, y, size, size, 0, 0, Jump2[jump2_count].GetWidth(), Jump2[jump2_count].GetHeight(), RGB(0, 0, 0));
	}
}

void Monster_Animation(int x, int y, HDC mem1DC)
{
	CImage Monster[3];
	static int monster_count;
	CImage Explosion[8];
	static int explosion_count;

	++monster_count;
	monster_count = monster_count % 3;
	Monster[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP19));
	Monster[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP20));
	Monster[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP21));

	if (Check == 1) {
		++explosion_count;
		explosion_count = explosion_count % 8;
		Explosion[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP22));
		Explosion[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP23));
		Explosion[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP24));
		Explosion[3].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP25));
		Explosion[4].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP26));
		Explosion[5].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP27));
		Explosion[6].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP28));
		Explosion[7].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP29));
		Explosion[explosion_count].TransparentBlt(mem1DC, x + 120, y, 240, 110, 0, 0, Explosion[explosion_count].GetWidth(), Explosion[explosion_count].GetHeight(), RGB(255, 255, 255));
	}

	Monster[monster_count].TransparentBlt(mem1DC, x, y, 140, 110, 0, 0, Monster[monster_count].GetWidth(), Monster[monster_count].GetHeight(), RGB(250, 235, 215));
}

void Explosion_Animation(int x, int y, HDC mem1DC)
{
	CImage Explosion[8];
	static int explosion_count;

	++explosion_count;
	explosion_count = explosion_count % 8;
	Explosion[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP22));
	Explosion[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP23));
	Explosion[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP24));
	Explosion[3].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP25));
	Explosion[4].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP26));
	Explosion[5].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP27));
	Explosion[6].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP28));
	Explosion[7].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP29));

	Explosion[explosion_count].TransparentBlt(mem1DC, x, y, 240, 120, 0, 0, Explosion[explosion_count].GetWidth(), Explosion[explosion_count].GetHeight(), RGB(255, 255, 255));
}