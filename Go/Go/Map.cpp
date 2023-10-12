#include "Map.h"
#include <iostream>

bool map::AllocateMemory()
{
	field = new char* [width];

	for (int i = 0; i < width; i++)
	{
		field[i] = new char[height]{'0'};
	}

	return true;
}

bool map::EmptyCheck(const int x_elem, const int y_elem) const
{
	if ((x_elem >= 0 && x_elem < width) &&
		(y_elem >= 0 && y_elem < height))
	{
		if (field[x_elem][y_elem] == '0')
			return true;
	}
	return false;
}

map::map(const unsigned int& width, const unsigned int& height): 
	width(width), height(height)
{
	AllocateMemory();

	ResetField();
}

map::~map()
{
	if (field)
	{
		for (int i = 0; i < width; i++)
			delete field[i];

		delete[] field;
	}
}

bool map::SetStone(const int& x_coord, const int& y_coord, const char& stone)
{
	if ((x_coord >= 0 && x_coord < width) && (y_coord >= 0 && y_coord < height))
	{
		field[x_coord][y_coord] = stone;
		return true;
	}
	return false;
}

void map::MoveStone(const int& PrevX_coord, const int& PrevY_coord, 
	const int& x_coord, const int& y_coord)
{
	if ((x_coord >= 0 && x_coord < width) && (y_coord >= 0 && y_coord < height) &&
		(PrevX_coord >= 0 && PrevX_coord < width) && (PrevY_coord >= 0 && PrevY_coord < height))
		if (PrevX_coord != x_coord || PrevY_coord != y_coord)
		{
			field[x_coord][y_coord] = field[PrevX_coord][PrevY_coord];
			field[PrevX_coord][PrevY_coord] = '0';
		}
}

const char& map::GetCell(const int& x_coord, const int& y_coord) const
{
	return field[x_coord][y_coord];
}

void map::ResetField()
{
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			field[i][j] = '0';
}
