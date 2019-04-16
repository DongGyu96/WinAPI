#include <windows.h> 
#include <atlimage.h>
#include "resource.h"
#define speed 5
HINSTANCE g_hInst;
LPCTSTR lpszClass = "Window Class Name";

//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc_1(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc_2(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
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
HWND hwndChild[2];
CImage img;
BOOL Copy;
int mWidth, mHeight, width, height;
int startX = 0, startY = 0, endX = 0, endY = 0;
double X, Y, X1, Y1;

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
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, 0, 0, rt.right / 2, rt.bottom, hWnd, NULL, g_hInst, NULL);

		hwndChild[1] = CreateWindowEx(WS_EX_DLGMODALFRAME, "Child Window Class Name_2", NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rt.right / 2, 0, rt.right / 2, rt.bottom, hWnd, NULL, g_hInst, NULL);
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
	static RECT rt;
	static HPEN hPen, oldPen;
	static HBRUSH hBrush;
	static BOOL drag, start;
	switch (iMsg) // ���� Ư���� ������ ���ǵǾ� ���� ���� 
	{
	case WM_CREATE:
		start = TRUE;
		drag = FALSE;
		Copy = FALSE;
		GetClientRect(hWnd, &rt);
		img.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		break;
	case WM_LBUTTONDOWN:
		if (start)
		{
			startX = LOWORD(lParam);
			startY = HIWORD(lParam);
			drag = TRUE;
		}
		hWnd = SetFocus(hwndChild[0]);
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);
		if (drag)
		{
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
			hBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
			Rectangle(hdc, startX, startY, endX, endY);
			DeleteObject(hBrush);
		}
		break;
	case WM_LBUTTONUP:
		if (start)
		{
			drag = FALSE;
			endX = LOWORD(lParam);
			endY = HIWORD(lParam);
			start = FALSE;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwndChild[0], &ps);

		mWidth = img.GetWidth();
		mHeight = img.GetHeight();
		width = endX - startX;
		height = endY - startY;
		X = (double)startX / (double)rt.right;
		Y = (double)startY / (double)rt.bottom;
		X1 = (double)width / (double)rt.right;
		Y1 = (double)height / (double)rt.bottom;
		width = mWidth * X1;
		height = mHeight * Y1;
		X = mWidth * X;
		Y = mHeight * Y;

		img.StretchBlt(hdc, 0, 0, rt.right, rt.bottom, 0, 0, img.GetWidth(), img.GetHeight(), SRCCOPY);

		hPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		hBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
		Rectangle(hdc, startX, startY, endX, endY);
		DeleteObject(hBrush);
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
		img.StretchBlt(hdc, startX, startY, endX - startX, endY - startY, X, Y, width, height, SRCCOPY);
		EndPaint(hwndChild[0], &ps);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_UP)
		{
			startY -= speed;
			endY -= speed;
		}
		else if (wParam == VK_DOWN)
		{
			startY += speed;
			endY += speed;
		}
		else if (wParam == VK_RIGHT)
		{
			startX += speed;
			endX += speed;
		}
		else if (wParam == VK_LEFT)
		{
			startX -= speed;
			endX -= speed;
		}
		else if (wParam == 'c' || wParam == 'C')
		{
			Copy = true;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		InvalidateRect(hwndChild[1], NULL, TRUE);
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
	static RECT rt;
	switch (iMsg) // ���� Ư���� ������ ���ǵǾ� ���� ���� 
	{
	case WM_CREATE:
		GetClientRect(hWnd, &rt);
		break;
	case WM_LBUTTONDOWN:
		hWnd = SetFocus(hwndChild[1]);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwndChild[1], &ps);

		if (Copy)
		{
			img.StretchBlt(hdc, 100, 100, rt.right - 200, rt.bottom - 200, X, Y, width, height, SRCCOPY);
		}
		EndPaint(hwndChild[1], &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefMDIChildProc(hWnd, iMsg, wParam, lParam);
}