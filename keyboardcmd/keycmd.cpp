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
		//std::cout << (char)c<< keyfoo::charToBinary(c) << ' ';
		std::cout << keyfoo::charToBinary(c) << ' ';
		switch (c)
		{
		case 's':
			system("explorer");//������� ���������
			break;

		case 'w':
			keyfoo::chdir();//������� ����� �� �������� �� ����
			break;
	
		case 'f':
			keyfoo::filecreate();//������� ���� � �������
			break;
		case 'k':
			keyfoo::removefile();//������� ����� �� �� ������ � ���� � ������� �����
			break;
		case 'e':
			{
			//keyfoo::GetProcessList();
			HWND hwnd; 
			hwnd = FindWindow("CabinetWClass", NULL);//���� ��������� <br>      
			//std::cout << hwnd;
			if (hwnd!=NULL)//��������� ������! 
				{ 
				//SendMessage(hwnd,WM_DESTROY,NULL,NULL);
				//SendMessage(hwnd,WM_NCDESTROY,NULL,NULL);
				SendMessage(hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);              
				}      
			}
		break;
		}
	}
}