#include "AI.h"
#include "timer.h"

using namespace std;

cAI::cAI()
	
{
}

cAI::~cAI()
{
}

//vector <Vector3> wayPoints;
const float AiSpeed = 10.f;

void cAI::init()
{
	srand(time(NULL));

}

void cAI::UpdateLeaderFSM(double dt)
{
	switch (LEADER_FSM)
	{
		case STATE_LEADER_FIGHTING:
		{
			break;
		}
		case STATE_LEADER_RETREAT:
		{
			break;
		}
		case STATE_LEADER_STANBY:
		{
			break;
		}
		case STATE_LEADER_SWAPOUT:
		{
			break;
		}
	}
}

void cAI::UpdateSoldierFSM(double dt)
{
	switch (SOLDIER_FSM)
	{

	}
}

void cAI::UpdateMedicFSM(double dt)
{
	switch (MEDIC_FSM)
	{

	}
}

void cAI::update(double dt)
{
	if (id == 1)
	{
		UpdateLeaderFSM(dt);
	}
	else if (id == 2)
	{
		UpdateSoldierFSM(dt);
	}

	else if (id == 3)
	{
		UpdateMedicFSM(dt);
	}
	
}
