#pragma once
#include "Map.h"

class gameController
{
private:
	const unsigned int width, height;
	unsigned int usedStone[2]{0}, playersScore[2]{ 0 };
	bool complete;
	bool blackMove;

	map gameMap;

	void PlayerMove(const bool setStone);
	int Update(const int& x_elem, const int& y_elem);
	void Update(const int& x_elem, const int& y_elem, int line);
	char DefineMove();

	int EvaluationFunc();

	void CheckInputValidation(int& x, int& y);
	void CheckInputValidation(int& prevX_coord, int& prevY_coord,
		int& x_coord, int& y_coord);
public:
	gameController(const unsigned int& x, const unsigned int& y);

	bool CompletedGame() { return complete; }

	void PlayerMove();
	void Restart();

	void TextModeOutput();

	
};