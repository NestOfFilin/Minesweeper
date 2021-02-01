#pragma once

#include "MinesweeperStep.h"

class MinesweeperRoundHistory
{
public:
	MinesweeperRoundHistory();
	~MinesweeperRoundHistory();

private:
	MinesweeperStep** m_pMsSteps;
};
