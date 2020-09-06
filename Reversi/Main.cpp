/*==== include ====*/
#include"Manager.h"
#include"Start.h"
#include"Game.h"
#include"Result.h"

/*==== function ====*/
void UpDate();
void Initialize();
void Finalize();

Scene_State current_state = Scene_State::Initialize_State;

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
	current_state = Scene_State::Start_State;
}

void Finalize()
{
	//å„Ç…ïœçX
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