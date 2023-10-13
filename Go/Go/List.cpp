#include "List.h"
#include <iostream>
#include <conio.h>

/*
* ����������� ��� ����������
*/
list::list() : countElements(0) { head = nullptr; }

/*
* ����������, ��������� ������
*/
list::~list() { Clear(); }

/*
* ��������� ������ ��� ����� ������� ���������������� ������
*/
bool list::PushForward(point beg, point end)
{
	if (head == nullptr)
	{
		try
		{
			head = new node(beg, end);
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
			node* curr = new node(beg, end, head);
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
bool list::PopForward(const unsigned int& index)
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
bool list::PopForward()
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
void list::Clear()
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
void list::ResetList()
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
point* list::operator[](const unsigned int index)
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
	return curr->val;
}

/*
* ������� 4:
* � ������ � ������� N ���������� ������ ������� �������� �� ��������� ��������� �� ��� �������, 
* ������� ������ ���. ���� ������ �������� ���, �� �������� ��� �� ����.
*/
void list::Task()
{
	if (head)
	{
		if (head->prev)		// ���� ������ ������ �� � ����������� ���������
			ResetList();

		node* curr = head;
		bool foundElem = false;
		while (curr->next)
		{
			node* next = curr;
			while (next->next)		// ����� ��������, ������� ����� ������ ��������
			{
				if (next->val > curr->val)
				{
					curr->val = next->val;
					foundElem = true;
					break;
				}
				else
				{
					next = next->next;
				}
			}

			if (!foundElem)			// �� ������� ������, ���� ������� �� ����� ������
				curr->val = 0;	// �� �� ���������� �����

			foundElem = false;
			curr = curr->next;
		}
		curr->val = 0;		// ��������� ������� ����������� ����� �����
	}
	else
	{
		std::cout << "List is empty" << '\n';
	}
}

/*
* ����� ������ � �������
*/
void list::Print()
{
	if (head != nullptr && head->prev)		// ���� ������ ������ �� � ����������� ���������
		ResetList();

	node* curr = head;
	if (curr == nullptr)
	{
		std::cout << "There's no object in list to show" << '\n';
	}
	else
	{
		while (curr->next)
		{
			std::cout << curr->val[0].GetX() << ' ' << curr->val[0].GetY() << ' '
				<< curr->val[1].GetX() << ' ' << curr->val[1].GetY() << '\n';
			curr = curr->next;
		}
		std::cout << curr->val[0].GetX() << ' ' << curr->val[0].GetY() << ' '
			<< curr->val[1].GetX() << ' ' << curr->val[1].GetY() << '\n';
	}
}
