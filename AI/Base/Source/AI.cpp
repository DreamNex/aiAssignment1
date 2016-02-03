#include "AI.h"
#include "timer.h"
using namespace MyAI;
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
	: offset(10.f)
	// Set another offset variable
	, wayPointIndex(0)
	, arrived(false)
	, isFighting(false)
	, Volunteer(false)
	, timer(0)
	, teamID(0)
	, startPoint(0)
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

	int randomIndex = RandomInteger(1, 3);
	FSM2 = STOP2;
	FSM1 = STOP1;
	//startPoint = RandomInteger(0, 7);
	//nextPoint = wayPoints[startPoint];
}

void cAI::SetAITarget(cAI* target)
{
	this->target = target;
}

FSM_TWO cAI::getState()
{
	return FSM2;
}

FSM_ONE cAI::getState2()
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
	for (int i = 0; i < mbController.Getmessage().length(); ++i)
	{
		mbController.Getmessage()[i] = tolower(mbController.Getmessage()[i]);
	}
	switch (FSM2)
	{
	case SWAP2:
	{
				  if (health <= 1)
				  {
					  if (rand() % 101 > 45)
					  {
						  if (mbController.Getmessage() == mbController.GetCommand(1))
						  {
							  isFighting = false;
							  SwapAI = true;
							  FSM2 = VOLUNTEER;
						  }
					  }

					  else
					  {

					  }
				  }
				  break;
	}
		
	case VOLUNTEER:
	{
					  if (mbController.GetSenderID() == teamID)
					  {

						  if (mbController.Getmessage() == mbController.GetCommand(1));
						  {

							  if (SwapAI == true)
							  {
								  if (rand() % 101 > 30);
								  {
									  Volunteer = true;
								  }
							  }
						  }
					  }
					 break;
	}

	case HEAL:
	{
		if (mbController.GetSenderID() == teamID)
		{
			if (mbController.Getmessage() == mbController.GetCommand(3))
			{

				//Set waypoint to the sender's position which will be a waypoint.
			}
		}
	}
	
	}// this bracket is the end of the switch case

	switch (FSM1)
	{

	case AGGRESIVE:
	{
		if (isFighting == true && health >= 4)
		{
			mbController.SetMessage(mbController.GetCommand(3));
			Vector3 direction;

			// Makes AI move toward waypoint
			if (arrived == false)
			{
				direction = fightingPoint - pos;
				vel = direction.Normalize() * AiSpeed * dt;
				pos += vel;
				if (pos == fightingPoint)
				{
					arrived = true;
				}
			}
			else
			{
				if (rand() % 100 > 50)
				{
					FSM1 = DODGE;
				}
				else
				{
					target->health--;
					//Translate it backwards abit to show that it got hit
					//target->pos.x-= 0.1f;
					// target->pos.y-= 0.1f;

				}
				if (health <= 2)
				{
					FSM1 = SWAP;
					mbController.SetMessage(mbController.GetCommand(0));
				}
				if (health <= 4)
				{
					FSM1 = ASSIST;
					mbController.SetMessage(mbController.GetCommand(1));
				}
			}
		}
		break;
	}

	case DODGE:
	{
		FSM1 = AGGRESIVE;
		break;
	}

	case SWAP:
	{
		if (mbController.Getmessage() == mbController.GetCommand(0) && mbController.GetSenderID() == teamID)
		{
			isFighting = false;
			SwapAI = true;
			FSM2 = SWAP2;
		}

		break;
	}

	case ASSIST:
	{
		if (mbController.Getmessage() == mbController.GetCommand(1) && mbController.GetSenderID() == teamID)
		{
			if (rand() % 100 > 45)
			{
				FSM2 = VOLUNTEER;
			}	
		}
		break;
	}

	}// this bracket is the end of the switch case

}

Vector3 cAI::getRandPos()
{
	Vector3 temp;
	temp.x = (float)RandomInteger(-30, 30);
	temp.y = (float)RandomInteger(-30, 30);
	return temp;
}