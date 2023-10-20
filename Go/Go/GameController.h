#pragma once
#include "Map.h"
#include "List.h"
#include <string>

class gameController
{
private:
	const unsigned int width, height;
	unsigned int usedStone[2]{0}, playersScore[2]{ 0 };
	char aiStone, playerStone;
	bool complete;
	bool opponentFirstMove;

	list playersHistComb[2];
	map gameMap;

	void PlayerMove(const bool setStone);
	int Update();
	bool FindHistoryComb(const point& beg, const point& end);
	char DefineMove();

	int CheckPattern(const std::string& position, const bool& aiMove, const char& stone);
	int Evaluation(const unsigned int x, const unsigned int y, const bool& ai);

	void CheckInputValidation(int& x, int& y);
	void CheckInputValidation(int& prevX_coord, int& prevY_coord,
		int& x_coord, int& y_coord);

	void DebugOutputMoveAnalyzer(const unsigned int x, const unsigned int y, const bool& ai);
	bool DefineZeroStr(const std::string& string);
public:
	gameController(const unsigned int& x, const unsigned int& y);

	bool CompletedGame() { return complete; }

	void PlayerMove();
	void Restart();

	void TextModeOutput();

	
};