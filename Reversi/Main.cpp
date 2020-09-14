/*==== include ====*/
#include"Manager.h"
#include"Board.h"
#include"Start.h"
#include"Game.h"
#include"Result.h"

/*==== function ====*/
int to_pos1(int x, int y);
Pos2 to_pos2(int pos1);
Board_State next_turn(Board_State current_turn);
void UpDate();
void Initialize();
void Finalize();
void create_window(int width, int height);
void set_font_size(int x, int y);
void disable_cursor();
void enable_escape_sequence();


Scene_State current_state = Scene_State::Initialize_State;
Board* main_board;

const int using_memory_size = 1 << 25;
void* using_memory = new void* [using_memory_size];
iMemoryAllocator* im = new Allocator(using_memory, using_memory_size);
Board_State player_turn=Board_State::Black;

int to_pos1(int x, int y)
{
	return BOARD_WIDTH * y + x;
}

Pos2 to_pos2(int pos1)
{
	int x = pos1 % BOARD_WIDTH;
	int y = pos1 / BOARD_WIDTH;
	Pos2 ret_pos(x, y);
	return ret_pos;
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
	//後に変更
	current_state = Scene_State::End_State;
}

//サイズを指定したコンソールウィンドウ作成
void create_window(int width, int height)
{
	HWND console = GetConsoleWindow();
	RECT rect;
	GetWindowRect(console, &rect);
	MoveWindow(console, rect.left, rect.top, width, height, TRUE);
}


//フォント変更(コンソールウィンドウも変更される）
void set_font_size(int x, int y)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	PCONSOLE_FONT_INFOEX console_current_font = new CONSOLE_FONT_INFOEX();
	console_current_font->cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(handle, 0, console_current_font);
	console_current_font->dwFontSize.X = x;
	console_current_font->dwFontSize.Y = y;
	SetCurrentConsoleFontEx(handle, 0, console_current_font);
	delete console_current_font;
}


//カーソル非表示
void disable_cursor()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(handle, &cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &cci);
}

//エスケープシーケンス実行可能にする
void enable_escape_sequence()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD console_mode;
	GetConsoleMode(handle, &console_mode);
	console_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(handle, console_mode);
}


int main()
{
	enable_escape_sequence();
	//disable_cursor();
	do
	{
		UpDate();
	} while (!(current_state == Scene_State::End_State));
	delete using_memory;
	delete im;
	return 0;
}