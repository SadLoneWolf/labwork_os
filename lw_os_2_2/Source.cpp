//#include<Windows.h>
//#include <windowsx.h>
//#include <CommCtrl.h>
//#include <tchar.h>
//
//#include "resource.h"
//
//
//LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//int WINAPI _tWinMain(HINSTANCE hInstance,
//	HINSTANCE hPrevInstance,
//	LPSTR     lpszCmdLine,
//	int       nCmdShow)
//{
//	HWND hWnd = NULL;
//	HWND hSaveDlg = NULL; //save as
//	HWND hRlsDlg = NULL; //replace
//	HACCEL hAccel = NULL;
//	MSG  msg;
//	BOOL bRet;
//
//	TCHAR szBuffer[100] = TEXT("");
//
//	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) }; // ��������� ��� ����������� ������ ����
//	// ������������ ����� ���� ��� ��������	����
//
//	wcex.style = CS_HREDRAW | CS_VREDRAW; //��� ����������� ���� ��� ��������� ������/������. ����� ��������� WM_Paint
//	wcex.lpfnWndProc = MainWindowProc; //  ��������� �� ��������� ���� ���������� �������� CallWindowProc.
//	wcex.hInstance = hInstance; //���������� ���������� ������� ��������� ������
//	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
//	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);//��� � �������� 
//	wcex.lpszClassName = TEXT("MainWindowClass"); // ��� ������ 
//	//aWndClass = RegisterClass(&wcex);
//	//if (aWndClass == 0) return FALSE;
//	if (0 == RegisterClassEx(&wcex))
//		return -1;
//		// ��������� ����������
//		// ��������� ���������� ������ �����������
//	LoadLibrary(TEXT("ComCtl32.dll"));
//
//	// ��������� ������� ������� ������
//	hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
//
//	// ������ ������� ���� �� ������ ������ �������� ������
//	hWnd = CreateWindowEx(
//		0, TEXT("MyWindowClass"), TEXT("SampleWin32"),
//		WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_VISIBLE, CW_USEDEFAULT,
//		0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
//
//	if (NULL == hWnd) // ���� �� ������� ������� ����
//	{
//		return -1; // ��������� ������ ����������
//	} // if
//
//// ShowWindow(hWnd, nCmdShow); // ���������� ������� ���� , ����� �� ������������ ���� ���� WS_VISIBLE
//
//	UpdateWindow(hWnd);// �������� � ���� ��������� WM_PAINT
//}
//LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	HDC hdc;             // ������ ��������� ����������
//	PAINTSTRUCT ps;      // ��������� ��� ���������
//
//	switch (msg)
//	{
//	case WM_PAINT:
//	{
//		hdc = BeginPaint(hwnd, &ps);
//		TextOut(hdc, 10, 20, L"WM_PAINT", 8);
//		EndPaint(hwnd, &ps);
//		return 0;
//	}
//
//	case WM_LBUTTONDOWN:
//	{
//		hdc = GetDC(hwnd);
//		TextOut(hdc, 10, 40, L"WM_LBUTTONDOWN", 14);
//		ReleaseDC(hwnd, hdc);
//		return 0;
//	}
//
//	case WM_DESTROY:
//	{
//		PostQuitMessage(0);
//		return 0;
//	}
//	}
//	return DefWindowProc(hwnd, msg, wParam, lParam);
//}