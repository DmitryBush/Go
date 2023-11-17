#pragma once
#include "Point.h"

class line
{
public:
	point dots[5];

	line(point first, point second, point third, point fourth,
		point five)
	{
		dots[0].SetPoint(first.GetX(), first.GetY());
		dots[1].SetPoint(second.GetX(), second.GetY());
		dots[2].SetPoint(third.GetX(), third.GetY());
		dots[3].SetPoint(fourth.GetX(), fourth.GetY());
		dots[4].SetPoint(five.GetX(), five.GetY());
	}

	line()
	{
		dots[0].SetPoint(0, 0);
		dots[1].SetPoint(0, 0);
		dots[2].SetPoint(0, 0);
		dots[3].SetPoint(0, 0);
		dots[4].SetPoint(0, 0);
	}
};
bool operator==(const line& arg_1, const line& arg_2);

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
		line val;
		node* prev, *next;

		node(point first, point second, point third,
			point fourth, point five, node* next = nullptr) :
			next(next), prev(nullptr), val(first, second, third, fourth, five) {}

		node(line points, node* next = nullptr) :
			next(next), prev(nullptr)
		{
			for (auto i = 0; i < 5; i++)
			{
				val.dots[i].SetPoint(points.dots[i].GetX(), points.dots[i].GetY());
			}
		}
	};
	
	node* head;
	unsigned int countElements;
public:
	list();
	~list();


	int GetCountOfElements() const { return countElements; }

	/*bool PushForward(point first, point second, point third,
		point fourth, point five);*/
	bool PushForward(line dots);

	bool PopForward(const unsigned int& id_Elem);
	bool PopForward();
	void Clear();

	bool IsEmpty() const { return head == nullptr; }
	void ResetList();

	line* operator[] (const unsigned int index);

	void Print();
};