#pragma once

enum MineCellState
{
	CloseCell = 0,
	MarkedCell = 1,
	OpenedCell = 2
};

class MineCell
{
private:
	/*
	* countOfMineAround:
	*	counts of mines around current cell
	* countsOfFlagsAround:
	*	counts of flags around current cell
	*/
	unsigned char 
		_countOfMinesAround = (unsigned char)0,
		_countsOfMarksAround = (unsigned char)0;
	MineCellState
		_state = MineCellState::CloseCell;

public:
	MineCell(unsigned char countOfMineAround);
	~MineCell() {  }

	// open cell if it not marked
	// return _countOfMineAround if _state = CloseCell or OpenedCell else 10
	unsigned char Open();
	// marks or removes mark. return current mark state of cell
	MineCellState Mark();

	MineCellState GetStatus();
	unsigned char GetCountsOfMinesAround();
	unsigned char GetCountsOfMarksAround();
	//return newCountsOfFlagsAround or 10 if newCountsOfFlagsAround > 8
	unsigned char SetCountsOfMarksAround(unsigned char newCountsOfMarksAround);
};
