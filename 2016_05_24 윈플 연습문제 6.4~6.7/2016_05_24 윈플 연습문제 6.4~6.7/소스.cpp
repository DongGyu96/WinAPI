#include <Windows.h>
#include "resource.h"

HINSTANCE g_hInst;
LPCTSTR IpszClass = "°è»ê±â";

BOOL CALLBACK Dlg6_1Proc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR IpszCmdParam, int nCmdShow)
{
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
   WndClass.lpszMenuName = MAKEINTRESOURCE(NULL);;
   WndClass.lpszClassName = IpszClass;
   WndClass.hIconSm = LoadCursor(NULL, IDI_APPLICATION);

   RegisterClassEx(&WndClass);

   hWnd = CreateWindow(IpszClass, IpszClass, WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_SYSMENU, 0, 0,
      800, 600, NULL, (HMENU)NULL, hInstance, NULL);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   while (GetMessage(&Message, 0, 0, 0))
   {
      TranslateMessage(&Message);
      DispatchMessage(&Message);
   }

   return Message.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   PAINTSTRUCT ps;
   HDC hDC;
   static HINSTANCE hInstance;

   switch (uMsg)
   {
   case WM_CREATE:
      g_hInst = hInstance;
      return 0;
   case WM_PAINT:
      hDC = BeginPaint(hWnd, &ps);
      EndPaint(hWnd, &ps);
      break;
   case WM_LBUTTONDOWN:
      DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, Dlg6_1Proc);
      InvalidateRect(hWnd, NULL, TRUE);
      break;
   case WM_DESTROY:
      PostQuitMessage(0);
   }

   return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK Dlg6_1Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
   static int n[2], cntn = 0, result, result2, key = 0, rcnt = 0, z, zn[2] = { 0, 0 };
   static char num[100], all[100], sum[10], reverse[10], re[10];
   static BOOL a = TRUE, zum = FALSE;
   switch (iMsg)
   {
   case WM_INITDIALOG:
      return true;
   case WM_COMMAND:
      switch (LOWORD(wParam))
      {
      case IDC_BUTTON1:
         num[cntn] = '1';
         SetDlgItemText(hDlg, IDC_EDIT1, num);
         cntn++;
         break;
      case IDC_BUTTON2:
         num[cntn] = '2';
         SetDlgItemText(hDlg, IDC_EDIT1, num);
         cntn++;
         break;
      case IDC_BUTTON3:
         num[cntn] = '3';
         SetDlgItemText(hDlg, IDC_EDIT1, num);
         cntn++;
         break;
      case IDC_BUTTON4:
         num[cntn] = '4';
         SetDlgItemText(hDlg, IDC_EDIT1, num);
         cntn++;
         break;
      case IDC_BUTTON5:
         num[cntn] = '5';
         SetDlgItemText(hDlg, IDC_EDIT1, num);
         cntn++;
         break;
      case IDC_BUTTON6:
         num[cntn] = '6';
         SetDlgItemText(hDlg, IDC_EDIT1, num);
         cntn++;
         break;
      case IDC_BUTTON7:
         num[cntn] = '7';
         SetDlgItemText(hDlg, IDC_EDIT1, num);
         cntn++;
         break;
      case IDC_BUTTON8:
         num[cntn] = '8';
         SetDlgItemText(hDlg, IDC_EDIT1, num);
         cntn++;
         break;
      case IDC_BUTTON9:
         num[cntn] = '9';
         SetDlgItemText(hDlg, IDC_EDIT1, num);
         cntn++;
         break;
      case IDC_BUTTON10:
         num[cntn] = '0';
         SetDlgItemText(hDlg, IDC_EDIT1, num);
         cntn++;
         break;
      case IDC_BUTTON11:
         n[key] = atoi(num);
         key++;
         strcat_s(all, num); 
         for (int i = 0; i < cntn; ++i)
         {
            num[i] = NULL;
         }
         cntn = 0;
         sum[0] = '.';
         strcat_s(all, sum);
         SetDlgItemText(hDlg, IDC_EDIT1, all);
         zum = TRUE;
         break;
      case IDC_BUTTON12:
         strcat_s(all, num);
         n[key] = atoi(num);
         for (int i = 0; i <= cntn; ++i)
            num[i] = NULL; 

         if (sum[0] == '+')
            result = n[0] + n[1];
         else if (sum[0] == '-')
            result = n[0] - n[1];
         else if (sum[0] == '*')
            result = n[0] * n[1];
         else if (sum[0] == '/')
            result = n[0] / n[1];
         
         if (zum)
         {
            if (sum[0] == '+')
               result2 = zn[0] + 0;
            else if (sum[0] == '-')
               result2 = zn[0] - 0;
            else if (sum[0] == '*')
               result2 = zn[0] * n[1];
            else if (sum[0] == '/')
               result2 = zn[0] / n[1];
         }
         sum[0] = '=';
         strcat_s(all, sum);

         wsprintf(num, "%d", result);
         strcat_s(all, num);
         if (zum)
         {
            strcat_s(all, ".");

            wsprintf(num, "%d", result2);
            strcat_s(all, num);
            zn[0] = result2;
         }
         SetDlgItemText(hDlg, IDC_EDIT1, all);
         key = 0;
         n[key] = result;
         strcpy(all, num);
         key++;
         a = FALSE;
         for (int i = 0; i < 10; ++i)
            num[i] = NULL;
         for (int i = 0; i < 10; ++i)
            sum[i] = NULL;
         zum = FALSE;
         break;
      case IDC_BUTTON13:
         strcat_s(all, num);

         if (a)
         {
            if (zum)
            {
               zn[key - 1] = atoi(num);
            }
            else
            {
               n[key] = atoi(num);
               key++;
            }
         }
         for (int i = 0; i <= cntn; ++i)
            num[i] = NULL;

         cntn = 0;
         sum[0] = '+';
         strcat_s(all, sum);
         SetDlgItemText(hDlg, IDC_EDIT1, "+");
         break;
      case IDC_BUTTON14:
         strcat_s(all, num);

         if (a)
         {
            if (zum)
            {
               zn[key - 1] = atoi(num);
            }
            else
            {
               n[key] = atoi(num);
               key++;
            }
         }
         for (int i = 0; i <= cntn; ++i)
            num[i] = NULL;

         cntn = 0;
         sum[0] = '-';
         strcat_s(all, sum);
         SetDlgItemText(hDlg, IDC_EDIT1, "-");
      
         break;
      case IDC_BUTTON15:
         strcat_s(all, num);

         if (a)
         {
            if (zum)
            {
               zn[key - 1] = atoi(num);
            }
            else
            {
               n[key] = atoi(num);
               key++;
            }
         }
         for (int i = 0; i <= cntn; ++i)
            num[i] = NULL;

         cntn = 0;
         sum[0] = '*';
         strcat_s(all, sum);
         SetDlgItemText(hDlg, IDC_EDIT1, "*");
         break;
      case IDC_BUTTON16:
         strcat_s(all, num);

         if (a)
         {
            if (zum)
            {
               zn[key - 1] = atoi(num);
            }
            else
            {
               n[key] = atoi(num);
               key++;
            }
         }
         for (int i = 0; i <= cntn; ++i)
            num[i] = NULL;

         cntn = 0;
         sum[0] = '/';
         strcat_s(all, sum);
         SetDlgItemText(hDlg, IDC_EDIT1, "/");
         break;
      case IDC_BUTTON17:
         rcnt = cntn - 1;
         reverse[cntn] = '\0';
         for (int i = 0; i < cntn; ++i)
         {
            reverse[rcnt] = num[i];
            rcnt--;
         }
         for (int i = 0; i < cntn; ++i)
         {
            num[i] = reverse[i];
            SetDlgItemText(hDlg, IDC_EDIT1, num);
         }
         break;
      case IDC_BUTTON18:
         for (int i = 0; i <= cntn; ++i)
            num[i] = NULL;
         cntn = 0;
         SetDlgItemText(hDlg, IDC_EDIT1, all);
         break;
      case IDC_BUTTON19:
         EndDialog(hDlg, 0);
         PostQuitMessage(0);
         break;
      case IDC_BUTTON20:
         for (int i = 0; i < 100; ++i)
            all[i] = NULL;
         
         for (int i = 0; i <= cntn; ++i)
            num[i] = NULL;

         for (int i = 0; i < 10; ++i)
            sum[i] = NULL;

         for (int i = 0; i <= key; ++i)
            n[i] = NULL;

         cntn = 0;
         key = 0;
         SetDlgItemText(hDlg, IDC_EDIT1, NULL);
         a = TRUE;
         break;
      case IDC_BUTTON21:
         if (a)
         {
            n[key] = atoi(num);
            itoa(n[key], num, 2);
         }
         else
            itoa(result, num, 2);
         SetDlgItemText(hDlg, IDC_EDIT1, num);

         break;
      case IDC_BUTTON22:
         //n[key] = atoi(num);
         itoa(n[key], num, 10);
         SetDlgItemText(hDlg, IDC_EDIT1, num);
         break;
      }
      InvalidateRect(hDlg, NULL, TRUE);
      break;
   case WM_CHAR:
      if (wParam == VK_ESCAPE)
      {
         EndDialog(hDlg, 0);
         PostQuitMessage(0);
      }
      InvalidateRect(hDlg, NULL, TRUE);
      break;
   } return 0;
}