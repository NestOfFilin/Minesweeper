#include "MineCell.h"

MineCell::MineCell(unsigned char countOfMineAround)
	: _countOfMinesAround(countOfMineAround) {  }

unsigned char MineCell::Open()
{
	if (_state == MineCellState::MarkedCell)
		return (unsigned char)10;
	else
	{
		_state = MineCellState::OpenedCell;
		return _countOfMinesAround;
	}
}

MineCellState MineCell::Mark()
{
	if (_state != MineCellState::OpenedCell)
		_state = (_state == MineCellState::CloseCell)
				? MineCellState::MarkedCell
				: MineCellState::CloseCell;
	return _state;
}

MineCellState MineCell::GetStatus() { return _state; }

unsigned char MineCell::GetCountsOfMinesAround()
{
	return _countOfMinesAround;
}

unsigned char MineCell::GetCountsOfMarksAround()
{
	return _countsOfMarksAround;
}

unsigned char MineCell::SetCountsOfMarksAround(unsigned char newCountsOfMarksAround)
{
	if (newCountsOfMarksAround > 8) return (unsigned char)10;
	_countsOfMarksAround = newCountsOfMarksAround;
	return _countsOfMarksAround;
}