#include "HistComb.h"

histComb::histComb()
{
}

/*
* ����� ��� �������� �������
*/
void histComb::Clear()
{
	for (auto i = 0; i < 4; i++)
	{
		combinations[i].Clear();
	}
}

/*
* ����� ��� ��������� ������� � ������� ����������
*/
list& histComb::GetList(ListOrient orient)
{
	switch (orient)
	{
	case horizontal:
		return combinations[0];
		break;
	case vertical:
		return combinations[1];
		break;
	case mainDiagonal:
		return combinations[2];
		break;
	case sideDiagonal:
		return combinations[3];
		break;
	default:
		throw("Invalid index");
		break;
	}
}
