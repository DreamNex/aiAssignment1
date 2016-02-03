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
	, health(10)
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
	//Situations
	if (health <= 2)
	{
		if (rand() % 101 > 50)
		{
			mbController.SetMessage(mbController.GetCommand(0));
			mbController.SetFromMessage(ID);
			mbController.SetSenderID(ID);
		}

		else
		{
			mbController.SetMessage(mbController.GetCommand(3));
			mbController.SetFromMessage(ID);
			mbController.SetSenderID(ID);
		}
		
	}

	if (Volunteer == true && health > 2)
	{
		mbController.SetMessage(mbController.GetCommand(3));
		mbController.SetFromMessage(ID);
		mbController.SetSenderID(ID);
	}

	//update message board

	if (mbController.Getmessage() == mbController.GetCommand(0) && ID != mbController.GetSenderID())
	{
		FSM1 = SWAP;
	}

	else if (mbController.Getmessage() == mbController.GetCommand(1))
	{
		FSM2 = VOLUNTEER;
		FSM1 = STOP1;
	}
	
	else if (mbController.Getmessage() == mbController.GetCommand(2))
	{
		if (ID == 3)
		{
			FSM2 = HEAL;
		}

		else
		{

		}
	}

	else if (mbController.Getmessage() == mbController.GetCommand(3) /*&& ID != mbController.GetSenderID()*/)
	{
		FSM2 = STOP2;
		FSM1 = AGGRESIVE;
	}

	//=============================================================================================================//
	// ==================================================STATES====================================================//
	//=============================================================================================================//


	switch (FSM2)
	{
	case SWAP2:
	{
				  if (rand() % 101 > 25)
				  {
					  FSM2 = VOLUNTEER;
					  FSM1 = STOP1;
				  }

				  else
				  {

				  }

				  break;
	}

	case VOLUNTEER:
	{
					  
							  if (rand() % 101 > 30)
							  {
								  FSM2 = STOP2;
								  FSM1 = AGGRESIVE;
								  Volunteer = true;
							  }

							  else
							  {
								  Volunteer = false;

							  }
						 
					 
					 break;
	}

	case HEAL:
	{
		
	}
	
	}// this bracket is the end of the switch case

	switch (FSM1)
	{

	case AGGRESIVE:
	{
			
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
							  vel.SetZero();
						  }
					  }
					  else
					  {
						  isFighting = true;
						  if (rand() % 100 > 50)
						  {
							  FSM1 = DODGE;
						  }
						  else
						  {
							  target->health--;
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
		
				 FSM1 = STOP1;
				 FSM2 = SWAP2;
		

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

	case RETREAT:
	{
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