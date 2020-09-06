/*==== include ====*/
#include"Manager.h"

/*==== function ====*/
void Game();

void Game()
{
	Board_State current_turn = Board_State::Black;
	while (!main_board->is_full())
	{
		main_board->search_candidate(current_turn);
		main_board->print_board();
		switch (current_turn)
		{
		case Board_State::Black:
			printf("BlackTurn\n");
			break;
		case Board_State::White:
			printf("WhiteTurn\n");
			break;
		default:
			printf("Error!\n");
			break;
		}
		int x, y;
		printf("À•W“ü—Í (x,y)\n");
		std::cin >> x;
		std::cin >> y;
		if ((x == -1 && y == -1)||(main_board->m_candidate_size==0))
		{
			printf("Pass!\n");
			current_turn = next_turn(current_turn);
			continue;
		}
		Pos2 input_pos(x,y);
		if (!main_board->enable_put(x, y, current_turn))
		{
			continue;
		}
		current_turn = next_turn(current_turn);
	}
	main_board->print_board();
	current_state = Scene_State::Result_State;
}