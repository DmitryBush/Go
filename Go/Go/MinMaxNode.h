#pragma once
#include "Point.h"
#include "ListPositions.h"

class minMaxNode
{
public:
	point currentPos, oldPos;
	char currTurn;
	minMaxNode* prev;

	minMaxNode(minMaxNode* prevNode,
		const int x = 0, const int y = 0, const char turn = 0) :
		currentPos(x, y), currTurn(turn), oldPos(0,0), prev(prevNode) {}

	minMaxNode(minMaxNode* prevNode,
		const int emptX = 0, const int emptY = 0, const int oldX = 0,
		const int oldY = 0, const char turn = 0):
		currentPos(emptX, emptY), oldPos(oldX, oldY), currTurn(turn), prev(prevNode) {}
};
