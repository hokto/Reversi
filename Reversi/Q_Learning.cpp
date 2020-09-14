/*==== include ====*/
#include"Q_Learning.h"
#include"RandomAI.h"

void copy_board(Board_State* target_board, Board_State* cp_board);
//4x4に対応されているので、後に変更
const int q_max_size = BOARD_WIDTH * BOARD_HEIGHT*BOARD_WIDTH*BOARD_HEIGHT*3;

Q_Learning::Q_Learning()
{
	
}

Q_Learning::~Q_Learning()
{
	
}

Pos2 Q_Learning::run(Board* board, Board_State ai_turn)
{
	return Pos2(-1, -1);
}

void Q_Learning::learning()
{
	srand((unsigned int)time(NULL));
	//重みの初期化
	weight = (double*)im->Alloc(BOARD_WIDTH * BOARD_HEIGHT * sizeof(double));
	feature_vector = (double*)im->Alloc(BOARD_WIDTH * BOARD_HEIGHT * sizeof(double));
	for (int y = 0;y < BOARD_HEIGHT;y++)
	{
		for (int x = 0;x < BOARD_WIDTH;x++)
		{
			weight[to_pos1(x, y)] = INIT_WEIGHT;
		}
	}
	/*weight[0] = 2.54754;
	weight[1] = 5.39999;
	weight[2] = 2.91979;
	weight[3] = 17.8752;
	weight[4] = 6.49243;
	weight[5] = 0.1;
	weight[6] = 0.1;
	weight[7] = 2.28247;
	weight[8] = 1.26807;
	weight[9] = 0.1;
	weight[10] = 0.1;
	weight[11] = 9.08741;
	weight[12] = 13.2499;
	weight[13] = 0.920091;
	weight[14] = 15.6531;
	weight[15] = 4.32584;*/

	Board_State current_turn = Board_State::Black;
	m_learning_enemy = new RandomAI;
	m_turn = current_turn;
	int turn_num = 0;
	int win_num = 0;
	int accum_win_num = 0;
	std::vector<double> win_rates;
	double max_rate = 0.0;
	for (int episode = 0;episode < EPISODE;episode++)
	{
		m_learning_board = new Board;
		do
		{
			m_learning_board->search_candidate(current_turn);
			//m_learning_board->print_board();
			if (m_learning_board->m_candidate_size == 0)
			{
				//互いにどこにも置けないならゲームを終了する
				m_learning_board->search_candidate(next_turn(current_turn));
				if (m_learning_board->m_candidate_size == 0)
				{
					break;
				}
				else
				{
					current_turn = next_turn(current_turn);
					continue;
				}
			}
			if (current_turn == m_turn)
			{
				step(episode);
			}
			else
			{
				Pos2 enemy_pos=m_learning_enemy->run(m_learning_board, current_turn);
				if (enemy_pos.get_X()==-1||enemy_pos.get_Y()==-1||!m_learning_board->enable_put(enemy_pos.get_X(), enemy_pos.get_Y(), current_turn))
				{
					current_turn = next_turn(current_turn);
					continue;
				}
			}
			current_turn = next_turn(current_turn);
		} while (!m_learning_board->is_full());
		turn_num++;
		int game_num = (int)m_learning_board->get_reward(m_turn) > 0 ? 1 : 0;;
		win_num += game_num;
		accum_win_num += game_num;
		if (episode % 100 == 0)
		{
			double rate = (double)win_num / 100.0;
			win_rates.push_back(rate);
			win_num = 0;
			if (rate > max_rate)
			{
				max_rate = rate;
			}
		}
		std::cout << (double)accum_win_num / (double)EPISODE << std::endl;
		delete m_learning_board;
	}
	std::string file_name = "./weight_" + std::to_string(BOARD_WIDTH) + "x" +\
		std::to_string(BOARD_WIDTH) + "_max_"+std::to_string((int)(max_rate*100))+".txt";
	std::ofstream ofs(file_name);
	for (int y = 0;y < BOARD_HEIGHT;y++)
	{
		for (int x = 0;x < BOARD_WIDTH;x++)
		{
			std::cout << weight[to_pos1(x, y)] << " ";
			ofs << weight[to_pos1(x, y)];
			if (x != BOARD_WIDTH)
			{
				ofs << " ";
			}
		}
		std::cout << "\n";
		ofs << "\n";
	}
	for (int i=0;i<win_rates.size();i++)
	{
		std::cout << i << ":" << win_rates[i] << std::endl;
	}
	std::cout << "Max_Rate:" << max_rate << std::endl;
	delete m_learning_enemy;
	im->Free(feature_vector);
	im->Free(weight);
}


