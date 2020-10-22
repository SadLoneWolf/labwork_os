#include <Windows.h>
#include <WindowsX.h>
#include <tchar.h>
#include <strsafe.h>
#include "DialogWork.h"
#include "resource.h"
#include <Psapi.h> // ��� GetModuleBaseName 

#define IDC_LIST_PROCESSES        2001
#define IDC_LIST_MODULES          2002

HANDLE hSampleJob = NULL; // ���������� �������

// ������� ��������� �������� ����
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ���������� ��������� WM_CREATE
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
// ���������� ��������� WM_DESTROY
void OnDestroy(HWND hwnd);
// ���������� ��������� WM_SIZE
void OnSize(HWND hwnd, UINT state, int cx, int cy);
// ���������� ��������� WM_COMMAND
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);


// ��������� ����������� ����
INT_PTR CALLBACK DialProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ���������� ��������� WM_INITDIALOG
BOOL Dialog_InitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
// ���������� ��������� WM_CLOSE
void Dialog_Close(HWND hwnd);
// ���������� ��������� WM_COMMAND

void Dialog_Command(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

// �������, ������������� �������� �������
void LoadProcessesToListBox(HWND hwndCtl);
// �������, ������������� ����������� ������
void LoadModulesToListBox(HWND hwndCtl, DWORD dwProcessId);
// �������, ������������� �������� � �������
void LoadProcessesInJobToListBox(HWND hwndCtl, HANDLE hJob = NULL);

// �������, ��������� ���������� ��������
BOOL WaitProcessById(DWORD dwProcessId, DWORD dwMilliseconds, LPDWORD lpExitCode);
// �������, ����������� ������ ��������� � ����� �������
BOOL StartGroupProcessesAsJob(HANDLE hJob, LPCTSTR lpszCmdLine[], DWORD nCount, BOOL bInheritHandles, DWORD dwCreationFlags);
// �������, ������� ���������� ������ ��������������� ���������� � ������� ���������
BOOL EnumProcessesInJob(HANDLE hJob, DWORD* lpidProcess, DWORD cb, LPDWORD lpcbNeeded);

// ------------------------------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpszCmdLine, int nCmdShow)
{
	// ������������ ������� ����� �������� ����...

	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = MainWindowProc; // ������� ���������
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 2);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wcex.lpszClassName = TEXT("MainWindowClass"); // ��� ������
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	


	if (0 == RegisterClassEx(&wcex)) // ������������ �����
	{
		return -1; // ��������� ������ ����������
	} 

	LoadLibrary(TEXT("ComCtl32.dll"));//��� ��������� ������ �����������

	// ��������� ������� ������� ������
	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	// ������� ������� ���� �� ������ ������ �������� ������

	HWND hWnd = CreateWindowEx(0, TEXT("MainWindowClass"), TEXT("Process"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (NULL == hWnd)
	{
		// �� ������� ������� ������� ����
		return -1; // ��������� ������ ����������
	} // if

	
	// ������� �������
	hSampleJob = CreateJobObject(NULL, TEXT("FirstJob"));

	ShowWindow(hWnd, nCmdShow); // ���������� ������� ����

	MSG  msg;
	BOOL bRet;

	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != FALSE)
	{
		if (bRet == -1)
		{
			
		} // if
		else if (!TranslateAccelerator(hWnd, hAccel, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} // if
	} // while

	// ��������� ���������� �������
	CloseHandle(hSampleJob);

	return (int)msg.wParam;
} 

// ------------------------------------------------------------------------------------------------
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_SIZE, OnSize);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
	} 
	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// �������� ��������������� ���������
}

