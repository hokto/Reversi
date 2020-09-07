#pragma once
/*==== include ====*/
#include<iostream>
#include"Board.h"
#include"iMemoryAllocator.h"
#include"Allocator.h"

//後に可変にするかも
const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 10;
//画面遷移用ステートマシーン
enum class Scene_State
{
	Initialize_State,
	Start_State,
	Game_State,
	Result_State,
	Finalize_State,
	End_State,
};


//座標管理
class Pos2
{
private:
	int m_x;
	int m_y;
public:
	Pos2()
	{
		m_x = -1;
		m_y = -1;
	}
	Pos2(int x, int y)
	{
		m_x = x;
		m_y = y;
	}

	int get_X()
	{
		return m_x;
	}
	int get_Y()
	{
		return m_y;
	}

	Pos2 operator=(Pos2 rhs)
	{
		this->m_x = rhs.m_x;
		this->m_y = rhs.m_y;
		return *this;
	}

	Pos2 operator+(Pos2 lhs)
	{
		Pos2 temp_pos;
		temp_pos.m_x = this->m_x+lhs.m_x;
		temp_pos.m_y = this->m_y+lhs.m_y;
		return temp_pos;
	}

	Pos2 operator-(Pos2 lhs)
	{
		Pos2 temp_pos;
		temp_pos.m_x = this->m_x-lhs.m_x;
		temp_pos.m_y = this->m_y-lhs.m_y;
		return temp_pos;
	}

	bool is_between()
	{
		return (0 <= m_x && m_x <= BOARD_WIDTH && 0 <= m_y && m_y <= BOARD_HEIGHT);
	}
};

//盤面状態
enum class Board_State
{
	None,
	Black,
	White,
};

class Board
{
private:
	int* m_candidate_pos;
public:
	Board();
	~Board();
	Board_State* m_board;
	int m_candidate_size;
	bool is_full();
	bool enable_reverse(Pos2 search_pos, Pos2 select_direction, Board_State turn);
	bool enable_put(int x, int y, Board_State turn);
	void print_board();
	void search_candidate(Board_State turn);
	int get_candidate(int idx);
};

bool is_enemy_piece(Board_State my_state, Board_State target_state);

const Pos2 Left(-1, 0);
const Pos2 Right(1, 0);
const Pos2 Up(0, -1);
const Pos2 Down(0, 1);
const Pos2 Left_Up(-1,-1);
const Pos2 Left_Down(-1,1);
const Pos2 Right_Up(1,-1);
const Pos2 Right_Down(1,1);
const Pos2 direction[8] = { Left,Right,Up,Down,Left_Up,Left_Down,Right_Up,Right_Down };

//二次元の座標から、一次元の座標空間に変換
int to_pos1(int x, int y);

//一次元の座標から、二次元の座標空間に変換
Pos2 to_pos2(int pos1);
Board_State next_turn(Board_State current_turn);
extern Scene_State current_state;

//動的に確保する
extern Board* main_board;

extern iMemoryAllocator* im; 

extern Board_State player_turn;