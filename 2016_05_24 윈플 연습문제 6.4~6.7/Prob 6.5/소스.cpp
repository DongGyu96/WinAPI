#include <Windows.h>
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR IpszClass = " ";

BOOL CALLBACK Dlg6_1Proc(HWND, UINT, WPARAM, LPARAM);
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
	WndClass.lpszMenuName = MAKEINTRESOURCE(NULL);;
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
	static HINSTANCE hInstance;

	switch (uMsg)
	{
	case WM_CREATE:
		g_hInst = hInstance;
		return 0;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dlg6_1Proc);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK Dlg6_1Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static int selection, key = 3, cnt = 0;
	static HWND hCombo = NULL, hList;
	static char name[10][30], num[30], select[3][10] = { { "  여자" },{ "  남자" },{ "   " } }, all[15][100] = { "" }, year[10];
	char save[100];

	switch (iMsg)
	{
	case WM_INITDIALOG:
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO2, NULL);
		hList = GetDlgItem(hDlg, IDC_LIST1);
		hCombo = GetDlgItem(hDlg, IDC_COMBO1);
		for (int i = 1900; i < 2017; ++i)
		{
			wsprintf(year, "%d", i);
			SendMessage(hCombo, CB_ADDSTRING, 10, (LPARAM)year);
		}
		CheckRadioButton(hDlg, IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);
		key = 0;
		return true;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RADIO1:
			key = 0;
			break;
		case IDC_RADIO2:
			key = 1;
			break;
		case IDC_BUTTON1:
			for (int i = 0; i <= cnt; ++i)
			{
				for (int j = i + 1; j <= cnt; ++j)
				{
					if (strcmp(name[i], name[j]) > 0)
					{
						strcpy_s(save, all[i]);
						strcpy_s(all[i], all[j]);
						strcpy_s(all[j], save);

						strcpy_s(save, name[i]);
						strcpy_s(name[i], name[j]);
						strcpy_s(name[j], save);
					}
				}
			}
			for (int i = 0; i <= cnt; ++i)
				SendMessage(hList, LB_DELETESTRING, 0, 0);

			for (int i = 0; i <= cnt; ++i)
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)all[i]);
			SendMessage(hList, LB_DELETESTRING, 0, 0);
			break;
		case IDC_BUTTON5:
			EndDialog(hDlg, 0);
			PostQuitMessage(0);
			break;
		case IDC_BUTTON2:
			SetDlgItemText(hDlg, IDC_EDIT1, NULL);
			SetDlgItemText(hDlg, IDC_EDIT2, NULL);
			break;
		case IDC_BUTTON3:
			GetDlgItemText(hDlg, IDC_EDIT1, name[cnt], 20);
			GetDlgItemText(hDlg, IDC_EDIT2, num, 20);
			GetDlgItemText(hDlg, IDC_COMBO1, year, 20);

			strcpy(all[cnt], "이름: ");
			strcat_s(all[cnt], name[cnt]);
			strcat_s(all[cnt], "  전화번호: ");
			strcat_s(all[cnt], num);
			strcat_s(all[cnt], " 출생년도: ");
			strcat_s(all[cnt], year);
			strcat_s(all[cnt], select[key]);

			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)all[cnt]);
			cnt++;
			break;
		case IDC_BUTTON4:
			SendMessage(hList, LB_DELETESTRING, selection, 0);
			for (int i = selection; i < cnt; ++i)
			{
				strcpy_s(all[i], all[i + 1]);
			}
			cnt--;
			for (int i = 0; i <= cnt + 1; ++i)
				SendMessage(hList, LB_DELETESTRING, 0, 0);

			for (int i = 0; i <= cnt; ++i)
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)all[i]);
			break;
		case IDC_COMBO1:
			if (HIWORD(wParam) == CBN_SELCHANGE)
				selection = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
			break;
		case IDC_LIST1:
			if (HIWORD(wParam) == LBN_SELCHANGE)
				selection = SendMessage(hList, LB_GETCURSEL, 0, 0);
			break;
		}
		InvalidateRect(hDlg, NULL, TRUE);
		break;
	case WM_CHAR:
		if (wParam == VK_ESCAPE)
		{
			EndDialog(hDlg, 0);
			PostQuitMessage(0);
		}
		InvalidateRect(hDlg, NULL, TRUE);
		break;
	} return 0;
}