/*�������� �������� ���������� � 2-�� listbox � ����*/
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	CreateWindowEx(0, TEXT("Static"), TEXT("��������:"), WS_CHILD | WS_VISIBLE | SS_SIMPLE,
		10, 10, 400, 20, hwnd, NULL, lpCreateStruct->hInstance, NULL);

	CreateWindowEx(0, TEXT("Static"), TEXT("������:"), WS_CHILD | WS_VISIBLE | SS_SIMPLE,
		420, 10, 400, 20, hwnd, NULL, lpCreateStruct->hInstance, NULL);

	// ������� ������ ��� ������������ ���������
	HWND hwndCtl = CreateWindowEx(0, TEXT("ListBox"), TEXT(""), WS_CHILD | WS_VISIBLE | LBS_STANDARD,
		10, 30, 400, 400, hwnd, (HMENU)IDC_LIST_PROCESSES, lpCreateStruct->hInstance, NULL);

	// �������� ������ ��������� �������� ������
	LoadProcessesToListBox(hwndCtl);

	// ������� ������ ��� ������������ ����������� �������
	CreateWindowEx(0, TEXT("ListBox"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_BORDER,
		420, 30, 400, 400, hwnd, (HMENU)IDC_LIST_MODULES, lpCreateStruct->hInstance, NULL);

	return TRUE;
} 

/*��� ���������� ������ � �����������*/
void OnDestroy(HWND hwnd)
{
	PostQuitMessage(0); // ���������� ��������� WM_QUIT
} 

