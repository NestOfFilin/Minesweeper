#pragma once

#include <stack>
#include <map>

using std::stack;
using std::map;

#include "MineCell.h"

enum MineFieldState
{
	Cleaned = 0,
	Undermined = 1,
	Razminirovivaetsya = 2
};


struct OpenedMineCells
{
	unsigned char CountOfMineAround, x, y;
	OpenedMineCells* Next;
};

struct Coord
{
	unsigned char x, y;
};

class MineField
{
private:
	MineCell*** 
		_field;
	unsigned int 
		_cellCount, 
		_mineCount, 
		_markCount = 0, 
		_openedCellCount = 0;
	unsigned char 
		_width, 
		_heigth;
	MineFieldState 
		_state = MineFieldState::Razminirovivaetsya;

private:
	unsigned char** GenerateMineLoyoutTmp();
	void DestroyMineLoyoutTmp(unsigned char**& fieldTemp);

	stack<MineCell*>* GetAroundCellsStack(unsigned char x, unsigned char y);
public:
	MineField(unsigned char width, unsigned char heigth, unsigned int mineCount);
	~MineField();

	MineCellState operator() (unsigned char x, unsigned char y);

	OpenedMineCells* OpenCell(unsigned char x, unsigned char y);
	OpenedMineCells* OpenCellsAround(unsigned char x, unsigned char y);

	MineCellState MarkCell(unsigned char x, unsigned char y);

	MineFieldState GetStatus();
	unsigned int GetMarkCount();
};
