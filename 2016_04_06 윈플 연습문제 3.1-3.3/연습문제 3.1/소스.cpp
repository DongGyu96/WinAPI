#include <windows.h>
#include <stdio.h>
#include <string.h>

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
	static RECT head, body;
	static int speed = 50;
	static int temp = 0;
	static direction myDirection = right;
	static int newone = 0;
	static bool move = false;
	static int jumptime = 0;
	static int nt = 300, nb = 340, nl = 300, nr = 340;

	// �޽��� ó���ϱ� 
	switch (uMsg)
	{

	case WM_CREATE:
		body.left = 0;
		body.top = 0;
		body.right = 40;
		body.bottom = 40;
		head.left = 40;
		head.top = 0;
		head.right = 80;
		head.bottom = 40;
		SetTimer(hWnd, 1, 250, NULL);

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
		Ellipse(hdc, body.left, body.top, body.right, body.bottom);
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
		if (wParam == VK_DOWN)
		{
			move = true; //����Ű�� ������ �⺻ �̵��� ���� �ʴ´�
			temp = wParam;    //temp�� ����Ű�� �Ѱ���
		}
		else if (wParam == VK_RIGHT)
		{
			move = true;
			temp = wParam;
		}
		else if (wParam == VK_LEFT)
		{
			move = true;
			temp = wParam;
		}
		else if (wParam == VK_UP)
		{
			move = true;
			temp = wParam;
		}
		else if (wParam == VK_SPACE) //����
		{
			SetTimer(hWnd, 2, 20, NULL); // ���� Ÿ�̸Ӹ� �ҷ��´�
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

				if (temp == VK_RIGHT)  // ������ Ű ��������
				{
					myDirection = right;
					if (head.right <= 770 || body.right <= 770) //������ ���� ��� �� ����
					{

						head.left += speed;
						head.right += speed;
						body.top = head.top; //������ �׻� �Ӹ��� �ڿ� �ִ�
						body.bottom = head.bottom;
						body.left = head.left - 40; // �Ӹ��� ���ʿ� ������ -40
						body.right = head.right - 40;
					}
					else
						move = false; // ���� ������ �⺻ �̵�
				}

				if (temp == VK_DOWN)  // �Ʒ��� Ű ��������
				{
					myDirection = down;

					if (head.bottom <= 550) // �ٴڿ� ��� �� ����
					{
						head.top += speed;
						head.bottom += speed;
						body.top = head.top - 40; //������ �׻� �Ӹ��� �ڿ� �ִ� �Ӹ��� ���ʿ� ������ �Ӹ��� -40
						body.bottom = head.bottom - 40;
						body.left = head.left;
						body.right = head.right;
					}
					else
						move = false;
				}
				if (temp == VK_UP)  // ���� Ű ��������
				{
					myDirection = up;
					if (head.top >= 0)
					{
						head.top -= speed;
						head.bottom -= speed;
						body.top = head.top + 40; // �Ӹ��� �Ʒ��ʿ� ������ +40
						body.bottom = head.bottom + 40;
						body.left = head.left;
						body.right = head.right;
					}
					else
						move = false;
				}
				if (temp == VK_LEFT)  // ���� Ű ��������
				{
					myDirection = left;
					if (head.left >= 40 || body.left >= 40)
					{
						head.left -= speed;
						head.right -= speed;
						body.top = head.top;
						body.bottom = head.bottom;
						body.left = head.left + 40; // �Ӹ��� �����ʿ� ������ +40
						body.right = head.right + 40;
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

					body.left = head.left - 40; //������ �׻� �Ӹ��� ���ʿ��ִ�
					body.right = head.right - 40;
					body.top = head.top;
					body.bottom = head.bottom;

					if (head.right >= 900) // ������ ���� ������
					{
						head.right = 800; //ȭ���� ����� �ʱ� ���ؼ� �׻� ������ ������ �������� ������ش�
						head.left = 760;

						body.right = head.right - 40;
						body.left = head.left - 40;

						myDirection = left;

					}
				}
				else if (myDirection == left) //������ ������ ����
				{

					head.left -= speed;
					head.right -= speed;

					body.left = head.left + 40;
					body.right = head.right + 40;
					body.top = head.top;
					body.bottom = head.bottom;

					if (head.left <= 0) //���� ���� ������
					{
						head.left = 0; // ȭ���� ����� �ʱ� ���ؼ� �׻� ���� ������ �������� ������ش�
						head.right = 40;

						body.left = head.left + 40;
						body.right = head.right + 40;

						myDirection = right;

					}
				}
				else if (myDirection == down) //�Ʒ������� ���� ��
				{
					head.top += speed;
					head.bottom += speed;

					body.left = head.left;
					body.right = head.right;
					body.top = head.top - 40; //������ �Ӹ��� �ڿ� �ִ�
					body.bottom = head.bottom - 40;

					if (head.top >600)
					{
						head.top = 520;
						head.bottom = 560;

						body.top = head.top + 40;
						body.bottom = head.bottom + 40;

						myDirection = up;
					}
				}
				else if (myDirection == up)
				{
					head.top -= speed;
					head.bottom -= speed;

					body.left = head.left;
					body.right = head.right;
					body.top = head.top + 40; //������ �Ӹ��� �ڿ� �ִ�
					body.bottom = head.bottom + 40;

					if (head.top <= 0)
					{
						head.top = 40;
						head.bottom = 80;

						body.top = head.top - 40;
						body.bottom = head.bottom - 40;

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
					body.left -= 3;
					body.right -= 3;
				}
				else if (jumptime<40)
				{

					jumptime++;
					head.left += 3;
					head.right += 3;
					body.left += 3;
					body.right += 3;
				}
			}
			else if (myDirection == left || myDirection == right)
			{
				if (jumptime < 20)
				{
					jumptime++;

					head.top -= 3; // ���� �ö󰡴���
					head.bottom -= 3;
					body.top -= 3;
					body.bottom -= 3;

				}
				else if (jumptime < 40)
				{
					jumptime++;
					head.top += 3; //�Ʒ��� ����������
					head.bottom += 3;
					body.top += 3;
					body.bottom += 3;
				}
			}
			InvalidateRect(hWnd, NULL, true);
		}
		break;
		}
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
