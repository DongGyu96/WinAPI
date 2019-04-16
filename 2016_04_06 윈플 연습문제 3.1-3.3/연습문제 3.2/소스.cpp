#include <Windows.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

HINSTANCE g_hlnst;
LPCTSTR lpszClass = "window program 3 - 2";
void moveToRectangle(struct figure[], int arrNum, int top, int right, int bottom, int left);
void moveToTri(struct figure arr[], int arrNum, int top, int right, int bottom, int left);

LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM IParam);

int WINAPI WinMain(HINSTANCE hlnstance, HINSTANCE hPrevlnstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hlnst = hlnstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hlnstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass,
		WS_OVERLAPPEDWINDOW,
		0, 0, 1200, 800,
		NULL, (HMENU)NULL,
		hlnstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}
typedef struct figure {
	int L;
	int T;
	int B;
	int R;
}figure;
typedef struct Tri {
	int x1;
	int y1;
	int x2;
	int y2;
	int x3;
	int y3;
}Tri;

static bool cC1 = true; // 공전 방향 반대
static bool cC2 = true;
static bool mM = true; //이동 도형 -> 사각형 아님 그반대
static bool rR = true; //배경도형 -> 사각형으로 바뀜 아님 그반대
static bool tT = true; // 배경도형 - > 삼각형으로 바뀜 아님 그반대
static bool nN = true; // 이동 도형 - > 사각형  ( Tri용)