double Q_Learning::get_q_max_value(int depth)
{
	//最初の盤面の保存
	Board_State* prev_board = new Board_State[BOARD_WIDTH * BOARD_HEIGHT];
	copy_board(m_learning_board->m_board, prev_board);
	m_learning_board->search_candidate(m_turn);
	int* cp_candidate = new int[m_learning_board->m_candidate_size];
	int cp_candidate_size = m_learning_board->m_candidate_size;
	for (int i = 0;i < m_learning_board->m_candidate_size;i++)
	{
		cp_candidate[i] = m_learning_board->get_candidate(i);
	}
	if (m_learning_board->is_full())
	{
		update_feature_vector();
		return cal_q_value();
	}
	if (depth == 0)
	{
		update_feature_vector();
		double max_q_value = cal_q_value();
		for (int i = 0;i < cp_candidate_size;i++)
		{
			int candidate_idx = cp_candidate[i];
			Pos2 select_pos = to_pos2(candidate_idx);
			Board_State* cp_board = new Board_State[BOARD_WIDTH * BOARD_HEIGHT];
			copy_board(m_learning_board->m_board, cp_board);
			if (!m_learning_board->enable_put(select_pos.get_X(), select_pos.get_Y(), m_turn))
			{
				im->Free(cp_board);
				continue;
			}
			update_feature_vector();
			double q_value = cal_q_value();
			if (max_q_value < q_value)
			{
				max_q_value = q_value;
			}
			copy_board(cp_board,m_learning_board->m_board);
			delete cp_board;
		}
		return max_q_value;
	}
	//敵AIの実行
	Board_State enemy_turn = next_turn(m_turn);
	Pos2 enemy_pos = m_learning_enemy->run(m_learning_board, enemy_turn);
	//コマを置く(置けない場合もある)
	m_learning_board->enable_put(enemy_pos.get_X(), enemy_pos.get_Y(), enemy_turn);
	update_feature_vector();
	double max_q_value = cal_q_value();
	if (m_learning_board->is_full())
	{
		return max_q_value;
	}
	for (int i=0;i<cp_candidate_size;i+=cp_candidate_size/10+1)
	{
		int candidate_idx = cp_candidate[i];
		Board_State* cp_board = new Board_State[BOARD_WIDTH * BOARD_HEIGHT];
		copy_board(m_learning_board->m_board, cp_board);
		Pos2 select_pos = to_pos2(candidate_idx);
		if (!m_learning_board->enable_put(select_pos.get_X(), select_pos.get_Y(), m_turn))
		{
			delete cp_board;
			continue;
		}
		update_feature_vector();
		double q_value = get_q_max_value(depth - 1);
		if (max_q_value < q_value)
		{
			max_q_value = q_value;
		}
		copy_board(cp_board,m_learning_board->m_board);
		delete cp_board;
	}
	delete cp_candidate;
	copy_board(prev_board, m_learning_board->m_board);
	delete prev_board;
	m_learning_board->search_candidate(m_turn);
	update_feature_vector();
	return max_q_value;
}

int Q_Learning::get_q_max_index()
{
	int max_q_index = -1;
	double max_q_value = 0.0;
	m_learning_board->search_candidate(m_turn);
	for (int i = 0;i < m_learning_board->m_candidate_size;i++)
	{
		Board_State* cp_board = (Board_State*)im->Alloc(BOARD_WIDTH * BOARD_HEIGHT * sizeof(Board_State));
		copy_board(m_learning_board->m_board, cp_board);
		int candidate_idx = m_learning_board->get_candidate(i);
		Pos2 select_pos = to_pos2(candidate_idx);
		if (!m_learning_board->enable_put(select_pos.get_X(), select_pos.get_Y(), m_turn))
		{
			im->Free(cp_board);
			continue;
		}
		update_feature_vector();
		double q_value = cal_q_value();
		if (max_q_value < q_value)
		{
			max_q_value = q_value;
			max_q_index = candidate_idx;
		}
		copy_board(cp_board, m_learning_board->m_board);
		im->Free(cp_board);
	}
	update_feature_vector();
	return max_q_index;
}

void Q_Learning::step(int episode)
{
	m_learning_board->search_candidate(m_turn);
	if (m_learning_board->is_full())
	{
		return;
	}
	int epsilon = (rand() % 100 + 1) ;
	int action = -1;
	if (epsilon*(episode)/EPISODE >= EPSILON)
	{
		action = get_q_max_index();
	}
	else
	{
		int rand_idx = rand() % (m_learning_board->m_candidate_size);
		action = m_learning_board->get_candidate(rand_idx);
	}
	Pos2 select_pos = to_pos2(action);
	if (action == -1 || !m_learning_board->enable_put(select_pos.get_X(), select_pos.get_Y(), m_turn))
	{
		return;
	}
	update_feature_vector();
	double current_q_value = cal_q_value();
	double reward = (double)m_learning_board->get_reward(m_turn)/(double)(BOARD_WIDTH*BOARD_HEIGHT);
	//重さの更新
	double next_q_value = get_q_max_value(DEPTH);
	double td_error = reward + GAMMA * next_q_value - current_q_value;
	weight[action] = weight[action] + ALPHA * td_error * feature_vector[action];
}

void copy_board(Board_State* target_board, Board_State* cp_board)
{
	for (int i = 0;i < BOARD_HEIGHT * BOARD_WIDTH;i++)
	{
		cp_board[i] = target_board[i];
	}
}

//Q値の計算
double Q_Learning::cal_q_value()
{
	double sum_q_value = 0.0;
	for(int y=0;y<BOARD_HEIGHT;y++)
	{
		for (int x = 0;x < BOARD_WIDTH;x++)
		{
			sum_q_value += (double)feature_vector[to_pos1(x,y)] * weight[to_pos1(x,y)];
		}
	}
	return sum_q_value;
}

void Q_Learning::update_feature_vector()
{
	for (int y = 0;y < BOARD_HEIGHT;y++)
	{
		for (int x = 0;x < BOARD_WIDTH;x++)
		{
			int idx = to_pos1(x, y);
			if(m_learning_board->m_board[idx]==Board_State::None)
			{
				feature_vector[idx] = 0.0;
			}
			else if (m_learning_board->m_board[idx] == m_turn)
			{
				feature_vector[idx] = 1.0/(double)(BOARD_WIDTH*BOARD_HEIGHT);
			}
			else
			{
				feature_vector[idx] = -1.0/(double)(BOARD_WIDTH*BOARD_HEIGHT);
			}
		}
	}
}