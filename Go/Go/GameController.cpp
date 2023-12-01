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
{}

/*
* Метод для определения хода (ИИ или игрока)
*/
void gameController::PlayerMove()
{
	while (!complete)
	{
		system("cls");
		TextModeOutput();

		if (opponentFirstMove)
		{
			if (aiStone == 'b')
			{
				int alpha = 1 << 31, beta = ~(1 << 31);
				listPositions pos;
				minMaxNode* root = nullptr;
				point** matrix = nullptr;
				try
				{
					root = new minMaxNode(nullptr, 0, 0, 'a');
					matrix = new point * [width];
					for (auto i = 0; i < width; i++)
					{
						matrix[i] = new point[width]{};
					}
				}
				catch (const std::exception&)
				{
					throw("bad allocation");
				}

				if (usedStone[DefineIterator(true)] >= 12)
				{
					int score = MoveAlphaBeta(root, alpha, beta, matrix, 32);
					std::cout << score << '\n';

					/*for (auto i = 0; i < width; i++)
					{
						for (auto j = 0; j < width; j++)
						{
							std::cout << matrix[i][j].GetX() << ' ';
						}
						std::cout << '\n';
					}
					system("pause");*/
					int* index = nullptr;
					for (auto i = 0; i < width; i++)
					{
						for (auto j = 0; j < width; j++)
						{
							if (-1 == matrix[i][j].GetX() &&
								score == matrix[i][j].GetY())
							{
								pos.PushForward(i, j);
							}
						}
					}

					if (pos.GetCountOfElements() > 0)
					{
						index = FindMoveBestPosition(pos);
						aiMove(index);
					}

					if (index)
						delete[] index;
				}
				else
				{
					int score = AlphaBeta(root, alpha, beta, matrix, 32);
					int index = 0;
					for (auto i = 0; i < width; i++)
					{
						for (auto j = 0; j < width; j++)
						{
							if (score == matrix[i][j].GetX())
							{
								pos.PushForward(i, j);
							}
						}
					}

					if (pos.GetCountOfElements() > 0)
					{
						index = FindBestPosition(pos);
						aiMove(pos[index]->GetX(), pos[index]->GetY());
					}
					else
					{
						aiMove(0, 0);
					}
				}

				for (auto i = 0; i < width; i++)
				{
					delete matrix[i];
				}
				delete[] matrix;
			}
			else
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
		}
		else
		{
			if (aiStone == 'w')
			{
				int alpha = 1 << 31, beta = ~(1 << 31);
				listPositions pos;
				minMaxNode* root = nullptr;
				point** matrix = nullptr;
				try
				{
					root = new minMaxNode(nullptr, 0, 0, 'a');
					matrix = new point* [width];
					for (auto i = 0; i < width; i++)
					{
						matrix[i] = new point[width] {};
					}
				}
				catch (const std::exception&)
				{
					throw("bad allocation");
				}

				if (usedStone[DefineIterator(true)] >= 12)
				{
					int score = MoveAlphaBeta(root, alpha, beta, matrix, 32);
					std::cout << score << '\n';

				/*for (auto i = 0; i < width; i++)
				{
					for (auto j = 0; j < width; j++)
					{
						std::cout << matrix[i][j].GetX() << ' ';
					}
					std::cout << '\n';
				}
				system("pause");*/
					int* index = nullptr;
					for (auto i = 0; i < width; i++)
					{
						for (auto j = 0; j < width; j++)
						{
							if (-1 == matrix[i][j].GetX() &&
								score == matrix[i][j].GetY())
							{
								pos.PushForward(i, j);
							}
						}
					}

					if (pos.GetCountOfElements() > 0)
					{
						index = FindMoveBestPosition(pos);
						aiMove(index);
					}
					
					if (index)
						delete[] index;
				}
				else
				{
					int score = AlphaBeta(root, alpha, beta, 
						matrix, 12 - usedStone[DefineIterator(true)]);
					int index = 0;
					for (auto i = 0; i < width; i++)
					{
						for (auto j = 0; j < width; j++)
						{
							if (score == matrix[i][j].GetX())
							{
								pos.PushForward(i, j);
							}
						}
					}

					if (pos.GetCountOfElements() > 0)
					{
						index = FindBestPosition(pos);
						aiMove(pos[index]->GetX(), pos[index]->GetY());
					}
					else
					{
						aiMove(0, 0);
					}
				}
				
				for (auto i = 0; i < width; i++)
				{
					delete matrix[i];
				}
				delete[] matrix;
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
}

/*
* Метод для хода ИИ
*/
void gameController::aiMove(const int& x, const int& y)
{
	gameMap.SetStone(x, y, DefineMove());
	Update();

	usedStone[DefineIterator(true)]++;
	opponentFirstMove = !opponentFirstMove;
}

/*
* Метод для хода ИИ
*/
void gameController::aiMove(int* prevCoord)
{
	for (auto i = 0; i < 4; i++)
	{
		std::cout << prevCoord[i] << ' ';
	}
	system("pause");
	gameMap.MoveStone(prevCoord[0], prevCoord[1], prevCoord[2], prevCoord[3]);
	Update();
	opponentFirstMove = !opponentFirstMove;
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
		opponentFirstMove = !opponentFirstMove;
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

	complete = completeCheck();
	return 0;
}

bool gameController::completeCheck()
{
	for (int i = 0; i < 2; i++)
		if (playersScore[i] >= 10)
			return true;
	return false;
}

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

bool gameController::FindHistoryComb(const line& comb, 
	ListOrient orient, const unsigned int iter)
{
	for (int i = 0; i < playersHistComb[iter].GetList(orient).GetCountOfElements(); i++)
		for (auto j = 0; j < 5; j++)
			if (*playersHistComb[iter].GetList(orient)[i] == comb)
				return true;
	return false;
}

void gameController::GetResults()
{
	system("cls");
	TextModeOutput();
	if (playersScore[DefineIterator(true)] == 10)
		std::cout << "Победил ИИ" << '\n';
	else if(playersScore[DefineIterator(false)] == 10)
		std::cout << "Победил игрок" << '\n';
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

listPositions gameController::FindMovePos(const unsigned int& prevX, 
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
			patternEval += 20000000;
		// Ударная четыре
		if (position.find("bbbbw") != std::string::npos
			|| position.find("wbbbb") != std::string::npos
			|| position.find("b0bbb") != std::string::npos
			|| position.find("bb0bb") != std::string::npos
			|| position.find("bbb0b") != std::string::npos)
			patternEval += 1000000;
		// Живая четверка
		if (position.find("0bbbb0") != std::string::npos)
			patternEval += 2000000;
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
			patternEval += 100000;
		// живая тройка
		if (position.find("0bbb0") != std::string::npos
			|| position.find("0b0bb0") != std::string::npos
			|| position.find("0bb0b0") != std::string::npos)
			patternEval += 200000;
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
			patternEval += 10000;
		// живая двойка
		if (position.find("0bb0") != std::string::npos
			|| position.find("0b00b0") != std::string::npos
			|| position.find("0b0b0") != std::string::npos)
			patternEval += 20000;
		// один камень
		if (position.find("b") != std::string::npos)						
			patternEval += 1000;
		if (DefineZeroStr(position)) // нет элементов
			patternEval += 200;
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
			patternEval += 20000000;
		// Ударная четыре
		if (position.find("wwwwb") != std::string::npos
			|| position.find("bwwww") != std::string::npos
			|| position.find("w0www") != std::string::npos
			|| position.find("ww0ww") != std::string::npos
			|| position.find("www0w") != std::string::npos)
			patternEval += 1000000;
		// Живая четверка
		if (position.find("0wwww0") != std::string::npos)
			patternEval += 2000000;
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
			patternEval += 100000;
		// живая тройка
		if (position.find("0www0") != std::string::npos
			|| position.find("0w0ww0") != std::string::npos
			|| position.find("0ww0w0") != std::string::npos)
			patternEval += 200000;
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
			patternEval += 10000;
		// живая двойка
		if (position.find("0ww0") != std::string::npos 
			|| position.find("0w00w0") != std::string::npos
			|| position.find("0w0w0") != std::string::npos)
			patternEval += 20000;
		// один камень
		if (position.find("w") != std::string::npos)
			patternEval += 1000;
		if (DefineZeroStr(position))	// нет элементов
			patternEval += 200;
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

int gameController::EvaluateField()
{
	int eval = 0;
	for (auto i = 0; i < width; i++)
	{
		for (auto j = 0; j < width; j++)
		{
			eval += Evaluation(i, j);
		}
	}
	return eval;
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

/*
* Метод находящий одинаковые позиции
*/
bool gameController::FindPrevPos(const minMaxNode* node) const
{
	if (node)
	{
		if (node->prev)
		{
			minMaxNode* curr = node->prev;
			while (curr->prev)
			{
				if (node->currentPos == curr->currentPos && 
					node->currTurn == curr->currTurn)
					return true;
				curr = curr->prev;
			}

			if (node->currentPos == curr->currentPos &&
				node->currTurn == curr->currTurn)
				return true;
		}
	}
	return false;
}

/*
* Метод отвечающий за стратегию минимакс
*/
int gameController::AlphaBeta(minMaxNode* node, int& alpha, int& beta
	, point** matrix, const unsigned int depth)
{
	int evaluation = 0;
	
	// Условие победы в игре ГО-БАН
	if (completeCheck() && node->currTurn == 'a')
		return ~(1 << 31);
	else if (completeCheck())
		return 1 << 31;

	if (depth == 0)
		return EvaluateField();

	// Выполняется, если ИИ зашел на клетку более одного раза
	if (FindPrevPos(node))
		return EvaluateField();
	
	// Текущий ход - ИИ
	if (node->currTurn == 'a')
	{
		evaluation = 1 << 31;
		for (auto i = 0; i < width; i++)
		{
			for (auto j = 0; j < width; j++)
			{
				// Минимакс для второго этапа игры
				if (usedStone[DefineIterator(true)] >= 12
					&& gameMap.EmptyCheck(i, j))
				{
					listPositions newPos; int tmpEval = alpha;
					MoveStagePositions(node, newPos, i, j);
					for (auto k = 0; k < newPos.GetCountOfElements(); k++)
					{
						gameMap.MoveStone(newPos[k]->GetX(), newPos[k]->GetY(),
							i, j);
						minMaxNode* nextNode = nullptr;
						try
						{
							nextNode = new minMaxNode(node, i, j, 'p');
						}
						catch (const std::exception&)
						{
							throw("bad allocation");
						}
						int* stonePerIter = MinMaxUpdate(nextNode);

						// расчет последующих позиций
						evaluation = std::max(evaluation,
							AlphaBeta(nextNode, alpha, beta, matrix, depth - 1));

						// Отмена постановки фишки и освобождение ресурсов
						if (stonePerIter[4] > 0)
						{
							for (auto s = 0; s < stonePerIter[horizontal]; s++)
							{
								playersHistComb[DefineIterator(true)]
									.GetList(horizontal).PopForward();
							}

							for (auto s = 0; s < stonePerIter[vertical]; s++)
							{
								playersHistComb[DefineIterator(true)]
									.GetList(vertical).PopForward();
							}

							for (auto s = 0; s < stonePerIter[mainDiagonal]; s++)
							{
								playersHistComb[DefineIterator(true)]
									.GetList(mainDiagonal).PopForward();
							}

							for (auto s = 0; s < stonePerIter[sideDiagonal]; s++)
							{
								playersHistComb[DefineIterator(true)]
									.GetList(sideDiagonal).PopForward();
							}
							playersScore[DefineIterator(true)] -= stonePerIter[4];
						}
						gameMap.MoveStone(i, j, 
							newPos[k]->GetX(), newPos[k]->GetY());
						try
						{
							delete[] stonePerIter;
							delete nextNode;
						}
						catch (const std::exception&)
						{
							throw("bad allocation");
						}
						
						//if (evaluation > alpha)
						//{
						//	tmpEval = evaluation;
						//	if (evaluation > beta)
						//		return evaluation;
						//	alpha = evaluation;
						//	//matrix[i][j] = alpha;
						//	matrix[newPos[k]->GetX()][newPos[k]->GetY()]
						//		.SetPoint(alpha, 0);
						//	matrix[i][j]
						//		.SetPoint(-1, alpha);
						//}
						if (evaluation > tmpEval)
						{
							tmpEval = evaluation;
						}

						if (alpha >= beta)
						{
							break;
						}
					}
					return tmpEval;
				}
				else if (gameMap.EmptyCheck(i, j))	// Минимакс для первого этапа игры
				{
					// Выделение памяти и постановка фишки
					gameMap.SetStone(i, j, aiStone);
					minMaxNode* nextNode = nullptr;
					try
					{
						nextNode = new minMaxNode(node, i, j, 'p');
					}
					catch (const std::exception&)
					{
						throw("bad allocation");
					}
					usedStone[DefineIterator(true)]++;
					int* stonePerIter = MinMaxUpdate(nextNode);

					// расчет последующих позиций
					evaluation = std::max(evaluation, 
						AlphaBeta(nextNode, alpha, beta, matrix, depth - 1));

					// Отмена постановки фишки и освобождение ресурсов
					if (stonePerIter[4] > 0)
					{
						for (auto s = 0; s < stonePerIter[horizontal]; s++)
						{
							playersHistComb[DefineIterator(true)]
								.GetList(horizontal).PopForward();
						}

						for (auto s = 0; s < stonePerIter[vertical]; s++)
						{
							playersHistComb[DefineIterator(true)]
								.GetList(vertical).PopForward();
						}

						for (auto s = 0; s < stonePerIter[mainDiagonal]; s++)
						{
							playersHistComb[DefineIterator(true)]
								.GetList(mainDiagonal).PopForward();
						}

						for (auto s = 0; s < stonePerIter[sideDiagonal]; s++)
						{
							playersHistComb[DefineIterator(true)]
								.GetList(sideDiagonal).PopForward();
						}
						playersScore[DefineIterator(true)] -= stonePerIter[4];
					}
					gameMap.SetStone(i, j, '0');
					usedStone[DefineIterator(true)]--;
					try
					{
						delete[] stonePerIter;
						delete nextNode;
					}
					catch (const std::exception&)
					{
						throw("bad allocation");
					}

					if (evaluation > alpha) 
					{
						alpha = evaluation;
						matrix[i][j].SetPoint(alpha, 0);
						//matrix[i][j] = alpha;
					}
				}
				if (alpha >= beta)
				{
					break;
				}
			}
			if (alpha >= beta) 
			{
				break;
			}
		}
		return alpha;
	}
	else
	{
		evaluation = ~(1 << 31);
		for (auto i = 0; i < width; i++)
		{
			for (auto j = 0; j < width; j++)
			{
				if (usedStone[DefineIterator(false)] >= 12
					&& gameMap.EmptyCheck(i, j))	// Минимакс для второго этапа игры
				{
					listPositions newPos; int tmpEval = beta;
					MoveStagePositions(node, newPos, i, j);
					for (unsigned int k = 0; k < newPos.GetCountOfElements(); k++)
					{
						gameMap.MoveStone(newPos[k]->GetX(), newPos[k]->GetY(),
							i, j);
						TextModeOutput();
						minMaxNode* nextNode = nullptr;
						try
						{
							nextNode = new minMaxNode(node, i, j, 'a');
						}
						catch (const std::exception&)
						{
							throw("bad allocation");
						}
						int* stonePerIter = MinMaxUpdate(nextNode);

						// расчет последующих позиций
						evaluation = std::min(evaluation,
							AlphaBeta(nextNode, alpha, beta, matrix, depth - 1));

						// Отмена постановки фишки и освобождение ресурсов
						if (stonePerIter[4] > 0)
						{
							for (auto s = 0; s < stonePerIter[horizontal]; s++)
							{
								playersHistComb[DefineIterator(false)]
									.GetList(horizontal).PopForward();
							}

							for (auto s = 0; s < stonePerIter[vertical]; s++)
							{
								playersHistComb[DefineIterator(false)]
									.GetList(vertical).PopForward();
							}

							for (auto s = 0; s < stonePerIter[mainDiagonal]; s++)
							{
								playersHistComb[DefineIterator(false)]
									.GetList(mainDiagonal).PopForward();
							}

							for (auto s = 0; s < stonePerIter[sideDiagonal]; s++)
							{
								playersHistComb[DefineIterator(false)]
									.GetList(sideDiagonal).PopForward();
							}
							playersScore[DefineIterator(false)] -= stonePerIter[4];
						}
						gameMap.MoveStone(i, j,
							newPos[k]->GetX(), newPos[k]->GetY());
						try
						{
							delete[] stonePerIter;
							delete nextNode;
						}
						catch (const std::exception&)
						{
							throw("bad allocation");
						}

						/*if (evaluation < beta)
						{
							beta = evaluation;
							matrix[newPos[k]->GetX()][newPos[k]->GetY()]
								.SetPoint(beta, 0);
							matrix[i][j]
								.SetPoint(-1, beta);
						}*/

						if (evaluation < tmpEval)
						{
							tmpEval = evaluation;
						}

						if (alpha >= beta)
						{
							break;
						}
					}
					return tmpEval;
				}
				else if (gameMap.EmptyCheck(i, j))	// Минимакс для первого этапа игры
				{
					// Выделение памяти и постановка фишки
					gameMap.SetStone(i, j, playerStone);
					minMaxNode* nextNode = nullptr;
					try
					{
						nextNode = new minMaxNode(node, i, j, 'a');
					}
					catch (const std::exception&)
					{
						throw("bad deallocation");
					}
					usedStone[DefineIterator(false)]++;
					int* stonePerIter = MinMaxUpdate(nextNode);

					// расчет последующих позиций
					evaluation = std::min(
						AlphaBeta(nextNode, alpha, beta, matrix, depth - 1),
						evaluation);

					// Отмена постановки фишки и освобождение ресурсов
					if (stonePerIter[4])
					{
						for (auto s = 0; s < stonePerIter[horizontal]; s++)
						{
							playersHistComb[DefineIterator(false)]
								.GetList(horizontal).PopForward();
						}

						for (auto s = 0; s < stonePerIter[vertical]; s++)
						{
							playersHistComb[DefineIterator(false)]
								.GetList(vertical).PopForward();
						}

						for (auto s = 0; s < stonePerIter[mainDiagonal]; s++)
						{
							playersHistComb[DefineIterator(false)]
								.GetList(mainDiagonal).PopForward();
						}

						for (auto s = 0; s < stonePerIter[sideDiagonal]; s++)
						{
							playersHistComb[DefineIterator(false)]
								.GetList(sideDiagonal).PopForward();
						}
						playersScore[DefineIterator(false)] -= stonePerIter[4];
					}
					gameMap.SetStone(i, j, '0');
					usedStone[DefineIterator(false)]--;
					try
					{
						delete[] stonePerIter;
						delete nextNode;
					}
					catch (const std::exception&)
					{
						throw("bad deallocation");
					}

					if (evaluation < beta)
					{
						beta = evaluation;
						matrix[i][j].SetPoint(beta, 0);
						//matrix[i][j] = beta;
					}
				}
				if (alpha >= beta)
				{
					break;
				}
			}
			if (alpha >= beta) {
				break;
			}
		}
		return beta;
	}
}

int gameController::MoveAlphaBeta(minMaxNode* node, int& alpha, int& beta,
	point** matrix, const unsigned int depth)
{
	int evaluation = 0;

	// Условие победы в игре ГО-БАН
	if (completeCheck() && node->currTurn == 'a')
		return ~(1 << 31);
	else if (completeCheck())
		return 1 << 31;

	if (depth == 0)
		return EvaluateField();

	// Выполняется, если ИИ зашел на клетку более одного раза
	/*if (FindPrevPos(node))
		return EvaluateField();*/

	if (node->currTurn == 'a')
	{
		evaluation = 1 << 31;
		for (auto i = 0; i < width; i++)
		{
			for (auto j = 0; j < width; j++)
			{
				if(gameMap.EmptyCheck(i, j))
				{
					listPositions newPos; //int tmpEval = alpha;
					MoveStagePositions(node, newPos, i, j);
					for (auto k = 0; k < newPos.GetCountOfElements(); k++)
					{
						gameMap.MoveStone(newPos[k]->GetX(), newPos[k]->GetY(),
							i, j);
						minMaxNode* nextNode = nullptr;
						try
						{
							nextNode = new minMaxNode(node, i, j, 
								newPos[k]->GetX(), newPos[k]->GetY(), 'p');
						}
						catch (const std::exception&)
						{
							throw("bad allocation");
						}
						int* stonePerIter = MinMaxUpdate(nextNode);

						// расчет последующих позиций
						evaluation = std::max(evaluation,
							MoveAlphaBeta(nextNode, alpha, beta, matrix, depth - 1));

						// Отмена постановки фишки и освобождение ресурсов
						if (stonePerIter[4] > 0)
						{
							for (auto s = 0; s < stonePerIter[horizontal]; s++)
							{
								playersHistComb[DefineIterator(true)]
									.GetList(horizontal).PopForward();
							}

							for (auto s = 0; s < stonePerIter[vertical]; s++)
							{
								playersHistComb[DefineIterator(true)]
									.GetList(vertical).PopForward();
							}

							for (auto s = 0; s < stonePerIter[mainDiagonal]; s++)
							{
								playersHistComb[DefineIterator(true)]
									.GetList(mainDiagonal).PopForward();
							}

							for (auto s = 0; s < stonePerIter[sideDiagonal]; s++)
							{
								playersHistComb[DefineIterator(true)]
									.GetList(sideDiagonal).PopForward();
							}
							playersScore[DefineIterator(true)] -= stonePerIter[4];
						}
						gameMap.MoveStone(i, j,
							newPos[k]->GetX(), newPos[k]->GetY());
						try
						{
							delete[] stonePerIter;
							delete nextNode;
						}
						catch (const std::exception&)
						{
							throw("bad allocation");
						}

						if (evaluation > alpha)
						{
							alpha = evaluation;
							//matrix[i][j] = alpha;
							/*matrix[newPos[k]->GetX()][newPos[k]->GetY()]
								.SetPoint(alpha, 0);*/
							/*if (copyGameMap.EmptyCheck(i, j))
							{
								matrix[i][j]
									.SetPoint(-1, alpha);
							}*/
							OriginSearch(node, matrix, evaluation);
						}

						/*if (alpha >= beta)
						{
							break;
						}*/
					}
				}
				if (alpha >= beta)
				{
					break;
				}
			}
			if (alpha >= beta)
			{
				break;
			}
		}
		return alpha;
	}
	else
	{
		evaluation = ~(1 << 31);
		for (auto i = 0; i < width; i++)
		{
			for (auto j = 0; j < width; j++)
			{
				if (gameMap.EmptyCheck(i, j))
				{
					listPositions newPos; //int tmpEval = beta;
					MoveStagePositions(node, newPos, i, j);
					for (unsigned int k = 0; k < newPos.GetCountOfElements(); k++)
					{
						gameMap.MoveStone(newPos[k]->GetX(), newPos[k]->GetY(),
							i, j);
						//TextModeOutput();
						minMaxNode* nextNode = nullptr;
						try
						{
							nextNode = new minMaxNode(node, i, j, 
								newPos[k]->GetX(), newPos[k]->GetY(), 'a');
						}
						catch (const std::exception&)
						{
							throw("bad allocation");
						}
						int* stonePerIter = MinMaxUpdate(nextNode);

						// расчет последующих позиций
						evaluation = std::min(evaluation,
							MoveAlphaBeta(nextNode, alpha, beta, matrix, depth - 1));

						// Отмена постановки фишки и освобождение ресурсов
						if (stonePerIter[4] > 0)
						{
							for (auto s = 0; s < stonePerIter[horizontal]; s++)
							{
								playersHistComb[DefineIterator(false)]
									.GetList(horizontal).PopForward();
							}

							for (auto s = 0; s < stonePerIter[vertical]; s++)
							{
								playersHistComb[DefineIterator(false)]
									.GetList(vertical).PopForward();
							}

							for (auto s = 0; s < stonePerIter[mainDiagonal]; s++)
							{
								playersHistComb[DefineIterator(false)]
									.GetList(mainDiagonal).PopForward();
							}

							for (auto s = 0; s < stonePerIter[sideDiagonal]; s++)
							{
								playersHistComb[DefineIterator(false)]
									.GetList(sideDiagonal).PopForward();
							}
							playersScore[DefineIterator(false)] -= stonePerIter[4];
						}
						gameMap.MoveStone(i, j,
							newPos[k]->GetX(), newPos[k]->GetY());
						try
						{
							delete[] stonePerIter;
							delete nextNode;
						}
						catch (const std::exception&)
						{
							throw("bad allocation");
						}

						if (evaluation < beta)
						{
							beta = evaluation;
							/*matrix[newPos[k]->GetX()][newPos[k]->GetY()]
								.SetPoint(beta, 0);*/
							/*matrix[i][j]
								.SetPoint(beta, 0);*/
							OriginSearch(node, matrix, evaluation);
						}

						/*if (alpha >= beta)
						{
							break;
						}*/
					}
				}
				if (alpha >= beta)
				{
					break;
				}
			}
			if (alpha >= beta)
			{
				break;
			}
		}
		return beta;
	}
}

void gameController::OriginSearch(minMaxNode* node, point** matrix, int val)
{
	if (node)
	{
		if (node->prev)
		{
			if (!node->prev->prev)
			{
				matrix[node->currentPos.GetX()][node->currentPos.GetY()]
					.SetPoint(-1, val);
				matrix[node->oldPos.GetX()][node->oldPos.GetY()].
					SetPoint(val, 0);
			}
			else if (node->prev->prev)
			{
				minMaxNode* curr = node->prev;
				while (curr->prev)
				{
					if (!curr->prev->prev)
					{
						matrix[curr->currentPos.GetX()][curr->currentPos.GetY()]
							.SetPoint(-1, val);
						matrix[curr->oldPos.GetX()][curr->oldPos.GetY()].
							SetPoint(val, 0);
					}
					curr = curr->prev;
				}

				/*matrix[curr->currentPos.GetX()][curr->currentPos.GetY()]
					.SetPoint(-1, val);*/
					/*matrix[curr->oldPos.GetX()][curr->oldPos.GetY()].
						SetPoint(val, 0);*/
			}
		}

		//if (node->prev->prev)
		//{
		//	minMaxNode* curr = node->prev;
		//	while (curr->prev)
		//	{
		//		if (!curr->prev->prev)
		//		{
		//			matrix[curr->currentPos.GetX()][curr->currentPos.GetY()]
		//				.SetPoint(-1, val);
		//		}
		//		curr = curr->prev;
		//	}

		//	/*matrix[curr->currentPos.GetX()][curr->currentPos.GetY()]
		//		.SetPoint(-1, val);*/
		//	/*matrix[curr->oldPos.GetX()][curr->oldPos.GetY()].
		//		SetPoint(val, 0);*/
		//}
		//else 
		//{

		//}
		//{
		//	matrix[node->currentPos.GetX()][node->currentPos.GetY()]
		//		.SetPoint(-1, val);
		//	/*matrix[node->oldPos.GetX()][node->oldPos.GetY()].
		//		SetPoint(val, 0);*/
		//}
	}
}

/*
* Метод для поиска камней вокруг пустой ячейки 2 этапа игры
*/
void gameController::MoveStagePositions(minMaxNode* node, listPositions& pos,
	const int& emptX, const int& emptY)
{
	// Цикл поиска камня для перемещения в пустую ячейку
	for (auto k = -1; k < 2; k++)
	{
		if (gameMap.GetCell(emptX + k, emptY) == DefineMove(node))
			pos.PushForward(emptX + k, emptY);

		if (gameMap.GetCell(emptX, emptY + k) == DefineMove(node))
			pos.PushForward(emptX, emptY + k);

		if (gameMap.GetCell(emptX + k, emptY + k) == DefineMove(node))
			pos.PushForward(emptX + k, emptY + k);

		if (gameMap.GetCell(emptX - k, emptY - k) == DefineMove(node))
			pos.PushForward(emptX - k, emptY - k);
	}
}

char gameController::DefineMove(const minMaxNode* node) const
{
	if (node->currTurn == 'a')
		return aiStone;
	else
		return playerStone;
}

/*
* Метод определяющий итератор фишки
*/
unsigned int gameController::DefineIterator(const bool defineAI)
{
	if (defineAI)
	{
		return DefineIteratorHelper(aiStone);
	}
	else
	{
		return DefineIteratorHelper(playerStone);
	}
}

unsigned int gameController::DefineIteratorHelper(const char stone) const
{
	if (stone == 'w')
		return 1;
	return 0;
}

/*
* Метод определяющий итератор фишки
*/
unsigned int gameController::DefineIterator(const minMaxNode* node, 
	const bool defineAI) const
{
	return DefineIteratorHelper(node->currTurn == 'a' ? aiStone : playerStone);
}

/*
* Метод определяющий итератор фишки
*/
unsigned int gameController::DefineNextMoveIterator(minMaxNode* node)
{
	if (node->currTurn == 'a')
	{
		if (playerStone == 'w')
			return 1;
		return 0;
	}
	else
	{
		if (aiStone == 'w')
			return 1;
		return 0;
	}
}

/*
* Метод определяющий ход игры
*/
char gameController::DefineNextMove(minMaxNode* node)
{
	if (node->currTurn == 'a')
		return 'p';
	return 'a';
}

/*
* Метод присуждающий очки за линии 5 в ряд
*/
int* gameController::MinMaxUpdate(minMaxNode* node)
{
	int* score = nullptr;
	try
	{
		score = new int[5] {0};
	}
	catch (const std::exception&)
	{
		throw("bad allocation");
	}

	// Проверка по горизонтали
	for (int i = 0; i < width; ++i)
	{
		line dots;
		for (int j = 0; j < width - 4; ++j)
		{
			bool sequence = true;
			for (int k = 0; k < 5; ++k)
			{
				if (gameMap.GetCell(i, j + k) != DefineMove())
				{
					sequence = false;
					break;
				}
				dots.dots[k].SetPoint(i, j + k);
			}

			/*if (opponentFirstMove && sequence &&
				!FindHistoryComb(dots, horizontal))
			{
				playersScore[DefineNextMoveIterator(node)]++;
				score[4]++;
				score[horizontal]++;
				playersHistComb[DefineNextMoveIterator(node)]
					.GetList(horizontal).PushForward(dots);
			}*/
			if (sequence && 
				!FindHistoryComb(dots, horizontal, DefineIterator(node, true)))
			{
				playersScore[DefineNextMoveIterator(node)]++;
				score[4]++;
				score[horizontal]++;
				playersHistComb[DefineNextMoveIterator(node)]
					.GetList(horizontal).PushForward(dots);
			}
		}
	}

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

			/*if (opponentFirstMove && sequence &&
				!FindHistoryComb(dots, vertical))
			{
				playersScore[DefineNextMoveIterator(node)]++;
				score[4]++;
				score[vertical]++;
				playersHistComb[DefineNextMoveIterator(node)]
					.GetList(vertical).PushForward(dots);
			}*/
			if (sequence && 
				!FindHistoryComb(dots, vertical, DefineNextMoveIterator(node)))
			{
				playersScore[DefineNextMoveIterator(node)]++;
				score[4]++;
				score[vertical]++;
				playersHistComb[DefineNextMoveIterator(node)]
					.GetList(vertical).PushForward(dots);
			}
		}
	}

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

			/*if (opponentFirstMove && sequence &&
				!FindHistoryComb(dots, mainDiagonal))
			{
				playersScore[DefineNextMoveIterator(node)]++;
				score[4]++;
				score[mainDiagonal]++;
				playersHistComb[DefineNextMoveIterator(node)]
					.GetList(mainDiagonal).PushForward(dots);
			}*/
			if (sequence && 
				!FindHistoryComb(dots, mainDiagonal, DefineNextMoveIterator(node)))
			{
				playersScore[DefineNextMoveIterator(node)]++;
				score[4]++;
				score[mainDiagonal]++;
				playersHistComb[DefineNextMoveIterator(node)]
					.GetList(mainDiagonal).PushForward(dots);
			}
		}
	}

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

			/*if (opponentFirstMove && sequence &&
				!FindHistoryComb(dots, sideDiagonal))
			{
				playersScore[DefineNextMoveIterator(node)]++;
				score[4]++;
				score[sideDiagonal]++;
				playersHistComb[DefineNextMoveIterator(node)]
					.GetList(sideDiagonal).PushForward(dots);
			}*/
			if (sequence && 
				!FindHistoryComb(dots, sideDiagonal, DefineNextMoveIterator(node)))
			{
				playersScore[DefineNextMoveIterator(node)]++;
				score[4]++;
				score[sideDiagonal]++;
				playersHistComb[DefineNextMoveIterator(node)]
					.GetList(sideDiagonal).PushForward(dots);
			}
		}
	}
	return score;
}