/*������ ������ ������ � ������*/
void OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	if (state != SIZE_MINIMIZED)
	{
		// �������� ������ ������ ��� ������������ ���������
		MoveWindow(GetDlgItem(hwnd, IDC_LIST_PROCESSES), 10, 30, 400, cy - 40, TRUE);

		// �������� ������ ������ ��� ������������ ����������� �������
		MoveWindow(GetDlgItem(hwnd, IDC_LIST_MODULES), 420, 30, cx - 430, cy - 40, TRUE);
	} // if
} // OnSize

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDC_LIST_PROCESSES:
	{
		/*��� ������ ������ �� ������ ���������, ��� �������� ������������ ������*/
		if (LBN_SELCHANGE == codeNotify) // ������ ������ ������� � ������ ���������
		{
	
		int iItem = ListBox_GetCurSel(hwndCtl);//���������� ������

		if (iItem != -1)
			{
			
			DWORD dwProcessId = (DWORD)ListBox_GetItemData(hwndCtl, iItem);// ���������� ������������� 
		
			LoadModulesToListBox(GetDlgItem(hwnd, IDC_LIST_MODULES), dwProcessId);// �������� ������ ����������� �������
			} 
		} 
	}
		break;

	case ID_RELOADPROCESS: // ���������� ������ ���������
	{
		HWND hwndList = GetDlgItem(hwnd, IDC_LIST_PROCESSES);

		LoadProcessesToListBox(hwndList);// �������� ������ ���������
		
		ListBox_ResetContent(GetDlgItem(hwnd, IDC_LIST_MODULES));// ������� ������ �������
	}
	break;
	/*
	case ID_WAITPROCCESS: // �������� ��������
	{
		DWORD dwMilliseconds = INFINITE;

		HWND hwndList = GetDlgItem(hwnd, IDC_LIST_PROCESSES);

		// ���������� ������ ���������� �������� � ������ ���������
		int iItem = ListBox_GetCurSel(hwndList);

		if (iItem != -1)
		{
			TCHAR szText[] = TEXT("* ��� �������� ���������� ������ �������� ������� \"��\".\r\n \
* ��� ����, ������� ���������� ������ �������� � ������� 30 ���. -- \"���\".\r\n \
* ��� ����������� ������� \"������\".");

			int mr = MessageBox(hwnd, szText, TEXT("�������� ��������"), MB_ICONQUESTION | MB_YESNOCANCEL | MB_DEFBUTTON1);

			switch (mr)
			{
			case IDYES: // �������� ���������� ������ ��������
				dwMilliseconds = INFINITE;
				break;

			case IDNO: // �������� ���������� ������ �������� � ������� 30 ���
				dwMilliseconds = 30000;
				break;

			case IDCANCEL: // ������
				iItem = -1;
				break;
			} // switch
		} // if

		if (iItem != -1)
		{
			DWORD dwProcessId = (DWORD)ListBox_GetItemData(hwndList, iItem); // ���������� ������������� ��������

			// ������� ���������� ������ ��������
			DWORD dwExitCode;
			BOOL bRet = WaitProcessById(dwProcessId, dwMilliseconds, &dwExitCode);

			if ((FALSE != bRet) && (STILL_ACTIVE != dwExitCode)) // ���� ������� ��� ��������
			{
				MessageBox(hwnd, TEXT("������� ��������"), TEXT("�������� ��������"), MB_ICONINFORMATION | MB_OK);

				// ������� ������� �� ������
				ListBox_DeleteString(hwndList, iItem);
				// ������� ������ �������
				ListBox_ResetContent(GetDlgItem(hwnd, IDC_LIST_MODULES));
			} // if
			else if (FALSE != bRet) // ���� ������� ����� ��������
			{
				MessageBox(hwnd, TEXT("������� ����� ��������"), TEXT("�������� ��������"), MB_ICONWARNING | MB_OK);
			} // if
			else
			{
				MessageBox(hwnd, TEXT("�������� ������"), NULL, MB_ICONERROR | MB_OK);
			} // else
		} // if
	}
	break;

	case ID_ENDPROCESS: // ���������� ��������
	{
		HWND hwndList = GetDlgItem(hwnd, IDC_LIST_PROCESSES);

		// ���������� ������ ���������� �������� � ������ ���������
		int iItem = ListBox_GetCurSel(hwndList);

		if (iItem != -1)
		{
			int mr = MessageBox(hwnd, TEXT("��������� �������?"), TEXT("���������� ��������"), MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2);
			if (IDYES != mr) iItem = -1;
		} // if

		if (iItem != -1)
		{
			// ���������� ������������� ��������
			DWORD dwProcessId = (DWORD)ListBox_GetItemData(hwndList, iItem);
			// ��������� �������
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessId);

			BOOL bRet = FALSE;

			if (NULL != hProcess)
			{
				bRet = TerminateProcess(hProcess, 0); // ��������� �������
				CloseHandle(hProcess);
			} // if

			if (FALSE != bRet)
			{
				MessageBox(hwnd, TEXT("������� ��������"), TEXT("���������� ��������"), MB_ICONINFORMATION | MB_OK);

				// ������� ������� �� ������
				ListBox_DeleteString(hwndList, iItem);
				// ������� ������ �������
				ListBox_ResetContent(GetDlgItem(hwnd, IDC_LIST_MODULES));
			} // if
			else
			{
				MessageBox(hwnd, TEXT("��������� ��������� �������"), TEXT("���������� ��������"), MB_ICONWARNING | MB_OK);
			} // else
		} // if
	}
	break;
	*/
	case ID_NEWPROCESS: // �������� ����� ��������� � �������
	{
		TCHAR szFileName[1024] = TEXT("");
		OPENFILENAME ofn = { sizeof(OPENFILENAME) };

		ofn.hwndOwner = hwnd;
		ofn.hInstance = GetWindowInstance(hwnd);
		ofn.lpstrFilter = TEXT("��������� (*.exe)\0*.exe\0");
		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = _countof(szFileName);
		ofn.lpstrTitle = TEXT("��������� ���������");
		ofn.Flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;
		ofn.lpstrDefExt = TEXT("exe");

		if (GetOpenFileName(&ofn) != FALSE) 
		{
			BOOL bRet = FALSE;

			//(?������� �� ��������)
			UINT nCount = 0;// ���������� ���������� ��������� ������
			for (LPCTSTR p = szFileName; (*p) != 0; p += _tcslen(p) + 1) ++nCount;

			if (nCount-- > 1) // ���� ������� ��������� ������
			{
				LPCTSTR lpszName = szFileName + _tcslen(szFileName) + 1;

				
				LPTSTR *aCmdLine = new LPTSTR[nCount];// ������ ������ ����� ��� ���������� ������

				for (UINT i = 0; i < nCount; ++i)
				{
					
					aCmdLine[i] = new TCHAR[MAX_PATH];// �������� ������ ��� ��������� ������

					
					StringCchPrintf(aCmdLine[i], MAX_PATH, TEXT("%s\\%s"), szFileName, lpszName);// ��������� ��������� ������

					lpszName += _tcslen(lpszName) + 1;
				} // for

				
				bRet = StartGroupProcessesAsJob(hSampleJob, (LPCTSTR *)aCmdLine, nCount, FALSE, 0);// ������ ������ ��������� � ����� �������

				// ����������� ���������� ������
				for (UINT i = 0; i < nCount; ++i) delete[] aCmdLine[i];
				// ������� ������ �����
				delete[] aCmdLine;
			} // if
			else
			{
				LPCTSTR aCmdLine[1] = { szFileName };

				// ������ ������� � ����� �������
				bRet = StartGroupProcessesAsJob(hSampleJob, aCmdLine, 1, FALSE, 0);
			} // else

			if (FALSE != bRet)
			{
				HWND hwndList = GetDlgItem(hwnd, IDC_LIST_PROCESSES);

				// �������� ������ ��������� � ��������� �������
				LoadProcessesInJobToListBox(hwndList, hSampleJob);

				// ������� ������ �������
				ListBox_ResetContent(GetDlgItem(hwnd, IDC_LIST_MODULES));
			} // if
			else
			{
				MessageBox(hwnd, TEXT("�������� ������"), NULL, MB_ICONERROR | MB_OK);
			} // else
		} // if
	}
	break;
	
	case ID_CURRENT_WORKING_PROCESS : // �������� � ������� �������
	{
		HWND hwndList = GetDlgItem(hwnd, IDC_LIST_PROCESSES);
		// �������� ������ ��������� � ������� �������
		LoadProcessesInJobToListBox(hwndList);
		// ������� ������ �������
		ListBox_ResetContent(GetDlgItem(hwnd, IDC_LIST_MODULES));
	}
	break;


	case ID_GROUP_PROCESS_IN_TASK: // ��������, �������������� � �������
	{
		HWND hwndList = GetDlgItem(hwnd, IDC_LIST_PROCESSES);

		// �������� ������ ��������� � ��������� �������
		LoadProcessesInJobToListBox(hwndList, hSampleJob);

		// ������� ������ �������
		ListBox_ResetContent(GetDlgItem(hwnd, IDC_LIST_MODULES));
	}
	break;
	
	case ID_PRIORCHANGE: // ��������� ����������
	{
		// ������� ���������� ���������� ����������
		HINSTANCE hInstance = GetWindowInstance(hwnd);
		// ������� ��������� ���������� ���� ��� ��������� ����������
		DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, DialProc);
	}
	break; 
	} // switch
} // OnCommand

