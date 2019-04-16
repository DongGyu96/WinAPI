#include <windows.h>
#include <stdlib.h>
#include <time.h>
#define size 2
#define speed 5

HINSTANCE g_hInst;
LPCTSTR lpszClass = "window program";

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevinstance, LPSTR lpszCmdParam, int nCmdshow) {
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

	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(
		lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 700, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdshow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT IMessage, WPARAM wParam, LPARAM lParam) {
	static int x1[10] = { 0 }, x2[10] = { 0 }, y1[10] = { 0 }, y2[10] = { 0 }, px[10] = { 0 }, py[10] = { 10 };
	static int startX, startY, oldX, oldY, endX, endY;
	static int count = 0;
	static int distX = 0, distY = 0;
	static int select = -1;
	static int n;
	static bool drag = FALSE;
	static bool move = FALSE;
	HBRUSH hBrush, oldBrush;
	HPEN hPen, oldPen;
	HDC hdc;
	PAINTSTRUCT ps;
	switch (IMessage) {
	case WM_CREATE:
		srand((unsigned)time(NULL));
		SetTimer(hWnd, 1, 2000, NULL);
		break;
	case WM_LBUTTONDOWN:
		startX = oldX = LOWORD(lParam);
		startY = oldY = HIWORD(lParam);
		for (int i = 0; i < 10; ++i)
		{
			if ((startX <= x2[i] && startX >= x1[i]) && (startY <= y2[i] && startY >= y1[i]))
			{
				select = i;
				SetTimer(hWnd, 4, 160, NULL);
				break;
			}
		}
		move = FALSE;
		drag = TRUE;
		break;
	case WM_LBUTTONUP:
		move = TRUE;
		drag = FALSE;
		SetTimer(hWnd, 5, 10000, NULL);
		break;
	case WM_MOUSEMOVE:
		hdc = GetDC(hWnd);
		if (drag)
		{
			distX = 0, distY = 0;
			SetROP2(hdc, R2_XORPEN);
			SelectObject(hdc, (HPEN)GetStockObject(WHITE_PEN));

			endX = LOWORD(lParam);
			endY = HIWORD(lParam);

			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, oldX, oldY);

			MoveToEx(hdc, startX, startY, NULL);
			LineTo(hdc, endX, endY);
			oldX = endX;
			oldY = endY;
			distX = endX - px[select];
			distX /= 15;
			distY = endY - py[select];
			distY /= 15;
		}
		ReleaseDC(hWnd, hdc);
		break;
	case WM_TIMER:
		hdc = GetDC(hWnd);
		switch (wParam) {
		case 1:
			n = rand() % 80 + 60;
			if (count == select)
			{
				drag = FALSE;
				move = FALSE;
				select = -1;
			}
			if (count != 10)
			{
				x1[count] = rand() % 900;
				y1[count] = rand() % 600;
				x2[count] = x1[count] + n;
				y2[count] = y1[count] + n;
				px[count] = (x1[count] + x2[count]) / 2;
				py[count] = (y1[count] + y2[count]) / 2;
				count++;
			}
			else if (count == 10)
			{
				count = 0;
			}
			SetTimer(hWnd, 2, 600, NULL);
			SetTimer(hWnd, 3, 3000, NULL);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 2:
			for (int i = 0; i < 10; ++i)
			{
				if (x2[i] != 0)
				{
					if (x2[i] - x1[i] <= 0)
					{
						x1[i] = 0;
						x2[i] = 0;
						y1[i] = 0;
						y2[i] = 0;
					}
					else
					{
						x1[i] += size;
						y1[i] += size;
						x2[i] -= size;
						y2[i] -= size;
						px[i] = (x1[i] + x2[i]) / 2;
						py[i] = (y1[i] + y2[i]) / 2;
					}
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 3:
			x1[count] = 0;
			x2[count] = 0;
			y1[count] = 0;
			y2[count] = 0;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 4:
			if (move)
			{
				px[select] = (x1[select] + x2[select]) / 2;
				py[select] = (y1[select] + y2[select]) / 2;
				if (distX > 0)
				{
					if (endX > px[select])
					{
						x1[select] += distX;
						x2[select] += distX;
						y1[select] += distY;
						y2[select] += distY;
					}
				}
				else
				{
					if (endX < px[select])
					{
						x1[select] += distX;
						x2[select] += distX;
						y1[select] += distY;
						y2[select] += distY;
					}
				}
			}
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 5:
			move = FALSE;
			select = -1;
		}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		hBrush = CreateSolidBrush(RGB(100, 255, 100));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

		MoveToEx(hdc, startX, startY, NULL);
		LineTo(hdc, oldX, oldY);
		for (int i = 0; i < 10; ++i)
		{
			if (i == select)
			{
				hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, hPen);
				Ellipse(hdc, x1[i], y1[i], x2[i], y2[i]);
				SelectObject(hdc, oldPen);
				DeleteObject(hPen);
			}
			else
			{
				hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
				oldPen = (HPEN)SelectObject(hdc, hPen);
				Ellipse(hdc, x1[i], y1[i], x2[i], y2[i]);
				SelectObject(hdc, oldPen);
				DeleteObject(hPen);
			}
		}

		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		KillTimer(hWnd, 3);
		KillTimer(hWnd, 5);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, IMessage, wParam, lParam);
}