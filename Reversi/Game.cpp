/*==== include ====*/
#include"Manager.h"

/*==== function ====*/
void Game();

void Game()
{
	Board_State current_turn = Board_State::Black;
	while (!main_board->is_full())
	{
		main_board->print_board();
		int x, y;
		printf("À•W“ü—Í (x,y)\n");
		std::cin >> x;
		std::cin >> y;
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