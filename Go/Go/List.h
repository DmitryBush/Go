#pragma once
#include "Point.h"

class list
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
		point* val;
		node* prev, *next;

		node(point beg, point end, node* next = nullptr):
			next(next), prev(nullptr) 
		{
			val = new point[2];
			val[0].SetPoint(beg.GetX(),beg.GetY()); val[1].SetPoint(end.GetX(), end.GetY());
		}
	};
	
	node* head;
	unsigned int countElements;
public:
	list();
	~list();


	int GetCountOfElements() const { return countElements; }

	bool PushForward(point beg, point end);

	bool PopForward(const unsigned int& id_Elem);
	bool PopForward();
	void Clear();

	bool IsEmpty() const { return head == nullptr; }
	void ResetList();

	point* operator[] (const unsigned int index);

	void Task();
	void Print();
};