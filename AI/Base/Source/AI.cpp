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

Vector3 cAI::getFinishP()
{
	return fightpoint;
}

//SET FUNCTIONS
void cAI::setID(int id)
{
	this->id = id;
}

void cAI::setFinishP(Vector3 fightpoint)
{
	this->fightpoint = fightpoint;
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
		case STATE_SOLDIER_FIGHTING:
		{
			Vector3 direction;

			if (pos != fightpoint)
			{
				direction = fightpoint - pos;
				vel = direction.Normalize() * AiSpeed * dt;
				pos += vel;
			}
			else if (pos == fightpoint)
			{
				pos = fightpoint;
				vel.SetZero();
				health--;
			}

			if (health <= 2)
			{
				SOLDIER_FSM = STATE_SOLDIER_RETREAT;
			}
			break;
		}
		case STATE_SOLDIER_RETREAT:
		{
			Vector3 direction;

			if (pos != startpoint)
			{
				direction = pos - startpoint;
				vel = direction.Normalize() * AiSpeed * dt;
				pos += vel;
			}
			else if (pos == startpoint)
			{
				pos = startpoint;
				vel.SetZero();
				SOLDIER_FSM = STATE_SOLDIER_STANDBY;
			}
			break;
		}
		case STATE_SOLDIER_STANDBY:
		{
			break;
		}
		case STATE_SOLDIER_SWAPOUT:
		{
			SOLDIER_FSM = STATE_SOLDIER_FIGHTING;
			break;
		}
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
