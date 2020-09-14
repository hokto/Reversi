#pragma once
/*==== include ====*/
#include"GameAI.h"
#include<fstream>
#include<time.h>
#include<string>
#include<vector>

/*==== const ====*/
const double ALPHA = 0.25;
const double GAMMA = 0.85;
const int EPSILON = 20;
const double INIT_WEIGHT = 0.1;
const int EPISODE = 10000;
const int DEPTH = 1;

class Q_Learning : public GameAI
{
private:
	Board* m_learning_board;
	Board_State m_turn;
	GameAI* m_learning_enemy;
	double* feature_vector;
	double* weight;
public:
	Q_Learning();
	~Q_Learning();
	Pos2 run(Board* board, Board_State ai_turn);
	void learning();
	int get_q_max_index();
	double get_q_max_value(int depth);
	void step(int episode);
	double cal_q_value();
	void update_feature_vector();
};