#include <windows.h>
#include "resource.h"
#include <atlimage.h>
#include <math.h>
#define speed 5
#define PI 3.141592

HINSTANCE g_hInst;
LPCTSTR lpszClass = "window program";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

int score;

void Move_Right_Animation(int, int, HDC, BOOL, int);
void Move_Left_Animation(int, int, HDC, BOOL, int);
void Coin_Animation(int, int, HDC);
void Goal_Coin_Animation(int, int, HDC);
void Portal_Animation(int, int, HDC);

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
		lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, 0, 0, 1500, 900, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdshow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT IMessage, WPARAM wParam, LPARAM lParam) {
	static RECT rt, rc;
	static CImage background, station, land;
	static RECT block[6], Coin[6], Goal_block[3], Goal_Coin, player;
	static HBITMAP hBit;
	static HDC hdc, mem1dc;
	static BOOL block_move[6], Move, Right, Turn, Drop, Player[6], look_coin[7];
	static HPEN hPen, oldPen;
	static char text[30];
	static HINSTANCE hlnstance;
	static int x, y, startX, startY, endX, endY, turnX, turnY, radian, r, move_count;
	PAINTSTRUCT ps;
	switch (IMessage) {
	case WM_CREATE:
		GetClientRect(hWnd, &rt);
		background.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		station.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		land.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
		for (int i = 0; i < 6; ++i)
		{
			block_move[i] = true;
			Player[i] = false;
		}
		score = 0;
		Goal_block[0].left = 0;
		Goal_block[0].right = 360;
		Goal_block[0].top = 720;
		Goal_block[0].bottom = 770;
		Goal_block[1].left = 0;
		Goal_block[1].right = 50;
		Goal_block[1].top = 640;
		Goal_block[1].bottom = 770;
		Goal_block[2].left = 310;
		Goal_block[2].right = 360;
		Goal_block[2].top = 640;
		Goal_block[2].bottom = 770;
		Goal_Coin.left = 140;
		Goal_Coin.right = 220;
		Goal_Coin.bottom = 720;
		Goal_Coin.top = 640;

		block[0].left = 0;
		block[0].top = 200;
		block[0].right = 160;
		block[0].bottom = 250;
		Coin[0].left = 60;
		Coin[0].right = 100;
		Coin[0].bottom = 200;
		Coin[0].top = 160;

		block[1].left = 300;
		block[1].top = 400;
		block[1].right = 460;
		block[1].bottom = 450;
		Coin[1].left = 360;
		Coin[1].right = 400;
		Coin[1].bottom = 400;
		Coin[1].top = 360;


		block[2].left = 800;
		block[2].top = 400;
		block[2].right = 960;
		block[2].bottom = 450;
		Coin[2].left = 860;
		Coin[2].right = 900;
		Coin[2].bottom = 400;
		Coin[2].top = 360;


		block[3].left = 1000;
		block[3].top = 600;
		block[3].right = 1160;
		block[3].bottom = 650;
		Coin[3].left = 1060;
		Coin[3].right = 1100;
		Coin[3].bottom = 600;
		Coin[3].top = 560;


		block[4].left = 900;
		block[4].top = 100;
		block[4].right = 1060;
		block[4].bottom = 150;
		Coin[4].left = 960;
		Coin[4].right = 1000;
		Coin[4].bottom = 100;
		Coin[4].top = 60;


		block[5].left = 1200;
		block[5].top = 300;
		block[5].right = 1360;
		block[5].bottom = 350;
		Coin[5].left = 1260;
		Coin[5].right = 1300;
		Coin[5].bottom = 300;
		Coin[5].top = 260;
		
		x = rt.right / 2 - 25;
		y = 200;
		startX = endX = x + 25;
		startY = endY = 275;
		radian = 0;
		Move = false;
		Right = true;
		Turn = false;
		Drop = false;
		for (int i = 0; i < 7; ++i)
		{
			look_coin[i] = true;
		}
		SetTimer(hWnd, 1, 50, NULL);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_RIGHT)
		{
			if (Move)
			{
				if (((startX + speed >= 0 && startX + speed <= 310) || (startX + speed >= 360 && startX + speed <= rt.right)) || startY < 650) 
				{
					x += speed;
					endX += speed;
					startX += speed;
					move_count++;
					move_count = move_count % 5;
					Right = true;
					Drop = true;
				}
			}
			else
			{
				if (!Turn && !Drop)
				{
					endX += speed;
					turnX = endX;
					radian = 0;
					if (endX > startX)
						r = endX - startX;
				}
				else if (Drop)
				{
					x += speed;
					endX += speed;
					startX += speed;
				}
			}
		}
		else if (wParam == VK_LEFT)
		{
			if (Move)
			{
				if (((startX - speed < 310 && startX - speed > 0) || (startX - speed > 360 && startX - speed < rt.right)) || startY < 650)
				{
					x -= speed;
					endX -= speed;
					startX -= speed;
					move_count++;
					move_count = move_count % 5;
					Right = false;
					Drop = true;
				}
			}
			else
			{
				if (!Turn && !Drop)
				{
					endX -= speed;
					turnY = endY;
					radian = 180;
					if (endX < startX)
						r = startX - endX;
				}
				else if (Drop)
				{
					x -= speed;
					endX -= speed;
					startX -= speed;
				}
			}
		}
		else if (wParam == VK_DOWN || wParam == VK_UP)
		{
			if(!Move)
				Turn = true;
		}

		if (wParam == VK_SPACE)
		{
			if (Move)
			{
				y -= speed * 3;
				startY -= speed * 3;
				endY -= speed * 3;
				Drop = true;
			}
			else
			{
				Drop = true;
				Turn = false;
				endX = startX = x + 25;
				endY = startY = y + 75;
				radian = 0;
			}
		}
		InvalidateRgn(hWnd, NULL, false);
		break;
	case WM_KEYUP:
		move_count = 0;
		InvalidateRgn(hWnd, NULL, false);
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

		background.StretchBlt(mem1dc, 0, 0, rt.right, rt.bottom, 0, 0, background.GetWidth(), background.GetHeight(), SRCCOPY);
		station.TransparentBlt(mem1dc, 0, 0, rt.right, rt.bottom, 0, 0, station.GetWidth(), station.GetHeight(), RGB(255, 0, 255));
		for (int i = 0; i < 6; ++i)
		{
			land.StretchBlt(mem1dc, block[i].left, block[i].top, block[i].right - block[i].left, block[i].bottom - block[i].top, 0, 0, land.GetWidth(), land.GetHeight(), SRCCOPY);
			if(look_coin[i])
				Coin_Animation(Coin[i].left, Coin[i].top, mem1dc);
		}
		if(look_coin[6])
			Goal_Coin_Animation(Goal_Coin.left, Goal_Coin.top, mem1dc);
		for (int i = 0; i < 3; ++i)
		{
			land.StretchBlt(mem1dc, Goal_block[i].left, Goal_block[i].top, Goal_block[i].right - Goal_block[i].left, Goal_block[i].bottom - Goal_block[i].top, 0, 0, land.GetWidth(), land.GetHeight(), SRCCOPY);
		}
		wsprintf(text, "스코어 : %d", score);
		TextOut(mem1dc, 0, 0, text, strlen(text));
		player.left = x;
		player.right = x + 50;
		player.top = y;
		player.bottom = y + 75;

		if (Right)
			Move_Right_Animation(x, y, mem1dc, Move, move_count);
		else
			Move_Left_Animation(x, y, mem1dc, Move, move_count);

		Portal_Animation(800, 680, mem1dc);

		hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
		oldPen = (HPEN)SelectObject(mem1dc, hPen);
		MoveToEx(mem1dc, startX, startY, NULL);
		LineTo(mem1dc, endX, endY);
		SelectObject(mem1dc, oldPen);
		DeleteObject(hPen);
		if (Turn)
		{
			endX = startX + cos((radian*PI) / 180.0) * r;
			endY = startY + sin((radian*PI) / 180.0) * r;
			x = endX - 25;
			y = endY - 75;
			radian += speed;
		}
		for (int i = 0; i < 6; ++i)
		{
			Player[i] = false;
		}

		for (int i = 0; i < 6; ++i)
		{
			if (startX >= block[i].left && startX <= block[i].right&& startY >= block[i].top - 10 && startY <= block[i].top + 5)
			{
				Drop = false;
				Player[i] = true;
				Move = false;
			}
		}
		for (int i = 0; i < 3; ++i)
		{
			if (startX >= Goal_block[i].left && startX <= Goal_block[i].right && startY >= Goal_block[i].top - 10 && startY <= Goal_block[i].top + 5)
			{
				Drop = false;
				Move = true;
			}
		}
		if (startY >= 760)
		{
			Drop = false;
			Move = true;
		}

		if (Drop)
		{
			y += speed * 2;
			endY += speed * 2;
			startY += speed * 2;
		}

		if (block_move[0])
		{
			block[0].left += speed;
			block[0].right += speed;
			Coin[0].left += speed;
			Coin[0].right += speed;
			if (block[0].right == 350)
				block_move[0] = false;
			if (Player[0] && !Turn)
			{
				x += speed;
				endX += speed;
				startX += speed;
			}
		}
		else
		{
			block[0].left -= speed;
			block[0].right -= speed;
			Coin[0].left -= speed;
			Coin[0].right -= speed;
			if (block[0].left == 0)
				block_move[0] = true;
			if (Player[0] && !Turn)
			{
				x -= speed;
				endX -= speed;
				startX -= speed;
			}
		}
		if (block_move[1])
		{
			block[1].top += speed;
			block[1].bottom += speed;
			Coin[1].top += speed;
			Coin[1].bottom += speed;
			if (block[1].bottom == 600)
				block_move[1] = false;
			if (Player[1] && !Turn)
			{
				y += speed;
				endY += speed;
				startY += speed;
			}
		}
		else
		{
			block[1].top -= speed;
			block[1].bottom -= speed;
			Coin[1].top -= speed;
			Coin[1].bottom -= speed;
			if (block[1].bottom == 250)
				block_move[1] = true;
			if (Player[1] && !Turn)
			{
				y -= speed;
				endY -= speed;
				startY -= speed;
			}
		}

		if (block_move[2])
		{
			block[2].left += speed;
			block[2].right += speed;
			Coin[2].left += speed;
			Coin[2].right += speed;
			if (block[2].right == 1200)
				block_move[2] = false;
			if (Player[2] && !Turn)
			{
				x += speed;
				endX += speed;
				startX += speed;
			}

		}
		else
		{
			block[2].left -= speed;
			block[2].right -= speed;
			Coin[2].left -= speed;
			Coin[2].right -= speed;
			if (block[2].left == 700)
				block_move[2] = true;
			if (Player[2] && !Turn)
			{
				x -= speed;
				endX -= speed;
				startX -= speed;
			}
		}

		if (block_move[3])
		{
			block[3].left += speed;
			block[3].right += speed;
			Coin[3].left += speed;
			Coin[3].right += speed;
			if (block[3].left == 1200)
				block_move[3] = false;
			if (Player[3] && !Turn)
			{
				x += speed;
				endX += speed;
				startX += speed;
			}
		}
		else
		{
			block[3].left -= speed;
			block[3].right -= speed;
			Coin[3].left -= speed;
			Coin[3].right -= speed;
			if (block[3].left == 800)
				block_move[3] = true;
			if (Player[3] && !Turn)
			{
				x -= speed;
				endX -= speed;
				startX -= speed;
			}
		}

		if (block_move[4])
		{
			block[4].left -= speed;
			block[4].right -= speed;
			Coin[4].left -= speed;
			Coin[4].right -= speed;
			if (block[4].left == 300)
				block_move[4] = false;
			if (Player[4] && !Turn)
			{
				x -= speed;
				endX -= speed;
				startX -= speed;
			}
		}
		else
		{
			block[4].left += speed;
			block[4].right += speed;
			Coin[4].left += speed;
			Coin[4].right += speed;
			if (block[4].left == 800)
				block_move[4] = true;
			if (Player[4] && !Turn)
			{
				x += speed;
				endX += speed;
				startX += speed;
			}
		}

		if (block_move[5])
		{
			block[5].top -= speed;
			block[5].bottom -= speed;
			Coin[5].top -= speed;
			Coin[5].bottom -= speed;
			if (block[5].top == 100)
				block_move[5] = false;
			if (Player[5] && !Turn)
			{
				y -= speed;
				endY -= speed;
				startY -= speed;
			}
		}
		else
		{
			block[5].top += speed;
			block[5].bottom += speed;
			Coin[5].top += speed;
			Coin[5].bottom += speed;
			if (block[5].bottom == 500)
				block_move[5] = true;
			if (Player[5] && !Turn)
			{
				y += speed;
				endY += speed;
				startY += speed;
			}
		}

		if (!Turn)
		{
			for (int i = 0; i < 6; ++i)
			{
				if (IntersectRect(&rc, &player, &Coin[i]))
				{
					score += 10;
					Coin[i].left = 0;
					Coin[i].right = 0;
					Coin[i].top = 0;
					Coin[i].bottom = 0;
					look_coin[i] = false;
				}
			}
			if (IntersectRect(&rc, &player, &Goal_Coin))
			{
				score += 100;
				Goal_Coin.left = 0;
				Goal_Coin.right = 0;
				Goal_Coin.top = 0;
				Goal_Coin.bottom = 0;
				look_coin[6] = false;
			}
			if (startX > 810 && startX < 890 && startY > 720 && startY < 780)
			{
				x = rt.right / 2 - 25;
				y = 200;
				startX = endX = x + 25;
				startY = endY = 275;
				radian = 0;
				Move = false;
				Right = true;
				Turn = false;
				Drop = false;
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

void Move_Right_Animation(int x, int y, HDC mem1dc, BOOL Move, int right_count)
{
	CImage Run[5];

	Run[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP19));
	Run[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP20));
	Run[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP21));
	Run[3].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP22));
	Run[4].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP23));

	if (Move)
		Run[right_count].TransparentBlt(mem1dc, x, y, 50, 75, 0, 0, Run[right_count].GetWidth(), Run[right_count].GetHeight(), RGB(255, 0, 255));
	else
		Run[0].TransparentBlt(mem1dc, x, y, 50, 75, 0, 0, Run[0].GetWidth(), Run[0].GetHeight(), RGB(255, 0, 255));
}

