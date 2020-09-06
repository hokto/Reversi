#pragma once
/*==== include ====*/
#include<iostream>



//��ɉςɂ��邩��
const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 10;
//��ʑJ�ڗp�X�e�[�g�}�V�[��
enum class Scene_State : int
{
	Initialize_State,
	Start_State,
	Game_State,
	Result_State,
	Finalize_State,
	End_State,
};

//�Ֆʏ��
enum class Board_State : int
{
	None,
	Black,
	White,
};

//���W�Ǘ�
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
	Pos2(int _x, int _y)
	{
		m_x = _x;
		m_y = _y;
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
		Pos2 temp_pos;
		temp_pos.m_x = rhs.m_x;
		temp_pos.m_y = rhs.m_y;
		return temp_pos;
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
};

extern Scene_State current_state;