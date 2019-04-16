#include <Windows.h>
#include <cmath>

#define ID_SCR1 100 
#define ID_SCR2 101 
#define ID_R1 102
#define ID_R2 103
#define ID_R3 104
#define IDC_BUTTON1 105
#define IDC_BUTTON2 106

HINSTANCE g_hInst;
LPCTSTR IpszClass = "궤도를 따라 공전하는 원 만들기";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpszCmdParam, int nCmdShow)
{
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
	WndClass.lpszClassName = IpszClass;
	WndClass.hIconSm = LoadCursor(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(IpszClass, IpszClass, WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_SYSMENU, 0, 0,
		800, 600, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	HBRUSH MyBrush = 0, OldBrush = 0;
	static HWND scroll1 = NULL, scroll2 = NULL, R1, R2, R3, hButton1, hButton2;

	static int x, y, key, speed1 = 5, speed2 = 2, cSpeed;
	static POINT satellite, satellite2;
	static int track1, track2, fRadius1, fRadius2;
	static double fRadian1 = 0, fRadian2 = 180;

	switch (uMsg)
	{
	case WM_CREATE:
		hButton1 = CreateWindow("button", "반대", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 540, 430, 50, 25, hWnd, (HMENU)IDC_BUTTON1, g_hInst, NULL);
		hButton2 = CreateWindow("button", "종료", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 600, 430, 50, 25, hWnd, (HMENU)IDC_BUTTON2, g_hInst, NULL);

		scroll1 = CreateWindow("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, 500, 80, 200, 20, hWnd, (HMENU)ID_SCR1, g_hInst, NULL);
		scroll2 = CreateWindow("scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, 500, 130, 200, 20, hWnd, (HMENU)ID_SCR2, g_hInst, NULL);
		SetScrollRange(scroll1, SB_CTL, 1, 10, TRUE);
		SetScrollPos(scroll1, SB_CTL, 0, TRUE);
		SetScrollRange(scroll2, SB_CTL, 1, 10, TRUE);
		SetScrollPos(scroll2, SB_CTL, 0, TRUE);

		CreateWindow("button", "Size", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 500, 190, 200, 200, hWnd, (HMENU)0, g_hInst, NULL);

		R1 = CreateWindow("button", "Small", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP, 550, 230, 100, 30, hWnd, (HMENU)ID_R1, g_hInst, NULL);
		R2 = CreateWindow("button", "Normal", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 550, 280, 100, 30, hWnd, (HMENU)ID_R2, g_hInst, NULL);
		R3 = CreateWindow("button", "Big", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, 550, 330, 100, 30, hWnd, (HMENU)ID_R3, g_hInst, NULL);

		CheckRadioButton(hWnd, ID_R1, ID_R3, ID_R2);

		x = 250;
		y = 230;
		track1 = 80;
		fRadius1 = 15;
		track2 = 150;
		fRadius2 = 20;
		SetTimer(hWnd, 1, 100, NULL);

		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_R1:
			fRadius1 = 5;
			fRadius2 = 5;
			break;
		case ID_R2:
			fRadius1 = 15;
			fRadius2 = 15;
			break;
		case ID_R3:
			fRadius1 = 25;
			fRadius2 = 25;
			break;
		case IDC_BUTTON1:
			if (key == 0)
				key = 1;
			else
				key = 0;
			break;
		case IDC_BUTTON2:
			PostQuitMessage(0);
			break;
		}
		return 0;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);

		MyBrush = CreateSolidBrush(RGB(255, 255, 255));
		OldBrush = (HBRUSH)SelectObject(hDC, MyBrush);
		Ellipse(hDC, x - track2, y - track2, x + track2, y + track2);
		satellite.x = (LONG)(x + cos(fRadian2 * 3.14 / 180) * track2);
		satellite.y = (LONG)(y + sin(fRadian2 * 3.14 / 180) * track2);
		MyBrush = CreateSolidBrush(RGB(255, 255, 0));
		OldBrush = (HBRUSH)SelectObject(hDC, MyBrush);
		Ellipse(hDC, satellite.x - fRadius2, satellite.y - fRadius2, satellite.x + fRadius2, satellite.y + fRadius2);

		MyBrush = CreateSolidBrush(RGB(255, 255, 255));
		OldBrush = (HBRUSH)SelectObject(hDC, MyBrush);
		Ellipse(hDC, x - track1, y - track1, x + track1, y + track1);
		satellite.x = (LONG)(x + cos(fRadian1 * 3.14 / 180) * track1);
		satellite.y = (LONG)(y + sin(fRadian1 * 3.14 / 180) * track1);
		MyBrush = CreateSolidBrush(RGB(0, 255, 255));
		OldBrush = (HBRUSH)SelectObject(hDC, MyBrush);
		Ellipse(hDC, satellite.x - fRadius1, satellite.y - fRadius1, satellite.x + fRadius1, satellite.y + fRadius1);
		MyBrush = CreateSolidBrush(RGB(255, 0, 0));
		OldBrush = (HBRUSH)SelectObject(hDC, MyBrush);
		Ellipse(hDC, x - 10, y - 10, x + 10, y + 10);

		SelectObject(hDC, OldBrush);
		DeleteObject(MyBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_HSCROLL:
		if ((HWND)lParam == scroll1)
			cSpeed = speed1;
		if ((HWND)lParam == scroll2)
			cSpeed = speed2;
		switch (LOWORD(wParam))
		{
		case SB_LINELEFT:
			cSpeed = max(1, cSpeed - 1);
			break;
		case SB_LINERIGHT:
			cSpeed = min(10, cSpeed + 1);
			break;
		case SB_PAGELEFT:
			cSpeed = max(1, cSpeed - 10);
			break;
		case SB_PAGERIGHT:
			cSpeed = min(10, cSpeed + 10);
			break;
		case SB_THUMBTRACK:
			cSpeed = HIWORD(wParam);
			break;
		}
		if ((HWND)lParam == scroll1)
			speed1 = cSpeed;
		if ((HWND)lParam == scroll2)
			speed2 = cSpeed;
		SetScrollPos((HWND)lParam, SB_CTL, cSpeed, TRUE);
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_TIMER:
		if (wParam == 1)
		{
			for (int i = 0; i < 2; ++i)
			{
				if (key == 0)
				{
					fRadian1 += speed1;
					fRadian2 += speed2;

					if (fRadian1 == 360)
						fRadian1 = 0;
					else if (fRadian2 == 360)
						fRadian2 = 0;

				}
				else if (key == 1)
				{
					fRadian1 -= speed1;
					fRadian2 -= speed2;

					if (fRadian1 == 0)
						fRadian1 = 360;
					else if (fRadian2 == 0)
						fRadian2 = 360;
				}
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hWnd, 1);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}