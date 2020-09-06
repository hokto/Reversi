/*==== include ====*/
#include"Board.h"


/*==== function ====*/
bool is_enemy_piece(Board_State my_state, Board_State target_state);


bool is_enemy_piece(Board_State my_state, Board_State target_state)
{
	if (my_state == Board_State::None || target_state == Board_State::None)
	{
		return false;
	}
	else if (my_state == target_state)
	{
		return false;
	}
	else
	{
		return true;
	}
}

Board::Board()
{
	//”Õ–Ê‚ğˆêŸŒ³”z—ñ‚Å“®“I‚ÉŠm•Û
	m_board = (Board_State*)im->Alloc(BOARD_HEIGHT * BOARD_WIDTH*sizeof(Board_State));
	//”Õ–Ê‰Šú‰»
	for (int y = 0;y < BOARD_HEIGHT;y++)
	{
		for (int x = 0;x < BOARD_WIDTH;x++)
		{
			m_board[to_pos1(x, y)] = Board_State::None;
		}
	}
	m_board[to_pos1(BOARD_WIDTH / 2 - 1, BOARD_HEIGHT / 2 - 1)] = Board_State::White;
	m_board[to_pos1(BOARD_WIDTH / 2 - 1, BOARD_HEIGHT / 2)] = Board_State::Black;
	m_board[to_pos1(BOARD_WIDTH / 2, BOARD_HEIGHT / 2 - 1)] = Board_State::Black;
	m_board[to_pos1(BOARD_WIDTH / 2, BOARD_HEIGHT / 2)] = Board_State::White;
}

Board::~Board()
{
	im->Free(m_board);
}

bool Board::is_full()
{
	for (int y = 0;y < BOARD_HEIGHT;y++)
	{
		for (int x = 0;x < BOARD_WIDTH;x++)
		{
			if (m_board[to_pos1(x,y)] == Board_State::None)
			{
				return false;
			}
		}
	}
	return true;
}

bool Board::enable_put(int x, int y,Board_State turn)
{
	if (m_board[to_pos1(x, y)] != Board_State::None)
	{
		return false;
	}
	Pos2 current_pos(x, y);
	bool is_put = false;
	for (Pos2 d : direction)
	{
		Pos2 next_pos = current_pos + d;
		if (next_pos.is_between() && is_enemy_piece(turn, m_board[to_pos1(next_pos.get_X(), next_pos.get_Y())]))
		{
			bool is_reversed = enable_reverse(next_pos+d,d,turn);
			if (is_reversed)
			{
				m_board[to_pos1(current_pos.get_X(), current_pos.get_Y())] = turn;
				m_board[to_pos1(next_pos.get_X(), next_pos.get_Y())] = turn;
			}
			if (!is_put)
			{
				is_put = is_reversed;
			}
		}
	}
	return is_put;
}

bool Board::enable_reverse(Pos2 search_pos, Pos2 select_direction, Board_State turn)
{
	int exchange_search_pos1 = to_pos1(search_pos.get_X(), search_pos.get_Y());
	if (m_board[exchange_search_pos1]==turn)
	{
		return true;
	}
	else
	{
		if (m_board[exchange_search_pos1] == Board_State::None)
		{
			return false;
		}
		Pos2 next_search_pos=search_pos+select_direction;
		bool is_reversed=enable_reverse(next_search_pos,select_direction,turn);
		if (is_reversed)
		{
			m_board[exchange_search_pos1] = turn;
		}
		return is_reversed;
	}
}

void Board::print_board()
{
	for (int y = 0;y < BOARD_HEIGHT;y++)
	{
		for (int x = 0;x < BOARD_WIDTH;x++)
		{
			switch (m_board[to_pos1(x, y)])
			{
			case Board_State::None:
				printf("*");
				break;
			case Board_State::Black:
				printf("B");
				break;
			case Board_State::White:
				printf("W");
				break;
			default:
				break;
			}
		}
		printf("\n");
	}
}