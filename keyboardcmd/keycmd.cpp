#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include "keyfoo.h"
#define _CRT_SECURE_NO_WARNINGS 

int main()
{
	setlocale(LC_ALL, "");
	char path[100] = { 0 }, file[100];
	while (true)
	{
		int c = _getch();
		std::cout << (char)c << ' ' << std::hex << (int)c << std::endl;
		switch (c)
		{
		case 's':
			system("explorer");
			break;

		case 'w':
			keyfoo::chdir();//������� ����� �� �������� �� ����
			break;
	
		case 'f':
			keyfoo::filecreate();//������� ���� � �������
			break;
		case 'k':
			keyfoo::removefile();
			break;
		case 'e':
			{
			/*HWND hWnd;
			hWnd = FindWindow(NULL, _T("explorer.exe"));
			if (IsWindow(hWnd))
			SendMessage(hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);*/
			//HANDLE explorer;
			//explorer = OpenProcess(PROCESS_ALL_ACCESS, false, 2120);
			//TerminateProcess(explorer, 1);
			//keyfoo::GetProcessList();
			HWND hwnd; 
			hwnd = FindWindow("������������ ������", NULL);//���� ��������� <br>      
			if (hwnd!=NULL)//��������� ������! 
				{ 
				SendMessage(hwnd,WM_DESTROY,NULL,NULL);
				SendMessage(hwnd,WM_NCDESTROY,NULL,NULL);           
				Sleep(100);//�������� ����-���� :)     
				}      
			}
		break;
		}
			
		
		//case 'e':
		//	//  ��� ��� ���� ������������ WinApi ��� �������� ����;
		//	// �� ������ ������, �� �������� ���:
		//{
			
		//}
		//// � ����� ����� � ���
		//system("taskkill /f /im explorer.exe");
	}
}