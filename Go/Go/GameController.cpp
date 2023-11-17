#include "GameController.h"
#include "Point.h"
#include <iostream>
#include <conio.h>

gameController::gameController(const unsigned int& x, const unsigned int& y):
	width(x), height(y), gameMap(width, width), complete(false), opponentFirstMove(true)
{
	if (opponentFirstMove)
	{
		aiStone = 'w';
		playerStone = 'b';
	}
	else
	{
		aiStone = 'b';
		playerStone = 'w';
	}
}

gameController::~gameController()
{
	delete[] usedStone;
	delete[] playersScore;
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

		//DebugOutputMoveAnalyzer();
		//opponentFirstMove = !opponentFirstMove;
	}
	else
	{
		std::cout << "Введите предыдущюю строку и столбец" << '\n';
		std::cin >> prevX_coord >> prevY_coord;
		CheckInputValidation(prevX_coord, prevY_coord, 
			"Введите предыдущюю строку и столбец");

		system("cls");
		TextModeOutput();
		std::cout << "Введите строку и столбец" << '\n';
		std::cin >> x >> y;
		CheckInputValidation(prevX_coord, prevY_coord, x, y);

		gameMap.MoveStone(prevX_coord - 1, prevY_coord - 1, x - 1, y - 1);
		Update();

		DebugOutputMoveAnalyzer();
		opponentFirstMove = !opponentFirstMove;
	}
}

