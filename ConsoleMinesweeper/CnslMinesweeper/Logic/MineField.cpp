#include "MineField.h"

#include <iostream>
#include <ctime>

MineField::MineField(unsigned char width, unsigned char heigth, unsigned int mineCount)
	: _width(width), _heigth(heigth), _mineCount(mineCount), 
	_cellCount(width* heigth)
{
	unsigned char** fieldTemp = GenerateMineLoyoutTmp();
	
	_field = new MineCell**[heigth];
	for (unsigned char y = 0u; y < heigth; y++)
	{
		_field[y] = new MineCell*[width];
		for (unsigned char x = 0u; x < width; x++)
		{
			_field[y][x] = new MineCell(fieldTemp[y][x]);
		}
	}

	DestroyMineLoyoutTmp(fieldTemp);
}

MineField::~MineField()
{
	for (unsigned char y = 0u; y < _heigth; y++)
	{
		for (unsigned char x = 0u; x < _width; x++)
		{
			delete _field[y][x];
		}
		delete[] _field[y];
	}
	delete[] _field;
}

stack<MineCell*>* MineField::GetAroundCellsStack(unsigned char x, unsigned char y)
{
	if (y >= _heigth || x >= _width) return nullptr;

	stack<MineCell*>* aroundCellsStack = new stack<MineCell*>();
	bool isBottom = (y == _heigth - 1u), isTop = (y == 0u),
		isRight = (x == _width - 1u), isLeft = (x == 0u);

	aroundCellsStack->push((!isBottom && !isRight) ? _field[y + 1u][x + 1u] : nullptr);
	aroundCellsStack->push(!isBottom ? _field[y + 1u][x] : nullptr);
	aroundCellsStack->push((!isBottom && !isLeft) ? _field[y + 1u][x - 1u] : nullptr);
	
	aroundCellsStack->push(!isRight ? _field[y][x + 1u] : nullptr);
	aroundCellsStack->push(!isLeft ? _field[y][x - 1u] : nullptr);

	aroundCellsStack->push((!isTop && !isRight) ? _field[y - 1u][x + 1u] : nullptr);
	aroundCellsStack->push(!isTop ? _field[y - 1u][x] : nullptr);
	aroundCellsStack->push((!isTop && !isLeft) ? _field[y - 1u][x - 1u] : nullptr);
	return aroundCellsStack;
}

MineCellState MineField::operator() (unsigned char x, unsigned char y)
{
	return _field[y][x]->GetStatus();
}

OpenedMineCells* MineField::OpenCell(unsigned char x, unsigned char y)
{
	// проверку на корректность координат
	OpenedMineCells* cells = nullptr;
	if (_field[y][x]->GetStatus() == MineCellState::CloseCell)
	{
		unsigned char mineCount = _field[y][x]->Open();
		cells = new OpenedMineCells;
		cells->CountOfMineAround = mineCount;
		cells->x = x;
		cells->y = y;
		cells->Next = nullptr;

		// здесь после смены состо€ни€ нет немедленного выхода из функции
		// т.к. последн€€ открыта€ клетка может оказатьс€ с миной
		if (++_openedCellCount + _mineCount == _cellCount)
			_state = MineFieldState::Cleaned;

		switch (mineCount)
		{
		case 0: cells->Next = OpenCellsAround(x, y); break;
		case 9: _state = MineFieldState::Undermined; break;
		}
	}
	return cells;
}

MineCellState MineField::MarkCell(unsigned char x, unsigned char y)
{
	// проверку на корректность координат
	MineCellState resCellState = _field[y][x]->Mark();

	unsigned char dMarksCount = resCellState == MineCellState::MarkedCell ? 1
		: resCellState == MineCellState::CloseCell ? -1 : 0;
	_markCount += dMarksCount;

	if (dMarksCount != 0)
	{
		stack<MineCell*>* aroundCellsStack = GetAroundCellsStack(x, y);

		while (aroundCellsStack->size() != 0)
		{
			MineCell* cell = aroundCellsStack->top();
			if (cell != nullptr)
				cell->SetCountsOfMarksAround(cell->GetCountsOfMarksAround() + dMarksCount);
			aroundCellsStack->pop();
		}
		delete aroundCellsStack;
	}

	return resCellState;
}