#pragma region Dialog Box
INT_PTR CALLBACK DialProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		BOOL bRet = HANDLE_WM_INITDIALOG(hwndDlg, wParam, lParam, Dialog_InitDialog);
		return SetDlgMsgResult(hwndDlg, uMsg, bRet);
	}

	case WM_CLOSE:
		HANDLE_WM_CLOSE(hwndDlg, wParam, lParam, Dialog_Close);
		return TRUE;

	case WM_COMMAND:
		HANDLE_WM_COMMAND(hwndDlg, wParam, lParam, Dialog_Command);
		return TRUE;
	} // switch

	return FALSE;
} // DialogProc

// ------------------------------------------------------------------------------------------------
BOOL Dialog_InitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	HWND hwndCtl;

	// ��������� ���������� ������ "������ ���������� ��������"
	hwndCtl = GetDlgItem(hwnd, IDC_COMBO_PRIOR_CLASS);

	constexpr LPCTSTR szPriorityClasses[6] = {
		TEXT("��������� �������"),
		TEXT("�������"),
		TEXT("���� ��������"),
		TEXT("�������"),
		TEXT("���� ��������"),
		TEXT("������")
	};

	constexpr DWORD dwPriorityClasses[6] = {
		REALTIME_PRIORITY_CLASS,
		HIGH_PRIORITY_CLASS,
		ABOVE_NORMAL_PRIORITY_CLASS,
		NORMAL_PRIORITY_CLASS,
		BELOW_NORMAL_PRIORITY_CLASS,
		IDLE_PRIORITY_CLASS
	};

	DWORD dwPriorityClass = GetPriorityClass(GetCurrentProcess());

	for (int i = 0; i < _countof(dwPriorityClasses); ++i)
	{
		int iItem = ComboBox_AddString(hwndCtl, szPriorityClasses[i]);
		ComboBox_SetItemData(hwndCtl, iItem, dwPriorityClasses[i]);

		if (dwPriorityClasses[i] == dwPriorityClass)
		{
			ComboBox_SetCurSel(hwndCtl, iItem);
		} // if
	} // for

	// ��������� ���������� ������ "������������� ���������� �������"
	hwndCtl = GetDlgItem(hwnd, IDC_COMBO_PRIOR);

	constexpr LPCTSTR szPriorities[7] = {
		TEXT("��������� �� �������"),
		TEXT("������������"),
		TEXT("���� ��������"),
		TEXT("�������"),
		TEXT("���� ��������"),
		TEXT("�����������"),
		TEXT("�������������")
	};

	constexpr DWORD dwPriorities[7] = {
		THREAD_PRIORITY_TIME_CRITICAL,
		THREAD_PRIORITY_HIGHEST,
		THREAD_PRIORITY_ABOVE_NORMAL,
		THREAD_PRIORITY_NORMAL,
		THREAD_PRIORITY_BELOW_NORMAL,
		THREAD_PRIORITY_LOWEST,
		THREAD_PRIORITY_IDLE
	};

	DWORD dwPriority = GetThreadPriority(GetCurrentThread());

	for (int i = 0; i < _countof(dwPriorities); ++i)
	{
		int iItem = ComboBox_AddString(hwndCtl, szPriorities[i]);
		ComboBox_SetItemData(hwndCtl, iItem, dwPriorities[i]);

		if (dwPriorities[i] == dwPriority)
		{
			ComboBox_SetCurSel(hwndCtl, iItem);
		} // if
	} // for

	return TRUE;
} // Dialog_InitDialog