int gameController::Update()
{
	// Проверка по горизонтали
	for (int i = 0; i < width; ++i)
	{
		line dots;
		for (int j = 0; j < width - 4; ++j)
		{
			bool sequence = true;
			for (int k = 0; k < 5; ++k)
			{
				if (gameMap.GetCell(i,j + k) != DefineMove())
				{
					sequence = false;
					break;
				}
				dots.dots[k].SetPoint(i, j + k);
			}

			if (opponentFirstMove && sequence &&
				!FindHistoryComb(dots, horizontal))
			{
				playersScore[0]++;
				playersHistComb[0].GetList(horizontal).PushForward(dots);
			}
			else if (sequence && !FindHistoryComb(dots, horizontal))
			{
				playersScore[1]++;
				playersHistComb[1].GetList(horizontal).PushForward(dots);
			}
		}
	}
	//for (int i = 0; i < width; i++) // по строке
	//{
	//	int lenRow = 0; point beg, end;
	//	bool begOfSeries = true;
	//	for (int j = 0; j < height; j++)
	//	{
	//		if (gameMap.GetCell(i, j) == DefineMove())
	//		{
	//			if (begOfSeries)
	//			{
	//				beg.SetPoint(i, j);
	//				begOfSeries = false;
	//			}
	//			lenRow++;
	//			end.SetPoint(i, j);
	//		}	
	//		else
	//		{
	//			lenRow = 0; begOfSeries = true;
	//			beg.SetPoint(0, 0); end.SetPoint(0, 0);
	//		}
	//		if (lenRow == 5)
	//		{
	//			if (opponentFirstMove && !FindHistoryComb(beg, end))
	//			{
	//				playersScore[0]++; playersHistComb[0].PushForward(beg, end);
	//			}
	//			else if (!FindHistoryComb(beg, end))
	//			{
	//				playersScore[1]++; playersHistComb[1].PushForward(beg, end);
	//			}
	//		}
	//	}
	//}
	
	// Проверка по вертикали
	for (int i = 0; i < width - 4; ++i)
	{
		line dots;
		for (int j = 0; j < width; ++j)
		{
			bool sequence = true;

			for (int k = 0; k < 5; ++k)
			{
				if (gameMap.GetCell(i + k, j) != DefineMove())
				{
					sequence = false;
					break;
				}
				dots.dots[k].SetPoint(i + k, j);
			}

			if (opponentFirstMove && sequence &&
				!FindHistoryComb(dots, vertical))
			{
				playersScore[0]++;
				playersHistComb[0].GetList(vertical).PushForward(dots);
			}
			else if (sequence && !FindHistoryComb(dots, vertical))
			{
				playersScore[1]++;
				playersHistComb[1].GetList(vertical).PushForward(dots);
			}
		}
	}

	//for (int j = 0; j < width; j++) // По столбцу
	//{
	//	int lenColumn = 0; point beg, end;
	//	bool begOfSeries = true;
	//	for (int i = 0; i < height; i++)
	//	{
	//		if (gameMap.GetCell(i, j) == DefineMove())
	//		{
	//			if (begOfSeries)
	//			{
	//				beg.SetPoint(i, j);
	//				begOfSeries = false;
	//			}
	//			lenColumn++;
	//			end.SetPoint(i, j);
	//		}
	//		else
	//		{
	//			begOfSeries = true; lenColumn = 0;
	//			beg.SetPoint(0, 0); end.SetPoint(0, 0);
	//		}
	//		
	//		if (lenColumn == 5)
	//		{
	//			if (opponentFirstMove && !FindHistoryComb(beg, end))
	//			{
	//				playersScore[0]++; playersHistComb[0].PushForward(beg, end);
	//			}
	//			else if (!FindHistoryComb(beg, end))
	//			{
	//				playersScore[1]++; playersHistComb[1].PushForward(beg, end);
	//			}
	//		}
	//	}
	//}

	// Проверка по диагонали (слева направо)
	for (int i = 0; i < width - 4; ++i)
	{
		line dots;
		for (int j = 0; j < width - 4; ++j)
		{
			bool sequence = true;
			for (int k = 0; k < 5; ++k)
			{
				if (gameMap.GetCell(i + k, j + k) != DefineMove())
				{
					sequence = false;
					break;
				}
				dots.dots[k].SetPoint(i + k, j + k);
			}

			if (opponentFirstMove && sequence &&
				!FindHistoryComb(dots, mainDiagonal))
			{
				playersScore[0]++;
				playersHistComb[0].GetList(mainDiagonal).PushForward(dots);
			}
			else if (sequence && !FindHistoryComb(dots, mainDiagonal))
			{
				playersScore[1]++;
				playersHistComb[1].GetList(mainDiagonal).PushForward(dots);
			}
		}
	}

	//for (int i = 0, lenDiagonal = 0; i < width; i++) // Главная диагональ
	//{
	//	point beg, end;
	//	bool begOfSeries = true;
	//	if (gameMap.GetCell(i, i) == DefineMove())
	//	{
	//		if (begOfSeries)
	//		{
	//			beg.SetPoint(i, i);
	//			begOfSeries = false;
	//		}
	//		lenDiagonal++;
	//		end.SetPoint(i, i);
	//	}
	//	else
	//	{
	//		lenDiagonal = 0; begOfSeries = true;
	//		beg.SetPoint(0, 0); end.SetPoint(0, 0);
	//	}

	//	if (lenDiagonal == 5)
	//	{
	//		if (opponentFirstMove && !FindHistoryComb(beg, end))
	//		{
	//			playersScore[0]++; playersHistComb[0].PushForward(beg, end);
	//		}
	//		else if (!FindHistoryComb(beg, end))
	//		{
	//			playersScore[1]++; playersHistComb[1].PushForward(beg, end);
	//		}
	//	}
	//}
	//std::string position;
	//for (auto x = 0; x < width; x++)
	//{
	//	for (auto y = 0; y < width; y++)
	//	{
	//		point beg(0, 0), end(0, 0); bool begOfSeries = true; int lenDiagonal = 0;
	//		if (x <= y) // проверка главной диагонали
	//		{
	//			beg.SetPoint(0, y - x);
	//			for (int i = 0, j = y - x; i < width && j < width;
	//				i++, j++)
	//			{
	//				if (gameMap.GetCell(i, j) == DefineMove())
	//				{
	//					if (begOfSeries)
	//					{
	//						beg.SetPoint(i, j);
	//						begOfSeries = false;
	//					}
	//					position += gameMap.GetCell(i, j);
	//					lenDiagonal++;
	//					end.SetPoint(i, j);
	//					if (lenDiagonal == 5)
	//					{
	//						if (opponentFirstMove && !FindHistoryComb(beg, end))
	//						{
	//							playersScore[0]++; playersHistComb[0].PushForward(beg, end);
	//							position.clear(); lenDiagonal = 0;
	//							beg.SetPoint(i, j); end.SetPoint(i, j);
	//						}
	//						else if (!FindHistoryComb(beg, end))
	//						{
	//							playersScore[1]++; playersHistComb[1].PushForward(beg, end);
	//							position.clear(); lenDiagonal = 0;
	//							beg.SetPoint(i, j); end.SetPoint(i, j);
	//						}
	//					}
	//				}
	//				else
	//				{
	//					lenDiagonal = 0; begOfSeries = true;
	//					beg.SetPoint(i, j); end.SetPoint(i, j);
	//				}
	//				/*position += gameMap.GetCell(i, j);
	//				end.SetPoint(i, j);*/
	//			}
	//		}
	//		else
	//		{
	//			beg.SetPoint(x - y, 0);
	//			for (int i = x - y, j = 0; i < width && j < width;
	//				i++, j++)
	//			{
	//				if (gameMap.GetCell(i, j) == DefineMove())
	//				{
	//					if (begOfSeries)
	//					{
	//						beg.SetPoint(i, j);
	//						begOfSeries = false;
	//					}
	//					position += gameMap.GetCell(i, j);
	//					end.SetPoint(i, j);
	//				}
	//				/*position += gameMap.GetCell(i, j);
	//				end.SetPoint(i, j);*/
	//			}
	//		}
	//		/*if (CheckPattern(position,GamePattern::LineFive,DefineMove()))
	//			if (opponentFirstMove && !FindHistoryComb(beg, end))
	//			{
	//				playersScore[0]++; playersHistComb[0].PushForward(beg, end);
	//			}
	//			else if (!FindHistoryComb(beg, end))
	//			{
	//				playersScore[1]++; playersHistComb[1].PushForward(beg, end);
	//			}
	//		position.clear();*/
	//	}
	//}

	// Проверка по диагонали (справа налево)
    for (int i = 4; i < width; ++i)
    {
        line dots;
        for (int j = 0; j < width - 4; ++j)
        {
            bool sequence = true;
            for (int k = 0; k < 5; ++k) 
            {
                if (gameMap.GetCell(i - k, j + k) != DefineMove())
                {
                    sequence = false;
                    break;
                }
                dots.dots[k].SetPoint(i - k, j + k);
            }
            
			if (opponentFirstMove && sequence &&
				!FindHistoryComb(dots, sideDiagonal))
			{
				playersScore[0]++;
				playersHistComb[0].GetList(sideDiagonal).PushForward(dots);
			}
			else if (sequence && !FindHistoryComb(dots, sideDiagonal))
			{
				playersScore[1]++;
				playersHistComb[1].GetList(sideDiagonal).PushForward(dots);
			}
        }
    }

	//for (int i = width - 1, lenSideDiagonal = 0; i >= 0; i--) // Побочная диагональ
	//{
	//	point beg, end;
	//	bool begOfSeries = true;
	//	if (gameMap.GetCell(i, (width - 1) - i) == DefineMove())
	//	{
	//		if (begOfSeries)
	//		{
	//			beg.SetPoint(i, (width - 1) - i);
	//			begOfSeries = false;
	//		}
	//		lenSideDiagonal++;
	//		end.SetPoint(i, (width - 1) - i);
	//	}
	//	else
	//	{
	//		lenSideDiagonal = 0; begOfSeries = true;
	//		beg.SetPoint(0, 0); end.SetPoint(0, 0);
	//	}

	//	if (lenSideDiagonal == 5)
	//	{
	//		if (opponentFirstMove && !FindHistoryComb(beg, end))
	//		{
	//			playersScore[0]++; playersHistComb[0].PushForward(beg, end);
	//		}
	//		else if (!FindHistoryComb(beg, end))
	//		{
	//			playersScore[1]++; playersHistComb[1].PushForward(beg, end);
	//		}
	//	}
	//}

	for (int i = 0; i < 2; i++)
		if (playersScore[i] == 10)
			complete = true;
	return 0;
}

