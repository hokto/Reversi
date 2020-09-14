/*==== include ====*/
#include"Manager.h"
#include"GameAI.h"
#include"RandomAI.h"
#include"Q_Learning.h"
/*==== function ====*/
void Game();

void Game()
{
	Board_State current_turn = Board_State::Black;
	GameAI* ai;
	//ai = new RandomAI;
	ai = new Q_Learning;
	ai->learning();
	current_state = Scene_State::Result_State;
	return;
	do
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
		if (main_board->m_candidate_size == 0)
		{
			//互いにどこにも置けないならゲームを終了する
			main_board->search_candidate(next_turn(current_turn));
			if (main_board->m_candidate_size == 0)
			{
				break;
			}
			else
			{
				printf("Pass!\n");
				current_turn = next_turn(current_turn);
				continue;
			}
		}
		Pos2 select_pos;
		if (player_turn != current_turn)
		{
			select_pos = ai->run(main_board,current_turn);
		}
		else
		{
			int x = -1, y = -1;
			printf("座標入力 (x,y)\n");
			std::cin >> x;
			std::cin >> y;
			if (x == -1 && y == -1)
			{
				printf("Pass!\n");
				current_turn = next_turn(current_turn);
				continue;
			}
			Pos2 input_pos(x, y);
			select_pos = input_pos;
		}
		if (!main_board->enable_put(select_pos.get_X(), select_pos.get_Y(), current_turn))
		{
			continue;
		}
		current_turn = next_turn(current_turn);
	}while (!main_board->is_full());
	main_board->search_candidate(current_turn);
	main_board->print_board();
	current_state = Scene_State::Result_State;
	delete ai;
}