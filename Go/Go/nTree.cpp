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

void nTree::Task(nTreeNode* node, unsigned int& maxHeight, unsigned int currHeight)
{
	if (node == nullptr)
		return;

	if (node->value % 2 != 0)
	{
		if (currHeight > maxHeight)
			maxHeight = currHeight;
		for (auto i = 0; i < node->sons.GetCountElements(); i++)
			Task(node->sons.at(i), maxHeight, currHeight + 1);
	}
}

void nTree::Clear(nTreeNode* node)
{
	if (node == nullptr)
		return;

	for (auto i = 0; i < node->sons.GetCountElements(); i++)
		Clear(node->sons.at(i));
	node->sons.Clear();
}

void nTree::PrintCurr(nTreeNode* node, unsigned int depth)
{
	if (node == nullptr)
	{
		if (depth == currHeight + 1)
			std::cout << "Empty tree";
		return;
	}

	/*if (depth == currHeight)
		std::cout << "Current depth " << currHeight + 1 << '\n';*/
	if (depth == currHeight + 1)
		std::cout << node->value << ' ';
	for (auto i = 0; i < node->sons.GetCountElements(); i++)
	{
		PrintCurr(node->sons.at(i), depth + 1);
	}
		
}

nTree::nTree()
{
	countElements = 0, height = 0, currHeight = 0;
	root = nullptr, current = nullptr;
}

void nTree::Push(const int& val)
{
	if (root == nullptr)
	{
		root = new nTreeNode(val);
		countElements++;
		height = 1;
		currHeight = 1;
	}
	else if (current == nullptr)
	{
		root->sons.Push(val);
		countElements++;
		DefineHeight(root, 0);
	}
	else if (current->sons.GetCountElements())
	{
		current->sons.Push(val);
		countElements++;
		DefineHeight(root, 0);
	}
	else
	{
		current->sons.Push(val);
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

void nTree::PrintCurr()
{
	if (root == nullptr)
		std::cout << "Your tree is empty";
	else if (current == nullptr)
	{
		std::cout << "Current depth " << currHeight + 1 << '\n';
		PrintCurr(root, 1);
	}
	else
	{
		std::cout << "Current depth " << currHeight + 1 << '\n';
		PrintCurr(current, 2);
	}
	std::cout << '\n';
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

void nTree::Clear()
{
	Clear(root);
	root = nullptr, current = nullptr;
	countElements = 0, height = 0, currHeight = 0;
}

void nTree::Print()
{
	if (root == nullptr)
		std::cout << "Your tree is empty";
	else
	{
		NTreePass(root, 0,[](nTreeNode* node, unsigned int maxHeight)
			{
				std::cout << node->value << ' ';
			});
	}
	std::cout << '\n';
}

void nTree::Task()
{
	unsigned int maxDepth = 0;
	if (root->value % 2 != 0)
	{
		Task(root, maxDepth, 1);
	}
	std::cout << "The maximum path along odd vertices is: " << maxDepth << '\n';
}

nTree::~nTree()
{
	Clear();
}
