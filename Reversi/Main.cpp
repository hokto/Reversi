/*==== include ====*/
#include"Manager.h"
#include"Board.h"
#include"Start.h"
#include"Game.h"
#include"Result.h"

/*==== function ====*/
int to_pos1(int x, int y);
Board_State next_turn(Board_State current_turn);
void UpDate();
void Initialize();
void Finalize();

Scene_State current_state = Scene_State::Initialize_State;
Board* main_board;

const int using_memory_size = 1 << 20;
void* using_memory = new void* [using_memory_size];
iMemoryAllocator* im = new Allocator(using_memory, using_memory_size);


int to_pos1(int x, int y)
{
	return BOARD_HEIGHT * y + x;
}

Board_State next_turn(Board_State current_turn)
{
	if (current_turn == Board_State::White)
	{
		return Board_State::Black;
	}
	else
	{
		return Board_State::White;
	}
}

void UpDate()
{
	using Scene = Scene_State;
	switch (current_state)
	{
	case Scene::Initialize_State:
		Initialize();
		break;
	case Scene::Start_State:
		Start();
		break;
	case Scene::Game_State:
		Game();
		break;
	case Scene::Result_State:
		Result();
		break;
	case Scene::Finalize_State:
		Finalize();
		break;
	default:
		current_state = Scene::End_State;
		break;
	}
}

void Initialize()
{
	main_board = new Board;
	current_state = Scene_State::Start_State;
}

void Finalize()
{
	delete main_board;
	//Œã‚É•ÏX
	current_state = Scene_State::End_State;
}
int main()
{
	do
	{
		UpDate();
	} while (!(current_state == Scene_State::End_State));
	delete using_memory;
	delete im;
	return 0;
}