LRESULT CALLBACK WndProc(HWND hWnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	RECT rect;
	HBRUSH hBrush, oldBrush;
	static int wide;
	static int high;
	static int degree1 = 0;
	static int degree2 = 0;
	static int degree3 = 0;
	static int degree4 = 0;
	static figure R[4]; // 배경 사각형
	static Tri T[4][3]; // 배경 삼각형
	static figure rev[4]; // 공전 도형
	static figure ret[4]; // 공전 도형( tri용)

	srand((unsigned int)time(NULL));
	switch (iMessage)
	{
	case WM_CREATE:
		R[0].L = 200;
		R[0].T = 100;
		R[0].R = 400;
		R[0].B = 300;
		R[1].L = 800;
		R[1].T = 100;
		R[1].R = 1000;
		R[1].B = 300;
		R[2].L = 200;
		R[2].T = 500;
		R[2].R = 400;
		R[2].B = 700;
		R[3].L = 800;
		R[3].T = 500;
		R[3].R = 1000;
		R[3].B = 700;
		T[0][0].x1 = 300;
		T[0][0].y1 = 100;
		T[0][1].x2 = 200;
		T[0][1].y2 = 300;
		T[0][2].x3 = 400;
		T[0][2].y3 = 300;
		T[1][0].x1 = 900;
		T[1][0].y1 = 100;
		T[1][1].x2 = 800;
		T[1][1].y2 = 300;
		T[1][2].x3 = 1000;
		T[1][2].y3 = 300;
		T[2][0].x1 = 300;
		T[2][0].y1 = 500;
		T[2][1].x2 = 200;
		T[2][1].y2 = 700;
		T[2][2].x3 = 400;
		T[2][2].y3 = 700;
		T[3][0].x1 = 900;
		T[3][0].y1 = 500;
		T[3][1].x2 = 800;
		T[3][1].y2 = 700;
		T[3][2].x3 = 1000;
		T[3][2].y3 = 700;


		rev[0].L = 190;
		rev[0].T = 80;
		rev[0].R = 230;
		rev[0].B = 120;
		rev[1].L = 800;
		rev[1].T = 90;
		rev[1].R = 820;
		rev[1].B = 110;
		rev[2].L = 195;
		rev[2].T = 485;
		rev[2].R = 225;
		rev[2].B = 515;
		rev[3].L = 770;
		rev[3].T = 470;
		rev[3].R = 830;
		rev[3].B = 530;

		ret[0].L = 290;
		ret[0].T = 90;
		ret[0].R = 310;
		ret[0].B = 110;
		ret[1].L = 880;
		ret[1].T = 80;
		ret[1].R = 920;
		ret[1].B = 120;
		ret[2].L = 285;
		ret[2].T = 485;
		ret[2].R = 315;
		ret[2].B = 515;
		ret[3].L = 875;
		ret[3].T = 475;
		ret[3].R = 925;
		ret[3].B = 525;

		SetTimer(hWnd, 1, 60, NULL);
		SetTimer(hWnd, 2, 80, NULL);
		SetTimer(hWnd, 3, 100, NULL);
		SetTimer(hWnd, 4, 120, NULL);
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_CHAR:
		if (wParam == 'q')
			PostQuitMessage(0);

		if (wParam == 'c' || wParam == 'C')
		{
			if (cC1)
				cC1 = false;
			else
				cC1 = true;
		}

		if (wParam == 'v' || wParam == 'V')
		{
			if (cC2)
				cC2 = false;
			else
				cC2 = true;
		}

		if (wParam == 'm' || wParam == 'M')
		{
			if (mM)
				mM = false;
			else
				mM = true;
		}

		if (wParam == 'r' || wParam == 'R')
		{
			if (rR)
				rR = false;
			else
				rR = true;
		}

		if (wParam == 't' || wParam == 'T')
		{
			if (tT)
				tT = false;
			else
				tT = true;
		}

		if (wParam == 'n' || wParam == 'N')
		{
			if (nN)
				nN = false;
			else
				nN = true;
		}

		InvalidateRect(hWnd, NULL, false);
		break;
	case WM_TIMER:
		switch (wParam) {
		case 1:
			if (rR == false)
				moveToRectangle(rev, 0, 100, 400, 300, 200);
			if (rR == true)
			{
				degree1 += 3;
			}
			if (cC2 == false)
				degree1 -= 6;
			if (tT == false)
			{
				moveToTri(ret, 0, 100, 400, 300, 200);
			}
			break;
		case 2:
			if (rR == false)
				moveToRectangle(rev, 1, 100, 1000, 300, 800);
			else if (rR == true)
			{
				degree2 += 3;
			}
			if (cC2 == false)
				degree2 -= 6;
			if (tT == false)
			{
				moveToTri(ret, 1, 100, 1000, 300, 800);
			}
			break;
		case 3:
			if (rR == false)
				moveToRectangle(rev, 2, 500, 400, 700, 200);
			else if (rR == true)
			{
				degree3 += 3;
			}
			if (cC2 == false)
				degree3 -= 6;
			if (tT == false)
			{
				moveToTri(ret, 2, 500, 400, 700, 200);
			}
			break;
		case 4:
			if (rR == false)
				moveToRectangle(rev, 3, 500, 1000, 700, 800);
			else if (rR == true)
			{
				degree4 += 3;
			}
			if (cC2 == false)
				degree4 -= 6;
			if (tT == false)
			{
				moveToTri(ret, 3, 500, 1000, 700, 800);
			}
			break;
		}
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_PAINT:
		GetClientRect(hWnd, &rect);
		hDC = BeginPaint(hWnd, &ps);
		wide = rect.right / 2;
		high = rect.bottom / 2;
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
			{
				Rectangle(hDC, i*wide, j*high, (i + 1)*wide, (j + 1)*high); // left top right bottom
			}
		if (rR == true && tT == true)
		{
			Ellipse(hDC, R[0].L, R[0].T, R[0].R, R[0].B);
			Ellipse(hDC, R[1].L, R[1].T, R[1].R, R[1].B);
			Ellipse(hDC, R[2].L, R[2].T, R[2].R, R[2].B);
			Ellipse(hDC, R[3].L, R[3].T, R[3].R, R[3].B);
			if (mM == true)
			{
				if (cC2 == true)
				{
					hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
					Ellipse(hDC, (R[0].L + 100) + (sin(degree1*3.14 / 180) * 100) - 10, (R[0].T + 100) + (cos(degree1*3.14 / 180) * 100) - 10,
						(R[0].L + 120) + (sin(degree1*3.14 / 180) * 100) - 10, (R[0].T + 120) + (cos(degree1*3.14 / 180) * 100) - 10);
					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);

					hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
					Ellipse(hDC, (R[1].L + 100) + (cos(degree2*3.14 / 180) * 100) - 30, (R[1].T + 100) + (sin(degree2*3.14 / 180) * 100) - 30,
						(R[1].L + 140) + (cos(degree2*3.14 / 180) * 100) - 10, (R[1].T + 140) + (sin(degree2*3.14 / 180) * 100) - 10);
					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);

					hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
					Ellipse(hDC, (R[2].L + 100) + (sin(degree3*3.14 / 180) * 100) - 50, (R[2].T + 100) + (cos(degree3*3.14 / 180) * 100) - 50,
						(R[2].L + 160) + (sin(degree3*3.14 / 180) * 100) - 10, (R[2].T + 160) + (cos(degree3*3.14 / 180) * 100) - 10);
					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);

					hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
					Ellipse(hDC, (R[3].L + 100) + (cos(degree4*3.14 / 180) * 100) - 70, (R[3].T + 100) + (sin(degree4*3.14 / 180) * 100) - 70,
						(R[3].L + 180) + (cos(degree4*3.14 / 180) * 100) - 10, (R[3].T + 180) + (sin(degree4*3.14 / 180) * 100) - 10);
					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);
				}
				if (cC2 == false)
				{
					hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
					Ellipse(hDC, (R[0].L + 100) + (sin(degree1*3.14 / 180) * 100) - 10, (R[0].T + 100) + (cos(degree1*3.14 / 180) * 100) - 10,
						(R[0].L + 120) + (sin(degree1*3.14 / 180) * 100) - 10, (R[0].T + 120) + (cos(degree1*3.14 / 180) * 100) - 10);
					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);

					hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
					Ellipse(hDC, (R[1].L + 100) + (cos(degree2*3.14 / 180) * 100) - 30, (R[1].T + 100) + (sin(degree2*3.14 / 180) * 100) - 30,
						(R[1].L + 140) + (cos(degree2*3.14 / 180) * 100) - 10, (R[1].T + 140) + (sin(degree2*3.14 / 180) * 100) - 10);
					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);

					hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
					Ellipse(hDC, (R[2].L + 100) + (sin(degree3*3.14 / 180) * 100) - 50, (R[2].T + 100) + (cos(degree3*3.14 / 180) * 100) - 50,
						(R[2].L + 160) + (sin(degree3*3.14 / 180) * 100) - 10, (R[2].T + 160) + (cos(degree3*3.14 / 180) * 100) - 10);
					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);

					hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
					Ellipse(hDC, (R[3].L + 100) + (cos(degree4*3.14 / 180) * 100) - 70, (R[3].T + 100) + (sin(degree4*3.14 / 180) * 100) - 70,
						(R[3].L + 180) + (cos(degree4*3.14 / 180) * 100) - 10, (R[3].T + 180) + (sin(degree4*3.14 / 180) * 100) - 10);
					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);
				}
			}
			if (mM == false)
			{
				if (cC2 == true)
				{
					hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
					Rectangle(hDC, (R[0].L + 100) + (sin(degree1*3.14 / 180) * 100) - 10, (R[0].T + 100) + (cos(degree1*3.14 / 180) * 100) - 10,
						(R[0].L + 120) + (sin(degree1*3.14 / 180) * 100) - 10, (R[0].T + 120) + (cos(degree1*3.14 / 180) * 100) - 10);
					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);

					hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
					Rectangle(hDC, (R[1].L + 100) + (cos(degree2*3.14 / 180) * 100) - 30, (R[1].T + 100) + (sin(degree2*3.14 / 180) * 100) - 30,
						(R[1].L + 140) + (cos(degree2*3.14 / 180) * 100) - 10, (R[1].T + 140) + (sin(degree2*3.14 / 180) * 100) - 10);
					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);

					hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
					Rectangle(hDC, (R[2].L + 100) + (sin(degree3*3.14 / 180) * 100) - 50, (R[2].T + 100) + (cos(degree3*3.14 / 180) * 100) - 50,
						(R[2].L + 160) + (sin(degree3*3.14 / 180) * 100) - 10, (R[2].T + 160) + (cos(degree3*3.14 / 180) * 100) - 10);
					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);

					hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
					Rectangle(hDC, (R[3].L + 100) + (cos(degree4*3.14 / 180) * 100) - 70, (R[3].T + 100) + (sin(degree4*3.14 / 180) * 100) - 70,
						(R[3].L + 180) + (cos(degree4*3.14 / 180) * 100) - 10, (R[3].T + 180) + (sin(degree4*3.14 / 180) * 100) - 10);
					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);

				}
				if (cC2 == false)
				{
					hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
					Rectangle(hDC, (R[0].L + 100) + (sin(degree1*3.14 / 180) * 100) - 10, (R[0].T + 100) + (cos(degree1*3.14 / 180) * 100) - 10,
						(R[0].L + 120) + (sin(degree1*3.14 / 180) * 100) - 10, (R[0].T + 120) + (cos(degree1*3.14 / 180) * 100) - 10);
					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);

					hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
					Rectangle(hDC, (R[1].L + 100) + (cos(degree2*3.14 / 180) * 100) - 30, (R[1].T + 100) + (sin(degree2*3.14 / 180) * 100) - 30,
						(R[1].L + 140) + (cos(degree2*3.14 / 180) * 100) - 10, (R[1].T + 140) + (sin(degree2*3.14 / 180) * 100) - 10);
					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);

					hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
					Rectangle(hDC, (R[2].L + 100) + (sin(degree3*3.14 / 180) * 100) - 50, (R[2].T + 100) + (cos(degree3*3.14 / 180) * 100) - 50,
						(R[2].L + 160) + (sin(degree3*3.14 / 180) * 100) - 10, (R[2].T + 160) + (cos(degree3*3.14 / 180) * 100) - 10);
					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);

					hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
					oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
					Rectangle(hDC, (R[3].L + 100) + (cos(degree4*3.14 / 180) * 100) - 70, (R[3].T + 100) + (sin(degree4*3.14 / 180) * 100) - 70,
						(R[3].L + 180) + (cos(degree4*3.14 / 180) * 100) - 10, (R[3].T + 180) + (sin(degree4*3.14 / 180) * 100) - 10);
					SelectObject(hDC, oldBrush);
					DeleteObject(hBrush);
				}
			}
		}
		if (rR == false)
		{
			Rectangle(hDC, R[0].L, R[0].T, R[0].R, R[0].B);
			Rectangle(hDC, R[1].L, R[1].T, R[1].R, R[1].B);
			Rectangle(hDC, R[2].L, R[2].T, R[2].R, R[2].B);
			Rectangle(hDC, R[3].L, R[3].T, R[3].R, R[3].B);
			if (mM == true)
			{
				hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Ellipse(hDC, rev[0].L, rev[0].T, rev[0].R, rev[0].B);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);

				hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Ellipse(hDC, rev[1].L, rev[1].T, rev[1].R, rev[1].B);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);

				hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Ellipse(hDC, rev[2].L, rev[2].T, rev[2].R, rev[2].B);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);

				hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Ellipse(hDC, rev[3].L, rev[3].T, rev[3].R, rev[3].B);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);
			}
			else if (mM == false)
			{
				hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Rectangle(hDC, rev[0].L, rev[0].T, rev[0].R, rev[0].B);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);

				hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Rectangle(hDC, rev[1].L, rev[1].T, rev[1].R, rev[1].B);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);

				hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Rectangle(hDC, rev[2].L, rev[2].T, rev[2].R, rev[2].B);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);

				hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Rectangle(hDC, rev[3].L, rev[3].T, rev[3].R, rev[3].B);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);
			}
		}
		if (tT == false && rR == true)
		{
			for (int i = 0; i < 4; ++i)
			{
				POINT  tri[3] = { { T[i][0].x1, T[i][0].y1 },
				{ T[i][1].x2, T[i][1].y2 },
				{ T[i][2].x3, T[i][2].y3 } };
				Polygon(hDC, tri, 3);
			}
			if (nN == true)
			{
				hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Rectangle(hDC, ret[0].L, ret[0].T, ret[0].R, ret[0].B);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);

				hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Rectangle(hDC, ret[1].L, ret[1].T, ret[1].R, ret[1].B);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);

				hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Rectangle(hDC, ret[2].L, ret[2].T, ret[2].R, ret[2].B);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);

				hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Rectangle(hDC, ret[3].L, ret[3].T, ret[3].R, ret[3].B);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);
			}
			if (nN == false)
			{
				hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Ellipse(hDC, ret[0].L, ret[0].T, ret[0].R, ret[0].B);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);

				hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Ellipse(hDC, ret[1].L, ret[1].T, ret[1].R, ret[1].B);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);

				hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Ellipse(hDC, ret[2].L, ret[2].T, ret[2].R, ret[2].B);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);

				hBrush = CreateSolidBrush((RGB(rand() % 255, rand() % 255, rand() % 255)));
				oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				Ellipse(hDC, ret[3].L, ret[3].T, ret[3].R, ret[3].B);
				SelectObject(hDC, oldBrush);
				DeleteObject(hBrush);
			}
		}
		hBrush = CreateSolidBrush((RGB(255, 0, 0)));
		oldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		Ellipse(hDC, 280, 180, 320, 220);
		Ellipse(hDC, 880, 180, 920, 220);
		Ellipse(hDC, 280, 580, 320, 620);
		Ellipse(hDC, 880, 580, 920, 620);
		SelectObject(hDC, oldBrush);
		DeleteObject(hBrush);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		KillTimer(hWnd, 3);
		KillTimer(hWnd, 4);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
