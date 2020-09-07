/*==== include ====*/
#include"RandomAI.h"
#include<time.h>

Pos2 RandomAI::run(Board* board, Board_State ai_turn)
{
	srand((unsigned int)time(NULL));
	int select_idx = rand() % board->m_candidate_size;
	Pos2 ret_pos = to_pos2(board->get_candidate(select_idx));
	return ret_pos;
}