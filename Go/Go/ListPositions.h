#pragma once
#include "Point.h"

class listPositions
{
private:
	/*
	* Класс узел, недоступный за границами методов класса,
	* включающий в себя информацию о точке, указатель на предыдущий/следующий элемент
	* и конструктор с параметрами по умолчанию
	*/
	class node
	{
	public:
		point val;
		node* prev, *next;

		node(const int& x = 0, const int& y = 0, node* next = nullptr):
			next(next), prev(nullptr), val(x,y) {}
	};
	
	node* head;
	unsigned int countElements;
public:
	listPositions();
	listPositions(const listPositions& arg);
	~listPositions();


	int GetCountOfElements() const { return countElements; }

	bool PushForward(const int x, const int y);

	bool PopForward(const unsigned int& id_Elem);
	bool PopForward();
	void Clear();

	bool IsEmpty() const { return head == nullptr; }
	void ResetList();

	point* operator[] (const unsigned int index);
	point& GetPoint();
};