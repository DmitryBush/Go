#pragma once
#include "List.h"

enum ListOrient
{
	horizontal = 0, vertical = 1, mainDiagonal = 2, sideDiagonal = 3
};

class histComb
{
private:
	list combinations[4];

public:
	histComb();

	void Clear();
	list& GetList(ListOrient orient);
};