/*
* метод определяющий лучшую позицую для ИИ
*/
int gameController::FindBestPosition(listPositions& pos)
{
	int val = 1 << 31, index = 0;
	point dot(pos[0]->GetX(), pos[0]->GetY());
	gameMap.SetStone(dot.GetX(), dot.GetY(), aiStone);
	/*for (auto i = 0; i < width; i++)
	{
		for (auto j = 0; j < width; j++)
		{
			val = Evaluation(i, j);
		}
	}*/
	val = EvaluateField();
	gameMap.SetStone(dot.GetX(), dot.GetY(), '0');

	for (auto i = 0; i < pos.GetCountOfElements(); i++)
	{
		int tmpVal = 1 << 31;
		dot.SetPoint(pos[i]->GetX(), pos[i]->GetY());
		gameMap.SetStone(dot.GetX(), dot.GetY(), aiStone);
		tmpVal = EvaluateField();

		if (tmpVal > val)
		{
			val = tmpVal;
			index = i;
		}
		gameMap.SetStone(dot.GetX(), dot.GetY(), '0');
	}
	return index;
}

/*
* метод определяющий лучшую позицую для ИИ во 2 части игры
*/
int* gameController::FindMoveBestPosition(listPositions& pos)
{
	int val = 1 << 31;
	int* index = new int[4] {0};
	point dot(pos[0]->GetX(), pos[0]->GetY());

	for (auto k = -1; k < 2; k++)
	{
		int tmpVal = 1 << 31;
		if (gameMap.GetCell(dot.GetX() + k, dot.GetY()) == DefineMove())
		{
			gameMap.MoveStone(dot.GetX() + k, dot.GetY(), dot.GetX(), dot.GetY());
			tmpVal = EvaluateField();
			gameMap.MoveStone(dot.GetX(), dot.GetY(), dot.GetX() + k, dot.GetY());

			if (tmpVal > val)
			{
				val = tmpVal;
				index[0] = dot.GetX() + k;
				index[1] = dot.GetY();
				index[2] = dot.GetX();
				index[3] = dot.GetY();
			}
		}

		if (gameMap.GetCell(dot.GetX(), dot.GetY() + k) == DefineMove())
		{
			gameMap.MoveStone(dot.GetX(), dot.GetY() + k, dot.GetX(), dot.GetY());
			tmpVal = EvaluateField();
			gameMap.MoveStone(dot.GetX(), dot.GetY(), dot.GetX(), dot.GetY() + k);

			if (tmpVal > val)
			{
				val = tmpVal;
				index[0] = dot.GetX();
				index[1] = dot.GetY() + k;
				index[2] = dot.GetX();
				index[3] = dot.GetY();
			}
		}

		if (gameMap.GetCell(dot.GetX() + k, dot.GetY() + k) == DefineMove())
		{
			gameMap.MoveStone(dot.GetX() + k, dot.GetY() + k, dot.GetX(), dot.GetY());
			tmpVal = EvaluateField();
			gameMap.MoveStone(dot.GetX(), dot.GetY(), dot.GetX() + k, dot.GetY() + k);

			if (tmpVal > val)
			{
				val = tmpVal;
				index[0] = dot.GetX() + k;
				index[1] = dot.GetY() + k;
				index[2] = dot.GetX();
				index[3] = dot.GetY();
			}
		}

		if (gameMap.GetCell(dot.GetX() - k, dot.GetY() - k) == DefineMove())
		{
			gameMap.MoveStone(dot.GetX() - k, dot.GetY() - k, dot.GetX(), dot.GetY());
			tmpVal = EvaluateField();
			gameMap.MoveStone(dot.GetX(), dot.GetY(), dot.GetX() - k, dot.GetY() - k);

			if (tmpVal > val)
			{
				val = tmpVal;
				index[0] = dot.GetX() - k;
				index[1] = dot.GetY() - k;
				index[2] = dot.GetX();
				index[3] = dot.GetY();
			}
		}
	}
	/*for (auto x = 0; x < width; x++)
	{
		for (auto y = 0; y < width; y++)
		{
			for (auto k = -1; k < 2; k++)
			{
				if (gameMap.GetCell(x + k, y) == DefineMove())
				{
					gameMap.MoveStone(x + k, y, x, y);
					val = Evaluation(x, y);
					gameMap.MoveStone(x, y, x + k, y);
					break;
				}

				if (gameMap.GetCell(x, y + k) == DefineMove())
				{
					gameMap.MoveStone(x, y + k, x, y);
					val = Evaluation(x, y);
					gameMap.MoveStone(x, y, x , y + k);
					break;
				}

				if (gameMap.GetCell(x + k, y + k) == DefineMove())
				{
					gameMap.MoveStone(x + k, y + k, x, y);
					val = Evaluation(x, y);
					gameMap.MoveStone(x, y, x + k, y + k);
					break;
				}

				if (gameMap.GetCell(x - k, y - k) == DefineMove())
				{
					gameMap.MoveStone(x - k, y - k, x, y);
					val = Evaluation(x, y);
					gameMap.MoveStone(x, y, x - k, y - k);
					break;
				}
			}
		}
	}*/

	for (auto i = 1; i < pos.GetCountOfElements(); i++)
	{
		int tmpVal = 1 << 31;
		dot.SetPoint(pos[i]->GetX(), pos[i]->GetY());
		gameMap.SetStone(dot.GetX(), dot.GetY(), aiStone);
		
		for (auto k = -1; k < 2; k++)
		{
			int tmpVal = 1 << 31;
			if (gameMap.GetCell(dot.GetX() + k, dot.GetY()) == DefineMove())
			{
				gameMap.MoveStone(dot.GetX() + k, dot.GetY(), dot.GetX(), dot.GetY());
				tmpVal = EvaluateField();
				gameMap.MoveStone(dot.GetX(), dot.GetY(), dot.GetX() + k, dot.GetY());

				if (tmpVal > val)
				{
					val = tmpVal;
					index[0] = dot.GetX() + k;
					index[1] = dot.GetY();
					index[2] = dot.GetX();
					index[3] = dot.GetY();
				}
			}

			if (gameMap.GetCell(dot.GetX(), dot.GetY() + k) == DefineMove())
			{
				gameMap.MoveStone(dot.GetX(), dot.GetY() + k, dot.GetX(), dot.GetY());
				tmpVal = EvaluateField();
				gameMap.MoveStone(dot.GetX(), dot.GetY(), dot.GetX(), dot.GetY() + k);

				if (tmpVal > val)
				{
					val = tmpVal;
					index[0] = dot.GetX();
					index[1] = dot.GetY() + k;
					index[2] = dot.GetX();
					index[3] = dot.GetY();
				}
			}

			if (gameMap.GetCell(dot.GetX() + k, dot.GetY() + k) == DefineMove())
			{
				gameMap.MoveStone(dot.GetX() + k, dot.GetY() + k, dot.GetX(), dot.GetY());
				tmpVal = EvaluateField();
				gameMap.MoveStone(dot.GetX(), dot.GetY(), dot.GetX() + k, dot.GetY() + k);

				if (tmpVal > val)
				{
					val = tmpVal;
					index[0] = dot.GetX() + k;
					index[1] = dot.GetY() + k;
					index[2] = dot.GetX();
					index[3] = dot.GetY();
				}
			}

			if (gameMap.GetCell(dot.GetX() - k, dot.GetY() - k) == DefineMove())
			{
				gameMap.MoveStone(dot.GetX() - k, dot.GetY() - k, dot.GetX(), dot.GetY());
				tmpVal = EvaluateField();
				gameMap.MoveStone(dot.GetX(), dot.GetY(), dot.GetX() - k, dot.GetY() - k);

				if (tmpVal > val)
				{
					val = tmpVal;
					index[0] = dot.GetX() - k;
					index[1] = dot.GetY() - k;
					index[2] = dot.GetX();
					index[3] = dot.GetY();
				}
			}
		}
	}
	return index;
}