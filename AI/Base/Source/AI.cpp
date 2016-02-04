#include "AI.h"
#include "timer.h"

using namespace std;


float GetDistance(float x1, float y1, float x2, float y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

int RandomInteger(int lowerLimit, int upperLimit)
{
	return rand() % (upperLimit - lowerLimit + 1) + lowerLimit;
}


cAI::cAI()
	: 
	// Set another offset variable
	wayPointIndex(0)
	, Volunteer(false)
	, health(50)
	, id(0)
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

	if (id == 1)//leader
	{
		FSM1 = FSM1_NIL;
		FSM2 = STATE_SWAPIN;

	}
	else if (id == 2)
	{
		FSM1 = FSM1_NIL;
		FSM2 = FSM2_NIL;
	}
	else if (id == 3)
	{
		FSM1 = FSM1_NIL;
		FSM2 = STATE_HEAL;
	}

	int randomIndex = RandomInteger(1, 3);
}



cAI::FSM_TWO cAI::getState()
{
	return FSM2;
}

cAI::FSM_ONE cAI::getState2()
{
	return FSM1;
}

void cAI::SetFightPt(Vector3 fightingPoint)
{
	this->fightingPoint = fightingPoint;
}

Vector3 cAI::GetFightPt()
{
	return fightingPoint;
}

void cAI::update(double dt)
{
	//=============================================================================================================//
	//===================================================STATES====================================================//
	//=============================================================================================================//

	switch (FSM1)
	{
		case STATE_ATTACK:
		{
			if (pos == fightingPoint)
			{
				mbController.SetMsg("Attacking");
				health--;
			}
			
			if (rand() % 101 > 50)
			{
				FSM1 = STATE_DODGE;
				mbController.SetMsg("Dodge");
			}

			if (health <= 10)
			{
				FSM1 = STATE_MOVE;
				FSM2 = STATE_SWAPIN;
				mbController.SetMsg("Swapping Out");
			}

			break;
		}

		case STATE_DODGE:
		{
			FSM1 = STATE_ATTACK;
			break;
		}

		case STATE_MOVE:
		{
			if (mbController.GetMsg() == "Reached")
			{
				Vector3 direction;
				direction = fightingPoint - pos;
				vel = direction.Normalize() * AiSpeed *dt;
				pos += vel;
				if (pos.x == fightingPoint.x && pos.y == fightingPoint.y && pos.z == fightingPoint.z)
				{
					vel.SetZero();
					FSM1 = STATE_ATTACK;
				}
			}
			break;
		}
	}

	switch (FSM2)
	{
		case STATE_HEAL:
		{
			if (mbController.GetMsg() == "Healing");
			{
				if (id == 3)
				{
					Vector3 direction;
					direction = startwPoint - pos;
					vel = direction.Normalize() * AiSpeed *dt;
					pos += vel;
					if (pos.x == startwPoint.x && pos.y == startwPoint.y && pos.z == startwPoint.z)
					{
						vel.SetZero();
						health++;
						if (health == 100)
						{
							mbController.SetMsg("Reached");
						}
					}
				}
			}
			break;
		}

		case STATE_SWAPOUT:
		{
			if (mbController.GetMsg() == "Swapping Out")
			{
				Vector3 direction;
				direction = startwPoint - pos;
				vel = direction.Normalize() * AiSpeed *dt;
				pos += vel;
				if (pos.x == startwPoint.x && pos.y == startwPoint.y && pos.z == startwPoint.z)
				{
					vel.SetZero();
					mbController.SetMsg("Healing");
				}
			}
			break;
		}

		case STATE_SWAPIN:
		{
			
			if (mbController.GetMsg() == "Swapping Out")
			{
				Vector3 direction;
				direction = fightingPoint - pos;
				vel = direction.Normalize() * AiSpeed *dt;
				pos += vel;
				if (pos.x == fightingPoint.x && pos.y == fightingPoint.y && pos.z == fightingPoint.z)
				{
					vel.SetZero();
					mbController.SetMsg("Reached");
				}
			}
			break;
		}
	}
}