//bool gameController::FindHistoryComb(const point& beg, const point& end)
//{
//	if (opponentFirstMove)
//		for (int i = 0; i < playersHistComb[0].GetCountOfElements(); i++)
//			if (playersHistComb[0][i][0] == beg && playersHistComb[0][i][1] == end)
//				return true;
//	else
//		for (int i = 0; i < playersHistComb[1].GetCountOfElements(); i++)
//			if (playersHistComb[1][i][0] == beg && playersHistComb[1][i][1] == end)
//				return true;
//	return false;
//}

bool gameController::FindHistoryComb(const line& comb, ListOrient orient)
{
	if (opponentFirstMove)
	{
		for (int i = 0; i < playersHistComb[0].GetList(orient).GetCountOfElements(); i++)
			for (auto j = 0; j < 5; j++)
				if (*playersHistComb[0].GetList(orient)[i] == comb)
					return true;
	}
	else
	{
		for (int i = 0; i < playersHistComb[1].GetList(orient).GetCountOfElements(); i++)
			for (auto j = 0; j < 5; j++)
				if (*playersHistComb[1].GetList(orient)[i] == comb)
					return true;
	}
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
	playersHistComb[0].GetList(mainDiagonal).Print();
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
		std::cout << i + 1 << '\n';
	}

	std::cout << "  ";
	for (int i = 1; i <= height; i++)
	{
		std::cout << i << ' ';
	}
	std::cout << '\n';
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