void moveToTri(struct figure arr[], int arrNum, int top, int right, int bottom, int left) {  // TOP RIGHT BOTTOM LEFT
	static int flag1 = 0;
	static int flag2 = 0;
	static int flag3 = 0;
	static int flag4 = 0;

	if (cC1 == true)
	{
		if (arrNum == 0)
		{
			if (flag1 == 0)
			{
				arr[arrNum].L += 8;
				arr[arrNum].R += 8;
				arr[arrNum].T += 16;
				arr[arrNum].B += 16;
				if (arr[arrNum].B >= bottom)
					flag1 = 1;
			}
			if (flag1 == 1)
			{
				arr[arrNum].L -= 8;
				arr[arrNum].R -= 8;
				if (arr[arrNum].L <= left)
					flag1 = 2;
			}
			if (flag1 == 2)
			{
				arr[arrNum].L += 8;
				arr[arrNum].R += 8;
				arr[arrNum].T -= 16;
				arr[arrNum].B -= 16;
				if (arr[arrNum].T <= top)
					flag1 = 0;
			}
		}
		if (arrNum == 1)
		{
			if (flag2 == 0)
			{
				arr[arrNum].L += 8;
				arr[arrNum].R += 8;
				arr[arrNum].T += 16;
				arr[arrNum].B += 16;
				if (arr[arrNum].B >= bottom)
					flag2 = 1;
			}
			if (flag2 == 1)
			{
				arr[arrNum].L -= 8;
				arr[arrNum].R -= 8;
				if (arr[arrNum].L <= left)
					flag2 = 2;
			}
			if (flag2 == 2)
			{
				arr[arrNum].L += 8;
				arr[arrNum].R += 8;
				arr[arrNum].T -= 16;
				arr[arrNum].B -= 16;
				if (arr[arrNum].T <= top)
					flag2 = 0;
			}
		}
		if (arrNum == 2)
		{
			if (flag3 == 0)
			{
				arr[arrNum].L += 8;
				arr[arrNum].R += 8;
				arr[arrNum].T += 16;
				arr[arrNum].B += 16;
				if (arr[arrNum].B >= bottom)
					flag3 = 1;
			}
			if (flag3 == 1)
			{
				arr[arrNum].L -= 8;
				arr[arrNum].R -= 8;
				if (arr[arrNum].L <= left)
					flag3 = 2;
			}
			if (flag3 == 2)
			{
				arr[arrNum].L += 8;
				arr[arrNum].R += 8;
				arr[arrNum].T -= 16;
				arr[arrNum].B -= 16;
				if (arr[arrNum].T <= top)
					flag3 = 0;
			}
		}
		if (arrNum == 3)
		{
			if (flag4 == 0)
			{
				arr[arrNum].L += 8;
				arr[arrNum].R += 8;
				arr[arrNum].T += 16;
				arr[arrNum].B += 16;
				if (arr[arrNum].B >= bottom)
					flag4 = 1;
			}
			if (flag4 == 1)
			{
				arr[arrNum].L -= 8;
				arr[arrNum].R -= 8;
				if (arr[arrNum].L <= left)
					flag4 = 2;
			}
			if (flag4 == 2)
			{
				arr[arrNum].L += 8;
				arr[arrNum].R += 8;
				arr[arrNum].T -= 16;
				arr[arrNum].B -= 16;
				if (arr[arrNum].T <= top)
					flag4 = 0;
			}
		}
	}
	if (cC1 == false)
	{
		if (arrNum == 0)
		{
			if (flag1 == 0)
			{
				arr[arrNum].L -= 8;
				arr[arrNum].R -= 8;
				arr[arrNum].T -= 16;
				arr[arrNum].B -= 16;
				if (arr[arrNum].T <= top)
					flag1 = 2;
			}
			if (flag1 == 1)
			{
				arr[arrNum].L += 8;
				arr[arrNum].R += 8;
				if (arr[arrNum].R >= right)
					flag1 = 0;
			}
			if (flag1 == 2)
			{
				arr[arrNum].L -= 8;
				arr[arrNum].R -= 8;
				arr[arrNum].T += 16;
				arr[arrNum].B += 16;
				if (arr[arrNum].L <= left)
					flag1 = 1;
			}
		}
		if (arrNum == 1)
		{
			if (flag2 == 0)
			{
				arr[arrNum].L -= 8;
				arr[arrNum].R -= 8;
				arr[arrNum].T -= 16;
				arr[arrNum].B -= 16;
				if (arr[arrNum].T <= top)
					flag2 = 2;
			}
			if (flag2 == 1)
			{
				arr[arrNum].L += 8;
				arr[arrNum].R += 8;
				if (arr[arrNum].R >= right)
					flag2 = 0;
			}
			if (flag2 == 2)
			{
				arr[arrNum].L -= 8;
				arr[arrNum].R -= 8;
				arr[arrNum].T += 16;
				arr[arrNum].B += 16;
				if (arr[arrNum].L <= left)
					flag2 = 1;
			}
		}
		if (arrNum == 2)
		{
			if (flag3 == 0)
			{
				arr[arrNum].L -= 8;
				arr[arrNum].R -= 8;
				arr[arrNum].T -= 16;
				arr[arrNum].B -= 16;
				if (arr[arrNum].T <= top)
					flag3 = 2;
			}
			if (flag3 == 1)
			{
				arr[arrNum].L += 8;
				arr[arrNum].R += 8;
				if (arr[arrNum].R >= right)
					flag3 = 0;
			}
			if (flag3 == 2)
			{
				arr[arrNum].L -= 8;
				arr[arrNum].R -= 8;
				arr[arrNum].T += 16;
				arr[arrNum].B += 16;
				if (arr[arrNum].L <= left)
					flag3 = 1;
			}
		}
		if (arrNum == 3)
		{
			if (flag4 == 0)
			{
				arr[arrNum].L -= 8;
				arr[arrNum].R -= 8;
				arr[arrNum].T -= 16;
				arr[arrNum].B -= 16;
				if (arr[arrNum].T <= top)
					flag4 = 2;
			}
			if (flag4 == 1)
			{
				arr[arrNum].L += 8;
				arr[arrNum].R += 8;
				if (arr[arrNum].R >= right)
					flag4 = 0;
			}
			if (flag4 == 2)
			{
				arr[arrNum].L -= 8;
				arr[arrNum].R -= 8;
				arr[arrNum].T += 16;
				arr[arrNum].B += 16;
				if (arr[arrNum].L <= left)
					flag4 = 1;
			}
		}
	}
}



