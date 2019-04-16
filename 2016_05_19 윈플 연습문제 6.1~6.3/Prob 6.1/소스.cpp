#include <windows.h>
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = "윈플 문제 6-1번";

int move, Shape;

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
	WndClass.lpszMenuName = NULL;  WndClass.lpszClassName = lpszClass;
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

	HDC hDC;
	PAINTSTRUCT ps;
	static HINSTANCE hlnstance;
	static int rudX, rudY, rudS;
	static int speed, flag;

	switch (iMessage) {
	case WM_CREATE:
		SetTimer(hWnd, 1, 100, NULL);
		g_hInst = hlnstance;
		rudX = 0;
		rudY = 0;
		rudS = 50;
		speed = 20;
		flag = 1;
		break;

	case WM_LBUTTONDOWN:
		DialogBox(hlnstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dlg6_1Proc);
		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			if (move == 1) {
				if (flag == 1) { // ↘
					rudX += speed;
					rudY += speed;
					if (rudX >= 730)
						flag = 2;
					if (rudY >= 520)
						flag = 3;
				}
				else if (flag == 2) { // ↙
					rudX -= speed;
					rudY += speed;
					if (rudX <= 0)
						flag = 1;
					if (rudY >= 520)
						flag = 4;
				}
				else if (flag == 3) { // ↗
					rudX += speed;
					rudY -= speed;
					if (rudX >= 730)
						flag = 4;
					if (rudY <= 0)
						flag = 1;
				}
				else if (flag == 4) { // ↖
					rudX -= speed;
					rudY -= speed;
					if (rudX <= 0)
						flag = 3;
					if (rudY <= 0)
						flag = 2;
				}
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		if (Shape == 0) {
			Ellipse(hDC, rudX, rudY, rudX + rudS, rudY + rudS);
		}
		else if (Shape == 1) {
			Rectangle(hDC, rudX, rudY, rudX + rudS, rudY + rudS);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}
BOOL CALLBACK Dlg6_1Proc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	HDC hDC;
	PAINTSTRUCT ps;
	HWND hButton;
	static int rudX, rudY, rudS;
	static int speed, flag;

	switch (iMessage) {
	case WM_INITDIALOG:
		rudX = 0;
		rudY = 0;
		rudS = 50;
		speed = 20;
		hButton = GetDlgItem(hDlg, IDC_BUTTON2);
		EnableWindow(hButton, FALSE);
		hButton = GetDlgItem(hDlg, IDC_BUTTON3);
		EnableWindow(hButton, FALSE);
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
		return true;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;

		case IDOK:
			hButton = GetDlgItem(hDlg, IDOK);    
			EnableWindow(hButton, FALSE);
			hButton = GetDlgItem(hDlg, IDC_BUTTON2);   
			EnableWindow(hButton, TRUE);

			SetTimer(hDlg, 1, 100, NULL);
			flag = 1;
			InvalidateRect(hDlg, NULL, TRUE);
			break;

		case IDC_BUTTON2:
			hButton = GetDlgItem(hDlg, IDOK);
			EnableWindow(hButton, TRUE);
			hButton = GetDlgItem(hDlg, IDC_BUTTON2);
			EnableWindow(hButton, FALSE);

			KillTimer(hDlg, 1);
			InvalidateRect(hDlg, NULL, TRUE);
			break;

		case IDOK2:
			hButton = GetDlgItem(hDlg, IDOK2);
			EnableWindow(hButton, FALSE);
			hButton = GetDlgItem(hDlg, IDC_BUTTON3);
			EnableWindow(hButton, TRUE);
			move = 1;
			break;

		case IDC_BUTTON3:
			hButton = GetDlgItem(hDlg, IDOK2);
			EnableWindow(hButton, TRUE);
			hButton = GetDlgItem(hDlg, IDC_BUTTON3);
			EnableWindow(hButton, FALSE);
			move = 0;
			break;

		case IDC_RADIO1:
			Shape = 0;
			InvalidateRect(hDlg, NULL, TRUE);
			break;

		case IDC_RADIO2:
			Shape = 1;
			InvalidateRect(hDlg, NULL, TRUE);
			break;
		}
		break;

	case WM_PAINT:
		hDC = BeginPaint(hDlg, &ps);
		if (Shape == 0) {
			Ellipse(hDC, rudX, rudY, rudX + rudS, rudY + rudS);
		}
		else if (Shape == 1) {
			Rectangle(hDC, rudX, rudY, rudX + rudS, rudY + rudS);
		}
		EndPaint(hDlg, &ps);
		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			if (flag == 1) { // ↘
				rudX += speed;
				rudY += speed;
				if (rudX >= 420)
					flag = 2;
				if (rudY >= 240)
					flag = 3;
			}
			else if (flag == 2) { // ↙
				rudX -= speed;
				rudY += speed;
				if (rudX <= 0)
					flag = 1;
				if (rudY >= 240)
					flag = 4;
			}
			else if (flag == 3) { // ↗
				rudX += speed;
				rudY -= speed;
				if (rudX >= 420)
					flag = 4;
				if (rudY <= 0)
					flag = 1;
			}
			else if (flag == 4) { // ↖
				rudX -= speed;
				rudY -= speed;
				if (rudX <= 0)
					flag = 3;
				if (rudY <= 0)
					flag = 2;
			}
		}
		InvalidateRect(hDlg, NULL, TRUE);
		break;
	}
	return 0;
}