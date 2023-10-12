#pragma once

class map
{
private:
	char** field;
	const unsigned int& width, & height;

	bool AllocateMemory();
public:
	map(const unsigned int& width, const unsigned int& height);
	~map();

	bool SetStone(const int& x_coord, const int& y_coord, const char& stone);
	void MoveStone(const int& PrevX_coord, const int& PrevY_coord,
		const int& x_coord, const int& y_coord);

	const char& GetCell(const int& x_coord, const int& y_coord) const;

	void ResetField();

	bool EmptyCheck(const int x_elem, const int y_elem) const;
};