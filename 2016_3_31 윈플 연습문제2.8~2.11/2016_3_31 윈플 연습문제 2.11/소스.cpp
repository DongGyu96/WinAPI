#include <windows.h>
#include <time.h>
#include <stdlib.h>
#define ARRAY_SIZE 5
#define LENGTH 20

typedef struct Shape {
	int stat;
	int x1;
	int y1;
	int x2;
	int y2;
	int red, green, blue;
}Shape;

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
		lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1100, 1000, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdshow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT IMessage, WPARAM wParam, LPARAM lParam) {
	static int red = 0;
	static int select = 0;
	static Shape shape[ARRAY_SIZE] = { 0,0,0,0,0,0,0,0 };
	static POINT point[ARRAY_SIZE][6] = { 0 };
	static int min[5] = { 0 };
	static int count = 0;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	HDC hdc;
	PAINTSTRUCT ps;
	switch (IMessage) {
	case WM_CREATE:
		srand((unsigned)time(NULL));
		ShowCaret(hWnd);
		break;
	case WM_KEYDOWN:
		if (wParam == 'e' || wParam == 'E')
		{
			shape[count].stat = 1;
			shape[count].x1 = rand() % 40;
			shape[count].y1 = rand() % 40;
			shape[count].x2 = rand() % 40;
			shape[count].y2 = rand() % 40;
			if (shape[count].x1 > shape[count].x2)
			{
				int temp;
				temp = shape[count].x1;
				shape[count].x1 = shape[count].x2;
				shape[count].x2 = temp;
			}
			if (shape[count].y1 > shape[count].y2)
			{
				int temp;
				temp = shape[count].y1;
				shape[count].y1 = shape[count].y2;
				shape[count].y2 = temp;
			}
			min[count] = shape[count].y2;
			shape[count].red = rand() % 256;
			shape[count].green = rand() % 256;
			shape[count].blue = rand() % 256;
			count++;
		}
		else if (wParam == 't' || wParam == 'T')
		{
			shape[count].stat = 2;
			for (int n = 0; n < 3; ++n)
			{
				point[count][n] = { (rand() % 40) * LENGTH, (rand() % 40) * LENGTH };
			}
			min[count] = point[count][2].y;
			shape[count].red = rand() % 256;
			shape[count].green = rand() % 256;
			shape[count].blue = rand() % 256;
			count++;
		}
		else if (wParam == 'r' || wParam == 'R')
		{
			shape[count].stat = 3;
			shape[count].x1 = rand() % 40;
			shape[count].y1 = rand() % 40;
			shape[count].x2 = rand() % 40;
			shape[count].y2 = rand() % 40;
			if (shape[count].x1 > shape[count].x2)
			{
				int temp;
				temp = shape[count].x1;
				shape[count].x1 = shape[count].x2;
				shape[count].x2 = temp;
			}
			if (shape[count].y1 > shape[count].y2)
			{
				int temp;
				temp = shape[count].y1;
				shape[count].y1 = shape[count].y2;
				shape[count].y2 = temp;
			}
			min[count] = shape[count].y2;
			shape[count].red = rand() % 256;
			shape[count].green = rand() % 256;
			shape[count].blue = rand() % 256;
			count++;
		}
		else if (wParam >= 49 && wParam <= 53)
		{
			select = wParam - 48;
		}
		else if (wParam == VK_DELETE)
		{
			if (select >= 1 && select <= 5)
			{
				shape[select - 1].stat = 0;
			}
		}
		else if (wParam == VK_UP)
		{
			if (select >= 1 && select <= 5)
			{
				if (shape[select - 1].stat != 2)
				{
					shape[select - 1].y1--;
					shape[select - 1].y2--;
				}
				else
				{
					for (int i = 0; i < 3; ++i)
					{
						point[select - 1][i].y -= LENGTH;
					}
				}
			}
		}
		else if (wParam == VK_LEFT)
		{
			if (select >= 1 && select <= 5)
			{
				if (shape[select - 1].stat != 2)
				{
					shape[select - 1].x1--;
					shape[select - 1].x2--;
				}
				else
				{
					for (int i = 0; i < 3; ++i)
					{
						point[select - 1][i].x -= LENGTH;
					}
				}
			}
		}
		else if (wParam == VK_RIGHT)
		{
			if (select >= 1 && select <= 5)
			{
				if (shape[select - 1].stat != 2)
				{
					shape[select - 1].x1++;
					shape[select - 1].x2++;
				}
				else
				{
					for (int i = 0; i < 3; ++i)
					{
						point[select - 1][i].x += LENGTH;
					}
				}
			}
		}
		else if (wParam == VK_DOWN)
		{
			if (select >= 1 && select <= 5)
			{
				if (shape[select - 1].stat != 2)
				{
					shape[select - 1].y1++;
					shape[select - 1].y2++;
				}
				else
				{
					for (int i = 0; i < 3; ++i)
					{
						point[select - 1][i].y += LENGTH;
					}
				}
			}
		}
		else if (wParam == VK_PRIOR)
		{
			if (select >= 1 && select <= 5)
			{
				if (shape[select - 1].stat != 2)
				{
					shape[select - 1].x1--;
					shape[select - 1].y1--;
					shape[select - 1].x2++;
					shape[select - 1].y2++;
				}
				else
				{
					point[select - 1][0].y -= LENGTH;
					point[select - 1][0].x -= LENGTH;
					point[select - 1][1].y += LENGTH;
					point[select - 1][1].x += LENGTH;
					point[select - 1][2].y += LENGTH;
					point[select - 1][2].x += LENGTH;
				}
			}
		}
		else if (wParam == VK_NEXT)
		{
			if (select >= 1 && select <= 5)
			{
				if (shape[select - 1].stat != 2)
				{
					shape[select - 1].x1++;
					shape[select - 1].y1++;
					shape[select - 1].x2--;
					shape[select - 1].y2--;
					if (min[select -1] > shape[select - 1].y2)
					{
						shape[select - 1].stat = (rand() % 3) + 1;
						if (shape[select - 1].stat == 2)
						{
							for (int n = 0; n < 3; ++n)
							{
								point[select - 1][n] = { (rand() % 40) * LENGTH, (rand() % 40) * LENGTH };
							}
							min[count] = point[select - 1][2].y;
							shape[select - 1].red = rand() % 256;
							shape[select - 1].green = rand() % 256;
							shape[select - 1].blue = rand() % 256;
						}
						else
						{
							shape[select - 1].x1 = rand() % 40;
							shape[select - 1].y1 = rand() % 40;
							shape[select - 1].x2 = rand() % 40;
							shape[select - 1].y2 = rand() % 40;
							if (shape[select - 1].x1 > shape[select - 1].x2)
							{
								int temp;
								temp = shape[select - 1].x1;
								shape[select - 1].x1 = shape[select - 1].x2;
								shape[select - 1].x2 = temp;
							}
							if (shape[select - 1].y1 > shape[select - 1].y2)
							{
								int temp;
								temp = shape[select - 1].y1;
								shape[select - 1].y1 = shape[select - 1].y2;
								shape[select - 1].y2 = temp;
							}
							min[select - 1] = shape[select - 1].y2;
							shape[select - 1].red = rand() % 256;
							shape[select - 1].green = rand() % 256;
							shape[select - 1].blue = rand() % 256;
						}
					}
				}
				else
				{
					point[select - 1][0].y += LENGTH;
					point[select - 1][0].x += LENGTH;
					point[select - 1][1].y -= LENGTH;
					point[select - 1][1].x -= LENGTH;
					point[select - 1][2].y -= LENGTH;
					point[select - 1][2].x -= LENGTH;
					if (min[select - 1] > point[select - 1][2].y)
					{
						shape[select - 1].stat = (rand() % 3) + 1;
						if (shape[select - 1].stat == 2)
						{
							for (int n = 0; n < 3; ++n)
							{
								point[select - 1][n] = { (rand() % 40) * LENGTH, (rand() % 40) * LENGTH };
							}
							min[count] = point[select - 1][2].y;
							shape[select - 1].red = rand() % 256;
							shape[select - 1].green = rand() % 256;
							shape[select - 1].blue = rand() % 256;
						}
						else
						{
							shape[select - 1].x1 = rand() % 40;
							shape[select - 1].y1 = rand() % 40;
							shape[select - 1].x2 = rand() % 40;
							shape[select - 1].y2 = rand() % 40;
							if (shape[select - 1].x1 > shape[select - 1].x2)
							{
								int temp;
								temp = shape[select - 1].x1;
								shape[select - 1].x1 = shape[select - 1].x2;
								shape[select - 1].x2 = temp;
							}
							if (shape[select - 1].y1 > shape[select - 1].y2)
							{
								int temp;
								temp = shape[select - 1].y1;
								shape[select - 1].y1 = shape[select - 1].y2;
								shape[select - 1].y2 = temp;
							}
							min[select - 1] = shape[select - 1].y2;
							shape[select - 1].red = rand() % 256;
							shape[select - 1].green = rand() % 256;
							shape[select - 1].blue = rand() % 256;
						}
					}
				}
			}
		}
		if (count == 5)
		{
			count = 0;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (int i = 0; i < 40; ++i)
		{
			for (int j = 0; j < 40; ++j)
			{
				Rectangle(hdc, LENGTH*j, LENGTH*i, LENGTH*(j + 1), LENGTH*(i + 1));
			}
		}
		for (int i = 0; i < ARRAY_SIZE; ++i)
		{
			int width = 1;
			hBrush = CreateSolidBrush(RGB(shape[i].red, shape[i].green, shape[i].blue));
			oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			if (i == select - 1)
			{
				red = 255;
				width = 5;
			}
			hPen = CreatePen(PS_SOLID, width, RGB(red, 0, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			if (shape[i].stat != 0)
			{
				if (shape[i].stat == 1)
				{
					Ellipse(hdc, shape[i].x1* LENGTH, shape[i].y1* LENGTH, shape[i].x2* LENGTH, shape[i].y2* LENGTH);
				}
				else if (shape[i].stat == 2)
				{
					Polygon(hdc, point[i], 3);
				}
				else if (shape[i].stat == 3)
				{
					Rectangle(hdc, shape[i].x1* LENGTH, shape[i].y1* LENGTH, shape[i].x2*LENGTH, shape[i].y2* LENGTH);
				}
				red = 0;
				width = 0;
			}
			else
			{
				continue;
			}
		}
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, IMessage, wParam, lParam);
}