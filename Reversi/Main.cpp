/*==== include ====*/
#include"Manager.h"
#include"Start.h"
#include"Game.h"
#include"Result.h"

/*==== function ====*/
int to_pos1(int x, int y);
void UpDate();
void Initialize();
void Finalize();

Scene_State current_state = Scene_State::Initialize_State;
int* board;

const int using_memory_size = 1 << 20;
void* using_memory = new void* [using_memory_size];
iMemoryAllocator* im = new Allocator(using_memory, using_memory_size);


int to_pos1(int x, int y)
{
	return BOARD_HEIGHT * y + x;
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
	//盤面を一次元配列で動的に確保
	board = (int*)im->Alloc(BOARD_HEIGHT * BOARD_WIDTH);

	//盤面初期化
	for (int y = 0;y < BOARD_HEIGHT;y++)
	{
		for (int x = 0;x < BOARD_WIDTH;x++)
		{
			board[to_pos1(x,y)] = Board_State::None;
		}
	}
	board[to_pos1(BOARD_WIDTH / 2 - 1, BOARD_HEIGHT / 2 - 1)] = Board_State::White;
	board[to_pos1(BOARD_WIDTH / 2 - 1, BOARD_HEIGHT / 2)] = Board_State::Black;
	board[to_pos1(BOARD_WIDTH / 2, BOARD_HEIGHT / 2 - 1)] = Board_State::Black;
	board[to_pos1(BOARD_WIDTH / 2, BOARD_HEIGHT / 2)] = Board_State::White;
	current_state = Scene_State::Start_State;
}

void Finalize()
{
	for (int y = 0;y < BOARD_HEIGHT;y++)
	{
		for (int x = 0;x < BOARD_WIDTH;x++)
		{
			std::cout << board[to_pos1(x,y)] << " ";
		}
		std::cout<<std::endl;
	}
	im->Free(board);
	//後に変更
	current_state = Scene_State::End_State;
}
int main()
{
	do
	{
		UpDate();
	} while (!(current_state == Scene_State::End_State));
	return 0;
}