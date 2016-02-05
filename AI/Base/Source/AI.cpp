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

	cout << "FSM1: " << FSM1 << endl;
	switch (FSM1)
	{
		case STATE_ATTACK:
		{
			pos = fightingPoint;
			if (id == 1)
			{
				mbController.SetLeaderM("Attacking");
				health--;
			}
			else if (id == 2)
			{
				mbController.SetSoldierM("Attacking");
				health--;
			}
			else if (id == 3)
			{
				mbController.SetMedicM("Attacking");
				health--;
			}
			
			
			if (rand() % 101 > 50)
			{
				FSM1 = STATE_DODGE;
				if (id == 1)
				{
					mbController.SetLeaderM("Dodge");
				}
				else if (id == 2)
				{
					mbController.SetSoldierM("Dodge");
				}
				else if (id == 3)
				{
					mbController.SetMedicM("Dodge");
				}
			}

			if (health <= 10)
			{
				FSM1 = STATE_MOVE;
				FSM2 = STATE_SWAPIN;
				if (id == 1)
				{
					mbController.SetLeaderM("Swapping out");
				}
				else if (id == 2)
				{
					mbController.SetSoldierM("Swapping out");
				}
				else if (id == 3)
				{
					mbController.SetMedicM("Swapping out");
				}
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
				Vector3 direction;
				direction = fightingPoint - pos;
				vel = direction * AiSpeed *dt;
				pos += vel;
				if (pos.x >= fightingPoint.x && pos.y >= fightingPoint.y && pos.z >= fightingPoint.z)
				{
					
					//vel.SetZero();
					FSM1 = STATE_ATTACK;
					
				}
				break;
		}

		case STATE_STANDBY:
		{
							  if (id == 1)
							  {
								  FSM1 = STATE_MOVE;
							  }

							  else if (id == 2)
							  {
								  FSM1 = FSM1_NIL;
							  }
							  else if (id == 3)
							  {
								  FSM1 = FSM1_NIL;
							  }
							  break;
		}
		default:
			FSM1 = STATE_STANDBY;		
			break;
	}

	switch (FSM2)
	{
		case STATE_HEAL:
		{
			//if (mbController.GetMsg() == "Healing");
			{
				if (id == 3)
				{
					Vector3 direction;
					direction = startwPoint - pos;
					vel = direction * AiSpeed *dt;
					pos += vel;
					if (pos.x <= startwPoint.x && pos.y <= startwPoint.y && pos.z <= startwPoint.z)
					{
						//vel.SetZero();
						health++;
						if (health == 100)
						{
							if (id == 1)
							{
								mbController.SetLeaderM("Reached");
							}
							else if (id == 2)
							{
								mbController.SetSoldierM("Reached");
							}
							else if (id == 3)
							{
								mbController.SetMedicM("Reached");
							}
						}
					}
				}
			}
			break;
		}

		case STATE_SWAPOUT:
		{
			//if (mbController.GetMsg() == "Swapping Out")
			{
				Vector3 direction = (1, 1, 1);
				direction = startwPoint - pos;
				vel = direction * AiSpeed *dt;
				pos += vel;
				if (pos.x <= startwPoint.x && pos.y <= startwPoint.y && pos.z <= startwPoint.z)
				{
					//vel.SetZero();
					if (id == 1)
					{
						mbController.SetLeaderM("Healing");
					}
					else if (id == 2)
					{
						mbController.SetSoldierM("Healing");
					}
					else if (id == 3)
					{
						mbController.SetMedicM("Healing");
					}
				}
			}
			break;
		}

		case STATE_SWAPIN:
		{
			
			//if (mbController.GetMsg() == "Swapping Out")
			{
				Vector3 direction;
				direction = fightingPoint - pos;	
				vel = direction * AiSpeed * dt;
				pos += vel;
				if (pos.x >= fightingPoint.x && pos.y >= fightingPoint.y && pos.z >= fightingPoint.z)
				{
					if (id == 1)
					{
						mbController.SetLeaderM("Reached");
					}
					else if (id == 2)
					{
						mbController.SetSoldierM("Reached");
					}
					else if (id == 3)
					{
						mbController.SetMedicM("Reached");
					}
				}
			}

			break;

		case STATE_STANDBY2:
		{
							   if (id == 1)
							   {
								   FSM2 = FSM2_NIL;
							   }

							   else if(id == 2)
							   {
								   FSM2 = FSM2_NIL;
							   }

							   else if(id == 3)
							   {
								   FSM2 = FSM2_NIL;
							   }
		}
			break;
		}
		default:
			STATE_STANDBY2;
			break;
	}
}