OpenedMineCells* MineField::OpenCellsAround(unsigned char x, unsigned char y)
{
	// проверку на корректность координат
	OpenedMineCells *cells = nullptr, *tailCell = cells;

	// гарантированно вернетс€ стек из 8 элементов
	stack<MineCell*>* aroundCellsStack = GetAroundCellsStack(x, y);
	for (short i = -1; i <= 1; i++)
		for (short j = -1; j <= 1; j++)
		{
			if (i == 0 && j == 0) continue;

			MineCell* cell = aroundCellsStack->top();
			if (cell != nullptr)
			{
				OpenedMineCells* openedCells = OpenCell(x + j, y + i);
				if (openedCells != nullptr)
				{
					if (cells == nullptr)
					{
						cells = openedCells;
						tailCell = cells;
					}
					else
						tailCell->Next = openedCells;

					while (tailCell->Next != nullptr)
						tailCell = tailCell->Next;
				}
			}
			aroundCellsStack->pop();
		}
	delete aroundCellsStack;
	return cells;
}

unsigned MineField::GetMarkCount() { return _markCount; }

MineFieldState MineField::GetStatus() { return _state; }

unsigned char** MineField::GenerateMineLoyoutTmp()
{
	srand((unsigned)time(0));
	unsigned char** fieldTemp = new unsigned char* [_heigth];

	for (unsigned char y = 0u; y < _heigth; y++)
	{
		fieldTemp[y] = new unsigned char[_width];
		for (unsigned char x = 0u; x < _width; x++)
		{
			fieldTemp[y][x] = 0u;
		}
	}

	for (unsigned i = 0; i < _mineCount;)
	{
		unsigned char xMineCrd = rand() % 255u % _width;
		unsigned char yMineCrd = rand() % 255u % _heigth;

		if (fieldTemp[yMineCrd][xMineCrd] != 9u)
		{
			fieldTemp[yMineCrd][xMineCrd] = 9u;

			bool isBottom = yMineCrd == _heigth - 1u,
				isTop = yMineCrd == 0u,
				isRight = xMineCrd == _width - 1u,
				isLeft = xMineCrd == 0u;

			if (!isBottom)
			{
				if (!isLeft && fieldTemp[yMineCrd + 1u][xMineCrd - 1u] != 9u) 
					fieldTemp[yMineCrd + 1u][xMineCrd - 1u]++;
				if (!isRight && fieldTemp[yMineCrd + 1u][xMineCrd + 1u] != 9u) 
					fieldTemp[yMineCrd + 1u][xMineCrd + 1u]++;
				if (fieldTemp[yMineCrd + 1u][xMineCrd] != 9u)
					fieldTemp[yMineCrd + 1u][xMineCrd]++;
			}
			if (!isTop)
			{
				if (!isLeft && fieldTemp[yMineCrd - 1u][xMineCrd - 1u] != 9u) 
					fieldTemp[yMineCrd - 1u][xMineCrd - 1u]++;
				if (!isRight && fieldTemp[yMineCrd - 1u][xMineCrd + 1u] != 9u) 
					fieldTemp[yMineCrd - 1u][xMineCrd + 1u]++;
				if (fieldTemp[yMineCrd - 1u][xMineCrd] != 9u)
					fieldTemp[yMineCrd - 1u][xMineCrd]++;
			}
			if (!isLeft && fieldTemp[yMineCrd][xMineCrd - 1u] != 9u) 
				fieldTemp[yMineCrd][xMineCrd - 1u]++;
			if (!isRight && fieldTemp[yMineCrd][xMineCrd + 1u] != 9u) 
				fieldTemp[yMineCrd][xMineCrd + 1u]++;

			i++;
		}
	}
	return fieldTemp;
}

void MineField::DestroyMineLoyoutTmp(unsigned char** &fieldTemp)
{
	for (unsigned char y = 0u; y < _heigth; y++)
	{
		delete[] fieldTemp[y];
	}
	delete[] fieldTemp;
}
