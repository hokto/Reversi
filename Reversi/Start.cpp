/*==== include ====*/
#include"Manager.h"
#include"Start.h"

/*==== function ====*/
void Start();

void Start()
{
	printf("æU(0)HŒãU(1)H\n");
	int input_turn;
	std::cin >> input_turn;
	if (input_turn == 0)
	{
		player_turn = Board_State::Black;
	}
	else
	{
		player_turn = Board_State::White;
	}
	current_state = Scene_State::Game_State;
}