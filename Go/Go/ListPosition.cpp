#include "ListPositions.h"
#include <iostream>
#include <conio.h>

/*
* ����������� ��� ����������
*/
listPositions::listPositions() : countElements(0) { head = nullptr; }

listPositions::listPositions(const listPositions& arg)
{
	head = arg.head;
	countElements = arg.GetCountOfElements();
}

/*
* ����������, ��������� ������
*/
listPositions::~listPositions() { Clear(); }

/*
* ��������� ������ ��� ����� ������� ���������������� ������
*/
bool listPositions::PushForward(const int x = 0, const int y = 0)
{
	if (head == nullptr)
	{
		try
		{
			head = new node(x, y);
			countElements++;			
			return true;
		}
		catch (const std::bad_alloc& ex)
		{
			std::cout << "An error has occured" << "(code: " << ex.what()
				<< "Not enough free RAM. Close other programs and try again" << std::endl;
			return false;
		}
	}
	else
	{
		try
		{	// ������� ��������� ������ ��� ���� � ���������� ���������� 
			// � ��� ����� �� �������
			node* curr = new node(x, y, head);
			head->prev = curr;					

			head = curr;
			countElements++;
			return true;
		}
		catch (const std::bad_alloc& ex)
		{
			std::cout << "An error has occured" << "(code: " << ex.what()
				<< "Not enough free RAM. Close other programs and try again" << std::endl;
			return false;
		}
	}
}

/*
* �������� �������� ���������������� ������, �� ���������� �������
*/
bool listPositions::PopForward(const unsigned int& index)
{
	int a = 1;
	node* del;

	if (head == nullptr)
	{
		std::cout << "List is empty" << std::endl;
		return false;
	}
	else if (index <= 0 && index > countElements)		// ������ ��������� � ����������� ��������
	{
		std::cout << "This element does not exist" << std::endl;
		return false;
	}
	else if (index == 1)
	{
		if (head->next == nullptr && head->prev == nullptr)		// � ������ ���� �������
		{
			delete head;
			head = nullptr;
			countElements = 0;

			return true;
		}
		else if (head->prev)			// ������ ������ �� ��������� � ������
		{
			ResetList();

			del = head;
			head = head->next;
			head->prev = nullptr;

			delete del;
			countElements--;

			return true;
		}
		else							// � ������ ����� ���������
		{
			del = head;
			head = head->next;
			head->prev = nullptr;

			delete del;
			countElements--;

			return true;
		}
	}
	else if (head->prev)		// ������ ������ �� ��������� � ������
	{
		ResetList();

		node* curr = head;
		while (curr->next)		// ���� ����������� �������� �� ��������� ������� �������
		{
			if (a + 1 == index && curr->next->next != nullptr)	// ���� ��������� ������� �� �������� ���������
			{
				del = curr->next;
				curr->next = curr->next->next;		// �� ������������� ��������� � ������� �������

				node* tmp = curr->next->next;
				tmp->prev = curr;

				delete del;
				countElements--;

				return true;
			}
			else if (a + 1 == index)	// ���� ��������� ������� ���������
			{
				del = curr->next;
				curr->next = nullptr;	// �� ������������� ��������� � ������� �������

				delete del;
				countElements--;

				return true;
			}
			else						// ���� ������ �� ������
			{
				curr = curr->next;
				a++;
			}
		}
		return false;
	}
	else
	{
		node* curr = head;
		while (curr->next)			// ���� ����������� �������� �� ��������� ������� �������
		{
			if (a + 1 == index && curr->next->next != nullptr)	// ���� ��������� ������� �� �������� ���������
			{
				del = curr->next;
				curr->next = curr->next->next;			// �� ������������� ��������� � ������� �������

				node* tmp = curr->next->next; tmp->prev = curr;

				delete del;
				countElements--;

				return true;
			}
			else if (a + 1 == index)	// ���� ��������� ������� ���������
			{
				del = curr->next;
				curr->next = nullptr;	// �� ������������� ��������� � ������� �������

				delete del;
				countElements--;

				return true;
			}
			else						// ����� ���� ������ �� ������
			{
				curr = curr->next;
				a++;
			}
		}
		return false;
	}
}

/*
* �������� �������� �������� ���������������� ������
*/
bool listPositions::PopForward()
{
	node* del;

	if (head == nullptr)
	{
		std::cout << "List is empty" << std::endl;
		return false;
	}
	else
	{
		if (head->next == nullptr && head->prev == nullptr)		// � ������ ���� �������
		{
			delete head;
			head = nullptr;
			countElements = 0;

			return true;
		}
		else if (head->prev)				// ������ ������ �� ��������� � ������
		{
			ResetList();
			del = head;
			head = head->next;
			head->prev = nullptr;

			delete del;
			countElements--;

			return true;
		}
		else
		{
			del = head;
			head = head->next;
			head->prev = nullptr;

			delete del;
			countElements--;

			return true;
		}
	}
}

/*
* ����� �������� ������
*/
void listPositions::Clear()
{
	node* del;
	if (head == nullptr) { return; }
	else
	{
		while (head->next)		// ���� ��������� �������� ������, ���� �� ���� ���� ���������
		{
			del = head;
			head = head->next;
			delete del;
		}
		delete head;			// �������� ���������� ��������
	}
	head = nullptr;
	countElements = 0;
}

/*
* ��������� ��������� ������ ������ � ����������� ���������
*/
void listPositions::ResetList()
{
	if (head == nullptr) {}
	else
	{
		while (head->prev)
		{
			head = head->prev;
		}
	}
}

/*
* ���������� ��������� ���������� ������, ������� ���������� ��������� �� �������
*/
point* listPositions::operator[](const unsigned int index)
{
	if (index < 0 || index > countElements)
		throw("Invalid index");

	if (head == nullptr)
		throw("Head is null");

	node* curr = head;
	for (unsigned int i = 0; i < index; i++) 
	{
		curr = curr->next;
		if (!curr) 
			throw("Invalid index");
	}
	return &curr->val;
}

point& listPositions::GetPoint()
{
	return head->val;
}
