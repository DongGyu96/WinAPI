#include <Windows.h>
#define IDC_BUTTON1 100
#define IDC_BUTTON2 101
#define IDC_BUTTON3 102
#define IDC_BUTTON4 103
#define IDC_BUTTON5 104
#define IDC_BUTTON6 105
#define IDC_BUTTON7 106
#define IDC_BUTTON8 107
#define IDC_BUTTON9 108

HINSTANCE g_hInst;
LPCTSTR IpszClass = "2개의 차일드 윈도우 컨트롤하기";

LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc2(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	//WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = FrameWndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "2개의 차일드 윈도우 컨트롤하기";
	//WndClass.hIconSm = LoadCursor(NULL, IDI_APPLICATION);

	RegisterClass(&WndClass);

	WndClass.lpfnWndProc = ChildWndProc;
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Child Window Class Name";

	RegisterClass(&WndClass);

	WndClass.lpfnWndProc = ChildWndProc2;
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Child Window Class Name2";

	RegisterClass(&WndClass);

	hWnd = CreateWindow(IpszClass, IpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return Message.wParam;
}

CLIENTCREATESTRUCT clientcreate;
static HWND hwndChild[2];
static int key[2][2] = { { 0, 0 },{ 1, 0 } }, save[2];

LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndClient;
	static HWND hButton1, hButton2, hButton3, hButton4, hButton5, hButton6, hButton7, hButton8, hButton9;
	RECT rect;

	switch (iMsg)
	{
	case WM_CREATE:
		GetClientRect(hwnd, &rect);
		hwndChild[0] = CreateWindowEx(WS_EX_CLIENTEDGE, "Child Window Class Name", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 0, 0, (rect.right / 2), rect.bottom - 100, hwnd, NULL, g_hInst, NULL);
		hwndChild[1] = CreateWindowEx(WS_EX_CLIENTEDGE, "Child Window Class Name2", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, (rect.right / 2), 0, rect.right, rect.bottom - 100, hwnd, NULL, g_hInst, NULL);
		hButton1 = CreateWindow("button", "좌우1", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 70, rect.bottom - 70, 50, 25, hwnd, (HMENU)IDC_BUTTON1, g_hInst, NULL);
		hButton2 = CreateWindow("button", "상하1", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 170, rect.bottom - 70, 50, 25, hwnd, (HMENU)IDC_BUTTON2, g_hInst, NULL);
		hButton3 = CreateWindow("button", "점프1", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 270, rect.bottom - 70, 50, 25, hwnd, (HMENU)IDC_BUTTON3, g_hInst, NULL);
		hButton4 = CreateWindow("button", "좌우2", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 370, rect.bottom - 70, 50, 25, hwnd, (HMENU)IDC_BUTTON4, g_hInst, NULL);
		hButton5 = CreateWindow("button", "상하2", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 470, rect.bottom - 70, 50, 25, hwnd, (HMENU)IDC_BUTTON5, g_hInst, NULL);
		hButton6 = CreateWindow("button", "점프2", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 570, rect.bottom - 70, 50, 25, hwnd, (HMENU)IDC_BUTTON6, g_hInst, NULL);
		hButton7 = CreateWindow("button", "정지", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 670, rect.bottom - 70, 50, 25, hwnd, (HMENU)IDC_BUTTON7, g_hInst, NULL);
		hButton8 = CreateWindow("button", "변화", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 770, rect.bottom - 70, 50, 25, hwnd, (HMENU)IDC_BUTTON8, g_hInst, NULL);
		hButton9 = CreateWindow("button", "종료", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 870, rect.bottom - 70, 50, 25, hwnd, (HMENU)IDC_BUTTON9, g_hInst, NULL);
		ShowWindow(hwndClient, SW_SHOW);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON1:
			key[0][1] = 1;
			break;
		case IDC_BUTTON2:
			key[0][1] = 2;
			break;
		case IDC_BUTTON3:
			save[0] = key[0][1];
			key[0][1] = 3;
			break;
		case IDC_BUTTON4:
			key[1][1] = 1;
			break;
		case IDC_BUTTON5:
			key[1][1] = 2;
			break;
		case IDC_BUTTON6:
			save[1] = key[1][1];
			key[1][1] = 3;
			break;
		case IDC_BUTTON7:
			key[0][1] = 0;
			key[1][1] = 0;
			break;
		case IDC_BUTTON8:
			if (key[0][0] == 0)
				key[0][0] = 1;
			else
				key[0][0] = 0;

			if (key[1][0] == 0)
				key[1][0] = 1;
			else
				key[1][0] = 0;
			break;
		case IDC_BUTTON9:
			PostQuitMessage(0);
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefFrameProc(hwnd, hwndClient, iMsg, wParam, lParam);
}


LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HBRUSH MyBrush, OldBrush;
	static HDC hDC;
	static RECT rect;
	static POINT shape = { 15, 15 };
	static int r = 10, cnt = 0, timecnt = 0;
	static BOOL directx = TRUE, directy = TRUE;

	switch (iMsg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &rect);
		SetTimer(hWnd, 1, 100, NULL);
		return 0;
	case WM_TIMER:
		if (wParam == 1)
		{
			if (key[0][1] == 1)
			{
				if (shape.x < rect.left + 20)
				{
					if (directy)
						shape.y += 5;
					else
						shape.y -= 5;
					timecnt++;
					if (timecnt == 4)
					{
						directx = TRUE;
						timecnt = 0;
					}
				}
				else if (shape.x > rect.right - 20)
				{
					if (directy)
						shape.y += 5;
					else
						shape.y -= 5;
					timecnt++;
					if (timecnt == 4)
					{
						directx = FALSE;
						timecnt = 0;
					}
				}
				if (directx)
					shape.x += 5;
				else
					shape.x -= 5;
			}
			else if (key[0][1] == 2)
			{
				if (shape.y < rect.top + 20)
				{
					if (directx)
						shape.x += 5;
					else
						shape.x -= 5;
					timecnt++;
					if (timecnt == 4)
					{
						directy = TRUE;
						timecnt = 0;
					}
				}
				else if (shape.y > rect.bottom - 20)
				{
					if (directx)
						shape.x += 5;
					else
						shape.x -= 5;
					timecnt++;
					if (timecnt == 4)
					{
						directy = FALSE;
						timecnt = 0;
					}
				}
				if (directy)
					shape.y += 5;
				else
					shape.y -= 5;
			}
			else if (key[0][1] == 3)
			{
				if (save[0] == 1 || save[0] == 0)
				{
					if (cnt < 5)
					{
						shape.y -= 5;
					}
					else if (cnt >= 5 && cnt < 10)
					{
						shape.y += 5;
					}
					cnt++;
					if (cnt == 10)
					{
						cnt = 0;
						key[0][1] = save[0];
					}
				}
				else if (save[0] == 2)
				{
					if (cnt < 5)
					{
						shape.x -= 5;
					}
					else if (cnt >= 5 && cnt < 10)
					{
						shape.x += 5;
					}
					cnt++;
					if (cnt == 10)
					{
						cnt = 0;
						key[0][1] = save[0];
					}
				}
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);
		if (key[0][0] == 0)
		{
			MyBrush = CreateSolidBrush(RGB(0, 0, 255));
			OldBrush = (HBRUSH)SelectObject(hDC, MyBrush);
			Ellipse(hDC, shape.x - r, shape.y - r, shape.x + r, shape.y + r);
		}
		else if (key[0][0] == 1)
		{
			MyBrush = CreateSolidBrush(RGB(255, 0, 0));
			OldBrush = (HBRUSH)SelectObject(hDC, MyBrush);
			Rectangle(hDC, shape.x - r, shape.y - r, shape.x + r, shape.y + r);
		}
		SelectObject(hDC, OldBrush);
		DeleteObject(MyBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hWnd, 1);
		return 0;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

LRESULT CALLBACK ChildWndProc2(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HBRUSH MyBrush, OldBrush;
	static HDC hDC;
	static RECT rect;
	static POINT shape = { 15, 15 };
	static int r = 10, cnt = 0, timecnt = 0;
	static BOOL directx = TRUE, directy = TRUE;

	switch (iMsg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &rect);
		SetTimer(hWnd, 1, 100, NULL);
		return 0;
	case WM_TIMER:
		if (wParam == 1)
		{
			if (key[1][1] == 1)
			{
				if (shape.x < rect.left + 30)
				{
					if (directy)
						shape.y += 10;
					else
						shape.y -= 10;
					timecnt++;
					if (timecnt == 4)
					{
						directx = TRUE;
						timecnt = 0;
					}
				}
				else if (shape.x > rect.right / 2 - 20)
				{
					if (directy)
						shape.y += 10;
					else
						shape.y -= 10;
					timecnt++;
					if (timecnt == 4)
					{
						directx = FALSE;
						timecnt = 0;
					}
				}
				if (directx)
					shape.x += 10;
				else
					shape.x -= 10;
			}
			else if (key[1][1] == 2)
			{
				if (shape.y < rect.top + 20)
				{
					if (directx)
						shape.x += 10;
					else
						shape.x -= 10;
					timecnt++;
					if (timecnt == 4)
					{
						directy = TRUE;
						timecnt = 0;
					}
				}
				else if (shape.y > rect.bottom - 20)
				{
					if (directx)
						shape.x += 10;
					else
						shape.x -= 10;
					timecnt++;
					if (timecnt == 4)
					{
						directy = FALSE;
						timecnt = 0;
					}
				}
				if (directy)
					shape.y += 10;
				else
					shape.y -= 10;
			}
			else if (key[1][1] == 3)
			{
				if (save[1] == 1 || save[1] == 0)
				{
					if (cnt < 5)
					{
						shape.y -= 5;
					}
					else if (cnt >= 5 && cnt < 10)
					{
						shape.y += 5;
					}
					cnt++;
					if (cnt == 10)
					{
						cnt = 0;
						key[1][1] = save[1];
					}
				}
				else if (save[1] == 2)
				{
					if (cnt < 5)
					{
						shape.x -= 5;
					}
					else if (cnt >= 5 && cnt < 10)
					{
						shape.x += 5;
					}
					cnt++;
					if (cnt == 10)
					{
						cnt = 0;
						key[1][1] = save[1];
					}
				}
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rect);

		if (key[1][0] == 0)
		{
			MyBrush = CreateSolidBrush(RGB(0, 0, 255));
			OldBrush = (HBRUSH)SelectObject(hDC, MyBrush);
			Ellipse(hDC, shape.x - r, shape.y - r, shape.x + r, shape.y + r);
		}
		else if (key[1][0] == 1)
		{
			MyBrush = CreateSolidBrush(RGB(255, 0, 0));
			OldBrush = (HBRUSH)SelectObject(hDC, MyBrush);
			Rectangle(hDC, shape.x - r, shape.y - r, shape.x + r, shape.y + r);
		}
		SelectObject(hDC, OldBrush);
		DeleteObject(MyBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hWnd, 1);
		return 0;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}