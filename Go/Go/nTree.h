#pragma once
#include "DynamicArr.h"
#include <functional>
#include "ListPositions.h"

class minMaxNode
{
public:
	point currentPos;
	char currTurn;
	listPositions pos;

	minMaxNode(listPositions& positionList,
		const int& x = 0, const int& y = 0, const char& turn = 0):
		currentPos(x, y), currTurn(turn), pos(positionList) {}
};

class nTreeNode
{
public:
	minMaxNode data;
	dynamicArr<nTreeNode> sons;

	nTreeNode(minMaxNode node):
		data(node) {}
};

class nTree
{
private:
	nTreeNode* root, *current;

	unsigned int countElements, height, currHeight;

	void DefineHeight(nTreeNode* node, unsigned int maxHeight);
	void NTreePass(nTreeNode* node, unsigned int maxHeight,
		const std::function<void(nTreeNode*, unsigned int)> t);
	void Clear(nTreeNode* node);
public:
	nTree();

	void Push(const int& x, const int& y, const char& turn,
		listPositions& positionList);
	void MoveNode(const int& node);
	void DeleteCurrHeight();

	unsigned int GetHeight() { return height; }
	void ResetToRoot();

	point& GetPosition();
	char GetTurn();
	listPositions& GetList();

	void Clear();

	~nTree();
};