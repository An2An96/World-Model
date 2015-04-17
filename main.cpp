#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <conio.h>
#include <time.h>

#include "useful.h"
#include "world.h"

using namespace std;

int main()
{
	//	settings
	setlocale(LC_ALL, "russian");
	srand((int)time(NULL));
	
	World* w = 0;
	cout << "1 - создать новое поле; 2 - загрузить из файла" << endl;
	char c = _getch();
	char* fname = new char[200];
	if (c == '1')
	{
		int x, y;
		cout << "Введите размеры поля" << endl;
		cin >> x >> y;
		w = new World(x, y);
	}
	else if(c == '2')
	{
		cout << "Введите имя файла для чтения: ";
		std::cin.getline(fname, 200);
		w = new World(fname);
	}
	if (!w)	{
		return false;
	}
	std::cout << "1 - сохранить; 2 - пауза; Esc - выход\n";
	system("pause");
	do
	{
		if (_kbhit())
		{
			c = _getch();
			if (c == '1')
			{
				cout << "Введите имя файла для записи: ";
				std::cin.get();
				std::cin.getline(fname, 200);
				bool is_saved = w->SaveWorld(fname);
				std::cout << "Сохранено\n";
				system("pause");
			}
			else if(c == '2')
			{
				c = _getch();
				if (c == '1')
				{
					cout << "Введите имя файла для записи: ";
					cin.get();
					cin.getline(fname, 200);
					if (w->SaveWorld(fname))	{
						cout << "Сохранено" << endl;
					}
					else {
						cout << "Не сохранено" << endl;
					}
					delete[] fname;
					system("pause");
				}
				else if(c == 27)	{
					break;
				}
			}
			else if (c == 27)	{
				break;
			}
		}
		system("CLS");		//	чистим экран

		std::cout << "1 - сохранить; 2 - пауза; Esc - выход\n";
		w->View();			//	выводим

	} while (w->PerformStep());
	delete w;
	delete[] fname;
	system("pause");
	return false;
}
