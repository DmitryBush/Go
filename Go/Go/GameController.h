#pragma once
#include "Map.h"
#include "List.h"
#include <string>
#include "nTree.h"
#include <functional>

enum GamePattern
{
	NoStone = 0, OneStone = 1, SleepingTwo = 2, LiveTwo = 3, SleepingThree = 4,
	LiveThree = 5, StrikingFour = 6, LiveFour = 7, LineFive = 8
};

enum InputValidation
{
	SetStone = 0, MoveStone = 1
};

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

	nTree MinMaxTree;

	void PlayerMove(const bool setStone);
	int Update();
	bool FindHistoryComb(const point& beg, const point& end);
	char DefineMove();

	int CheckPattern(const std::string& position, const char& stone);
	int Evaluation(const unsigned int x, const unsigned int y);
	//bool IsScoreLine(const unsigned int& x, const unsigned int& y);

	bool CheckPattern(const std::string& position, const GamePattern pattern,
		const char& stone);

	int MiniMax(nTreeNode* node, const unsigned int depth);
	int MoveStageMinMax(nTreeNode* node, const int& prevX, const int& prevY,
		int& evaluation, const unsigned int depth);
	bool FindPrevPos(nTreeNode* node);
	unsigned int DefineIterator(const bool defineAI);
	char DefineNextMove(nTreeNode* node);

	void CheckInputValidation(int& x, int& y);
	void CheckInputValidation(int& x, int& y, std::string suggestion);
	listPositions& FindMovePos(const unsigned int& prevX,
		const unsigned int& prevY);
	bool CheckCorrectMove(const int& prevX, const int& prevY,
		const int& x, const int& y);
	void CheckInputValidation(int& prevX_coord, int& prevY_coord,
		int& x_coord, int& y_coord);

	void DebugOutputMoveAnalyzer();
	bool DefineZeroStr(const std::string& string);
public:
	gameController(const unsigned int& x, const unsigned int& y);

	bool CompletedGame() { return complete; }

	void PlayerMove();
	void Restart();

	void TextModeOutput();

	~gameController();
};