void Move_Left_Animation(int x, int y, HDC mem1dc, BOOL Move, int left_count)
{
	CImage Run[5];

	Run[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP24));
	Run[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP25));
	Run[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP26));
	Run[3].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP27));
	Run[4].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP28));

	if (Move)
		Run[left_count].TransparentBlt(mem1dc, x, y, 50, 75, 0, 0, Run[left_count].GetWidth(), Run[left_count].GetHeight(), RGB(255, 0, 255));
	else
		Run[0].TransparentBlt(mem1dc, x, y, 50, 75, 0, 0, Run[0].GetWidth(), Run[0].GetHeight(), RGB(255, 0, 255));
}

void Coin_Animation(int x, int y, HDC mem1dc)
{
	CImage Coin[15];
	static int coin_count;

	coin_count++;
	coin_count = coin_count % 15;
	Coin[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
	Coin[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
	Coin[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));
	Coin[3].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));
	Coin[4].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));
	Coin[5].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP9));
	Coin[6].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP10));
	Coin[7].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));
	Coin[8].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12));
	Coin[9].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13));
	Coin[10].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP14));
	Coin[11].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP15));
	Coin[12].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP16));
	Coin[13].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP17));
	Coin[14].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP18));

	Coin[coin_count].TransparentBlt(mem1dc, x, y, 40, 40, 0, 0, Coin[coin_count].GetWidth(), Coin[coin_count].GetHeight(), RGB(255, 0, 255));
}

