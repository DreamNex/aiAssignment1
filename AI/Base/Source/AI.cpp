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
	, wayPointIndex(0)
	, arrived(false)
	, probabilityIdle(1.f)
	, probabilityDodge(40.f)
	, detected(false)
	, timer(0)
	, startPoint(0)

{
}

cAI::~cAI()
{
}

vector <Vector3> wayPoints;

void cAI::init()
{
	wayPoints.push_back(Vector3(-offset, -offset, 1));
	wayPoints.push_back(Vector3(-offset, offset, 1));
	wayPoints.push_back(Vector3(5, 5, 1));
	wayPoints.push_back(Vector3(offset, -offset, 1));
	wayPoints.push_back(Vector3(-offset * 5, -offset * 5, 1));
	wayPoints.push_back(Vector3(-offset * 5, offset * 3, 1));
	wayPoints.push_back(Vector3(offset * 3, offset * 3, 1));
	wayPoints.push_back(Vector3(offset * 3, -offset * 3, 1));
	//pos.Set(wayPoints[0].x, wayPoints[0].y);
	int randomIndex = RandomInteger(1, 3);
	FSM2 = PATROL;
	FSM1 = STOP1;
	startPoint = RandomInteger(0, 7);
	nextPoint = wayPoints[startPoint];
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

FSM_TWO cAI::getState()
{
	return FSM2;
}

FSM_ONE cAI::getState2()
{
	return FSM1;
}

void cAI::update(double dt)
{
	switch (FSM2)
	{
		case PATROL:
		{		
			
			Vector3 direction = (nextPoint - pos);
			float distance = GetDistance(pos.x, pos.y, nextPoint.x, nextPoint.y);

			if (distance < 1)
			{
				arrived = true;
			}
			else
			{
				vel = (direction.Normalize() *AiSpeed * dt);
				pos = pos + vel;
			}

			if (arrived) // set the nextpoint to a waypoint that isnt where the AI currently is in
			{
			
				randNum = RandomInteger(0, 7);
				wayPointIndex = randNum;
				
				if (nextPoint != wayPoints[wayPointIndex])
				{
					nextPoint = wayPoints[wayPointIndex];
				}
				else if (nextPoint == wayPoints[wayPointIndex] && wayPointIndex == (wayPoints.size() - 1))
				{
					wayPointIndex--;
					nextPoint = wayPoints[wayPointIndex];
				}
				else if (nextPoint == wayPoints[wayPointIndex] && wayPointIndex == 0)
				{
					wayPointIndex++;
					nextPoint = wayPoints[wayPointIndex];
				}
				else
				{
					randNum = RandomInteger(1, 6);
					if (randNum == wayPointIndex)
					{
						int relocate = RandomInteger(1, 2);
						if (relocate == 1)
						{
							wayPointIndex--;
							nextPoint = wayPoints[wayPointIndex];
						}

						else
						{
							wayPointIndex++;
							nextPoint = wayPoints[wayPointIndex];
						}
					}
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
				timer++;
				// state = SCAN;
				 if (timer >= 100)
				 {
					 FSM2 = PATROL;
					 timer = 0;
				 }
				
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
					   Vector3 direction = (target->pos - pos);
					   vel = (direction.Normalize() * AiSpeed * dt);
					   pos = pos + (vel);
				   }

				   randNum = RandomInteger(1, 100);
				   if (randNum <= probabilityDodge)
				   {
					   FSM1 = DODGE;
				   }
				   else
				   {
					   target->health--;
					   //Translate it backwards abit to show that it got hit
					   target->pos.x-= 0.1f;
					   target->pos.y-= 0.1f;
					   
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