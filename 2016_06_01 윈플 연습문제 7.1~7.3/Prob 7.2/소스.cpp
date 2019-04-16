#include <windows.h> 

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc_1(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc_2(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc_3(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc_4(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FrameWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	// ������ Ŭ���� ����ü �� ���� 
	//   WndClass.cbSize = sizeof(WndClass) ; 
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = FrameWndProc; // ������ ������ ���ν��� 
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Window Class Name";
	// ������ ������ Ŭ���� �̸� 
	//   WndClass.hIconSm = LoadIcon(NULL,IDI_APPLICATION); 


	// ������ Ŭ���� ��� 
	RegisterClass(&WndClass);

	WndClass.lpfnWndProc = ChildWndProc_1; // ���ϵ� ������ ���ν��� 
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Child Window Class Name_1";
	// ���ϵ� ������ Ŭ���� �̸�
	RegisterClass(&WndClass); // ���ϵ� ������ Ŭ���� ���

	WndClass.lpfnWndProc = ChildWndProc_2; // ���ϵ� ������ ���ν��� 
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Child Window Class Name_2";
	// ���ϵ� ������ Ŭ���� �̸�
	RegisterClass(&WndClass); // ���ϵ� ������ Ŭ���� ���

	WndClass.lpfnWndProc = ChildWndProc_3; // ���ϵ� ������ ���ν��� 
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Child Window Class Name_3";
	// ���ϵ� ������ Ŭ���� �̸�
	RegisterClass(&WndClass); // ���ϵ� ������ Ŭ���� ���

	WndClass.lpfnWndProc = ChildWndProc_4; // ���ϵ� ������ ���ν��� 
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Child Window Class Name_4";
	// ���ϵ� ������ Ŭ���� �̸�
	RegisterClass(&WndClass); // ���ϵ� ������ Ŭ���� ���


							  // ������ ���� 
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		1200, 800, NULL, (HMENU)NULL, hInstance, NULL);
	// ������ ��� 
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	// �̺�Ʈ ���� ó�� 
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

CLIENTCREATESTRUCT clientcreate; // ���ϵ� ������ �����ϱ� ���� ����ü
HWND hwndChild[4];

LRESULT CALLBACK FrameWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndClient; // Ŭ���̾�Ʈ ������ 
	MDICREATESTRUCT mdicreate;
	static int i;
	static RECT rt;
	switch (iMsg)
	{
	case WM_CREATE: // �޴��ڵ� ȹ�� -> â ���� �θ޴�(0) �ڵ� ȹ�� 
		GetClientRect(hWnd, &rt);
		hwndChild[0] = CreateWindowEx(WS_EX_DLGMODALFRAME, "Child Window Class Name_1", NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 0, 0, rt.right / 2, rt.bottom / 2, hWnd, NULL, g_hInst, NULL);

		hwndChild[1] = CreateWindowEx(WS_EX_DLGMODALFRAME, "Child Window Class Name_2", NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rt.right/2, 0, rt.right / 2, rt.bottom / 2, hWnd, NULL, g_hInst, NULL);

		hwndChild[2] = CreateWindowEx(WS_EX_DLGMODALFRAME, "Child Window Class Name_3", NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 0, rt.bottom/2, rt.right / 2, rt.bottom / 2, hWnd, NULL, g_hInst, NULL);

		hwndChild[3] = CreateWindowEx(WS_EX_DLGMODALFRAME, "Child Window Class Name_4", NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rt.right/2, rt.bottom/2, rt.right / 2, rt.bottom / 2, hWnd, NULL, g_hInst, NULL);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefFrameProc(hWnd, hwndClient, iMsg, wParam, lParam);
}

LRESULT CALLBACK ChildWndProc_1(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static char str[10][100];
	static int i = 0;
	static int count = 0;
	static SIZE size;
	static int home = 0;
	switch (iMsg) // ���� Ư���� ������ ���ǵǾ� ���� ���� 
	{
	case WM_CREATE:
		CreateCaret(hwndChild[0], NULL, 5, 15);
		ShowCaret(hwndChild[0]);
		break;
	case WM_LBUTTONDOWN:
		hWnd = SetFocus(hwndChild[0]);
		break;
	case WM_CHAR:
		hdc = GetDC(hwndChild[0]);
		if (wParam == VK_BACK)
		{
			count--;
			if (count == 0)
			{
				i--;
				count = strlen(str[i]);
			}
		}
		else if (wParam == VK_RETURN)
		{
			i++;
			count = 0;
		}
		else if (wParam == VK_ESCAPE)
		{
			for (int j = 0; j < i; ++j)
			{
				for (int cnt = 0; cnt < count; ++cnt)
				{
					str[j][cnt] = '\0';
				}
			}
			count = 0;
			i = 0;
		}
		else if (wParam == VK_TAB)
		{
			for (int j = 0; j < 4; ++j)
			{
				str[i][count++] = ' ';
			}
			str[i][count] = '\0';
		}
		else if (wParam == '.')
		{
			home++;
		}
		else
		{
			str[i][count++] = wParam;
		}
		str[i][count] = '\0';
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwndChild[0], &ps);
		GetTextExtentPoint(hdc, str[i], strlen(str[i]), &size);
		for (int line = 0; line <= i; ++line)
		{
			TextOut(hdc, 0, line*20, str[line], strlen(str[line]));
		}
		if (home == 0)
		{
			SetCaretPos(size.cx, i * 20);
		}
		else
		{
			SetCaretPos(0, i * 20);
			home = 0;
		}
		EndPaint(hwndChild[0], &ps);
		break;
	case WM_DESTROY:
		HideCaret(hwndChild[0]);
		DestroyCaret();
		PostQuitMessage(0);
		break;

	}

	return DefMDIChildProc(hWnd, iMsg, wParam, lParam);
	// ���ϵ� ������ �޽��� �Լ������� �޽��� ó���� �� ���������� ȣ���ϴ� 
	// �Լ��� DefMDIChildProc (hwnd, iMsg, wParam, lParam)�̴� 
}

LRESULT CALLBACK ChildWndProc_2(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int oldX, oldY, endX, endY;
	static BOOL Drag;

	switch (iMsg) // ���� Ư���� ������ ���ǵǾ� ���� ���� 
	{
	case WM_CREATE:
		Drag = false;
		break;
	case WM_LBUTTONDOWN:
		if (!Drag)
		{
			oldX = endX = LOWORD(lParam);
			oldY = endY = HIWORD(lParam);
			Drag = true;
		}
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);
		if (Drag)
		{
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
			MoveToEx(hdc, oldX, oldY, NULL);
			LineTo(hdc, endX, endY);
			oldX = endX;
			oldY = endY;
		}
		break;
	case WM_LBUTTONUP:
		Drag = false;
		break;
	case WM_PAINT:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefMDIChildProc(hWnd, iMsg, wParam, lParam);
}

LRESULT CALLBACK ChildWndProc_3(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT round, _rt;
	static int speed = 10;
	static BOOL UP, RIGHT;
	switch (iMsg) // ���� Ư���� ������ ���ǵǾ� ���� ���� 
	{
	case WM_CREATE:
		GetClientRect(hWnd, &_rt);
		round.left = 100;
		round.top = 100;
		round.right = 140;
		round.bottom = 140;
		UP = false;
		RIGHT = true;
		SetTimer(hWnd, 1, 40, NULL);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		Ellipse(hdc, round.left, round.top, round.right, round.bottom);

		EndPaint(hwndChild[2], &ps);
		break;
	case WM_TIMER:
		if (RIGHT)
		{
			round.left += speed;
			round.right += speed;
		}
		else
		{
			round.left -= speed;
			round.right -= speed;
		}
		if (UP)
		{
			round.top -= speed;
			round.bottom -= speed;
		}
		else
		{
			round.top += speed;
			round.bottom += speed;
		}

		if (round.right + speed > _rt.right)
		{
			RIGHT = false;
		}
		else if (round.left - speed < 0)
		{
			RIGHT = true;
		}
		if (round.bottom + speed > _rt.bottom)
		{
			UP = true;
		}
		else if (round.top - speed < 0)
		{
			UP = false;
		}
		InvalidateRect(hwndChild[2], NULL, TRUE);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefMDIChildProc(hWnd, iMsg, wParam, lParam);
}

LRESULT CALLBACK ChildWndProc_4(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static char str[10][100];
	static int i = 0;
	static int count = 0;
	static SIZE size;
	static int home = 0;
	switch (iMsg) // ���� Ư���� ������ ���ǵǾ� ���� ���� 
	{
	case WM_CREATE:
		CreateCaret(hwndChild[3], NULL, 5, 15);
		ShowCaret(hwndChild[3]);
		break;
	case WM_LBUTTONDOWN:
		hWnd = SetFocus(hwndChild[3]);
		break;
	case WM_CHAR:
		hdc = GetDC(hwndChild[3]);
		if (wParam == VK_BACK)
		{
			count--;
			if (count == 0)
			{
				i--;
				count = strlen(str[i]);
			}
		}
		else if (wParam == VK_RETURN)
		{
			i++;
			count = 0;
		}
		else if (wParam == VK_ESCAPE)
		{
			for (int j = 0; j < i; ++j)
			{
				for (int cnt = 0; cnt < count; ++cnt)
				{
					str[j][cnt] = '\0';
				}
			}
			count = 0;
			i = 0;
		}
		else if (wParam == VK_TAB)
		{
			for (int j = 0; j < 4; ++j)
			{
				str[i][count++] = ' ';
			}
			str[i][count] = '\0';
		}
		else if (wParam == '.')
		{
			home++;
		}
		else
		{
			str[i][count++] = wParam;
		}
		str[i][count] = '\0';
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwndChild[3], &ps);
		GetTextExtentPoint(hdc, str[i], strlen(str[i]), &size);
		for (int line = 0; line <= i; ++line)
		{
			TextOut(hdc, 0, line * 20, str[line], strlen(str[line]));
		}
		if (home == 0)
		{
			SetCaretPos(size.cx, i * 20);
		}
		else
		{
			SetCaretPos(0, i * 20);
			home = 0;
		}
		EndPaint(hwndChild[3], &ps);
		break;
	case WM_DESTROY:
		HideCaret(hwndChild[3]);
		DestroyCaret();
		PostQuitMessage(0);
		break;
	}
	return DefMDIChildProc(hWnd, iMsg, wParam, lParam);
}