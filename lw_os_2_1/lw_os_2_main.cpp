//#include "lw_os_2_1.h"
//
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
//	Register(hInstance);
//	/* (!Register(hInstance))
//		return -1;// �� ������� ���������������� ����� ������� ����� ��������� ������ */
//
//	// ��������� ����������
//	// ��������� ���������� ������ �����������
//	LoadLibrary(TEXT("ComCtl32.dll"));
//
//	// ��������� ������� ������� ������
//	hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));
//
//	// ������ ������� ���� �� ������ ������ �������� ������
//	hWnd = CreateWindowEx(
//		0, TEXT("MyWindowClass"), TEXT("SampleWin32"), 
//		WS_OVERLAPPEDWINDOW| WS_CAPTION| WS_VISIBLE, CW_USEDEFAULT,
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