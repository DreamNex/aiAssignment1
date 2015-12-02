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
	: offset(2.0f)
{
}

cAI::~cAI()
{
}

Vector3 nextPoint;
vector <Vector3> wayPoints, intrusionPoints, myStack;

void cAI::init()
{
	wayPoints.push_back(Vector3(-offset, -offset));
	wayPoints.push_back(Vector3(-offset, offset));
	wayPoints.push_back(Vector3(offset, offset));
	wayPoints.push_back(Vector3(offset, -offset));

	intrusionPoints.push_back(Vector3(-1.2f*offset, -0.3f*offset));
	intrusionPoints.push_back(Vector3(-1.2f*offset, 0.3f*offset));
	intrusionPoints.push_back(Vector3(1.2f*offset, 0.3f*offset));
	intrusionPoints.push_back(Vector3(1.2f*offset, -0.3f*offset));
	pos.Set(wayPoints[0].x, wayPoints[0].y);

	int randomIndex = RandomInteger(1, 3);
	pos.Set(intrusionPoints[randomIndex].x, intrusionPoints[randomIndex].y);
	FSM2 = PATROL;
	wayPointIndex = 1;
	arrived = false;
	probability = 30.0f;
}

void cAI::FiniteStateM2()
{
	switch (FSM2)
	{
		case PATROL:
			// if probability == true, idle
			randNum = RandomInteger(1, 100);
			if (randNum <= probability)
			{
				FSM2 = IDLE;
			}
			break;

		case IDLE:
			for (int timer = 0; timer < 100; timer++)
			{
				if (timer == 100)
				{
					FSM2 = PATROL;
				}
			}
			break;

		case SCAN:
			// if detect, go to fsm2
			FSM1 = ATTACK;
			FSM2 = STOP2;
			break;
	}
}

void cAI::update(double dt)
{
	FiniteStateM2();
	switch (FSM2)
	{
		case PATROL:
		{
			cout << "patrol\n";
		}
	}
	if (FSM2 == PATROL)
	{
		if (myStack.size() == 0)
			nextPoint = wayPoints[wayPointIndex];
		else
			nextPoint = myStack[myStack.size() - 1];

		Vector3 direction = (pos - nextPoint).Normalize();
		float distance = GetDistance(pos.x, pos.y, nextPoint.x, nextPoint.y);

		if (distance < AiSpeed)
		{
			pos = nextPoint;
			arrived = true;
		}
		else
			pos = pos + direction * AiSpeed;

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
	}
}