void gameController::CheckInputValidation(int& x, int& y, std::string suggestion)
{
	while (std::cin.fail() || !gameMap.GetCell(x - 1, y - 1))
	{
		system("cls");
		std::cin.clear();
		std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		TextModeOutput();
		std::cout << "An error has occuried" << '\n'
			<< suggestion << '\n';
		std::cin >> x >> y;
	}
}

listPositions& gameController::FindMovePos(const unsigned int& prevX, 
	const unsigned int& prevY)
{
	listPositions pos;

	for (auto i = -1; i < 2; i++)
	{
		if (gameMap.EmptyCheck(prevX + i, prevY))
			pos.PushForward(prevX + i, prevY);
		if (gameMap.EmptyCheck(prevX, prevY + i))
			pos.PushForward(prevX, prevY + i);
		if (gameMap.EmptyCheck(prevX + i, prevY + i))
			pos.PushForward(prevX + i, prevY + i);
		if (gameMap.EmptyCheck(prevX - i, prevY - i))
			pos.PushForward(prevX - i, prevY - i);
	}
	return pos;
}

bool gameController::CheckCorrectMove(const int& prevX, const int& prevY,
	const int& x, const int& y)
{
	for (int i = -1; i < 2; i++)
	{
		if (gameMap.EmptyCheck(prevX + i, prevY) && prevX + i == x
			&& prevY == y)
			return true;
		if (gameMap.EmptyCheck(prevX, prevY + i) && prevX == x
			&& prevY + i == y)
			return true;
		if (gameMap.EmptyCheck(prevX + i, prevY + i) && prevX + i == x
			&& prevY + i == y)
			return true;
		if (gameMap.EmptyCheck(prevX - i, prevY - i) && prevX - i == x
			&& prevY - i == y)
			return true;
	}
	return false;
}

