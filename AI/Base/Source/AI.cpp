#include "AI.h"

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
	,wayPointIndex(0)
	,arrived(false)
	,probabilityIdle(80.f)
	, probabilityDodge(40.f)
	, detected(false)

	, timer(0)

{
}

cAI::~cAI()
{
}

Vector3 nextPoint;
vector <Vector3> wayPoints, myStack;

void cAI::init()
{
	wayPoints.push_back(Vector3(-offset, -offset, 1));
	wayPoints.push_back(Vector3(-offset, offset, 1));
	wayPoints.push_back(Vector3(offset, offset, 1));
	wayPoints.push_back(Vector3(offset, -offset, 1));
	wayPoints.push_back(Vector3(offset * 3, -offset * 3, 1));
	wayPoints.push_back(Vector3(offset * 3, offset * 3, 1));
	wayPoints.push_back(Vector3(-offset * 3, offset * 3, 1));
	wayPoints.push_back(Vector3(-offset * 3, -offset * 3, 1));
	//pos.Set(wayPoints[0].x, wayPoints[0].y);
	int randomIndex = RandomInteger(1, 3);
	FSM2 = PATROL;
	FSM1 = STOP1;
}

bool cAI::isVisible2D(const Vector3 &Position, float rotation, float FOV, const Vector3 &ObjectPosition)
{
	float lookingOBJ = CalAnglefromPosition2D(ObjectPosition, Position);
	float cameraRotation = rotation;
	bool LO = false, CR = false;

	if (lookingOBJ - cameraRotation > 180.f)
	{
		lookingOBJ -= 360.f;
	}
	else if (lookingOBJ - cameraRotation < -180.f)
	{
		lookingOBJ += 360.f;
	}

	if ((lookingOBJ + FOV > cameraRotation && lookingOBJ - FOV < cameraRotation))
	{
		return true;
	}

	return false;
}

void cAI::update(double dt)
{
	switch (FSM2)
	{
		case PATROL:
		{


			if (myStack.size() == 0)
				nextPoint = wayPoints[wayPointIndex];
			else
				nextPoint = myStack[myStack.size() - 1];

			Vector3 direction = (nextPoint - pos).Normalize();
			float distance = GetDistance(pos.x, pos.y, nextPoint.x, nextPoint.y);

			if (distance < 1)
			{
				pos = nextPoint;
				arrived = true;
			}
			else
			{
				vel = (direction * AiSpeed * dt);
				pos = pos + vel;
			}

			if (arrived)
			{
				if (myStack.size() == 0)
				{
					if (wayPointIndex == wayPoints.size() - 1)
						wayPointIndex = 0;
					else
						wayPointIndex++;
				}
				else
				{
					myStack.clear();
				}
				arrived = false;
			}
			// if probability == true, idle
			randNum = RandomInteger(1, 100);
			if (randNum <= probabilityIdle)
			{
				FSM2 = IDLE;
			}

			break;

		}

		case IDLE:
		{
				// state = SCAN;
				 if (timer >= 100)
				 {
					 FSM2 = PATROL;
				 }
				 timer++;
				 break;
		}

		case DETECTED:
		{
						 if (detected == true)
						 {

							 FSM1 = ATTACK;
							 FSM2 = STOP2;
						 }
						 break;
		}
	
	}

	switch (FSM1)
	{
	case ATTACK:
	{

				   

				   if ((target->pos - pos).Length() <= 12)
				   {
					   vel.SetZero();
					   
				   }
				   else
				   {
					   Vector3 direction = (target->pos - pos).Normalize();
					   vel = (direction * AiSpeed * dt);
					   pos = pos + (vel);
				   }
				   if (missed != true)
				   {
					   target->health--;
					   //Translate it backwards abit to show that it got hit
					   //target->pos;
					   
				   }
				   else
				   {
					   FSM1 = DODGE;
				   }
				   break;
	}
	case DODGE:
	{
				  FSM1 = ATTACK;
				  break;
	}
	}

	if ((target->pos - pos).Length() <= 20 && (target->pos - pos).Length() >= 12)
	{
		vel.SetZero();

		if (detected == false)
		{

			detected = true;
			FSM2 = DETECTED;
		}
	}
	
}