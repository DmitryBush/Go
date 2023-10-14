#include "GameController.h"
#include "Point.h"
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
		Update();

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
		Update();

		blackMove = !blackMove;
	}
}

int gameController::Update()
{
	for (int i = 0; i < width; i++) // по строке
	{
		int lenRow = 0; point beg, end;
		bool begOfSeries = true;
		for (int j = 0; j < height; j++)
		{
			if (gameMap.GetCell(i, j) == DefineMove())
			{
				if (begOfSeries)
				{
					beg.SetPoint(i, j);
					begOfSeries = false;
				}
				lenRow++;
				end.SetPoint(i, j);
			}	
			else
			{
				lenRow = 0; begOfSeries = true;
				beg.SetPoint(0, 0); end.SetPoint(0, 0);
			}	
		}

		if (lenRow == 5)
		{
			if (blackMove && !FindHistoryComb(beg, end))
			{
				playersScore[0]++; playersHistory[0].PushForward(beg, end);
			}	
			else if (!FindHistoryComb(beg, end))
			{
				playersScore[1]++; playersHistory[1].PushForward(beg, end);
			}	
		}
	}

	for (int j = 0; j < width; j++) // По столбцу
	{
		int lenColumn = 0; point beg, end;
		bool begOfSeries = true;
		for (int i = 0; i < height; i++)
		{
			if (gameMap.GetCell(i, j) == DefineMove())
			{
				if (begOfSeries)
				{
					beg.SetPoint(i, j);
					begOfSeries = false;
				}
				lenColumn++;
				end.SetPoint(i, j);
			}
			else
			{
				begOfSeries = true; lenColumn = 0;
				beg.SetPoint(0, 0); end.SetPoint(0, 0);
			}
				
		}

		if (lenColumn == 5)
		{
			if (blackMove && !FindHistoryComb(beg, end))
			{
				playersScore[0]++; playersHistory[0].PushForward(beg, end);
			}
			else if (!FindHistoryComb(beg, end))
			{
				playersScore[1]++; playersHistory[1].PushForward(beg, end);
			}
		}
	}

	for (int i = 0, lenDiagonal = 0; i < width; i++) // Главная диагональ
	{
		point beg, end;
		bool begOfSeries = true;
		if (gameMap.GetCell(i, i) == DefineMove())
		{
			if (begOfSeries)
			{
				beg.SetPoint(i, i);
				begOfSeries = false;
			}
			lenDiagonal++;
			end.SetPoint(i, i);
		}
		else
		{
			lenDiagonal = 0; begOfSeries = true;
			beg.SetPoint(0, 0); end.SetPoint(0, 0);
		}

		if (lenDiagonal == 5)
		{
			if (blackMove && !FindHistoryComb(beg, end))
			{
				playersScore[0]++; playersHistory[0].PushForward(beg, end);
			}
			else if (!FindHistoryComb(beg, end))
			{
				playersScore[1]++; playersHistory[1].PushForward(beg, end);
			}
		}
	}

	for (int i = width - 1, lenSideDiagonal = 0; i >= 0; i--) // Побочная диагональ
	{
		point beg, end;
		bool begOfSeries = true;
		if (gameMap.GetCell(i, (width - 1) - i) == DefineMove())
		{
			if (begOfSeries)
			{
				beg.SetPoint(i, (width - 1) - i);
				begOfSeries = false;
			}
			lenSideDiagonal++;
			end.SetPoint(i, (width - 1) - i);
		}
		else
		{
			lenSideDiagonal = 0; begOfSeries = true;
			beg.SetPoint(0, 0); end.SetPoint(0, 0);
		}

		if (lenSideDiagonal == 5)
		{
			if (blackMove && !FindHistoryComb(beg, end))
			{
				playersScore[0]++; playersHistory[0].PushForward(beg, end);
			}
			else if (!FindHistoryComb(beg, end))
			{
				playersScore[1]++; playersHistory[1].PushForward(beg, end);
			}
		}
	}

	for (int i = 0; i < 2; i++)
		if (playersScore[i] == 10)
			complete = true;
	return 0;
}

bool gameController::FindHistoryComb(const point& beg, const point& end)
{
	if (blackMove)
		for (int i = 0; i < playersHistory[0].GetCountOfElements(); i++)
			if (playersHistory[0][i][0] == beg && playersHistory[0][i][1] == end)
				return true;
	else
		for (int i = 0; i < playersHistory[1].GetCountOfElements(); i++)
			if (playersHistory[1][i][0] == beg && playersHistory[1][i][1] == end)
				return true;
	return false;
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
	{
		usedStone[i] = 0; playersScore[i] = 0;
		playersHistory[i].Clear();
	}	
}

void gameController::TextModeOutput()
{
	std::cout << "Очки " << playersScore[0] << " - Черные фишки, "
		<< playersScore[1] << " - белые фишки\n";
	playersHistory[0].Print();
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

int gameController::EvaluationFunc(std::string position)
{
	int evaluation = playersScore[1] - playersScore[0];

	if (position == "0wwwww" || position == "wwwww0")
	{
		return evaluation + 100000000;
	}
	else if (position == "0www00" || position == "00www0")
	{
		return evaluation + 10000000;
	}
	else if (position == "0wwwwb" || position == "bwwww0")
	{
		return evaluation + 1000000;
	}
	else if (position == "00www0" || position == "www000" 
		|| position == "0www00" || position == "000www")
	{
		return evaluation + 100000;
	}
	else if (position == "00wwwb" || position == "wwwb00" || position == "0wwwb0"
		|| position == "0bwww0" || position == "bwww00")
	{
		return evaluation + 10000;
	}
	else if (position == "ww0000" || position == "0ww000" || position == "00ww00"
		|| position == "0000ww")
	{
		return evaluation + 1000;
	}
	else if (position == "wwb000" || position == "0wwb00" || position == "00wwb0"
		|| position == "000wwb" 
		|| position == "bww000" || position == "bww000" || position == "0bww00"
		|| position == "00bww0")
	{
		return evaluation + 100;
	}
	else if (position == "w0000" || position == "0w000" || position == "00w000"
		|| position == "000w00"
		|| position == "0000w0" || position == "00000w")
	{
		return evaluation + 10;
	}
	return evaluation + 1;
}