// ------------------------------------------------------------------------------------------------
void Dialog_Close(HWND hwnd)
{
	EndDialog(hwnd, IDCLOSE);
} // Dialog_Close

// ------------------------------------------------------------------------------------------------
void Dialog_Command(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDOK:
	{
		HWND hwndCtl;
		int iItem;

		// ������� ����� ����������

		hwndCtl = GetDlgItem(hwnd, IDC_COMBO_PRIOR_CLASS);

		// ������� ��������� ����� ����������
		iItem = ComboBox_GetCurSel(hwndCtl);
		DWORD dwPriorityClass = (iItem != -1) ? (DWORD)ComboBox_GetItemData(hwndCtl, iItem) : NORMAL_PRIORITY_CLASS;

		// ������ ����� ����������
		SetPriorityClass(GetCurrentProcess(), dwPriorityClass);


		// ������� ������������� ��������� ��� �������� ������

		hwndCtl = GetDlgItem(hwnd, IDC_COMBO_PRIOR);

		// ������� ������������� ��������� ������
		iItem = ComboBox_GetCurSel(hwndCtl);
		DWORD dwPriority = (iItem != -1) ? (DWORD)ComboBox_GetItemData(hwndCtl, iItem) : THREAD_PRIORITY_NORMAL;

		// ������ ������������� ��������� ��� �������� ������
		SetThreadPriority(GetCurrentThread(), dwPriority);

		EndDialog(hwnd, IDOK);
	}
	break;

	case IDCANCEL:
		EndDialog(hwnd, IDCANCEL);
		break;
	} // switch
} // Dialog_Command
#pragma endregion



