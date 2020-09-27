#include <Windows.h>
#include <WindowsX.h>
#include <CommCtrl.h>
#include <tchar.h>
#include "resource.h"
#define LEFT 300
#define TOP  240

//#include "afxres.h"

#define WM_ADDITEM WM_USER+1

HWND hWnd = NULL; //���������� ����
HWND hwndMonthCal = NULL; //��� ���������
HACCEL hAccel = NULL; //���������� ������������ 
HWND hSaveDlg = NULL; //save as
HWND hRlsDlg = NULL; //replace
	TCHAR szBuffer[100] = TEXT("");
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCRStr);
void OnDestroy(HWND hwnd);
RECT rc;
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,	LPSTR lpszCmdLine,int  nCmdShow)
{
	MSG  msg;
	BOOL bRet;
	
	// ������������ ������� ����� �������� ����...

	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = MainWindowProc; // ������� ���������
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wcex.lpszClassName = TEXT("MainWindowClass"); // ��� ������
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (0 == RegisterClassEx(&wcex)) // ������������ �����
	{
		// �� ������� ���������������� ����� ������� �����
		return -1; // ��������� ������ ����������
	} // if

	// ��������� ����������
	// ��������� ���������� ������ �����������
	LoadLibrary(TEXT("ComCtl32.dll"));

	// ��������� ������� ������� ������
	hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	// ������ ������� ���� �� ������ ������ �������� ������
	hWnd = CreateWindowEx(0, TEXT("MainWindowClass"), TEXT("LwOS"), WS_OVERLAPPEDWINDOW|DS_CENTER, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (NULL == hWnd) // ���� �� ������� ������� ����
	{
		return -1; // ��������� ������ ����������
	} // if

	ShowWindow(hWnd, nCmdShow); // ���������� ������� ����
	UpdateWindow(hWnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;             // ������ ��������� ����������
	PAINTSTRUCT ps;      // ��������� ��� ���������

	switch (msg)
	{
		HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
		/*case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
			TextOut(hdc, 10, 20, L"WM_PAINT", 8);
			EndPaint(hwnd, &ps);
			return 0;
		}*/

		case WM_LBUTTONDOWN:
		{
			hdc = GetDC(hwnd);
			TextOut(hdc, 10, 40, L"WM_LBUTTONDOWN", 14);
			ReleaseDC(hwnd, hdc);
			return 0;
		}
		case WM_LBUTTONUP:
		{
			hdc = GetDC(hwnd);
			TextOut(hdc, 10, 40, L"WM_LBUTTONUP", 14);
			ReleaseDC(hwnd, hdc);
			return 0;
		}
		case WM_MOUSEWHEEL:
		{
			hdc = GetDC(hwnd);
			TextOut(hdc, 10, 40, L"WM_MOUSEWHEEL", 14);
			ReleaseDC(hwnd, hdc);
			return 0;
		}
		case WM_SIZING:
		{
			hdc = GetDC(hwnd);
			TextOut(hdc, 10, 40, L"WM_SIZING", 14);
			ReleaseDC(hwnd, hdc);
			return 0;
		}
		case WM_LBUTTONDBLCLK:
		{
			hdc = GetDC(hwnd);
			TextOut(hdc, 300, 40, L"WM_LBUTTONDBLCLK", 14);
			ReleaseDC(hwnd, hdc);
			return 0;
		}
		case WM_SYSCHAR:
		{
			hdc = GetDC(hwnd);
			TextOut(hdc, 300, 40, L"WM_SYSCHAR", 14);
			ReleaseDC(hwnd, hdc);
			return 0;
		}
		case WM_SYSKEYUP:
		{
			hdc = GetDC(hwnd);
			TextOut(hdc, 300, 40, L"WM_SYSKEYUP", 14);
			ReleaseDC(hwnd, hdc);
			return 0;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		}
	return (DefWindowProc(hwnd, msg, wParam, lParam));
	/*switch (msg)
	{
		HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
	case WM_ADDITEM:
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);*/
}

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCRStr) 
{
	CreateWindowEx(0, TEXT("ListBox"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 10, 250, 410, hwnd, (HMENU)IDC_LIST1, lpCRStr->hInstance, NULL);
	hwndMonthCal=CreateWindowEx(0, TEXT("SysMonthCal32"), NULL, WS_CHILD | WS_VISIBLE|WS_BORDER| MCS_DAYSTATE| MCS_MULTISELECT, 163, 31, 140, 100, hwnd, (HMENU)IDC_MONTHCALENDAR1, lpCRStr->hInstance, NULL);
	// Return if creating the month calendar failed. 
	if (hwndMonthCal == NULL)
		return HRESULT_FROM_WIN32(GetLastError());

	// Get the size required to show an entire month.
	MonthCal_GetMinReqRect(hwndMonthCal, &rc);

	// Resize the control now that the size values have been obtained.
	SetWindowPos(hwndMonthCal, NULL, LEFT, TOP,	rc.right, rc.bottom, SWP_NOZORDER);

	// Set the calendar to the annual view.
	MonthCal_SetCurrentView(hwndMonthCal, MCMV_YEAR);
	
	
	
	CreateWindowEx(0, TEXT("Button"), TEXT("�������� ������"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 270, 10, 200, 40, hwnd, (HMENU)ID_NEW_RECORD, lpCRStr->hInstance, NULL);
	CreateWindowEx(0, TEXT("Button"), TEXT("������� ������"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 270, 55, 200, 40, hwnd, (HMENU)ID_NEW_RECORD, lpCRStr->hInstance, NULL);

	return TRUE;
}

void OnDestroy(HWND hwnd)
{
	int UserAnswer = MessageBox(hWnd, TEXT("��������� ������?"), TEXT("�����"), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1);
	if (IDYES == UserAnswer)
	{
		DestroyWindow(hWnd);
	}
}