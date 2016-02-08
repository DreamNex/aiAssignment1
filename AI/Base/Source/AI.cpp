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

int cAI::getState()
{
	return state;
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

void cAI::setState(int state)
{
	if (state == 0)
	{
		SOLDIER_FSM = STATE_SOLDIER_STANDBY;
		this->state = state;
	}
	else if (state == 1)
	{
		SOLDIER_FSM = STATE_SOLDIER_FIGHTING;
		this->state = state;
	}
	else if (state == 2)
	{
		SOLDIER_FSM = STATE_SOLDIER_RETREAT;
		this->state = state;
	}
	else if (state == 3)
	{
		SOLDIER_FSM = STATE_SOLDIER_SWAPOUT;
		this->state = state;
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

			//Move the player up
			if ((fightpoint - pos).Length() >= 1.5f)
			{
				Vector3 direction;
				vel = direction.Normalized() * AiSpeed * dt;
				pos += vel;
			}

			else
			{
				pos = fightpoint;
			}

			break;
		}
		case STATE_LEADER_RETREAT:
		{
			break;
		}
		case STATE_LEADER_STANDBY:
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
		case STATE_SOLDIER_FIGHTING:
		{
			
			health--;

			if (health <= 2)
			{
				SOLDIER_FSM = STATE_SOLDIER_RETREAT;
				break;
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


			//health++;
			/*if (health >= 100)
			{
				SOLDIER_FSM = STATE_SOLDIER_FIGHTING;
			}*/
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
