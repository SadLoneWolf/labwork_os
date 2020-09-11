#include <Windows.h>
#include <wchar.h>
#include <locale.h>
#include <stdio.h>
#include <ShlObj.h>
#include <STATILIBR.h>
#include <DYNLIB1.h>

#pragma comment (lib,"STATILIBR.lib")
#pragma comment (lib,"DYNLIB1.lib") // ������� ����������� � �����.����������

int wmain()
{
	_wsetlocale(LC_ALL, TEXT(""));
	wprintf(TEXT("\n����� ������ ����������� ����������\n\n"));
	PrintCompName();
	PrintDNSName();
	PrintUserName();
	PrintUserNameExtended();
	wprintf(TEXT("\n���� � ��������� ���������\n\n"));
	const long csdir[] =
	{
		CSIDL_DESKTOP,
		CSIDL_PROGRAMS,
		CSIDL_PERSONAL,
		CSIDL_STARTUP,
		CSIDL_SYSTEM,
		CSIDL_HISTORY
	};//������ ��������������� ��������� �����
	PrintSysDir(csdir, _countof(csdir)); 
	wprintf(TEXT("\n������ ������������ �������\n\n"));
	PrintOSinfo();
}
