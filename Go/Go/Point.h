#pragma once

class point
{
private:
	int x, y;
public:
	/*
	*  онструктор с параметрами по умолчанию
	*/
	point(int x = 0, int y = 0) : x(x), y(y) {}

	int GetX() const;
	int GetY() const;

	void EditX();
	void EditY();
	void SetPoint(int x, int y);
	void ShowPoint();

};

bool operator==(point arg_1, point arg_2);