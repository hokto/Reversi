#pragma once
#include"Manager.h"
class GameAI
{
public:
	virtual ~GameAI() {}
	virtual Pos2 run(Board* board, Board_State ai_turn)=0;
};