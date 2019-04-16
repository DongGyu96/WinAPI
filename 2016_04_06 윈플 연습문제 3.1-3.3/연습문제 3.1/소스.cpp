#include <windows.h>
#include <stdio.h>
#include <string.h>

HINSTANCE g_hInst;

enum direction // 현재 방향
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

	// 윈도우 클래스 구조체 값 설정 
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

	// 윈도우 클래스 등록 
	RegisterClass(&WndClass);

	// 윈도우 생성 
	RECT rcWindow = { 0, 0, 900, 600 };
	AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, false);
	hWnd = CreateWindow("Window Class Name", "", WS_OVERLAPPEDWINDOW, 0, 0, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, NULL, (HMENU)NULL, hInstance, NULL);

	// 윈도우 출력
	ShowWindow(hWnd, nCmdshow);
	UpdateWindow(hWnd);

	// 이벤트 루프 처리
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

	// 메시지 처리하기 
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
		///////////////////////////////////////////////////////머리 그리기
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

		///////////////////////////////////////////////////////몸통 그리기
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
			move = true; //방향키를 누르면 기본 이동은 하지 않는다
			temp = wParam;    //temp에 방향키를 넘겨줌
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
		else if (wParam == VK_SPACE) //점프
		{
			SetTimer(hWnd, 2, 20, NULL); // 점프 타이머를 불러온다
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

			if (move == true) //방향키를 눌렀을때
			{

				if (temp == VK_RIGHT)  // 오른쪽 키 눌렀을때
				{
					myDirection = right;
					if (head.right <= 770 || body.right <= 770) //오른쪽 벽에 닿기 전 동안
					{

						head.left += speed;
						head.right += speed;
						body.top = head.top; //몸통은 항상 머리의 뒤에 있다
						body.bottom = head.bottom;
						body.left = head.left - 40; // 머리의 왼쪽에 있으니 -40
						body.right = head.right - 40;
					}
					else
						move = false; // 벽에 닿으면 기본 이동
				}

				if (temp == VK_DOWN)  // 아래쪽 키 눌렀을때
				{
					myDirection = down;

					if (head.bottom <= 550) // 바닥에 닿기 전 동안
					{
						head.top += speed;
						head.bottom += speed;
						body.top = head.top - 40; //몸통은 항상 머리의 뒤에 있다 머리의 위쪽에 있으니 머리의 -40
						body.bottom = head.bottom - 40;
						body.left = head.left;
						body.right = head.right;
					}
					else
						move = false;
				}
				if (temp == VK_UP)  // 위쪽 키 눌렀을때
				{
					myDirection = up;
					if (head.top >= 0)
					{
						head.top -= speed;
						head.bottom -= speed;
						body.top = head.top + 40; // 머리의 아래쪽에 있으니 +40
						body.bottom = head.bottom + 40;
						body.left = head.left;
						body.right = head.right;
					}
					else
						move = false;
				}
				if (temp == VK_LEFT)  // 왼쪽 키 눌렀을때
				{
					myDirection = left;
					if (head.left >= 40 || body.left >= 40)
					{
						head.left -= speed;
						head.right -= speed;
						body.top = head.top;
						body.bottom = head.bottom;
						body.left = head.left + 40; // 머리의 오른쪽에 있으니 +40
						body.right = head.right + 40;
					}
					else
						move = false;
				}

				InvalidateRect(hWnd, NULL, true);
			}
			else if (move == false) // 방향키를 누르면 기본이동은 하지않는다
			{

				if (myDirection == right) //방향이 오른쪽인 동안  
				{
					head.left += speed;
					head.right += speed;

					body.left = head.left - 40; //몸통은 항상 머리의 왼쪽에있다
					body.right = head.right - 40;
					body.top = head.top;
					body.bottom = head.bottom;

					if (head.right >= 900) // 오른쪽 벽에 닿으면
					{
						head.right = 800; //화면을 벗어나지 않기 위해서 항상 오른쪽 끝으로 고정값을 만들어준다
						head.left = 760;

						body.right = head.right - 40;
						body.left = head.left - 40;

						myDirection = left;

					}
				}
				else if (myDirection == left) //방향이 왼쪽인 동안
				{

					head.left -= speed;
					head.right -= speed;

					body.left = head.left + 40;
					body.right = head.right + 40;
					body.top = head.top;
					body.bottom = head.bottom;

					if (head.left <= 0) //왼쪽 벽에 닿으면
					{
						head.left = 0; // 화면을 벗어나지 않기 위해서 항상 왼쪽 끝으로 고정값을 만들어준다
						head.right = 40;

						body.left = head.left + 40;
						body.right = head.right + 40;

						myDirection = right;

					}
				}
				else if (myDirection == down) //아래쪽으로 가는 중
				{
					head.top += speed;
					head.bottom += speed;

					body.left = head.left;
					body.right = head.right;
					body.top = head.top - 40; //몸통은 머리의 뒤에 있다
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
					body.top = head.top + 40; //몸통은 머리의 뒤에 있다
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
		case 2: //점프하는
		{
			if (jumptime == 40) //점프를 하고 나면
			{
				jumptime = 0; //점프값을 초기화
				KillTimer(hWnd, 2); //점프 타이머를 죽이고
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

					head.top -= 3; // 위로 올라가는중
					head.bottom -= 3;
					body.top -= 3;
					body.bottom -= 3;

				}
				else if (jumptime < 40)
				{
					jumptime++;
					head.top += 3; //아래로 떨어지는중
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
