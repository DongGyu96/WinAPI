#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "resource.h"
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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;

	// ������ Ŭ���� ��� 
	RegisterClass(&WndClass);

	// ������ ���� 
	RECT rcWindow = { 0, 0, 900, 900 };
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
	static RECT head, item[30], bomb[10], rt;
	static BOOL Start, Grid;
	static int count = 1, ncount, red, green, blue;
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
		GetClientRect(hWnd, &rt);
		Start = false;
		Grid = false;
		srand((unsigned)time(NULL));
		red = 255;
		blue = 0;
		green = 255;
		head.left = 40;
		head.top = 0;
		head.right = 80;
		head.bottom = 40;
		SetTimer(hWnd, 1, 160, NULL);
		SetTimer(hWnd, 3, 2000, NULL);
		SetTimer(hWnd, 4, 3000, NULL);

		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_GAME_START:
			Start = true;
			break;
		case ID_GAME_END:
			Start = false;
			break;
		case ID_COLOR_CYAN:
			red = 0;
			blue = 255;
			green = 255;
			break;
		case ID_COLOR_MAGENTA:
			red = 255;
			green = 0;
			blue = 255;
			break;
		case ID_COLOR_YELLOW:
			red = 255;
			green = 255;
			blue = 0;
			break;
		case ID_COLOR_RANDOM:
			red = rand() % 256;
			green = rand() % 256;
			blue = rand() % 256;
			break;
		case ID_FOOD_10:
			item_limit = 10;
			KillTimer(hWnd, 3);
			SetTimer(hWnd, 3, 2000, NULL);
			break;
		case ID_FOOD_20:
			item_limit = 20;
			KillTimer(hWnd, 3);
			SetTimer(hWnd, 3, 2000, NULL);
			break;
		case ID_FOOD_30:
			item_limit = 30;
			KillTimer(hWnd, 3);
			SetTimer(hWnd, 3, 2000, NULL);
			break;
		case ID_GRID_ON:
			Grid = true;
			break;
		case ID_GRID_OFF:
			Grid = false;
			break;
		}
		InvalidateRect(hWnd, NULL, true);
		break;
	case WM_LBUTTONDOWN:
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

		if (Grid)
		{
			for (int i = 0; i < 25; ++i)
			{
				for (int j = 0; j < 25; ++j)
				{
					Rectangle(hdc, rt.right / 25 * i, rt.bottom / 25 * j, rt.right / 25 * (i + 1), rt.bottom / 25 * (j + 1));
				}
			}
		}

		hPen = CreatePen(PS_SOLID, 2, RGB(0,0, 0));
		oldPen = (HPEN)SelectObject(hdc, hPen);
		hBrush = CreateSolidBrush(RGB(red, green, blue));
		oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		if (ncount % 2)
		{
			if (myDirection == right)
			{
				Pie(hdc, head.left, head.top, head.right, head.bottom, head.right, head.top + 10, head.right, head.bottom - 10);
			}
			else if (myDirection == up)
			{
				Pie(hdc, head.left, head.top, head.right, head.bottom, head.left, head.top - 20, head.right, head.top - 20);
			}
			else if (myDirection == left)
			{
				Pie(hdc, head.left, head.top, head.right, head.bottom, head.left, head.bottom - 10, head.left, head.top + 10);
			}
			else if (myDirection == down)
			{
				Pie(hdc, head.left, head.top, head.right, head.bottom, head.right, head.bottom + 20, head.left, head.bottom + 20);
			}
		}
		else
		{
			if (myDirection == right)
			{
				Pie(hdc, head.left, head.top, head.right, head.bottom, head.right, head.top, head.right, head.bottom);
			}
			else if (myDirection == up)
			{
				Pie(hdc, head.left, head.top, head.right, head.bottom, head.left, head.top, head.right, head.top);
			}
			else if (myDirection == left)
			{
				Pie(hdc, head.left, head.top, head.right, head.bottom, head.left, head.bottom, head.left, head.top);
			}
			else if (myDirection == down)
			{
				Pie(hdc, head.left, head.top, head.right, head.bottom, head.right, head.bottom, head.left, head.bottom);
			}
		}
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);

		if (newone>5)
		{
			hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
			oldPen = (HPEN)SelectObject(hdc, hPen);
			if (ncount % 2)
			{
				if (myDirection == right)
				{
					Pie(hdc, nl, nt, nr, nb, nr, nt + 10, nr, nb - 10);
				}
				else if (myDirection == up)
				{
					Pie(hdc, nl, nt, nr, nb, nl, nt - 20,nr, nt - 20);
				}
				else if (myDirection == left)
				{
					Pie(hdc, nl, nt, nr, nb, nl, nb - 10, nl, nt + 10);
				}
				else if (myDirection == down)
				{
					Pie(hdc, nl, nt, nr, nb, nr, nb + 20, nl, nb + 20);
				}
			}
			else
			{
				if (myDirection == right)
				{
					Pie(hdc, nl, nt, nr, nb, nr, nt, nr, nb);
				}
				else if (myDirection == up)
				{
					Pie(hdc, nl, nt, nr, nb, nl, nt, nr, nt);
				}
				else if (myDirection == left)
				{
					Pie(hdc, nl, nt, nr, nb, nl, nb, nl, nt);
				}
				else if (myDirection == down)
				{
					Pie(hdc, nl, nt, nr, nb, nr, nb, nl, nb);
				}
			}
			SelectObject(hdc, oldPen);
			DeleteObject(hPen);
		}

		///////////////////////////////////////////////////////���� �׸���
		hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
		oldPen = (HPEN)SelectObject(hdc, hPen);
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
		if (Start)
		{
			newone++;
			ncount++;

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
						if (head.right <= rt.right) //������ ���� ��� �� ����
						{

							head.left += speed;
							head.right += speed;
							for (int i = 0; i < item_count; ++i)
							{
								if (head.left < item[i].right && head.top <item[i].bottom && head.right >item[i].left && head.bottom > item[i].top)
								{
									item[i].left = 0;
									item[i].top = 0;
									item[i].bottom = 0;
									item[i].right = 0;
									speed += 2;
									InvalidateRect(hWnd, NULL, true);
									break;
								}
							}
							for (int i = 0; i < bomb_count; ++i)
							{
								if (head.left < bomb[i].right && head.top <bomb[i].bottom && head.right >bomb[i].left && head.bottom > bomb[i].top)
								{
									bomb[i].top = 0;
									bomb[i].left = 0;
									bomb[i].bottom = 0;
									bomb[i].right = 0;
									speed -= 2;
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
							for (int i = 0; i < item_count; ++i)
							{
								if (head.left < item[i].right && head.top <item[i].bottom && head.right >item[i].left && head.bottom > item[i].top)
								{
									item[i].left = 0;
									item[i].top = 0;
									item[i].bottom = 0;
									item[i].right = 0;
									speed += 2;
									InvalidateRect(hWnd, NULL, true);
									break;
								}
							}
							for (int i = 0; i < bomb_count; ++i)
							{
								if (head.left < bomb[i].right && head.top <bomb[i].bottom && head.right >bomb[i].left && head.bottom > bomb[i].top)
								{
									bomb[i].top = 0;
									bomb[i].left = 0;
									bomb[i].bottom = 0;
									bomb[i].right = 0;
									speed -= 2;
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
							for (int i = 0; i < item_count; ++i)
							{
								if (head.left < item[i].right && head.top <item[i].bottom && head.right >item[i].left && head.bottom > item[i].top)
								{
									item[i].left = 0;
									item[i].top = 0;
									item[i].bottom = 0;
									item[i].right = 0;
									speed += 2;
									InvalidateRect(hWnd, NULL, true);
									break;
								}
							}
							for (int i = 0; i < bomb_count; ++i)
							{
								if (head.left < bomb[i].right && head.top <bomb[i].bottom && head.right >bomb[i].left && head.bottom > bomb[i].top)
								{
									bomb[i].top = 0;
									bomb[i].left = 0;
									bomb[i].bottom = 0;
									bomb[i].right = 0;
									speed -= 2;
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
						if (head.left >= 40)
						{
							head.left -= speed;
							head.right -= speed;
							for (int i = 0; i < item_count; ++i)
							{
								if (head.left < item[i].right && head.top <item[i].bottom && head.right >item[i].left && head.bottom > item[i].top)
								{
									item[i].left = 0;
									item[i].top = 0;
									item[i].bottom = 0;
									item[i].right = 0;
									speed += 2;
									InvalidateRect(hWnd, NULL, true);
									break;
								}
							}
							for (int i = 0; i < bomb_count; ++i)
							{
								if (head.left < bomb[i].right && head.top <bomb[i].bottom && head.right >bomb[i].left && head.bottom > bomb[i].top)
								{
									bomb[i].top = 0;
									bomb[i].left = 0;
									bomb[i].bottom = 0;
									bomb[i].right = 0;
									speed -= 2;
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
						for (int i = 0; i < item_count; ++i)
						{
							if (head.left < item[i].right && head.top <item[i].bottom && head.right >item[i].left && head.bottom > item[i].top)
							{
								item[i].left = 0;
								item[i].top = 0;
								item[i].bottom = 0;
								item[i].right = 0;
								speed += 2;
								InvalidateRect(hWnd, NULL, true);
								break;
							}
						}
						for (int i = 0; i < bomb_count; ++i)
						{
							if (head.left < bomb[i].right && head.top <bomb[i].bottom && head.right >bomb[i].left && head.bottom > bomb[i].top)
							{
								bomb[i].top = 0;
								bomb[i].left = 0;
								bomb[i].bottom = 0;
								bomb[i].right = 0;
								speed -= 2;
								InvalidateRect(hWnd, NULL, true);
								break;
							}
						}

						if (head.right >= rt.right) // ������ ���� ������
						{
							head.right = rt.right; //ȭ���� ����� �ʱ� ���ؼ� �׻� ������ ������ �������� ������ش�
							head.left = rt.right - 40;
							for (int i = 0; i < item_count; ++i)
							{
								if (head.left < item[i].right && head.top <item[i].bottom && head.right >item[i].left && head.bottom > item[i].top)
								{
									item[i].left = 0;
									item[i].top = 0;
									item[i].bottom = 0;
									item[i].right = 0;
									speed += 2;
									InvalidateRect(hWnd, NULL, true);
									break;
								}
							}
							for (int i = 0; i < bomb_count; ++i)
							{
								if (head.left < bomb[i].right && head.top <bomb[i].bottom && head.right >bomb[i].left && head.bottom > bomb[i].top)
								{
									bomb[i].top = 0;
									bomb[i].left = 0;
									bomb[i].bottom = 0;
									bomb[i].right = 0;
									speed -= 2;
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
						for (int i = 0; i < item_count; ++i)
						{
							if (head.left < item[i].right && head.top <item[i].bottom && head.right >item[i].left && head.bottom > item[i].top)
							{
								item[i].left = 0;
								item[i].top = 0;
								item[i].bottom = 0;
								item[i].right = 0;
								speed += 2;
								InvalidateRect(hWnd, NULL, true);
								break;
							}
						}
						for (int i = 0; i < bomb_count; ++i)
						{
							if (head.left < bomb[i].right && head.top <bomb[i].bottom && head.right >bomb[i].left && head.bottom > bomb[i].top)
							{
								bomb[i].top = 0;
								bomb[i].left = 0;
								bomb[i].bottom = 0;
								bomb[i].right = 0;
								speed -= 2;
								InvalidateRect(hWnd, NULL, true);
								break;
							}
						}
						if (head.left <= 0) //���� ���� ������
						{
							head.left = 0; // ȭ���� ����� �ʱ� ���ؼ� �׻� ���� ������ �������� ������ش�
							head.right = 40;
							myDirection = right;

						}
					}
					else if (myDirection == down) //�Ʒ������� ���� ��
					{
						head.top += speed;
						head.bottom += speed;
						for (int i = 0; i < item_count; ++i)
						{
							if (head.left < item[i].right && head.top <item[i].bottom && head.right >item[i].left && head.bottom > item[i].top)
							{
								item[i].left = 0;
								item[i].top = 0;
								item[i].bottom = 0;
								speed += 2;
								InvalidateRect(hWnd, NULL, true);
								break;
							}
						}
						for (int i = 0; i < bomb_count; ++i)
						{
							if (head.left < bomb[i].right && head.top <bomb[i].bottom && head.right >bomb[i].left && head.bottom > bomb[i].top)
							{
								bomb[i].top = 0;
								bomb[i].left = 0;
								bomb[i].bottom = 0;
								bomb[i].right = 0;
								speed -= 2;
								InvalidateRect(hWnd, NULL, true);
								break;
							}
						}
						if (head.top >rt.bottom)
						{
							head.top = rt.bottom - 40;
							head.bottom = rt.bottom;
							myDirection = up;
						}
					}
					else if (myDirection == up)
					{
						head.top -= speed;
						head.bottom -= speed;
						for (int i = 0; i < item_count; ++i)
						{
							if (head.left < item[i].right && head.top <item[i].bottom && head.right >item[i].left && head.bottom > item[i].top)
							{
								item[i].left = 0;
								item[i].top = 0;
								item[i].bottom = 0;
								item[i].right = 0;
								speed += 2;
								InvalidateRect(hWnd, NULL, true);
								break;
							}
						}
						for (int i = 0; i < bomb_count; ++i)
						{
							if (head.left < bomb[i].right && head.top <bomb[i].bottom && head.right >bomb[i].left && head.bottom > bomb[i].top)
							{
								bomb[i].top = 0;
								bomb[i].left = 0;
								bomb[i].bottom = 0;
								bomb[i].right = 0;
								speed -= 2;
								InvalidateRect(hWnd, NULL, true);
								break;
							}
						}
						if (head.top <= 0)
						{
							head.top = 0;
							head.bottom = 40;
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
						nl -= 3;
						nr -= 3;
					}
					else if (jumptime<40)
					{

						jumptime++;
						head.left += 3;
						head.right += 3;
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
						nt -= 3;
						nb -= 3;

					}
					else if (jumptime < 40)
					{
						jumptime++;
						head.top += 3; //�Ʒ��� ����������
						head.bottom += 3;
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
				if (item_count >= item_limit)
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
				if (bomb_count >= 10)
				{
					KillTimer(hWnd, 4);
				}
				break;
			}
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
