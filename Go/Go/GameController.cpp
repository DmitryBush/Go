#include "GameController.h"
#include <conio.h>
#include <iostream>
#include <Windows.h>

gameController::gameController(const unsigned int& x, const unsigned int& y):
	width(x), height(y), gameMap(width, height), isDead(false),
	complete(false), blackMove(true)
{}

void gameController::PlayerMove()
{
	unsigned int x = 0, y = 0;

	while (true)
	{
		system("cls");
		TextModeOutput();

		if (blackMove)
		{
			if (usedStone[0] == 12)
			{
				std::cout << "Ваш ход - черные фишки" << '\n'
					<< "1. Передвинуть камень" << '\n';

				PlayerMove(false);
			}
			else
			{
				std::cout << "Ваш ход - черные фишки" << '\n'
					<< "1. Поставить камень" << '\n';

				PlayerMove(true);
				usedStone[0]++;
			}
		}
		else
		{
			if (usedStone[1] == 12)
			{
				std::cout << "Ваш ход - белые фишки" << '\n'
					<< "1. Передвинуть камень" << '\n';

				PlayerMove(false);
			}
			else
			{
				std::cout << "Ваш ход - белые фишки" << '\n'
					<< "1. Поставить камень" << '\n';

				PlayerMove(true);
				usedStone[1]++;
			}
		}	
	}
}

void gameController::PlayerMove(const bool setStone)
{
	unsigned int x = 0, y = 0, prevX_coord = 0, prevY_coord = 0;
	if (setStone)
	{
		std::cout << "Введите строку и столбец" << '\n';
		std::cin >> x;	
		std::cin >> y;

		if (gameMap.EmptyCheck(x - 1, y - 1))
		{
			gameMap.SetStone(x - 1, y - 1, DefineMove());
			Update(x - 1, y - 1);

			blackMove = !blackMove;
		}
		else
		{
			system("cls");

			TextModeOutput();
			std::cout << "Введите заново строку и столбец" << '\n';
			std::cin >> x;
			std::cin >> y;
		}
	}
	else
	{
		std::cout << "Введите предыдущюю строку и столбец" << '\n';
		std::cin >> prevX_coord;
		std::cin >> prevY_coord;

		std::cout << "Введите строку и столбец" << '\n';
		std::cin >> x;
		std::cin >> y;

		if (gameMap.GetCell(prevX_coord - 1, prevY_coord - 1) == DefineMove()
			&& gameMap.EmptyCheck(x - 1, y - 1))
		{
			gameMap.MoveStone(prevX_coord - 1, prevY_coord - 1, x - 1, y - 1);
			Update(x - 1, y - 1);

			blackMove = !blackMove;
		}
		else
		{
			system("cls");

			TextModeOutput();
			std::cout << "Введите заново строку и столбец" << '\n';
			std::cin >> x;
			std::cin >> y;
		}
	}
}

int gameController::Update(const int& x_elem, const int& y_elem)
{
	//if ((x_elem >= 0 && x_elem < width) && (y_elem >= 0 && y_elem < height))
	//{
	//	Update(x_elem - 1, y_elem + 1, 1);
	//	Update(x_elem, y_elem + 1, 1);
	//	Update(x_elem + 1, y_elem + 1, 1);

	//	Update(x_elem - 1, y_elem, 1);
	//	//Update(x_elem, y_elem, 1);
	//	Update(x_elem + 1, y_elem, 1);

	//	Update(x_elem - 1, y_elem - 1, 1);
	//	Update(x_elem, y_elem - 1, 1);
	//	Update(x_elem + 1, y_elem - 1, 1);
	//}
	for (int i = 0; i < width; i++) // по столбцу
	{
		int lenColumn = 0;
		for (int j = 0; j < height; j++)
		{
			if (gameMap.GetCell(i, j) == DefineMove())
				lenColumn++;
		}

		if (lenColumn == 5)
		{
			if (blackMove)
				playersScore[0]++;
			else
				playersScore[1]++;
		}
	}

	for (int j = 0; j < width; j++) // По строкам
	{
		int lenRow = 0;
		for (int i = 0; i < height; i++)
		{
			if (gameMap.GetCell(i, j) == DefineMove())
				lenRow++;
		}

		if (lenRow == 5)
		{
			if (blackMove)
				playersScore[0]++;
			else
				playersScore[1]++;
		}
	}

	for (int i = 0, lenDiagonal = 0; i < width; i++) // Главная диагональ
	{
		if (gameMap.GetCell(i, i) == DefineMove())
			lenDiagonal++;

		if (lenDiagonal == 5)
		{
			if (blackMove)
				playersScore[0]++;
			else
				playersScore[1]++;
		}
	}

	for (int i = width - 1, lenSideDiagonal = 0; i >= 0; i--) // Побочная диагональ
	{
		if (gameMap.GetCell(i, (width - 1) - i) == DefineMove())
			lenSideDiagonal++;

		if (lenSideDiagonal == 5)
		{
			if (blackMove)
				playersScore[0]++;
			else
				playersScore[1]++;
		}
	}

	for (int i = 0; i < 2; i++)
		if (playersScore[i] == 10)
			complete = true;
	return 0;
}

char gameController::DefineMove()
{
	if (blackMove)
		return 'b';
	return 'w';
}

void gameController::Restart()
{
	isDead = false;
	blackMove = true;

	gameMap.ResetField();
	for (auto i = 0; i < 2; i++)
		usedStone[i] = 0;
}

void gameController::TextModeOutput()
{
	std::cout << "Очки " << playersScore[0] << " - Черные фишки, "
		<< playersScore[1] << " - белые фишки\n";

	std::cout << "  ";
	for (int i = 1; i <= height; i++)
	{
		std::cout << i << ' ';
	}
	std::cout << '\n';

	for (int i = 0; i < width; i++)
	{
		std::cout << i + 1 << ' ';

		for (int j = 0; j < height; j++)
		{
			std::cout << gameMap.GetCell(i,j) << ' ';
		}
		std::cout << '\n';
	}
}

//void gameController::Update(const int& x_elem, const int& y_elem, int line)
//{
//	if (line == 5)
//	{
//		if (blackMove)
//			playersScore[0]++;
//		else
//			playersScore[1]++;
//		return;
//	}
//
//	if ((x_elem >= 0 && x_elem < width) && (y_elem >= 0 && y_elem < height))
//	{
//		if (gameMap.GetCell(x_elem,y_elem) == DefineMove())
//		{
//			Update(x_elem - 1, y_elem + 1, line + 1);
//			Update(x_elem, y_elem + 1, line + 1);
//			Update(x_elem + 1, y_elem + 1, line + 1);
//
//			Update(x_elem - 1, y_elem, line + 1);
//			//Update(x_elem, y_elem, line + 1);
//			Update(x_elem + 1, y_elem, line + 1);
//
//			Update(x_elem - 1, y_elem - 1, line + 1);
//			Update(x_elem, y_elem - 1, line + 1);
//			Update(x_elem + 1, y_elem - 1, line + 1);
//		}
//	}
//}
