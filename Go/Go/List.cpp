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
		{	// Попытка выделения памяти под узел с переданным параметром 
			// и его связь со списком
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
		return false;
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
* Задание 4:
* В списке А размера N необходимо каждый элемент заменить на ближайший следующий за ним элемент, 
* который больше его. Если такого элемента нет, то заменить его на ноль.
*/
void list::Task()
{
	if (head)
	{
		if (head->prev)		// Если Голова списка не в изначальном положение
			ResetList();

		node* curr = head;
		bool foundElem = false;
		while (curr->next)
		{
			node* next = curr;
			while (next->next)		// Поиск элемента, который будет больше текущего
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

			if (!foundElem)			// По условию задачи, если элемент не будет найден
				curr->val = 0;	// То он заменяется нулем

			foundElem = false;
			curr = curr->next;
		}
		curr->val = 0;		// Последний элемент обязательно будет нулем
	}
	else
	{
		std::cout << "List is empty" << '\n';
	}
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
			std::cout << curr->val[0].GetX() << ' ' << curr->val[0].GetY() << ' '
				<< curr->val[1].GetX() << ' ' << curr->val[1].GetY() << '\n';
			curr = curr->next;
		}
		std::cout << curr->val[0].GetX() << ' ' << curr->val[0].GetY() << ' '
			<< curr->val[1].GetX() << ' ' << curr->val[1].GetY() << '\n';
	}
}
