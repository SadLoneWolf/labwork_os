#define _CRT_SECURE_NO_WARNINGS
#include "keyfoo.h"


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

/***************������� ����� �����*************************************/
void chdir()
{
	TCHAR sPath[MAX_PATH], sPathTo2[MAX_PATH];
	cout << setw(5) << "������� ����: ";
	cin >> sPathTo2;
	if (strcmp(sPathTo2, "test") == 0)
	{
		strcpy_s(sPathTo2, "C:\\testdirect\\");
	}
	strcpy_s(sPath, sPathTo2);//����. �� 25/09/2019 ������� ����� �������� ��� ����� 
	SetCurrentDirectory(sPath);

}
/**********************************************************************/

/*********************������� �������� ����� � ������������ �����*******/
void removefile() //added 02/09/2019 21:37
{
	char ans[4], fname[MAX_PATH], dirname[MAX_PATH];
	//changeornot();
	do
	{
		cout << setw(5) << "��������� directory (dir) ��� file (fil)?";
		cin >> ans;
	} while ((strcmp(ans, "dir") > 0) && (strcmp(ans, "fil") > 0));
	cin.clear();
	if ((strcmp(ans, "fil") == 0))
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
		if ((strcmp(ans, "dir") == 0))
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