void gameController::CheckInputValidation(int& prevX, int& prevY,
	int& x, int& y)
{
	/*while (std::cin.fail()
		|| gameMap.GetCell(prevX - 1, prevY - 1) != DefineMove())
	{
		x = 0, y = 0;
		system("cls");
		std::cin.clear();
		std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		TextModeOutput();
		std::cout << "An error has occuried" << '\n'
			<< "Введите заново предыдущую строку и столбец элемента" << '\n';
		std::cin >> prevX >> prevY;
	}*/

	bool noValid = false;
	while (std::cin.fail()
		|| !gameMap.EmptyCheck(x - 1, y - 1)
		|| (noValid = //!CheckCorrectMove(prevX, prevY, x, y) || 
			gameMap.GetCell(prevX - 1, prevY - 1) != DefineMove()))
	{
		system("cls");
		std::cin.clear();
		std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		TextModeOutput();
		if (noValid)
		{
			std::cout << "An error has occuried" << '\n'
				<< "Введите заново предыдущую строку и столбец элемента" << '\n';
			std::cin >> prevX >> prevY;
		}
		std::cout << "An error has occuried" << '\n'
			<< "Введите заново строку и столбец" << '\n';
		std::cin >> x >> y;
	}

	/*while (std::cin.fail() 
		|| gameMap.GetCell(prevX - 1, prevY - 1) != DefineMove()
		|| !gameMap.EmptyCheck(x - 1, y - 1))
	{
		system("cls");
		std::cin.clear();
		std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
		if (gameMap.GetCell(prevX - 1, prevY - 1) != DefineMove())
		{
			TextModeOutput();
			std::cout << "An error has occuried" << '\n'
				<< "Введите заново предыдущую строку и столбец элемента" << '\n';
			std::cin >> prevX >> prevY;
		}

		if (!gameMap.EmptyCheck(x - 1, y - 1))
		{
			TextModeOutput();
			std::cout << "An error has occuried" << '\n'
				<< "Введите заново строку и столбец в перемещяемый элемент" << '\n';
			std::cin >> x >> y;
		}
	}*/
}

