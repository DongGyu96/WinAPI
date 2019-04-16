#include <Windows.h>

typedef struct
{
	int xPos;
	int yPos;
	int size;
	int dir_X;
	int dir_Y;
	int color_R;
	int color_G;
	int color_B;
} _BALL;

typedef struct
{
	int xPos;
	int yPos;
	int height;
	int width;
	int dir_X;
	int dir_Y;
} _BAR;

typedef struct
{
	int xPos;
	int yPos;
	int height;
	int width;
	int state;
	int color_R;
	int life;
} _BLOCK;

enum { LEFT = -1, UP = -1, RIGHT = 1, DOWN = 1 };
enum MODE { S_Mode, M_Mode };
enum { Slow = 100, Medium = 70, Fast = 40 };

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	HINSTANCE g_hInst;//
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = "API Programming";
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	RECT rcWindow = { 0, 0, 800, 700 };
	AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, false);
	hWnd = CreateWindow("API Programming", "API Programming", WS_OVERLAPPEDWINDOW, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush, oldBrush;
	static RECT rectView;
	static int mouse_X, mouse_Y;
	static char str[100];

	static _BAR Bar;
	static _BALL Ball;
	static _BLOCK Block[5][10];
	static int Speed;
	static BOOL Click, Start;
	static MODE Mode;                              // 'S', 'M' ��� ����
	static int Block_delete_num;                     // ���ŵ� ���� ����
	static int Block_change_color_num;                  // �����ִ� ���ٲ� ���� ����
	static int Time;                              // �����÷��� �ð�
	static int map[5][10] = {                        // �� ��
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } };
	int i, j;

	switch (iMsg) {
	case WM_CREATE:
		GetClientRect(hWnd, &rectView);
		Start = false;
		Ball.xPos = 400, Ball.yPos = 290;                     // Ball ���� �Ӽ�
		Ball.size = 30;
		Ball.dir_X = 1;
		Ball.dir_Y = 1;
		Ball.color_R = 255;
		Ball.color_G = 128;
		Ball.color_B = 128;

		Bar.xPos = 340;                        // Bar ���� �Ӽ�
		Bar.yPos = 600;
		Bar.height = 30;
		Bar.width = 160;

		for (i = 0; i < 2; ++i)                              // Block �ʱ�ȭ
		{
			for (j = 0; j < 10; ++j)
			{
				if (map[i][j] == 1)
				{
					Block[i][j].xPos = 80 * j;
					Block[i][j].yPos = 40 * i;
					Block[i][j].width = 80;
					Block[i][j].height = 40;
					Block[i][j].color_R = 255;
					Block[i][j].life = 2;                           // Block ������
				}
			}
		}
		Block_delete_num = 0;
		Block_change_color_num = 0;
		Speed = Medium; 
		SetTimer(hWnd, 1, Speed, NULL);
		break;
	case WM_CHAR:
		if (wParam == 'q' || wParam == 'Q')
			Speed = Slow;
		else if (wParam == 'w' || wParam == 'W')
			Speed = Medium;
		else if (wParam == 'e' || wParam == 'E')
			Speed = Fast;
		KillTimer(hWnd, 1);
		SetTimer(hWnd, 1, Speed, NULL);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDOWN:
		Click = TRUE;
		Bar.xPos = LOWORD(lParam);
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONUP:
		Click = FALSE;
		if (Mode == S_Mode)// S ��� �϶� ��ġ �ʱ�ȭ
		{
			Bar.xPos = 340;
			Bar.yPos = 600;
		}
		break;
	case WM_MOUSEMOVE:
		if (Click == TRUE)
		{
			Bar.xPos = LOWORD(lParam);
		}
		break;

	case WM_TIMER:
		if (Start)
		{
			switch (wParam)
			{
			case 1:                                          // ���� �̵� Ÿ�̸�
				Ball.xPos += 20 * Ball.dir_X;
				Ball.yPos += 20 * Ball.dir_Y;

				if (Ball.xPos < rectView.left + 30)                  // ���� �� �浹
					Ball.dir_X = RIGHT;
				else if (Ball.xPos > rectView.right - 30)            // ������ �� �浹
					Ball.dir_X = LEFT;
				else if (Ball.yPos < rectView.top + 30)                  // ���� �� �浹
					Ball.dir_Y = DOWN;
				else if (Ball.yPos > rectView.bottom - 40)            // �Ʒ� �� �浹
					Ball.xPos = 300, Ball.yPos = 290;               // �Ʒ��� ������ �� �� ��ġ �� ����

																	//////////////////////////////////////////////////////////////////////  �� �浹 üũ
																	// 1. ���� ���� �浹 üũ
				if (Bar.xPos <= Ball.xPos  && Ball.xPos <= Bar.xPos + Bar.width)   // ���� �Ʒ������� �� ��ġ���� ������
				{
					if (Ball.yPos + Ball.size >= Bar.yPos)                     // ���� �Ʒ������� ���� �����ο� �����ϸ�
						Ball.dir_Y = UP;
				}
				// 2. ���� ���ʸ� �浹 üũ
				if (Bar.yPos <= Ball.yPos && Ball.yPos + Ball.size <= Bar.yPos + Bar.height)
				{
					if (Ball.xPos + Ball.size >= Bar.xPos)
						Ball.dir_X = LEFT;
				}

				if (Bar.yPos <= Ball.yPos && Ball.yPos + Ball.size <= Bar.yPos + Bar.height)
					Ball.dir_X = RIGHT;

				for (i = 0; i < 2; ++i)
				{
					for (j = 0; j < 10; ++j)
					{
						if (Block[i][j].xPos <= Ball.xPos  && Ball.xPos <= Block[i][j].xPos + Block[i][j].width)
						{
							if (Block[i][j].yPos + Block[i][j].height == Ball.yPos - Ball.size)
							{
								Block[i][j].color_R = 0;
								Ball.dir_Y = DOWN;

								--Block[i][j].life;
							}
						}
					}
				}

				for (i = 0; i < 2; ++i)
				{
					for (j = 0; j < 10; ++j)
					{
						if (Block[i][j].life == 0)
						{
							map[i][j] = 0;
							Block[i][j].yPos = 0;
							Block[i][j].height = 0;
							Block[i][j].color_R = 255;
						}
					}
				}
				Block_delete_num = 0;
				Block_change_color_num = 0;

				for (i = 0; i < 2; ++i)
				{// ���ŵ� ������ ������ ���� üũ
					for (j = 0; j < 10; ++j)
					{
						if (map[i][j] == 0)
							++Block_delete_num;
						if (Block[i][j].life == 1)
							++Block_change_color_num;
					}
				}

				if (Block_delete_num == 20)                                 // ��� ���� ���Ž� ���� ����
					KillTimer(hWnd, 1);

				break;
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		hBrush = CreateSolidBrush(RGB(Ball.color_R, Ball.color_G, Ball.color_B));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Ellipse(hdc, Ball.xPos - Ball.size, Ball.yPos - Ball.size, Ball.xPos + Ball.size, Ball.yPos + Ball.size);         // Ball ����
		DeleteObject(hBrush);

		hBrush = CreateSolidBrush(RGB(0, 255, 255));
		SelectObject(hdc, hBrush);
		Rectangle(hdc, Bar.xPos, Bar.yPos, Bar.xPos + Bar.width, Bar.yPos + Bar.height);      // Bar ����
		DeleteObject(hBrush);

		for (i = 0; i < 2; ++i)                                                // Block ����
		{
			for (j = 0; j < 10; ++j)
			{
				if (map[i][j] == 1)
				{
					hBrush = CreateSolidBrush(RGB(Block[i][j].color_R, 200, 200));
					SelectObject(hdc, hBrush);
					Rectangle(hdc, Block[i][j].xPos, Block[i][j].yPos, Block[i][j].xPos + Block[i][j].width, Block[i][j].yPos + Block[i][j].height);      // Block ����
					DeleteObject(hBrush);
				}
			}
		}
		wsprintf(str, "����� ���� : %d", Block_delete_num);
		TextOut(hdc, 10, 660, str, strlen(str));
		wsprintf(str, "���̺��� ���� : %d", Block_change_color_num);
		TextOut(hdc, 10, 680, str, strlen(str));

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}