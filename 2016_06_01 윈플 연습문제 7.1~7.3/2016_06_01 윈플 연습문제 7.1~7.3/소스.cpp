#include "resource.h"

#include <windows.h> 

HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FrameWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);


static char str[10][10][99];
static char strcopy[10][10][99];

int newFile = 0;

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.lpszClassName = "Window Class Name";
	// ������ ������ Ŭ���� �̸� 
	//   WndClass.hIconSm = LoadIcon(NULL,IDI_APPLICATION); 


	// ������ Ŭ���� ��� 
	RegisterClass(&WndClass);


	WndClass.lpfnWndProc = ChildWndProc; // ���ϵ� ������ ���ν��� 
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Child Window Class Name";
	// ���ϵ� ������ Ŭ���� �̸�

	RegisterClass(&WndClass); // ���ϵ� ������ Ŭ���� ���


							  // ������ ���� 
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
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
static int index = 0;
static char buffer[10];
HWND hwndChild[10];

LRESULT CALLBACK FrameWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndClient; // Ŭ���̾�Ʈ ������ 


	MDICREATESTRUCT mdicreate;

	static int i;

	switch (iMsg)
	{
	case WM_CREATE: // �޴��ڵ� ȹ�� -> â ���� �θ޴�(0) �ڵ� ȹ�� 
		clientcreate.hWindowMenu = GetSubMenu(GetMenu(hWnd), 0);
		clientcreate.idFirstChild = 100; // ù ��° ���ϵ� ID 
		hwndClient = CreateWindow("MDICLIENT", NULL, WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE, // ������ ��Ÿ�� 
			0, 0, 0, 0, // ��ǥ 
			hWnd, NULL, g_hInst, (LPSTR)&clientcreate);
		ShowWindow(hwndClient, SW_SHOW); // Ŭ���̾�Ʈ ������ ���
		break;


	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_NEW: // ������ 


			newFile++;

			wsprintf(buffer, "Child Window Title Name %d", index);
			mdicreate.szClass = "Child Window Class Name";
			mdicreate.szTitle = buffer;
			mdicreate.hOwner = g_hInst;// ���α׷� �ν��Ͻ� �ڵ�, ���������� ���� 
			mdicreate.x = 0; // �ڽ� ������ X��ǥ 
			mdicreate.y = 0; // �ڽ� ������ Y��ǥ 
			mdicreate.cx = 800; // �ڽ� ������ �� 
			mdicreate.cy = 600; // �ڽ� ������ ���� 
			mdicreate.style = 0;
			mdicreate.lParam = 0; // MDI Ŭ���̾�Ʈ �����츦 ������ 

			hwndChild[index] = (HWND)SendMessage(hwndClient, WM_MDICREATE, 0, (LPARAM)(LPMDICREATESTRUCT)&mdicreate);
			SendMessage(hwndClient, WM_MDICASCADE, 0, 0);
			index++;

			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;


	}

	return DefFrameProc(hWnd, hwndClient, iMsg, wParam, lParam);
}

LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	static int count, yPos, line;
	static SIZE size;

	int i, j;
	static int w;

	switch (iMsg) // ���� Ư���� ������ ���ǵǾ� ���� ���� 
	{
	case WM_CREATE:
		count = 0;
		break;

	case WM_LBUTTONDOWN:
		for (int i = 0; i <= newFile; ++i)
		{
			if (hWnd == hwndChild[i])
			{
				w = i + 1;
				count = 0;
				break;
			}
		}
		break;

	case WM_CHAR:
		hdc = GetDC(hWnd);

		if (wParam == VK_BACK) {
			count--;
		}
		else if (wParam == VK_RETURN)
		{
			if (line < 9) {
				count = 0;
				yPos = yPos + 20;
				line++;
			}

		}
		else if (wParam == VK_ESCAPE)
		{
			for (i = 0; i <= line; i++)
			{
				for (j = 0; j <= count; j++)
				{
					strcopy[w][i][j] = str[w][i][j];
					str[newFile][i][j] = '\0';

				}
			}
			yPos = 0;
			//line = 0;

		}
		else if (wParam == VK_TAB)
		{
			for (i = 0; i <= line; i++)
			{
				for (j = 0; j <= count; j++)
				{
					str[w][i][j] = strcopy[w][i][j];
				}
			}
			yPos = yPos + 20;

		}
		else
			if (count < 98) {
				str[w][line][count++] = wParam;
			}

		str[w][line][count] = '\0';
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_PAINT:
		for (j = 0; j<10; j++)
		{
			hdc = BeginPaint(hwndChild[j - 1], &ps);


			for (i = 0; i <= line; i++) {
				GetTextExtentPoint(hdc, str[j][i], strlen(str[j][i]), &size);
				TextOut(hdc, 0, i * 20, str[j][i], strlen(str[j][i]));

			}

			EndPaint(hwndChild[j - 1], &ps);

		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}

	return DefMDIChildProc(hWnd, iMsg, wParam, lParam);

	// ���ϵ� ������ �޽��� �Լ������� �޽��� ó���� �� ���������� ȣ���ϴ� 
	// �Լ��� DefMDIChildProc (hwnd, iMsg, wParam, lParam)�̴� 
}