/*
* Метод распознавания паттернов ГО-БАНА
*/
int gameController::CheckPattern(const std::string& position,
	const char& currStone)
{
	if (currStone == 'b')
	{
		int patternEval = 0;
		// посследовательность из пяти
		if (position.find("bbbbb") != std::string::npos)
			patternEval += 100000000;
		// Ударная четыре
		if (position.find("bbbbw") != std::string::npos
			|| position.find("wbbbb") != std::string::npos
			|| position.find("b0bbb") != std::string::npos
			|| position.find("bb0bb") != std::string::npos
			|| position.find("bbb0b") != std::string::npos)
			patternEval += 1000000;
		// Живая четверка
		if (position.find("0bbbb0") != std::string::npos)
			patternEval += 10000000;
		// Спящая тройка
		if (position.find("bbbw") != std::string::npos
			|| position.find("wbbb") != std::string::npos 
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
		// живая тройка
		if (position.find("0bbb0") != std::string::npos
			|| position.find("0b0bb0") != std::string::npos
			|| position.find("0bb0b0") != std::string::npos)
			patternEval += 100000;
		// спящая двойка
		if (position.find("bbw") != std::string::npos
			|| position.find("wbb") != std::string::npos
			|| position.find("b0bw") != std::string::npos
			|| position.find("b00bw") != std::string::npos
			|| position.find("b000b") != std::string::npos
			|| position.find("w0b0b0w") != std::string::npos
			|| position.find("w0bb00w") != std::string::npos
			|| position.find("wb0b00") != std::string::npos
			|| position.find("wb00b0") != std::string::npos
			|| position.find("w00bb0w") != std::string::npos)
			patternEval += 100;
		// живая двойка
		if (position.find("0bb0") != std::string::npos
			|| position.find("0b00b0") != std::string::npos
			|| position.find("0b0b0") != std::string::npos)
			patternEval += 1000;
		// один камень
		if (position.find("b") != std::string::npos)						
			patternEval += 10;
		if (DefineZeroStr(position)) // нет элементов
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
		// посследовательность из пяти
		if (position.find("wwwww") != std::string::npos)
			patternEval += 100000000;
		// Ударная четыре
		if (position.find("wwwwb") != std::string::npos
			|| position.find("bwwww") != std::string::npos
			|| position.find("w0www") != std::string::npos
			|| position.find("ww0ww") != std::string::npos
			|| position.find("www0w") != std::string::npos)
			patternEval += 1000000;
		// Живая четверка
		if (position.find("0wwww0") != std::string::npos)
			patternEval += 10000000;
		// Спящая тройка
		if (position.find("wwwb") != std::string::npos
			|| position.find("bwww") != std::string::npos
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
		// живая тройка
		if (position.find("0www0") != std::string::npos
			|| position.find("0w0ww0") != std::string::npos
			|| position.find("0ww0w0") != std::string::npos)
			patternEval += 100000;
		// спящая двойка
		if (position.find("wwb") != std::string::npos
			|| position.find("bww") != std::string::npos
			|| position.find("w0wb") != std::string::npos
			|| position.find("w00wb") != std::string::npos
			|| position.find("w000w") != std::string::npos
			|| position.find("b0w0w0b") != std::string::npos
			|| position.find("b0ww00b") != std::string::npos
			|| position.find("bw0w00") != std::string::npos
			|| position.find("bw00w0") != std::string::npos
			|| position.find("b00ww0b") != std::string::npos)
			patternEval += 100;
		// живая двойка
		if (position.find("0ww0") != std::string::npos 
			|| position.find("0w00w0") != std::string::npos
			|| position.find("0w0w0") != std::string::npos)
			patternEval += 1000;
		// один камень
		if (position.find("w") != std::string::npos)
			patternEval += 10;
		if (DefineZeroStr(position))	// нет элементов
			patternEval += 1;
		if (aiStone == currStone)
		{
			return patternEval;
		}
		return -patternEval;
	}
}
/*
* Метод определяющий оценку для ИИ на поле
*/
int gameController::Evaluation(const unsigned int x,
	const unsigned int y)
{
	int evaluation = 0;
	std::string position;

	for (int i = 0, j = y; i < width; i++) // проверка столбцов
		position += gameMap.GetCell(i, j);
	evaluation += CheckPattern(position, aiStone);
	evaluation += CheckPattern(position, playerStone);
	position.clear();

	for (int i = x, j = 0; j < width; j++) // проверка строк
		position += gameMap.GetCell(i, j);
	evaluation += CheckPattern(position, aiStone);
	evaluation += CheckPattern(position, playerStone);
	position.clear();

	if (x + y < width) // проверка побочной диагонали
	{
		for (int i = 0, j = x + y; i < width && j >= 0;
			i++, j--)
			position += gameMap.GetCell(i, j);
	}
	else
	{
		for (int i = x + y - 7, j = width - 1;
			i < width && j >= 0; i++, j--)
			position += gameMap.GetCell(i, j);
	}
	evaluation += CheckPattern(position, aiStone);
	evaluation += CheckPattern(position, playerStone);
	position.clear();

	if (x <= y) // проверка главной диагонали
	{
		for (int i = 0, j = y - x; i < width && j < width;
			i++, j++)
			position += gameMap.GetCell(i, j);
	}
	else
	{
		for (int i = x - y, j = 0; i < width && j < width;
			i++, j++)
			position += gameMap.GetCell(i, j);
	}
	evaluation += CheckPattern(position, aiStone);
	evaluation += CheckPattern(position, playerStone);
	position.clear();

	return evaluation;
}


void gameController::DebugOutputMoveAnalyzer()
{
	int eval = 0;
	for (auto i = 0; i < width; i++)
	{
		for (auto j = 0; j < width; j++)
		{
			eval += Evaluation(i, j);
		}
	}
	std::cout << eval << '\n';
	system("pause");
}

bool gameController::DefineZeroStr(const std::string& string)
{
	for (auto i = 0; i < string.size(); i++)
		if (string[i] != '0')
			return false;
	return true;
}

bool gameController::CheckPattern(const std::string& position, const GamePattern pattern,
	const char& stone)
{
	switch (pattern)
	{
	case GamePattern::LineFive:
		if (stone == 'b')
		{
			if (position.find("bbbbb") != std::string::npos)
				return true;
			return false;
		}
		else
		{
			if (position.find("wwwww") != std::string::npos)
				return true;
			return false;
		}
		break;
	case GamePattern::LiveFour:
		break;
	case GamePattern::StrikingFour:
		break;
	case GamePattern::LiveThree:
		break;
	case GamePattern::SleepingThree:
		break;
	case GamePattern::LiveTwo:
		break;
	case GamePattern::SleepingTwo:
		break;
	case GamePattern::OneStone:
		break;
	case GamePattern::NoStone:
		break;
	default:
		break;
	}
}

//bool gameController::IsScoreLine(const unsigned int& x, const unsigned int& y)
//{
//	std::string position;
//
//	for (int i = 0, j = y; i < width; i++) // проверка столбцов
//		position += gameMap.GetCell(i, j);
//	if (CheckPattern(position, GamePattern::LineFive, 'b'))
//		return true;
//	else if (CheckPattern(position, GamePattern::LineFive, 'w'))
//		return true;
//	position.clear();
//
//	for (int i = x, j = 0; j < width; j++) // проверка строк
//		position += gameMap.GetCell(i, j);
//	position.clear();
//
//	if (x + y < width) // проверка побочной диагонали
//	{
//		for (int i = 0, j = x + y; i < width && j >= 0;
//			i++, j--)
//			position += gameMap.GetCell(i, j);
//	}
//	else
//	{
//		for (int i = x + y - 7, j = width - 1;
//			i < width && j >= 0; i++, j--)
//			position += gameMap.GetCell(i, j);
//	}
//	if (CheckPattern(position, GamePattern::LineFive, 'b'))
//		return true;
//	else if (CheckPattern(position, GamePattern::LineFive, 'w'))
//		return true;
//	position.clear();
//
//	if (x <= y) // проверка главной диагонали
//	{
//		for (int i = 0, j = y - x; i < width && j < width;
//			i++, j++)
//			position += gameMap.GetCell(i, j);
//	}
//	else
//	{
//		for (int i = x - y, j = 0; i < width && j < width;
//			i++, j++)
//			position += gameMap.GetCell(i, j);
//	}
//	if (CheckPattern(position, GamePattern::LineFive, 'b'))
//		return true;
//	else if (CheckPattern(position, GamePattern::LineFive, 'w'))
//		return true;
//	position.clear();
//}

bool gameController::FindPrevPos(minMaxNode* node)
{
	if (node)
	{
		if (node->prev)
		{
			minMaxNode* curr = node->prev;
			while (curr->prev)
			{
				if (node->currentPos == curr->currentPos)
					return true;
				curr = curr->prev;
			}

			if (node->currentPos == curr->currentPos)
				return true;
		}
	}
	return false;
}

int gameController::MiniMax(minMaxNode* node, const unsigned int depth)
{
	int evaluation = 0;
	if (complete && node->currTurn == 'a')
		return 1 << 31;
	else if (complete)
		return ~(1 << 31);

	if (depth == 0)
		return Evaluation(node->currentPos.GetX(), 
			node->currentPos.GetY());

	if (FindPrevPos(node))
		return Evaluation(node->currentPos.GetX(),
			node->currentPos.GetY());
	
	if (node->currTurn == 'a')
	{
		evaluation = 1 << 31;
		for (auto i = 0; i < width; i++)
		{
			for (auto j = 0; j < width; j++)
			{
				if (usedStone[DefineIterator(false)] == 12
					&& gameMap.EmptyCheck(i, j))
				{
					MoveStageMinMax(node, i, j, evaluation, depth);
				}
				else if (gameMap.EmptyCheck(i, j))
				{
					gameMap.SetStone(i, j, aiStone);
					minMaxNode* nextNode = new minMaxNode(node, i, j, 'p');
					usedStone[DefineIterator(true)]++;
					Update();
					evaluation = std::max(MiniMax(nextNode, depth - 1), 
						evaluation);
					gameMap.SetStone(i, j, ' ');
					usedStone[DefineIterator(true)]--;
				}
			}
		}
	}
	else
	{
		evaluation = ~(1 << 31);
		for (auto i = 0; i < width; i++)
		{
			for (auto j = 0; j < width; j++)
			{
				if (usedStone[DefineIterator(false)] == 12
					&& gameMap.GetCell(i, j) == playerStone)
				{
					MoveStageMinMax(node, i, j, evaluation, depth);
				}
				else if (gameMap.EmptyCheck(i, j))
				{
					gameMap.SetStone(i, j, playerStone);
					minMaxNode* nextNode = new minMaxNode(node, i, j, 'a');
					usedStone[DefineIterator(false)]++;
					Update();
					evaluation = std::min(
						MiniMax(nextNode, depth - 1),
						evaluation);
					gameMap.SetStone(i, j, ' ');
					usedStone[DefineIterator(false)]--;
				}
			}
		}
	}
	return evaluation;
}

int gameController::MoveStageMinMax(minMaxNode* node, const int& prevX, const int& prevY,
	int& evaluation, const unsigned int depth)
{
	for (auto k = -1; k < 2; k++)
	{
		if (gameMap.GetCell(prevX + k, prevY) == node->currTurn)
		{
			gameMap.MoveStone(prevX, prevY,
				prevX + k, prevY);
			minMaxNode* nextNode = new minMaxNode(node, prevX + k, prevY, 
				DefineNextMove(node));

			evaluation = std::max(
				MiniMax(nextNode, depth - 1),
				evaluation);
			gameMap.MoveStone(prevX + k, prevY,
				prevX, prevY);
		}
		if (gameMap.GetCell(prevX, prevY + k) == node->currTurn)
		{
			gameMap.MoveStone(prevX, prevY,
				prevX, prevY + k);
			minMaxNode* nextNode = new minMaxNode(node, prevX, prevY + k,
				DefineNextMove(node));
			
			evaluation = std::max(
				MiniMax(nextNode, depth - 1),
				evaluation);
			gameMap.MoveStone(prevX, prevY + k,
				prevX, prevY);
		}
		if (gameMap.GetCell(prevX + k, prevY + k) == node->currTurn)
		{
			gameMap.MoveStone(prevX, prevY,
				prevX + k, prevY + k);
			minMaxNode* nextNode = new minMaxNode(node, prevX + k, prevY + k,
				DefineNextMove(node));

			evaluation = std::max(
				MiniMax(nextNode, depth - 1),
				evaluation);
			gameMap.MoveStone(prevX + k, prevY + k,
				prevX, prevY);
		}
		if (gameMap.GetCell(prevX - k, prevY - k) == node->currTurn)
		{
			gameMap.MoveStone(prevX, prevY,
				prevX - k, prevY - k);
			minMaxNode* nextNode = new minMaxNode(node, prevX - k, prevY - k,
				DefineNextMove(node));

			evaluation = std::max(
				MiniMax(nextNode, depth - 1),
				evaluation);
			gameMap.MoveStone(prevX - k, prevY - k,
				prevX, prevY);
		}
	}
	return evaluation;
}

unsigned int gameController::DefineIterator(const bool defineAI)
{
	if (defineAI)
	{
		if (aiStone = 'w')
			return 1;
		return 0;
	}
	else
	{
		if (playerStone = 'w')
			return 1;
		return 0;
	}
}

char gameController::DefineNextMove(minMaxNode* node)
{
	if (node->currTurn == 'a')
		return 'p';
	return 'a';
}