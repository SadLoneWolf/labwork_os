//// =====================================
//// ������� Register
//// ��������� ����������� ������� ����,
//// ������������ �����������
//// =====================================
//#include "lw_os_2_1.h"
//
//int Register(HINSTANCE hInstance)
//{
//	ATOM aWndClass; // ���� ��� ���� ��������
//   
//	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) }; // ��������� ��� ����������� ������ ����
//	memset(&wcex, 0, sizeof(wcex));
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
//}