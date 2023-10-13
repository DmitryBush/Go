#include "Point.h"
#include <iostream>

/*
* Методы получения значений X и Y точек
*/
int point::GetX() const
{
	return x;
}

int point::GetY() const
{
	return y;
}

/*
* методы изменения значений точек
*/
void point::EditX()
{
	std::cin >> x;
}

void point::EditY()
{
	std::cin >> y;
}

void point::SetPoint(int x, int y)
{
	this->x = x; this->y = y;
}

/*
* Вывод значений точек в консоль
*/
void point::ShowPoint()
{
	std::cout << x << ' ' << y << std::endl;
}

/*
* Перегрузка оператора для сравнения точек
*/
bool operator==(point arg_1, point arg_2)
{
	if (arg_1.GetX() == arg_2.GetX() && arg_1.GetY() == arg_2.GetY())
	{
		return true;
	}
	else
	{
		return false;
	}
}