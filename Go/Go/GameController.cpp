#include "GameController.h"
#include "Point.h"
#include <iostream>
#include <conio.h>

gameController::gameController(const unsigned int& x, const unsigned int& y):
	width(x), height(y), gameMap(width, height), complete(false), opponentFirstMove(false)
{
	if (opponentFirstMove)
	{
		aiStone = 'w';
		playerStone = 'b';
	}
	else
	{
		aiStone = 'w';
		playerStone = 'b';
	}
}

void gameController::PlayerMove()
{
	while (true)
	{
		system("cls");
		TextModeOutput();

		if (opponentFirstMove)
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

		if (!opponentFirstMove)
		{
			DebugOutputMoveAnalyzer(x - 1, y - 1, true);
		}
		//opponentFirstMove = !opponentFirstMove;
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

		if (!opponentFirstMove)
		{
			DebugOutputMoveAnalyzer(x - 1, y - 1, true);
		}
		//opponentFirstMove = !opponentFirstMove;
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
			if (opponentFirstMove && !FindHistoryComb(beg, end))
			{
				playersScore[0]++; playersHistComb[0].PushForward(beg, end);
			}	
			else if (!FindHistoryComb(beg, end))
			{
				playersScore[1]++; playersHistComb[1].PushForward(beg, end);
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
			if (opponentFirstMove && !FindHistoryComb(beg, end))
			{
				playersScore[0]++; playersHistComb[0].PushForward(beg, end);
			}
			else if (!FindHistoryComb(beg, end))
			{
				playersScore[1]++; playersHistComb[1].PushForward(beg, end);
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
			if (opponentFirstMove && !FindHistoryComb(beg, end))
			{
				playersScore[0]++; playersHistComb[0].PushForward(beg, end);
			}
			else if (!FindHistoryComb(beg, end))
			{
				playersScore[1]++; playersHistComb[1].PushForward(beg, end);
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
			if (opponentFirstMove && !FindHistoryComb(beg, end))
			{
				playersScore[0]++; playersHistComb[0].PushForward(beg, end);
			}
			else if (!FindHistoryComb(beg, end))
			{
				playersScore[1]++; playersHistComb[1].PushForward(beg, end);
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
	if (opponentFirstMove)
		for (int i = 0; i < playersHistComb[0].GetCountOfElements(); i++)
			if (playersHistComb[0][i][0] == beg && playersHistComb[0][i][1] == end)
				return true;
	else
		for (int i = 0; i < playersHistComb[1].GetCountOfElements(); i++)
			if (playersHistComb[1][i][0] == beg && playersHistComb[1][i][1] == end)
				return true;
	return false;
}

char gameController::DefineMove()
{
	if (opponentFirstMove)
		return 'b';
	return 'w';
}

void gameController::Restart()
{
	opponentFirstMove = true;

	gameMap.ResetField();
	
	for (auto i = 0; i < 2; i++)
	{
		usedStone[i] = 0; playersScore[i] = 0;
		playersHistComb[i].Clear();
	}	
}

void gameController::TextModeOutput()
{
	std::cout << "Очки " << playersScore[0] << " - Черные фишки, "
		<< playersScore[1] << " - белые фишки\n";
	playersHistComb[0].Print();
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

int gameController::CheckPattern(const std::string& position, const bool& ai, 
	const char& currStone)
{
	if (currStone == 'b')
	{
		int patternEval = 0;

		if (position.find("bbbbb") != std::string::npos) //score
			patternEval += 100000000;
		if (position.find("bbbbw") != std::string::npos 
			|| position.find("wbbbb") != std::string::npos
			|| position.find("b0bbb") != std::string::npos
			|| position.find("bb0bb") != std::string::npos
			|| position.find("bbb0b") != std::string::npos) //chong4
			patternEval += 1000000;
		if (position.find("0bbbb0") != std::string::npos) // hoi4
			patternEval += 10000000;
		if (position.find("bbbw") != std::string::npos
			|| position.find("wbbb") != std::string::npos// mian 3
			|| position.find("b0bbw") != std::string::npos
			|| position.find("bb0bw") != std::string::npos
			|| position.find("bbb0w") != std::string::npos
			|| position.find("w0bbb") != std::string::npos
			|| position.find("wb0bb") != std::string::npos
			|| position.find("wbb0b") != std::string::npos
			|| position.find("b00bb") != std::string::npos
			|| position.find("bb00b") != std::string::npos
			|| position.find("bbb00") != std::string::npos
			|| position.find("b0b0b") != std::string::npos
			|| position.find("b0bb") != std::string::npos
			|| position.find("bb0b") != std::string::npos)
			patternEval += 10000;
		if (position.find("0bbb0") != std::string::npos
			|| position.find("0b0bb0") != std::string::npos
			|| position.find("0bb0b0") != std::string::npos)
			patternEval += 100000; // huo3
		if (position.find("bbw") != std::string::npos
			|| position.find("wbb") != std::string::npos// mian2
			|| position.find("b0bw") != std::string::npos
			|| position.find("b00bw") != std::string::npos
			|| position.find("b000b") != std::string::npos
			|| position.find("w0b0b0w") != std::string::npos
			|| position.find("w0bb00w") != std::string::npos
			|| position.find("wb0b00") != std::string::npos
			|| position.find("wb00b0") != std::string::npos
			|| position.find("w00bb0w") != std::string::npos)
			patternEval += 100;
		if (position.find("0bb0") != std::string::npos
			|| position.find("0b00b0") != std::string::npos
			|| position.find("0b0b0") != std::string::npos)
			patternEval += 1000; // huo2
		if (position.find("b") != std::string::npos)
			patternEval += 10;
		if (DefineZeroStr(position))
			patternEval += 1;
		if (aiStone == currStone)
		{
			return patternEval;
		}
		return -patternEval;
	}
	else
	{
		int patternEval = 0;

		if (position.find("wwwww") != std::string::npos) //score
			patternEval += 100000000;
		if (position.find("wwwwb") != std::string::npos 
			|| position.find("bwwww") != std::string::npos 
			|| position.find("w0www") != std::string::npos
			|| position.find("ww0ww") != std::string::npos
			|| position.find("www0w") != std::string::npos) //chong4
			patternEval += 1000000;
		if (position.find("0wwww0") != std::string::npos) // hoi4
			patternEval += 10000000;
		if (position.find("wwwb") != std::string::npos 
			|| position.find("bwww") != std::string::npos // mian 3
			|| position.find("w0wwb") != std::string::npos 
			|| position.find("ww0wb") != std::string::npos 
			|| position.find("www0b") != std::string::npos
			|| position.find("b0www") != std::string::npos 
			|| position.find("bw0ww") != std::string::npos 
			|| position.find("bww0w") != std::string::npos
			|| position.find("w00ww") != std::string::npos
			|| position.find("ww00w") != std::string::npos 
			|| position.find("www00") != std::string::npos
			|| position.find("w0w0w") != std::string::npos
			|| position.find("w0ww") != std::string::npos 
			|| position.find("ww0w") != std::string::npos)
			patternEval += 10000;
		if (position.find("0www0") != std::string::npos
			|| position.find("0w0ww0") != std::string::npos
			|| position.find("0ww0w0") != std::string::npos)
			patternEval += 100000; // huo3
		if (position.find("wwb") != std::string::npos
			|| position.find("bww") != std::string::npos// mian2
			|| position.find("w0wb") != std::string::npos
			|| position.find("w00wb") != std::string::npos
			|| position.find("w000w") != std::string::npos
			|| position.find("b0w0w0b") != std::string::npos
			|| position.find("b0ww00b") != std::string::npos
			|| position.find("bw0w00") != std::string::npos
			|| position.find("bw00w0") != std::string::npos
			|| position.find("b00ww0b") != std::string::npos)
			patternEval += 100;
		if (position.find("0ww0") != std::string::npos
			|| position.find("0w00w0") != std::string::npos
			|| position.find("0w0w0") != std::string::npos)
			patternEval += 1000; // huo2
		if (position.find("w") != std::string::npos)
			patternEval += 10;
		if (DefineZeroStr(position))
			patternEval += 1;
		if (aiStone == currStone)
		{
			return patternEval;
		}
		return -patternEval;
	}
}

int gameController::Evaluation(const unsigned int x, const unsigned int y, const bool& ai)
{
	int evaluation = 0;
	std::string position;

	for (int i = 0, j = y; i < width; i++) 
		position += gameMap.GetCell(i, j);
	evaluation += CheckPattern(position, ai, aiStone);
	evaluation += CheckPattern(position, false, playerStone);
	position.clear();

	for (int i = x, j = 0; j < width; j++)
		position += gameMap.GetCell(i, j);
	evaluation += CheckPattern(position, ai, aiStone);
	evaluation += CheckPattern(position, false, playerStone);
	position.clear();

	if (x + y < width) 
	{ 
		for (int i = 0, j = x + y; i < width && j >= 0; i++, j--)
			position += gameMap.GetCell(i, j);
	}
	else 
	{
		for (int i = x + y - 7, j = width - 1; i < width && j >= 0; i++, j--) 
			position += gameMap.GetCell(i, j);
	}
	evaluation += CheckPattern(position, ai, aiStone);
	evaluation += CheckPattern(position, false, playerStone);
	position.clear();

	if (x <= y) 
	{
		for (int i = 0, j = y - x; i < width && j < width; i++, j++)
			position += gameMap.GetCell(i, j);
	}
	else
	{
		for (int i = x - y, j = 0; i < width && j < width; i++, j++)
			position += gameMap.GetCell(i, j);
	}
	evaluation += CheckPattern(position, ai, aiStone);
	evaluation += CheckPattern(position, false, playerStone);
	position.clear();
	std::cout << evaluation << '\n';
	return evaluation;
}

void gameController::DebugOutputMoveAnalyzer(const unsigned int x, 
	const unsigned int y, const bool& ai)
{
	if (opponentFirstMove)
	{
		std::cout << "Information about Player unknown" << '\n';
		system("pause");
	}
	else
	{
		int eval = 0;
		for (auto i = 0; i < width; i++)
		{
			for (auto j = 0; j < width; j++)
			{
				if (gameMap.GetCell(i, j) != '0')
				{
					eval += Evaluation(i, j, ai);
				}
			}
		}
		std::cout << eval << '\n';
		system("pause");
	}
}

bool gameController::DefineZeroStr(const std::string& string)
{
	for (auto i = 0; i < string.size(); i++)
		if (string[i] != '0')
			return false;
	return true;
}
