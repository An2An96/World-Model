#include <iostream>
#include <cstdlib>
#include <time.h>

#include "useful.h"
#include "world.h"

int main()
{
	//	settings
	setlocale(LC_ALL, "russian");
	srand((int)time(NULL));
		   
	int i = 0;
	//World test(40, 10);		//	создаем мир
	World test("test.txt");
	do{
		system("CLS");		//	чистим экран
		test.View();		//	выводим
		if (++i == 20)	{
			test.SaveWorld("test.txt");
		}
	} while (test.PerformStep());

	system("CLS");		//	чистим экран
	test.View();		//	выводим

	system("pause");
	return true;
}
