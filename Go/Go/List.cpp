#include "List.h"
#include <iostream>
#include <conio.h>

/*
* Конструктор без параметров
*/
list::list() : countElements(0) { head = nullptr; }

/*
* Деструктор, очищающий список
*/
list::~list() { Clear(); }

/*
* Выделение памяти под новый элемент двунаправленного списка
*/
bool list::PushForward(line dots)
{
	if (head == nullptr)
	{
		try
		{
			head = new node(dots);
			countElements++;			
			return true;
		}
		catch (const std::bad_alloc& ex)
		{
			std::cout << "An error has occured" << "(code: " << ex.what()
				<< "Not enough free RAM. Close other programs and try again" << std::endl;
			throw("bad allocation");
			return false;
		}
	}
	else
	{
		try
		{	// Попытка выделения памяти под узел с переданным параметром 
			// и его связь со списком
			node* curr = new node(dots, head);
			head->prev = curr;					

			head = curr;
			countElements++;
			return true;
		}
		catch (const std::bad_alloc& ex)
		{
			std::cout << "An error has occured" << "(code: " << ex.what()
				<< "Not enough free RAM. Close other programs and try again" << std::endl;
			throw("bad allocation");
			return false;
		}
	}
}

/*
* Удаление элемента двунаправленного списка, по указанному индексу
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
	else if (index <= 0 && index > countElements)		// Индекс находится в невозможных границах
	{
		std::cout << "This element does not exist" << std::endl;
		return false;
	}
	else if (index == 1)
	{
		if (head->next == nullptr && head->prev == nullptr)		// В списке один элемент
		{
			delete head;
			head = nullptr;
			countElements = 0;

			return true;
		}
		else if (head->prev)			// Голова списка не находится в начале
		{
			ResetList();

			del = head;
			head = head->next;
			head->prev = nullptr;

			delete del;
			countElements--;

			return true;
		}
		else							// В списке много элементов
		{
			del = head;
			head = head->next;
			head->prev = nullptr;

			delete del;
			countElements--;

			return true;
		}
	}
	else if (head->prev)		// Голова списка не находится в начале
	{
		ResetList();

		node* curr = head;
		while (curr->next)		// Цикл проверяющий является ли следующий элемент искомым
		{
			if (a + 1 == index && curr->next->next != nullptr)	// Если следующий элемент не является последним
			{
				del = curr->next;
				curr->next = curr->next->next;		// То переприсвоить указатели и удалить элемент

				node* tmp = curr->next->next;
				tmp->prev = curr;

				delete del;
				countElements--;

				return true;
			}
			else if (a + 1 == index)	// Если следующий элемент последний
			{
				del = curr->next;
				curr->next = nullptr;	// То переприсвоить указатель и удалить элемент

				delete del;
				countElements--;

				return true;
			}
			else						// Идти дальше по списку
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
		while (curr->next)			// Цикл проверяющий является ли следующий элемент искомым
		{
			if (a + 1 == index && curr->next->next != nullptr)	// Если следующий элемент не является последним
			{
				del = curr->next;
				curr->next = curr->next->next;			// То переприсвоить указатели и удалить элемент

				node* tmp = curr->next->next; tmp->prev = curr;

				delete del;
				countElements--;

				return true;
			}
			else if (a + 1 == index)	// Если следующий элемент последний
			{
				del = curr->next;
				curr->next = nullptr;	// То переприсвоить указатель и удалить элемент

				delete del;
				countElements--;

				return true;
			}
			else						// Иначе идти дальше по списку
			{
				curr = curr->next;
				a++;
			}
		}
		return false;
	}
}

/*
* Удаление текущего элемента двунаправленного списка
*/
bool list::PopForward()
{
	node* del;

	if (head == nullptr)
	{
		std::cout << "List is empty" << std::endl;
		throw("empty list");
	}
	else
	{
		if (head->next == nullptr && head->prev == nullptr)		// В списке один элемент
		{
			delete head;
			head = nullptr;
			countElements = 0;

			return true;
		}
		else if (head->prev)				// Голова списка не находится в начале
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
* Метод очищения списка
*/
void list::Clear()
{
	node* del;
	if (head == nullptr) { return; }
	else
	{
		while (head->next)		// Цикл удаляющий элементы списка, пока за ними есть следующий
		{
			del = head;
			head = head->next;
			delete del;
		}
		delete head;			// Удаление последнего элемента
	}
	head = nullptr;
	countElements = 0;
}

/*
* Переводит указатель головы списка в изначальное состояние
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
* Перегрузка оператора квадратных скобок, который возвращяет указатель на элемент
*/
line* list::operator[](const unsigned int index)
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

/*
* Вывод списка в консоль
*/
void list::Print()
{
	if (head != nullptr && head->prev)		// Если Голова списка не в изначальном положение
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
			for (auto i = 0; i < 5; i++)
			{
				std::cout << curr->val.dots[i].GetX()
					<< ' ' << curr->val.dots[i].GetY() << ' ';
			}
			std::cout << '\n';
			curr = curr->next;
		}
		for (auto i = 0; i < 5; i++)
		{
			std::cout << curr->val.dots[i].GetX()
				<< ' ' << curr->val.dots[i].GetY() << ' ';
		}
		std::cout << '\n';
		curr = curr->next;
	}
}

bool operator==(const line& arg_1, const line& arg_2)
{
	if (arg_1.dots[0] == arg_2.dots[0] &&
		arg_1.dots[4] == arg_2.dots[4])
	{
		return true;
	}
	/*for (auto i = 0; i < 5; i++)
	{
		for (auto j = 0; j < 5; j++)
		{
			if (arg_1.dots[i] == arg_2.dots[j])
			{
				return true;
			}
		}
	}*/
	return false;
}