void LoadProcessesToListBox(HWND hwndCtl)
{
	ListBox_ResetContent(hwndCtl);//������� ������

	DWORD aProcessIds[1024], cbNeeded = 0;//������ ��� ID ��������� ���������
	BOOL bRet = EnumProcesses(aProcessIds, sizeof(aProcessIds), &cbNeeded);//��������� ������ ID ��������� �����������
	
	if (FALSE != bRet)
	{
		TCHAR szName[MAX_PATH], szBuffer[300];

		for (DWORD i = 0,
			n = cbNeeded / sizeof(DWORD); i < n; ++i)
		{
			DWORD dwProcessId = aProcessIds[i], cch = 0;
			if (0 == dwProcessId) continue;

			
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);//��������� ����������� �������� �� ��� ID

			if (NULL != hProcess)
			{
				cch = GetModuleBaseName(hProcess, NULL, szName, _countof(szName));// �������� ��� �������� ������ ��������

				CloseHandle(hProcess); // ��������� ������ ����
			} 

			if (0 == cch)
				StringCchCopy(szName, MAX_PATH, TEXT("��� �������� �� ����������"));

			StringCchPrintf(szBuffer, _countof(szBuffer), TEXT("%s (PID: %u)"), szName, dwProcessId);

			
			int iItem = ListBox_AddString(hwndCtl, szBuffer);

			ListBox_SetItemData(hwndCtl, iItem, dwProcessId);//������ � ListBox ����� ��������
		} // for
	} // if
} // LoadProcessesToListBox


void LoadModulesToListBox(HWND hwndCtl, DWORD dwProcessId)
{
	// ������ ��� ������ �� ������
	ListBox_ResetContent(hwndCtl);

	// ��������� ������ ���� "�������"
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);

	if (NULL != hProcess)
	{
		// ���������� ������ (� ������) ������ �������
		DWORD cb = 0;
		EnumProcessModulesEx(hProcess, NULL, 0, &cb, LIST_MODULES_ALL);

		// ��������� ���������� �������
		DWORD nCount = cb / sizeof(HMODULE);

		// �������� ������ ��� ������ �������
		HMODULE *hModule = new HMODULE[nCount];

		// �������� ������ �������
		cb = nCount * sizeof(HMODULE);
		BOOL bRet = EnumProcessModulesEx(hProcess, hModule, cb, &cb, LIST_MODULES_ALL);

		if (FALSE != bRet)
		{
			TCHAR szFileName[MAX_PATH];

			for (DWORD i = 0; i < nCount; ++i)
			{
				// �������� ��� ������������ ������
				bRet = GetModuleFileNameEx(hProcess, hModule[i], szFileName, MAX_PATH);
				if (FALSE != bRet) ListBox_AddString(hwndCtl, szFileName); // ��������� � ������ ����� ������
			} // for
		} // if

		delete[]hModule; // ����������� ������
		CloseHandle(hProcess); // ��������� ������ ����
	} // if
} // LoadModulesToListBox

void LoadProcessesInJobToListBox(HWND hwndCtl, HANDLE hJob)
{
	// ������ ��� ������ �� ������
	ListBox_ResetContent(hwndCtl);

	// ������� ������ ��������������� ���� ��������� ���������
	DWORD aProcessIds[1024], cbNeeded = 0;
	BOOL bRet = EnumProcessesInJob(hJob, aProcessIds, sizeof(aProcessIds), &cbNeeded);

	if (FALSE != bRet)
	{
		TCHAR szName[MAX_PATH], szBuffer[300];

		for (DWORD i = 0,
			n = cbNeeded / sizeof(DWORD); i < n; ++i)
		{
			DWORD dwProcessId = aProcessIds[i], cch = 0;

			// ��������� ������ ���� ��������
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);

			if (NULL != hProcess)
			{
				// �������� ��� �������� ������ ��������
				cch = GetModuleBaseName(hProcess, NULL, szName, _countof(szName));
				CloseHandle(hProcess); // ��������� ������ ����
			} // if

			if (0 == cch)
				StringCchCopy(szName, MAX_PATH, TEXT("����������� �������"));//� ����� ������� ����������?

			// ��������� ������ ��� ������
			StringCchPrintf(szBuffer, _countof(szBuffer), TEXT("%s (PID: %u)"), szName, dwProcessId);

			// ��������� � ������ ����� ������
			int iItem = ListBox_AddString(hwndCtl, szBuffer);
			// ��������� � ����� ������ ������������� ��������
			ListBox_SetItemData(hwndCtl, iItem, dwProcessId);
		} // for
	} // if
} // LoadProcessesInJobToListBox


