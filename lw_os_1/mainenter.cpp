#include <Windows.h>
#include <wchar.h>
#include <locale.h>
#include <stdio.h>

int wmain()
{
	_wsetlocale(LC_ALL, TEXT(""));
	wprintf(TEXT("����� ������ ����������� ����������"));
	//_wsetlocale(LC_ALL, );
}
