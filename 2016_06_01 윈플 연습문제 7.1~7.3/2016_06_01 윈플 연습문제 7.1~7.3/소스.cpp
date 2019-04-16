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

	// 윈도우 클래스 구조체 값 설정 

	//   WndClass.cbSize = sizeof(WndClass) ; 

	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = FrameWndProc; // 프레임 윈도우 프로시저 
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.lpszClassName = "Window Class Name";
	// 프레임 윈도우 클래스 이름 
	//   WndClass.hIconSm = LoadIcon(NULL,IDI_APPLICATION); 


	// 윈도우 클래스 등록 
	RegisterClass(&WndClass);


	WndClass.lpfnWndProc = ChildWndProc; // 차일드 윈도우 프로시저 
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = "Child Window Class Name";
	// 차일드 윈도우 클래스 이름

	RegisterClass(&WndClass); // 차일드 윈도우 클래스 등록


							  // 윈도우 생성 
	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	// 윈도우 출력 
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);


	// 이벤트 루프 처리 
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

CLIENTCREATESTRUCT clientcreate; // 차일드 생성을 관리하기 위한 구조체
static int index = 0;
static char buffer[10];
HWND hwndChild[10];

LRESULT CALLBACK FrameWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndClient; // 클라이언트 윈도우 


	MDICREATESTRUCT mdicreate;

	static int i;

	switch (iMsg)
	{
	case WM_CREATE: // 메뉴핸들 획득 -> 창 관리 부메뉴(0) 핸들 획득 
		clientcreate.hWindowMenu = GetSubMenu(GetMenu(hWnd), 0);
		clientcreate.idFirstChild = 100; // 첫 번째 차일드 ID 
		hwndClient = CreateWindow("MDICLIENT", NULL, WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE, // 윈도우 스타일 
			0, 0, 0, 0, // 좌표 
			hWnd, NULL, g_hInst, (LPSTR)&clientcreate);
		ShowWindow(hwndClient, SW_SHOW); // 클라이언트 윈도우 출력
		break;


	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_NEW: // 새문서 


			newFile++;

			wsprintf(buffer, "Child Window Title Name %d", index);
			mdicreate.szClass = "Child Window Class Name";
			mdicreate.szTitle = buffer;
			mdicreate.hOwner = g_hInst;// 프로그램 인스턴스 핸들, 전역번수로 선언 
			mdicreate.x = 0; // 자식 윈도우 X좌표 
			mdicreate.y = 0; // 자식 윈도우 Y좌표 
			mdicreate.cx = 800; // 자식 윈도우 폭 
			mdicreate.cy = 600; // 자식 윈도우 높이 
			mdicreate.style = 0;
			mdicreate.lParam = 0; // MDI 클라이언트 윈도우를 만들어라 

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

	switch (iMsg) // 현재 특별한 내용은 정의되어 있지 않음 
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

	// 차일드 윈도우 메시지 함수에서는 메시지 처리한 후 마지막으로 호출하는 
	// 함수가 DefMDIChildProc (hwnd, iMsg, wParam, lParam)이다 
}