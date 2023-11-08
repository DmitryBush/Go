#pragma once
#include "DynamicArr.h"
#include <functional>

class nTreeNode
{
public:
	int value;
	dynamicArr<nTreeNode> sons;

	nTreeNode(const int& val = 0): value(val) {}
};

class nTree
{
private:
	nTreeNode* root, *current;
	unsigned int countElements, height, currHeight;

	void DefineHeight(nTreeNode* node, unsigned int maxHeight);
	void NTreePass(nTreeNode* node, unsigned int maxHeight,
		const std::function<void(nTreeNode*, unsigned int)> t);
	void Task(nTreeNode* node, unsigned int& maxHeight, unsigned int currHeight);
	void Clear(nTreeNode* node);
	void PrintCurr(nTreeNode* node, unsigned int depth);
public:
	nTree();

	void Push(const int& val);
	void MoveNode(const int& node);
	void DeleteCurrHeight();
	void PrintCurr();

	unsigned int GetHeight() { return height; }
	void ResetToRoot();

	void Clear();

	void Print();
	void Task();

	~nTree();
};