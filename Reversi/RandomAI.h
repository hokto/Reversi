#pragma once
#include"GameAI.h"
class RandomAI : public GameAI
{
public:
	RandomAI() {}
	~RandomAI() {}
	Pos2 run(Board* board, Board_State ai_turn);
	void learning(){}
};