void moveToRectangle(struct figure arr[], int arrNum, int top, int right, int bottom, int left) {  // TOP RIGHT BOTTOM LEFT
	if (cC1 == true)
	{
		if (arr[arrNum].T < top && arr[arrNum].L <= right)
		{
			arr[arrNum].L += 10;
			arr[arrNum].R += 10;
		}
		if (arr[arrNum].R > right)
		{
			arr[arrNum].T += 10;
			arr[arrNum].B += 10;
		}
		if (arr[arrNum].B > bottom)
		{
			arr[arrNum].L -= 10;
			arr[arrNum].R -= 10;
		}
		if (arr[arrNum].L < left)
		{
			arr[arrNum].T -= 10;
			arr[arrNum].B -= 10;
		}
	}
	if (cC1 == false)
	{
		if (arr[arrNum].T < top && arr[arrNum].L >= left)
		{
			arr[arrNum].L -= 10;
			arr[arrNum].R -= 10;
		}
		if (arr[arrNum].L < left)
		{
			arr[arrNum].T += 10;
			arr[arrNum].B += 10;
		}
		if (arr[arrNum].B > bottom)
		{
			arr[arrNum].L += 10;
			arr[arrNum].R += 10;
		}
		if (arr[arrNum].R > right)
		{
			arr[arrNum].T -= 10;
			arr[arrNum].B -= 10;
		}
	}
}