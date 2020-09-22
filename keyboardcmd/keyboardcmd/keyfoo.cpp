#define _CRT_SECURE_NO_WARNINGS
#include "keyfoo.h"

namespace keyfoo
{
	/*********************�������� ����� � ������������ �����******************/
	void filecreate()
	{
		const int MAX = 2000;//������������ ����� ������
		char str[MAX], f_name[MAX_PATH]; //������ , ��� �����
		//changeornot();
		cout << setw(5) << "��� �����: ";
		cin >> f_name;

		ofstream fout(f_name); // ������ ������ ������ ofstream ��� ������ 
		cin.get(str, MAX, '/');//���� �� ����� ������ �������� ������ ����� ���� ������ ���������
		fout << str; // ������ ������ � ����
		fout.close(); // ��������� ����
	}
	/************************************************************************/


	/*********************������� �������� ����� � ������������ �����*******/
	void removefile() //added 02/09/2019 21:37
	{
		char ans[4], fname[MAX_PATH], dirname[MAX_PATH];
		//changeornot();
		do
		{
			cout << setw(5) << "��������� directory (d) ��� file (f)?";
			cin >> ans;
		} while ((strcmp(ans, "d") > 0) && (strcmp(ans, "f") > 0));
		cin.clear();
		if ((strcmp(ans, "f") == 0))
		{
			cout << "���� ��� ��������: ";
			cin >> fname;
			if (remove(fname))
			{
				cout << "������ �������� �����" << endl;
			}
			else
				cout << "���� ������" << endl;
		}
		else
		{
			if ((strcmp(ans, "d") == 0))
			{
				cout << "����� ��� ��������: ";
				cin >> dirname;
				if (_rmdir(dirname) == -1) //10/12/2019 15:38 �� ������� ��� �������, ����� �� ������� ����
				{
					cout << "������ �������� �����" << endl;
					cout << "error" << strerror(errno) << endl;
				}
				else
					cout << "����� �������" << endl;
			}
		}
	}
	/**********************************************************************/
	int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep)
	{
		puts("in filter.");
		if (code == EXCEPTION_ACCESS_VIOLATION)
		{
			puts("caught AV as expected.");
			return EXCEPTION_EXECUTE_HANDLER;
		}
		else
		{
			puts("didn't catch AV, unexpected.");
			return EXCEPTION_CONTINUE_SEARCH;
		};
	}
	/***************������� ����� �����*************************************/
	void chdir()
	{
		TCHAR sPath[MAX_PATH], sPathTo2[MAX_PATH];
		cout << setw(5) << "������� ����: ";
		cin >> sPathTo2;
		__try
		{
			ShellExecute(NULL, "open", sPathTo2, NULL, NULL, SW_SHOW);//��������
		}
		__except (filter(GetExceptionCode(), GetExceptionInformation()))
		{
			puts("in except");
		}

	}
	/**********************************************************************/
	HANDLE GetProcessHandle(LPSTR szExeName)
	{
		PROCESSENTRY32 Pc = { sizeof(PROCESSENTRY32) };
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
		if (Process32First(hSnapshot, &Pc)) {
			do {
				if (!strcmp(Pc.szExeFile, szExeName)) {
					return OpenProcess(PROCESS_ALL_ACCESS, TRUE, Pc.th32ProcessID);
				}
			} while (Process32Next(hSnapshot, &Pc));
		}
		return NULL;
	}
	//void closeexp()
	//{
	//	int x;
	//	HANDLE kill = GetProcessHandle("");
	//	printf("%i\n", kill);
	//	DWORD fdwExit = 0;
	//	GetExitCodeProcess(kill, &fdwExit);
	//	TerminateProcess(kill, fdwExit);
	//	x = CloseHandle(kill);
	//	printf("%i\n", x);
	//}
	void exp()
	{
		STARTUPINFO cif;
		ZeroMemory(&cif, sizeof(STARTUPINFO));
		PROCESS_INFORMATION pi;
		if (CreateProcess("c:\\windows\\explorer.exe", NULL,
			NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi) == TRUE)
		{
			cout << "process" << endl;
			cout << "handle " << pi.hProcess << endl;
			Sleep(1000);				// ���������
			TerminateProcess(pi.hProcess, NO_ERROR);	// ������ �������
		}
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
}