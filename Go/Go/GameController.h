#pragma once
#include "Map.h"
#include <functional>

class gameController
{
private:
	const unsigned int width, height;
	unsigned int usedStone[2]{0}, playersScore[2]{ 0 };
	bool isDead , complete;
	bool blackMove;

	map gameMap;

	void PlayerMove(const bool setStone);
	int Update(const int& x_elem, const int& y_elem);
	void Update(const int& x_elem, const int& y_elem, int line);
	char DefineMove();
public:
	gameController(const unsigned int& x, const unsigned int& y);

	bool IsDied() { return isDead; }
	bool CompletedGame() { return complete; }

	void PlayerMove();
	void Restart();

	void TextModeOutput();
};