#include "nTree.h"
#include <iostream>

void nTree::DefineHeight(nTreeNode* node, unsigned int maxHeight)
{
	if (node == nullptr)
		return;
	
	if (++maxHeight > height)
		height = maxHeight;
	for (auto i = 0; i < node->sons.GetCountElements(); i++)
	{
		DefineHeight(node->sons.at(i), maxHeight);
	}
}

void nTree::NTreePass(nTreeNode* node, unsigned int depth,
	const std::function<void(nTreeNode*, unsigned int)> t)
{
	if (node == nullptr)
		return;

	t(node, depth);
	for (auto i = 0; i < node->sons.GetCountElements(); i++)
		NTreePass(node->sons.at(i), depth, t);
}

void nTree::Clear(nTreeNode* node)
{
	if (node == nullptr)
		return;

	for (auto i = 0; i < node->sons.GetCountElements(); i++)
		Clear(node->sons.at(i));
	node->sons.Clear();
}

nTree::nTree()
{
	countElements = 0, height = 0, currHeight = 0;
	root = nullptr, current = nullptr;
}

void nTree::Push(const int& x, const int& y, const char& turn, 
	listPositions& positionList)
{
	if (root == nullptr)
	{
		root = new nTreeNode(minMaxNode(positionList,x,y,turn));
		countElements++;
		height = 1;
		currHeight = 1;
	}
	else if (current == nullptr)
	{
		root->sons.Push(minMaxNode(positionList, x, y, turn));
		countElements++;
		DefineHeight(root, 0);
	}
	else if (current->sons.GetCountElements())
	{
		current->sons.Push(minMaxNode(positionList, x, y, turn));
		countElements++;
		DefineHeight(root, 0);
	}
	else
	{
		current->sons.Push(minMaxNode(positionList, x, y, turn));
		countElements++;
		DefineHeight(root, 0);
	}
}

void nTree::MoveNode(const int& node)
{
	if (root == nullptr)
	{
		std::cout << "An error has occuried" << '\n'
			<< "Your tree is empty" << '\n';
	}
	else if (current == nullptr)
	{
		if (auto nodeElems = root->sons.GetCountElements())
		{
			if (node < 0 || node >= nodeElems)
			{
				std::cout << "An error has occuried" << '\n'
					<< "Invalid index" << '\n';
				return;
			}
			
			current = root->sons.at(node);
			currHeight = 2;
		}
		else
		{
			std::cout << "An error has occuried" << '\n'
				<< "The root has no sons" << '\n';
		}
	}
	else
	{
		if (auto nodeElems = current->sons.GetCountElements())
		{
			if (node < 0 || node >= nodeElems)
			{
				std::cout << "An error has occuried" << '\n'
					<< "Invalid index" << '\n';
				return;
			}

			current = current->sons.at(node);
			currHeight++;
		}
		else
		{
			std::cout << "An error has occuried" << '\n'
				<< "This node has no sons" << '\n';
		}
	}
}

void nTree::DeleteCurrHeight()
{
	if (root == nullptr)
	{
		std::cout << "An error has occuried" << '\n'
			<< "The root has no sons" << '\n';
	}
	else if (current == nullptr)
	{
		Clear();
	}
	else
	{
		Clear(current);
	}
}

void nTree::ResetToRoot()
{
	if (root)
	{
		current = nullptr, currHeight = 1;
	}
	else
	{
		current = nullptr, currHeight = 0;
	}
}

point& nTree::GetPosition()
{
	if (current)
	{
		return current->data.currentPos;
	}
	else if (root)
	{
		return root->data.currentPos;
	}
	else
	{
		throw("Empty tree");
	}
}

char nTree::GetTurn()
{
	if (current)
	{
		return current->data.currTurn;
	}
	else if (root)
	{
		return root->data.currTurn;
	}
	else
	{
		throw("Empty tree");
	}
}

listPositions& nTree::GetList()
{
	if (current)
	{
		return current->data.pos;
	}
	else if (root)
	{
		return root->data.pos;
	}
	else
	{
		throw("Empty tree");
	}
}

void nTree::Clear()
{
	Clear(root);
	root = nullptr, current = nullptr;
	countElements = 0, height = 0, currHeight = 0;
}

nTree::~nTree()
{
	Clear();
}
