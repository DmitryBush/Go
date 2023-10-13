#include "GameController.h"
#include <iostream>
#include <conio.h>

gameController::gameController(const unsigned int& x, const unsigned int& y):
	width(x), height(y), gameMap(width, height), complete(false), blackMove(true)
{}

void gameController::PlayerMove()
{
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
	int x = 0, y = 0, prevX_coord = 0, prevY_coord = 0;
	if (setStone)
	{
		std::cout << "Введите строку и столбец" << '\n';
		std::cin >> x >> y;
		CheckInputValidation(x, y);

		gameMap.SetStone(x - 1, y - 1, DefineMove());
		Update(x - 1, y - 1);

		blackMove = !blackMove;
	}
	else
	{
		std::cout << "Введите предыдущюю строку и столбец" << '\n';
		std::cin >> prevX_coord >> prevY_coord;
		CheckInputValidation(prevX_coord, prevY_coord);

		system("cls");
		TextModeOutput();
		std::cout << "Введите строку и столбец" << '\n';
		std::cin >> x >> y;
		CheckInputValidation(prevX_coord, prevY_coord, x, y);

		gameMap.MoveStone(prevX_coord - 1, prevY_coord - 1, x - 1, y - 1);
		Update(x - 1, y - 1);

		blackMove = !blackMove;
	}
}

int gameController::Update(const int& x_elem, const int& y_elem)
{
	playersScore[0] = 0; playersScore[1] = 0;
	for (int i = 0; i < width; i++) // по столбцу
	{
		int lenColumnBl = 0;
		int lenColumnWh = 0;
		for (int j = 0; j < height; j++)
		{
			if (gameMap.GetCell(i, j) == 'b')
				lenColumnBl++;
			else
				lenColumnBl = 0;

			if (gameMap.GetCell(i, j) == 'w')
				lenColumnWh++;
			else
				lenColumnWh = 0;
		}

		if (lenColumnBl == 5)
			playersScore[0]++;

		if (lenColumnWh == 5)
			playersScore[1]++;
	}

	for (int j = 0; j < width; j++) // По строкам
	{
		int lenRowBl = 0;
		int lenRowWh = 0;
		for (int i = 0; i < height; i++)
		{
			if (gameMap.GetCell(i, j) == 'b')
				lenRowBl++;
			else
				lenRowBl = 0;

			if (gameMap.GetCell(i, j) == 'w')
				lenRowWh++;
			else
				lenRowWh = 0;
		}

		if (lenRowBl == 5)
			playersScore[0]++;

		if (lenRowWh == 5)
			playersScore[1]++;
	}

	for (int i = 0, lenDiagonalBl = 0, lenDiagonalWh = 0; i < width; i++) // Главная диагональ
	{
		if (gameMap.GetCell(i, i) == 'b')
			lenDiagonalBl++;
		else
			lenDiagonalBl = 0;

		if (gameMap.GetCell(i, i) == 'w')
			lenDiagonalWh++;
		else
			lenDiagonalWh = 0;


		if (lenDiagonalBl == 5)
			playersScore[0]++;

		if (lenDiagonalWh == 5)
			playersScore[1]++;
	}

	for (int i = width - 1, 
		lenSideDiagBl = 0, lenSideDiagWh = 0; i >= 0; i--) // Побочная диагональ
	{
		if (gameMap.GetCell(i, (width - 1) - i) == 'b')
			lenSideDiagBl++;
		else
			lenSideDiagBl = 0;

		if (gameMap.GetCell(i, (width - 1) - i) == 'w')
			lenSideDiagWh++;
		else
			lenSideDiagWh = 0;

		if (lenSideDiagBl == 5)
			playersScore[0]++;

		if (lenSideDiagWh == 5)
			playersScore[1]++;
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

void gameController::CheckInputValidation(int& x, int& y)
{
	while (std::cin.fail() 
		|| !gameMap.GetCell(x - 1, y - 1)
		|| !gameMap.EmptyCheck(x - 1, y - 1))
	{
		system("cls");
		std::cin.clear();
		std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		TextModeOutput();
		std::cout << "An error has occuried" << '\n' 
			<< "Введите заново строку и столбец" << '\n';
		std::cin >> x >> y;
	}
}

void gameController::CheckInputValidation(int& prevX_coord, int& prevY_coord,
	int& x_coord, int& y_coord)
{
	while (std::cin.fail() 
		|| gameMap.GetCell(prevX_coord - 1, prevY_coord - 1) != DefineMove()
		|| !gameMap.EmptyCheck(x_coord - 1, y_coord - 1))
	{
		system("cls");
		std::cin.clear();
		std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		if (gameMap.GetCell(prevX_coord - 1, prevY_coord - 1) != DefineMove())
		{
			TextModeOutput();
			std::cout << "An error has occuried" << '\n'
				<< "Введите заново предыдущую строку и столбец элемента" << '\n';
			std::cin >> prevX_coord >> prevY_coord;
		}

		if (!gameMap.EmptyCheck(x_coord - 1, y_coord - 1))
		{
			TextModeOutput();
			std::cout << "An error has occuried" << '\n'
				<< "Введите заново строку и столбец в перемещяемый элемент" << '\n';
			std::cin >> x_coord >> y_coord;
		}
	}
}

int gameController::EvaluationFunc()
{
	int evaluation = playersScore[1] - playersScore[0];

	if (playersScore[1] == 10)
		evaluation += 100;

	if (playersScore[0] == 10)
		evaluation -= 100;

	return evaluation;
}
