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

//GET FUNCTIONS
int cAI::getID()
{
	return id;
}

int cAI::getStateSoldier()
{
	return stateSoldier;
}

int cAI::getStateLeader()
{
	return stateLeader;
}

Vector3 cAI::getFinishP()
{
	return fightpoint;
}

Vector3 cAI::getStartP()
{
	return startpoint;
}

//SET FUNCTIONS
void cAI::setID(int id)
{
	this->id = id;
}

void cAI::setStateSoldier(int stateSoldier)
{
	if (stateSoldier == 0)
	{
		SOLDIER_FSM = STATE_SOLDIER_STANDBY;
		this->stateSoldier = stateSoldier;
	}
	else if (stateSoldier == 1)
	{
		SOLDIER_FSM = STATE_SOLDIER_FIGHTING;
		this->stateSoldier = stateSoldier;
	}
	else if (stateSoldier == 2)
	{
		SOLDIER_FSM = STATE_SOLDIER_RETREAT;
		this->stateSoldier = stateSoldier;
	}
	else if (stateSoldier == 3)
	{
		SOLDIER_FSM = STATE_SOLDIER_SWAPOUT;
		this->stateSoldier = stateSoldier;
	}
}

void cAI::setStateLeader(int stateLeader)
{
	if (stateLeader == 0)
	{
		LEADER_FSM = STATE_LEADER_STANDBY;
		this->stateLeader = stateLeader;
	}
	else if (stateLeader == 1)
	{
		LEADER_FSM = STATE_LEADER_FIGHTING;
		this->stateLeader = stateLeader;
	}
	else if (stateLeader == 2)
	{
		LEADER_FSM = STATE_LEADER_RETREAT;
		this->stateLeader = stateLeader;
	}
	else if (stateLeader == 3)
	{
		LEADER_FSM = STATE_LEADER_SWAPOUT;
		this->stateLeader = stateLeader;
	}
}

void cAI::setFinishP(Vector3 fightpoint)
{
	this->fightpoint = fightpoint;
}

void cAI::setStartP(Vector3 startpoint)
{
	this->startpoint = startpoint;
}

void cAI::UpdateLeaderFSM(double dt)
{
	switch (LEADER_FSM)
	{
		case STATE_LEADER_FIGHTING:
		{

			if ((fightpoint - pos).Length() >= 1.5f)
			{
				Vector3 direction = fightpoint - pos;
				vel = direction.Normalized() * AiSpeed * dt;
				pos += vel;
			}

			else
			{
				pos = fightpoint;
				health--;
				if (health <= 2)
				{
					LEADER_FSM = STATE_LEADER_RETREAT;
				}
			}
			break;
		}
		case STATE_LEADER_RETREAT:
		{

			if ((startpoint - pos).Length() >= 1.5f)
			{
				Vector3 direction = startpoint - pos;
				vel = direction.Normalized() * AiSpeed * dt;
				pos += vel;
			}

			else
			{
				pos = startpoint;
				LEADER_FSM = STATE_LEADER_STANDBY;
			}

			break;
		}
		case STATE_LEADER_STANDBY:
		{
			if ((startpoint - pos).Length() >= 1.5f)
			{
				Vector3 direction = startpoint - pos;
				vel = direction.Normalized() * AiSpeed * dt;
				pos += vel;
			}
			else
			{
				pos = startpoint;
			}

			break;
		}
		case STATE_LEADER_SWAPOUT:
		{
			if ((fightpoint - pos).Length() >= 1.5f)
			{
				Vector3 direction = fightpoint - pos;
				vel = direction.Normalized() * AiSpeed * dt;
				pos += vel;
			}

			else
			{
				pos = fightpoint;
				LEADER_FSM = STATE_LEADER_FIGHTING;
			}
			break;
		}

		default:
			break;
	}
}

void cAI::UpdateSoldierFSM(double dt)
{
	switch (SOLDIER_FSM)
	{
		case STATE_SOLDIER_FIGHTING:
		{
			if ((fightpoint - pos).Length() >= 1.5f)
			{
				Vector3 direction = fightpoint - pos;
				vel = direction.Normalized() * AiSpeed * dt;
				pos += vel;
			}

			else
			{
				pos = fightpoint;
				health--;
				if (health <= 2)
				{
					SOLDIER_FSM = STATE_SOLDIER_RETREAT;
				}
			}
			
			break;
		}
		case STATE_SOLDIER_RETREAT:
		{

			if ((startpoint - pos).Length() >= 1.5f)
			{
				Vector3 direction = startpoint - pos;
				vel = direction.Normalized() * AiSpeed * dt;
				pos += vel;
			}

			else
			{
				pos = startpoint;
				SOLDIER_FSM = STATE_SOLDIER_STANDBY;
			}

			break;
		}
		case STATE_SOLDIER_STANDBY:
		{
			if ((startpoint - pos).Length() >= 1.5f)
			{
				Vector3 direction = startpoint - pos;
				vel = direction.Normalized() * AiSpeed * dt;
				pos += vel;
			}
			else
			{
				pos = startpoint;
			}

			break;
		}
		case STATE_SOLDIER_SWAPOUT:
		{
			if ((fightpoint - pos).Length() >= 1.5f)
			{
				Vector3 direction = fightpoint - pos;
				vel = direction.Normalized() * AiSpeed * dt;
				pos += vel;
			}

			else
			{
				pos = fightpoint;
				SOLDIER_FSM = STATE_SOLDIER_FIGHTING;
			}
			break;
		}
		default:
			break;
	}
}

void cAI::UpdateMedicFSM(double dt)
{
	// Drop if no time.
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
