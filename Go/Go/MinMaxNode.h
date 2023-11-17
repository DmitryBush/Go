#pragma once
#include "Point.h"
#include "ListPositions.h"

class minMaxNode
{
public:
	point currentPos;
	char currTurn;
	minMaxNode* prev;

	minMaxNode(minMaxNode* prevNode,
		const int& x = 0, const int& y = 0, const char turn = 0) :
		currentPos(x, y), currTurn(turn), prev(prevNode) {}
};