BOOL EnumProcessesInJob(HANDLE hJob, DWORD* lpidProcess, DWORD cb, LPDWORD lpcbNeeded)
{
	// ���������� ������������ ����� ���������,
	// �� ������� �������� ����� lpidProcess
	DWORD nCount = cb / sizeof(ULONG_PTR);

	if (NULL != lpidProcess && nCount > 0)
	{
		// ���������� ������ ����� ������ (� ������)
		// ��� �������� ��������������� � ���������
		DWORD cbJobPIL = sizeof(JOBOBJECT_BASIC_PROCESS_ID_LIST) + (nCount - 1) * sizeof(ULONG_PTR);

		// �������� ���� ������
		JOBOBJECT_BASIC_PROCESS_ID_LIST *pJobPIL = (JOBOBJECT_BASIC_PROCESS_ID_LIST *)malloc(cbJobPIL);

		if (NULL != pJobPIL)
		{
			// ��������� ������������ ����� ���������,
			// �� ������� ��������� ���������� ������
			pJobPIL->NumberOfAssignedProcesses = nCount;

			// ����������� ������ ��������������� ���������
			BOOL bRet = QueryInformationJobObject(hJob, JobObjectBasicProcessIdList, pJobPIL, cbJobPIL, NULL);

			if (FALSE != bRet)
			{
				// ���������� ���������� ��������������
				nCount = pJobPIL->NumberOfProcessIdsInList;  

				// �������� � ����� ������ ���������������
				CopyMemory(lpidProcess, pJobPIL->ProcessIdList, nCount * sizeof(ULONG_PTR));

				// ���������� ������ ����� ������ (� ������),
				// � ������� ���������� ������ ���������������
				if (NULL != lpcbNeeded)
					*lpcbNeeded = nCount * sizeof(ULONG_PTR);
			} // if

			free(pJobPIL); // ����������� ������
			return bRet;
		} // if
	} // if

	return FALSE;
} // EnumProcessesInJob

BOOL StartGroupProcessesAsJob(HANDLE hJob, LPCTSTR lpszCmdLine[], DWORD nCount, BOOL bInheritHandles, DWORD dwCreationFlags)
{
	// ���������, ������� �� ���������� ������� � �������
	BOOL bInJob = FALSE;
	IsProcessInJob(GetCurrentProcess(), NULL, &bInJob);

	if (FALSE != bInJob) // ���� �� (!)
	{
		// ��������� ��������� �� ��������� ��������,
		// ������� �� ����� ������������ ����� �������

		JOBOBJECT_BASIC_LIMIT_INFORMATION jobli = { 0 };

		QueryInformationJobObject(NULL,
			JobObjectBasicLimitInformation, &jobli, sizeof(jobli), NULL);

		DWORD dwLimitMask = JOB_OBJECT_LIMIT_BREAKAWAY_OK | JOB_OBJECT_LIMIT_SILENT_BREAKAWAY_OK;

		if ((jobli.LimitFlags & dwLimitMask) == 0)
		{
			/* ��� ����������� ��������
			   ������������� ���������� � ������� */
			return FALSE;
		} // if
	} // if

	// ��������� ��������...

	TCHAR szCmdLine[MAX_PATH];
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi = { 0 };

	for (DWORD i = 0; i < nCount; ++i)
	{
		StringCchCopy(szCmdLine, MAX_PATH, lpszCmdLine[i]);

		// ��������� ����� �������,
		// ��������������� ������ ��� �������� ������
		BOOL bRet = CreateProcess(NULL, szCmdLine, NULL, NULL,
			bInheritHandles, dwCreationFlags | CREATE_SUSPENDED | CREATE_BREAKAWAY_FROM_JOB, NULL, NULL, &si, &pi);

		if (FALSE != bRet)
		{
			// ��������� ����� ������� � �������
			AssignProcessToJobObject(hJob, pi.hProcess);
			// ������������ ������ ������ ������ ��������
			ResumeThread(pi.hThread);
			// ��������� ���������� ������ ������ ��������
			CloseHandle(pi.hThread);
			// ��������� ���������� ������ ��������
			CloseHandle(pi.hProcess);
		} // if
	} // for

	return TRUE;
} // StartGroupProcessesAsJob
