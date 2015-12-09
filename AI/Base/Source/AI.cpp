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
	,probabilityIdle(30.f)
	, probabilityDodge(40.f)
	, detected(false)
{
}

cAI::~cAI()
{
}

Vector3 nextPoint;
vector <Vector3> wayPoints, myStack;

void cAI::init()
{
	wayPoints.push_back(Vector3(-offset, -offset));
	wayPoints.push_back(Vector3(-offset, offset));
	wayPoints.push_back(Vector3(offset, offset));
	wayPoints.push_back(Vector3(offset, -offset));
	wayPoints.push_back(Vector3(offset * 5, -offset * 5));
	wayPoints.push_back(Vector3(offset * 5, offset * 5));
	wayPoints.push_back(Vector3(-offset * 5, offset * 5));
	wayPoints.push_back(Vector3(-offset * 5, -offset * 5));
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
				pos = pos + (direction * AiSpeed *dt);

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
				 pos = pos;
				// state = SCAN;
				 for (int timer = 0; timer < 500; timer++)
				 {
					 if (timer == 500)
					 {
						 FSM2 = PATROL;
					 }
				 }
				 break;
		}

		case DETECTED:
		{

						 FSM1 = ATTACK;
						 FSM2 = STOP2;
						 break;
		}
	
	}

	switch (FSM1)
	{
		case ATTACK:
		{
			randNum = RandomInteger(1, 100);
			if (randNum <= probabilityDodge)
			{
				FSM1 = DODGE;
			}
			else
			{
				target->health--;
			}
			break;
		}
		case DODGE:
		{
			FSM1 = ATTACK;
			break;
		}
	}

	if ((pos - target->pos).Length() < 4)
	{
		FSM2 = DETECTED;
		FSM1 = STOP1;
	}
}

FSM_TWO cAI::getState()
{
	return FSM2;
}

FSM_ONE cAI::getState2()
{
	return FSM1;
}