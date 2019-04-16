#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

HINSTANCE g_hInst;

enum direction // ���� ����
{
	right = 1,
	left,
	down,
	up
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdshow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	// ������ Ŭ���� ����ü �� ���� 
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = "Window Class Name";
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;

	// ������ Ŭ���� ��� 
	RegisterClass(&WndClass);

	// ������ ���� 
	RECT rcWindow = { 0, 0, 900, 600 };
	AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, false);
	hWnd = CreateWindow("Window Class Name", "", WS_OVERLAPPEDWINDOW, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, NULL, (HMENU)NULL, hInstance, NULL);

	// ������ ���
	ShowWindow(hWnd, nCmdshow);
	UpdateWindow(hWnd);

	// �̺�Ʈ ���� ó��
	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HPEN hPen, oldPen;
	HBRUSH hBrush, oldBrush;
	static RECT head, body[30], item[30], bomb[10];
	static int count = 1;
	static int speed = 50;
	static int temp = 0;
	static int X = 0, Y = 0;
	static int bx = 0, by = 0;
	static direction myDirection = right;
	static int newone = 0;
	static bool move = false;
	static int jumptime = 0;
	static int item_count = 0;
	static int bomb_count = 0;
	static int item_limit = 10;
	static int nt = 300, nb = 340, nl = 300, nr = 340;

	// �޽��� ó���ϱ� 
	switch (uMsg)
	{

	case WM_CREATE:
		srand((unsigned)time(NULL));
		body[0].left = 0;
		body[0].top = 0;
		body[0].right = 40;
		body[0].bottom = 40;
		head.left = 40;
		head.top = 0;
		head.right = 80;
		head.bottom = 40;
		SetTimer(hWnd, 1, 160, NULL);
		SetTimer(hWnd, 3, 2000, NULL);
		SetTimer(hWnd, 4, 3000, NULL);

		break;
	case WM_LBUTTONDOWN:
		bx = (body[0].right + body[0].left) / 2;
		by = (body[0].top + body[0].bottom) / 2;
		X = LOWORD(lParam);
		Y = HIWORD(lParam);
		if (X > bx)
		{
			if (abs(X - bx) > abs(Y - by))
			{
				temp = 0;
			}
			else
			{
				if (Y - by > 0)
				{
					temp = 1;
				}
				else
				{
					temp = 2;
				}
			}
		}
		else
		{
			if (abs(X - bx) > abs(Y - by))
			{
				temp = 3;
			}
			else
			{
				if (Y - by > 0)
				{
					temp = 1;
				}
				else
				{
					temp = 2;
				}
			}
		}
		move = true;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		///////////////////////////////////////////////////////�Ӹ� �׸���
		hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		Ellipse(hdc, head.left, head.top, head.right, head.bottom);
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);

		if (newone>5)
		{
			hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			Ellipse(hdc, nl, nt, nr, nb);
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);

		}

		///////////////////////////////////////////////////////���� �׸���
		hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		for (int i = 0; i < count; ++i)
		{
			Ellipse(hdc, body[i].left, body[i].top, body[i].right, body[i].bottom);
		}
		hBrush = CreateSolidBrush(RGB(255, 0, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		for (int i = 0; i < item_count; ++i)
		{
			Ellipse(hdc, item[i].left, item[i].top, item[i].right, item[i].bottom);
		}
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);

		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		for (int i = 0; i < bomb_count; ++i)
		{
			Rectangle(hdc, bomb[i].left, bomb[i].top, bomb[i].right, bomb[i].bottom);
		}
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
		EndPaint(hWnd, &ps);
		break;

	case WM_CHAR:
		if (wParam == '+')
		{
			speed += 5;
		}
		else if (wParam == '-')
		{
			speed -= 5;
		}
		break;
	case WM_KEYDOWN:
		if (wParam == VK_SPACE) //����
		{
			SetTimer(hWnd, 2, 20, NULL); // ���� Ÿ�̸Ӹ� �ҷ��´�
		}
		else if (wParam == VK_UP)
		{
			myDirection = up;
		}
		else if (wParam == VK_LEFT)
		{
			myDirection = left;
		}
		else if (wParam == VK_RIGHT)
		{
			myDirection = right;
		}
		else if (wParam == VK_DOWN)
		{
			myDirection = down;
		}
		break;

	case WM_TIMER:
		newone++;


		switch (wParam)
		{

		case 1:
		{
			if (move == true || move == false)
			{
				if (head.left > nl)
				{
					nl += speed*0.5;
					nr += speed*0.5;
				}
				if (head.left < nl)
				{
					nl -= speed*0.5;
					nr -= speed*0.5;
				}
				if (head.top > nt)
				{
					nt += speed*0.5;
					nb += speed*0.5;
				}
				if (head.top < nt)
				{
					nt -= speed*0.5;
					nb -= speed*0.5;
				}
			}

			if (move == true) //����Ű�� ��������
			{

				if (temp == 0)  // ������ Ű ��������
				{
					myDirection = right;
					if (head.right <= 770 || body[0].right <= 770) //������ ���� ��� �� ����
					{

						head.left += speed;
						head.right += speed;
						body[0].top = head.top; //������ �׻� �Ӹ��� �ڿ� �ִ�
						body[0].bottom = head.bottom;
						body[0].left = head.left - 40; // �Ӹ��� ���ʿ� ������ -40
						body[0].right = head.right - 40;
						for (int i = 1; i < count; ++i)
						{
							body[i].top = body[i - 1].top;
							body[i].bottom = body[i - 1].bottom;
							body[i].left = body[i - 1].left - 40;
							body[i].right = body[i - 1].right - 40;
						}
						for (int i = 0; i < item_count; ++i)
						{
							if (head.left < item[i].right && head.top <item[i].bottom && head.right >item[i].left && head.bottom > item[i].top)
							{
								item[i].left = 0;
								item[i].top = 0;
								item[i].bottom = 0;
								item[i].right = 0;
								item_limit--;
								if (item_limit == 0)
								{
									PostQuitMessage(0);
								}
								body[count].top = body[count - 1].top;
								body[count].bottom = body[count - 1].bottom;
								body[count].left = body[count - 1].left - 40;
								body[count].right = body[count - 1].right - 40;
								count++;
								InvalidateRect(hWnd, NULL, true);
								break;
							}
						}
						for (int i = 0; i < bomb_count; ++i)
						{
							if (head.left < bomb[i].right && head.top <bomb[i].bottom && head.right >bomb[i].left && head.bottom > bomb[i].top)
							{
								count--;
								if (count == 0)
								{
									PostQuitMessage(0);
								}
								body[count].top = 0;
								body[count].left = 0;
								body[count].bottom = 0;
								body[count].right = 0;
								bomb[i].top = 0;
								bomb[i].left = 0;
								bomb[i].bottom = 0;
								bomb[i].right = 0;
								InvalidateRect(hWnd, NULL, true);
								break;
							}
						}
					}
					else
						move = false; // ���� ������ �⺻ �̵�
				}

				if (temp == 1)  // �Ʒ��� Ű ��������
				{
					myDirection = down;

					if (head.bottom <= 550) // �ٴڿ� ��� �� ����
					{
						head.top += speed;
						head.bottom += speed;
						body[0].top = head.top - 40; //������ �׻� �Ӹ��� �ڿ� �ִ� �Ӹ��� ���ʿ� ������ �Ӹ��� -40
						body[0].bottom = head.bottom - 40;
						body[0].left = head.left;
						body[0].right = head.right;
						for (int i = 1; i < count; ++i)
						{
							body[i].top = body[i - 1].top - 40;
							body[i].bottom = body[i - 1].bottom - 40;
							body[i].left = body[i - 1].left;
							body[i].right = body[i - 1].right;
						}
						for (int i = 0; i < item_count; ++i)
						{
							if (head.left < item[i].right && head.top <item[i].bottom && head.right >item[i].left && head.bottom > item[i].top)
							{
								item[i].left = 0;
								item[i].top = 0;
								item[i].bottom = 0;
								item[i].right = 0;
								item_limit--;
								if (item_limit == 0)
								{
									PostQuitMessage(0);
								}
								body[count].top = body[count - 1].top-40;
								body[count].bottom = body[count - 1].bottom-40;
								body[count].left = body[count - 1].left;
								body[count].right = body[count - 1].right;
								count++;
								InvalidateRect(hWnd, NULL, true);
								break;
							}
						}
						for (int i = 0; i < bomb_count; ++i)
						{
							if (head.left < bomb[i].right && head.top <bomb[i].bottom && head.right >bomb[i].left && head.bottom > bomb[i].top)
							{
								count--;
								if (count == 0)
								{
									PostQuitMessage(0);
								}
								body[count].top = 0;
								body[count].left = 0;
								body[count].bottom = 0;
								body[count].right = 0;
								bomb[i].top = 0;
								bomb[i].left = 0;
								bomb[i].bottom = 0;
								bomb[i].right = 0;
								InvalidateRect(hWnd, NULL, true);
								break;
							}
						}
						
					}
					else
						move = false;
				}
				if (temp == 2)  // ���� Ű ��������
				{
					myDirection = up;
					if (head.top >= 0)
					{
						head.top -= speed;
						head.bottom -= speed;
						body[0].top = head.top + 40; // �Ӹ��� �Ʒ��ʿ� ������ +40
						body[0].bottom = head.bottom + 40;
						body[0].left = head.left;
						body[0].right = head.right;
						for (int i = 1; i < count; ++i)
						{
							body[i].top = body[i - 1].top + 40;
							body[i].bottom = body[i - 1].bottom + 40;
							body[i].left = body[i - 1].left;
							body[i].right = body[i - 1].right;
						}
						for (int i = 0; i < item_count; ++i)
						{
							if (head.left < item[i].right && head.top <item[i].bottom && head.right >item[i].left && head.bottom > item[i].top)
							{
								item[i].left = 0;
								item[i].top = 0;
								item[i].bottom = 0;
								item[i].right = 0;
								item_limit--;
								if (item_limit == 0)
								{
									PostQuitMessage(0);
								}
								body[count].top = body[count - 1].top + 40;
								body[count].bottom = body[count - 1].bottom + 40;
								body[count].left = body[count - 1].left;
								body[count].right = body[count - 1].right;
								count++;
								InvalidateRect(hWnd, NULL, true);
								break;
							}
						}
						for (int i = 0; i < bomb_count; ++i)
						{
							if (head.left < bomb[i].right && head.top <bomb[i].bottom && head.right >bomb[i].left && head.bottom > bomb[i].top)
							{
								count--;
								if (count == 0)
								{
									PostQuitMessage(0);
								}
								body[count].top = 0;
								body[count].left = 0;
								body[count].bottom = 0;
								body[count].right = 0;
								bomb[i].top = 0;
								bomb[i].left = 0;
								bomb[i].bottom = 0;
								bomb[i].right = 0;
								InvalidateRect(hWnd, NULL, true);
								break;
							}
						}
					}
					else
						move = false;
				}
				if (temp == 3)  // ���� Ű ��������
				{
					myDirection = left;
					if (head.left >= 40 || body[0].left >= 40)
					{
						head.left -= speed;
						head.right -= speed;
						body[0].top = head.top;
						body[0].bottom = head.bottom;
						body[0].left = head.left + 40; // �Ӹ��� �����ʿ� ������ +40
						body[0].right = head.right + 40;
						for (int i = 1; i < count; ++i)
						{
							body[i].top = body[i - 1].top;
							body[i].bottom = body[i - 1].bottom;
							body[i].left = body[i - 1].left + 40;
							body[i].right = body[i - 1].right + 40;
						}
						for (int i = 0; i < item_count; ++i)
						{
							if (head.left < item[i].right && head.top <item[i].bottom && head.right >item[i].left && head.bottom > item[i].top)
							{
								item[i].left = 0;
								item[i].top = 0;
								item[i].bottom = 0;
								item[i].right = 0;
								item_limit--;
								if (item_limit == 0)
								{
									PostQuitMessage(0);
								}
								body[count].top = body[count - 1].top;
								body[count].bottom = body[count - 1].bottom;
								body[count].left = body[count - 1].left + 40;
								body[count].right = body[count - 1].right + 40;
								count++;
								InvalidateRect(hWnd, NULL, true);
								break;
							}
						}
						for (int i = 0; i < bomb_count; ++i)
						{
							if (head.left < bomb[i].right && head.top <bomb[i].bottom && head.right >bomb[i].left && head.bottom > bomb[i].top)
							{
								count--;
								if (count == 0)
								{
									PostQuitMessage(0);
								}
								body[count].top = 0;
								body[count].left = 0;
								body[count].bottom = 0;
								body[count].right = 0;
								bomb[i].top = 0;
								bomb[i].left = 0;
								bomb[i].bottom = 0;
								bomb[i].right = 0;
								InvalidateRect(hWnd, NULL, true);
								break;
							}
						}
					}
					else
						move = false;
				}

				InvalidateRect(hWnd, NULL, true);
			}
			else if (move == false) // ����Ű�� ������ �⺻�̵��� �����ʴ´�
			{

				if (myDirection == right) //������ �������� ����  
				{
					head.left += speed;
					head.right += speed;
					body[0].left = head.left - 40; //������ �׻� �Ӹ��� ���ʿ��ִ�
					body[0].right = head.right - 40;
					body[0].top = head.top;
					body[0].bottom = head.bottom;
					for (int i = 1; i < count; ++i)
					{
						body[i].top = body[i - 1].top;
						body[i].bottom = body[i - 1].bottom;
						body[i].left = body[i - 1].left - 40;
						body[i].right = body[i - 1].right - 40;
					}
					for (int i = 0; i < item_count; ++i)
					{
						if (head.left < item[i].right && head.top <item[i].bottom && head.right >item[i].left && head.bottom > item[i].top)
						{
							item[i].left = 0;
							item[i].top = 0;
							item[i].bottom = 0;
							item[i].right = 0;
							item_limit--;
							if (item_limit == 0)
							{
								PostQuitMessage(0);
							}
							body[count].top = body[count - 1].top;
							body[count].bottom = body[count - 1].bottom;
							body[count].left = body[count - 1].left - 40;
							body[count].right = body[count - 1].right - 40;
							count++;
							InvalidateRect(hWnd, NULL, true);
							break;
						}
					}
					for (int i = 0; i < bomb_count; ++i)
					{
						if (head.left < bomb[i].right && head.top <bomb[i].bottom && head.right >bomb[i].left && head.bottom > bomb[i].top)
						{
							count--;
							if (count == 0)
							{
								PostQuitMessage(0);
							}
							body[count].top = 0;
							body[count].left = 0;
							body[count].bottom = 0;
							body[count].right = 0;
							bomb[i].top = 0;
							bomb[i].left = 0;
							bomb[i].bottom = 0;
							bomb[i].right = 0;
							InvalidateRect(hWnd, NULL, true);
							break;
						}
					}

					if (head.right >= 980) // ������ ���� ������
					{
						head.right = 960; //ȭ���� ����� �ʱ� ���ؼ� �׻� ������ ������ �������� ������ش�
						head.left = 920;
						body[0].right = head.right - 40;
						body[0].left = head.left - 40;
						for (int i = 1; i < count; ++i)
						{
							body[i].top = body[i - 1].top;
							body[i].bottom = body[i - 1].bottom;
							body[i].left = body[i - 1].left - 40;
							body[i].right = body[i - 1].right - 40;
						}
						for (int i = 0; i < item_count; ++i)
						{
							if (head.left < item[i].right && head.top <item[i].bottom && head.right >item[i].left && head.bottom > item[i].top)
							{
								item[i].left = 0;
								item[i].top = 0;
								item[i].bottom = 0;
								item[i].right = 0;
								item_limit--;
								if (item_limit == 0)
								{
									PostQuitMessage(0);
								}
								body[count].top = body[count - 1].top;
								body[count].bottom = body[count - 1].bottom;
								body[count].left = body[count - 1].left - 40;
								body[count].right = body[count - 1].right - 40;
								count++;
								InvalidateRect(hWnd, NULL, true);
								break;
							}
						}
						for (int i = 0; i < bomb_count; ++i)
						{
							if (head.left < bomb[i].right && head.top <bomb[i].bottom && head.right >bomb[i].left && head.bottom > bomb[i].top)
							{
								count--;
								if (count == 0)
								{
									PostQuitMessage(0);
								}
								body[count].top = 0;
								body[count].left = 0;
								body[count].bottom = 0;
								body[count].right = 0;
								bomb[i].top = 0;
								bomb[i].left = 0;
								bomb[i].bottom = 0;
								bomb[i].right = 0;
								InvalidateRect(hWnd, NULL, true);
								break;
							}
						}

						myDirection = left;

					}
				}
				else if (myDirection == left) //������ ������ ����
				{

					head.left -= speed;
					head.right -= speed;
					body[0].left = head.left + 40;
					body[0].right = head.right + 40;
					body[0].top = head.top;
					body[0].bottom = head.bottom;
					for (int i = 1; i < count; ++i)
					{
						body[i].top = body[i - 1].top;
						body[i].bottom = body[i - 1].bottom;
						body[i].left = body[i - 1].left + 40;
						body[i].right = body[i - 1].right + 40;
					}
					for (int i = 0; i < item_count; ++i)
					{
						if (head.left < item[i].right && head.top <item[i].bottom && head.right >item[i].left && head.bottom > item[i].top)
						{
							item[i].left = 0;
							item[i].top = 0;
							item[i].bottom = 0;
							item[i].right = 0;
							item_limit--;
							if (item_limit == 0)
							{
								PostQuitMessage(0);
							}
							body[count].top = body[count - 1].top;
							body[count].bottom = body[count - 1].bottom;
							body[count].left = body[count - 1].left + 40;
							body[count].right = body[count - 1].right + 40;
							count++;
							InvalidateRect(hWnd, NULL, true);
							break;
						}
					}
					for (int i = 0; i < bomb_count; ++i)
					{
						if (head.left < bomb[i].right && head.top <bomb[i].bottom && head.right >bomb[i].left && head.bottom > bomb[i].top)
						{
							count--;
							if (count == 0)
							{
								PostQuitMessage(0);
							}
							body[count].top = 0;
							body[count].left = 0;
							body[count].bottom = 0;
							body[count].right = 0;
							bomb[i].top = 0;
							bomb[i].left = 0;
							bomb[i].bottom = 0;
							bomb[i].right = 0;
							InvalidateRect(hWnd, NULL, true);
							break;
						}
					}
					if (head.left <= 0) //���� ���� ������
					{
						head.left = 0; // ȭ���� ����� �ʱ� ���ؼ� �׻� ���� ������ �������� ������ش�
						head.right = 40;
						body[0].left = head.left + 40;
						body[0].right = head.right + 40;
						for (int i = 1; i < count; ++i)
						{
							body[i].top = body[i - 1].top;
							body[i].bottom = body[i - 1].bottom;
							body[i].left = body[i - 1].left + 40;
							body[i].right = body[i - 1].right + 40;
						}
						myDirection = right;

					}
				}
				else if (myDirection == down) //�Ʒ������� ���� ��
				{
					head.top += speed;
					head.bottom += speed;
					body[0].left = head.left;
					body[0].right = head.right;
					body[0].top = head.top - 40; //������ �Ӹ��� �ڿ� �ִ�
					body[0].bottom = head.bottom - 40;
					for (int i = 1; i < count; ++i)
					{
						body[i].top = body[i - 1].top - 40;
						body[i].bottom = body[i - 1].bottom - 40;
						body[i].left = body[i - 1].left;
						body[i].right = body[i - 1].right;
					}
					for (int i = 0; i < item_count; ++i)
					{
						if (head.left < item[i].right && head.top <item[i].bottom && head.right >item[i].left && head.bottom > item[i].top)
						{
							item[i].left = 0;
							item[i].top = 0;
							item[i].bottom = 0;
							item[i].right = 0;
							item_limit--;
							if (item_limit == 0)
							{
								PostQuitMessage(0);
							}
							body[count].top = body[count - 1].top - 40;
							body[count].bottom = body[count - 1].bottom - 40;
							body[count].left = body[count - 1].left;
							body[count].right = body[count - 1].right;
							count++;
							InvalidateRect(hWnd, NULL, true);
							break;
						}
					}
					for (int i = 0; i < bomb_count; ++i)
					{
						if (head.left < bomb[i].right && head.top <bomb[i].bottom && head.right >bomb[i].left && head.bottom > bomb[i].top)
						{
							count--;
							if (count == 0)
							{
								PostQuitMessage(0);
							}
							body[count].top = 0;
							body[count].left = 0;
							body[count].bottom = 0;
							body[count].right = 0;
							bomb[i].top = 0;
							bomb[i].left = 0;
							bomb[i].bottom = 0;
							bomb[i].right = 0;
							InvalidateRect(hWnd, NULL, true);
							break;
						}
					}
					if (head.top >600)
					{
						head.top = 520;
						head.bottom = 560;
						body[0].top = head.top + 40;
						body[0].bottom = head.bottom + 40;
						for (int i = 1; i < count; ++i)
						{
							body[i].top = body[i - 1].top + 40;
							body[i].bottom = body[i - 1].bottom + 40;
							body[i].left = body[i - 1].left;
							body[i].right = body[i - 1].right;
						}
						myDirection = up;
					}
				}
				else if (myDirection == up)
				{
					head.top -= speed;
					head.bottom -= speed;
					body[0].left = head.left;
					body[0].right = head.right;
					body[0].top = head.top + 40; //������ �Ӹ��� �ڿ� �ִ�
					body[0].bottom = head.bottom + 40;
					for (int i = 1; i < count; ++i)
					{
						body[i].top = body[i - 1].top + 40;
						body[i].bottom = body[i - 1].bottom + 40;
						body[i].left = body[i - 1].left;
						body[i].right = body[i - 1].right;
					}
					for (int i = 0; i < item_count; ++i)
					{
						if (head.left < item[i].right && head.top <item[i].bottom && head.right >item[i].left && head.bottom > item[i].top)
						{
							item[i].left = 0;
							item[i].top = 0;
							item[i].bottom = 0;
							item[i].right = 0;
							body[count].top = body[count - 1].top + 40;
							body[count].bottom = body[count - 1].bottom + 40;
							body[count].left = body[count - 1].left;
							body[count].right = body[count - 1].right;
							count++;
							InvalidateRect(hWnd, NULL, true);
							break;
						}
					}
					for (int i = 0; i < bomb_count; ++i)
					{
						if (head.left < bomb[i].right && head.top <bomb[i].bottom && head.right >bomb[i].left && head.bottom > bomb[i].top)
						{
							count--;
							if (count == 0)
							{
								PostQuitMessage(0);
							}
							body[count].top = 0;
							body[count].left = 0;
							body[count].bottom = 0;
							body[count].right = 0;
							bomb[i].top = 0;
							bomb[i].left = 0;
							bomb[i].bottom = 0;
							bomb[i].right = 0;
							InvalidateRect(hWnd, NULL, true);
							break;
						}
					}
					if (head.top <= 0)
					{
						head.top = 40;
						head.bottom = 80;
						body[0].top = head.top - 40;
						body[0].bottom = head.bottom - 40;
						for (int i = 1; i < count; ++i)
						{
							body[i].top = body[i - 1].top - 40;
							body[i].bottom = body[i - 1].bottom - 40;
							body[i].left = body[i - 1].left;
							body[i].right = body[i - 1].right;
						}
						myDirection = down;
					}
				}
				InvalidateRect(hWnd, NULL, true);
			}
			break;
		}
		case 2: //�����ϴ�
		{
			if (jumptime == 40) //������ �ϰ� ����
			{
				jumptime = 0; //�������� �ʱ�ȭ
				KillTimer(hWnd, 2); //���� Ÿ�̸Ӹ� ���̰�
			}
			if (myDirection == up || myDirection == down)
			{
				if (jumptime<20)
				{
					jumptime++;
					head.left -= 3;
					head.right -= 3;
					body[0].left -= 3;
					body[0].right -= 3;
					nl -= 3;
					nr -= 3;
				}
				else if (jumptime<40)
				{

					jumptime++;
					head.left += 3;
					head.right += 3;
					body[0].left += 3;
					body[0].right += 3;
					nl += 3;
					nr += 3;
				}
			}
			else if (myDirection == left || myDirection == right)
			{
				if (jumptime < 20)
				{
					jumptime++;

					head.top -= 3; // ���� �ö󰡴���
					head.bottom -= 3;
					body[0].top -= 3;
					body[0].bottom -= 3;
					nt -= 3;
					nb -= 3;

				}
				else if (jumptime < 40)
				{
					jumptime++;
					head.top += 3; //�Ʒ��� ����������
					head.bottom += 3;
					body[0].top += 3;
					body[0].bottom += 3;
					nt += 3;
					nb += 3;
				}
			}
			InvalidateRect(hWnd, NULL, true);
			break;
		}
		case 3:
		{
			item[item_count].left = rand() % 800;
			item[item_count].top = rand() % 600;
			item[item_count].right = item[item_count].left + 40;
			item[item_count].bottom = item[item_count].top + 40;
			item_count++;
			if (item_count == item_limit)
			{
				KillTimer(hWnd, 3);
			}
			break;
		}
		case 4:
		{
			bomb[bomb_count].left = rand() % 800;
			bomb[bomb_count].top = rand() % 600;
			bomb[bomb_count].right = bomb[bomb_count].left + 40;
			bomb[bomb_count].bottom = bomb[bomb_count].top + 40;
			bomb_count++;
			if (bomb_count == 10)
			{
				KillTimer(hWnd, 4);
			}
			break;
		}
		}
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
