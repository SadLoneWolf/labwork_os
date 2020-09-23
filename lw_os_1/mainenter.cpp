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
	wprintf(TEXT("����� ������ ����������� ����������\n"));
	PrintCompName();
	PrintDNSName();
	PrintUserName();
	PrintUserNameExtended();
	wprintf(TEXT("���� � ��������� ���������\n"));
	const long csdir[] =
	{
		CSIDL_DESKTOP,
		CSIDL_INTERNET,
		CSIDL_PROGRAMS,
		CSIDL_PERSONAL


	};//����������� ������� �������� �����
	PrintSysDir(csdir, _countof(csdir)); 

}
