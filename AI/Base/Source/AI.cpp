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
	, probabilityDodge(30.f)
	, detected(false)
	, timer(0)
	, startPoint(0)
	

{
}

cAI::~cAI()
{
}

vector <Vector3> wayPoints;
const float AiSpeed = 10.f;

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
	FSM2 = STOP2;
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
	case SWAP2:
	{
				  break;
	}
		
	case VOLUNTEER:
	{
					  break;
	}

	case AGGRESSIVE:
	{
					   break;
	}
	
	}// this bracket is the end of the switch case

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
					   //target->pos.x-= 0.1f;
					  // target->pos.y-= 0.1f;
					   
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
			//FSM2 = DETECTED;
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