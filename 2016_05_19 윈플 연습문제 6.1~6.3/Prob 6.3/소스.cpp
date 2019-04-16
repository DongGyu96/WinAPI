#include <windows.h>
#include <math.h>
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = "윈플 문제 6-3번";

int red, blue, green, shape, n, height;
BOOL XMove, YMove;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Dlg6_1Proc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

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
	WndClass.lpszMenuName = NULL;  
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, 0, 0, 800, 600,
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

	HDC hdc;
	PAINTSTRUCT ps;
	static HINSTANCE hlnstance;
	static HPEN hPen, oldPen;
	static double f;
	static BOOL move;
	int y;
	switch (iMessage) {
	case WM_CREATE:
		SetTimer(hWnd, 1, 60, NULL);
		move = true;
		height = 100;
		shape = 1;
		n = 0;
		break;
	case WM_LBUTTONDOWN:
		DialogBox(hlnstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dlg6_1Proc);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetMapMode(hdc, MM_LOENGLISH);
		SetViewportOrgEx(hdc, 200, 150, NULL);
		MoveToEx(hdc, -2000, 0, NULL);
		LineTo(hdc, 2000, 0);
		MoveToEx(hdc, 0, -2000, NULL);
		LineTo(hdc, 0, 2000);
		if (shape == 1)
		{
			for (f = -500; f < 1500; f++)
			{
				y = (int)(sin(f*3.14 / 180) * height);
				SetPixel(hdc, (int)f + n, y, RGB(red, green, blue));
			}
		}
		else if (shape == 2)
		{
			for (f = -500; f < 1500; f++)
			{
				y = (int)(cos(f*3.14 / 180) * height);
				SetPixel(hdc, (int)f + n, y, RGB(red, green, blue));
			}
		}
		else if (shape == 3)
		{
			for (f = -500; f < 1500; f++)
			{
				y = (int)(tan(f*3.14 / 180) * height);
				SetPixel(hdc, (int)f + n, y, RGB(red, green, blue));
			}
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		if (wParam == 1)
		{
			if (XMove)
			{
				n -= 10;
				if (n == -720)
				{
					n = 0;
				}
			}
			if (YMove)
			{
				if (move)
				{
					height += 10;
					if (height == 500)
					{
						move = false;
					}
				}
				else
				{
					height -= 10;
					if (height == -400)
					{
						move = true;
					}
				}
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
BOOL CALLBACK Dlg6_1Proc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	HWND hButton;
	static BOOL RED, GREEN, BLUE;
	switch (iMessage) {
	case WM_INITDIALOG:
		RED = GREEN = BLUE = false;
		hButton = GetDlgItem(hDlg, IDOK3);
		EnableWindow(hButton, FALSE);
		hButton = GetDlgItem(hDlg, IDOK4);
		EnableWindow(hButton, FALSE);
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		case IDOK:
			XMove = true;
			hButton = GetDlgItem(hDlg, IDOK3);
			EnableWindow(hButton, TRUE);
			hButton = GetDlgItem(hDlg, IDOK4);
			EnableWindow(hButton, TRUE);
			hButton = GetDlgItem(hDlg, IDOK);
			EnableWindow(hButton, FALSE);
			break;
		case IDOK2:
			YMove = true;
			hButton = GetDlgItem(hDlg, IDOK3);
			EnableWindow(hButton, TRUE);
			hButton = GetDlgItem(hDlg, IDOK4);
			EnableWindow(hButton, TRUE);
			hButton = GetDlgItem(hDlg, IDOK2);
			EnableWindow(hButton, FALSE);
			break;
		case IDOK3:
			XMove = false;
			YMove = false;
			hButton = GetDlgItem(hDlg, IDOK3);
			EnableWindow(hButton, FALSE);
			hButton = GetDlgItem(hDlg, IDOK);
			EnableWindow(hButton, TRUE);
			hButton = GetDlgItem(hDlg, IDOK2);
			EnableWindow(hButton, TRUE);
			break;
		case IDOK4:
			height = 100;
			n = 0;
			hButton = GetDlgItem(hDlg, IDOK4);
			EnableWindow(hButton, FALSE);
			hButton = GetDlgItem(hDlg, IDOK);
			EnableWindow(hButton, TRUE);
			hButton = GetDlgItem(hDlg, IDOK2);
			EnableWindow(hButton, TRUE);
			break;
		case IDC_RADIO1:
			shape = 1;
			break;
		case IDC_RADIO2:
			shape = 2;
			break;
		case IDC_RADIO3:
			shape = 3;
			break;
		case IDC_CHECK1:
			if (RED)
			{
				red = 0;
				RED = false;
			}
			else
			{
				red = 255;
				RED = true;
			}
			break;
		case IDC_CHECK2:
			if (GREEN)
			{
				green = 0;
				GREEN = false;
			}
			else
			{
				green = 255;
				GREEN = true;
			}
			break;
		case IDC_CHECK3:
			if (BLUE)
			{
				blue = 0;
				BLUE = false;
			}
			else
			{
				blue = 255;
				BLUE = true;
			}
			break;
		}
		break;
	}
	return 0;
}