void Goal_Coin_Animation(int x, int y, HDC mem1dc)
{
	CImage Coin[15];
	static int goal_coin_count;

	goal_coin_count++;
	goal_coin_count = goal_coin_count % 15;
	Coin[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
	Coin[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
	Coin[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));
	Coin[3].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));
	Coin[4].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));
	Coin[5].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP9));
	Coin[6].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP10));
	Coin[7].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));
	Coin[8].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12));
	Coin[9].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13));
	Coin[10].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP14));
	Coin[11].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP15));
	Coin[12].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP16));
	Coin[13].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP17));
	Coin[14].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP18));

	Coin[goal_coin_count].TransparentBlt(mem1dc, x, y, 80, 80, 0, 0, Coin[goal_coin_count].GetWidth(), Coin[goal_coin_count].GetHeight(), RGB(255, 0, 255));
}

void Portal_Animation(int x, int y, HDC mem1dc)
{
	CImage Portal[11];
	static int portal_count;

	portal_count++;
	portal_count = portal_count % 11;
	Portal[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP29));
	Portal[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP30));
	Portal[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP31));
	Portal[3].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP32));
	Portal[4].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP33));
	Portal[5].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP34));
	Portal[6].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP35));
	Portal[7].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP36));
	Portal[8].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP37));
	Portal[9].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP38));
	Portal[10].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP39));

	Portal[portal_count].TransparentBlt(mem1dc, x, y, 100, 80, 0, 0, Portal[portal_count].GetWidth(), Portal[portal_count].GetHeight(), RGB(255, 0, 255));
}

BOOL CALLBACK DialogProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HDC hDC;
	PAINTSTRUCT ps;
	char Text[100];

	switch (iMessage) {
	case WM_INITDIALOG:
		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			EndDialog(hDlg, 0);
			PostQuitMessage(0);
			break;
		case IDOK:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	case WM_PAINT:
		hDC = BeginPaint(hDlg, &ps);
		wsprintf(Text, "현재 스코어는 %d 입니다.", score);
		TextOut(hDC, 0, 0, Text, strlen(Text));
		EndPaint(hDlg, &ps);
		break;
	}
	return 0;
}