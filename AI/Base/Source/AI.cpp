#include "AI.h"

struct myVector
{
	float x, y;
	myVector() :x(0), y(0){}
	myVector(float x, float y) :x(x), y(y){}
	void SetPosition(float _x, float _y){ x = _x; y = _y; }
	float GetX(){ return x; }
	float GetY(){ return y; }
	float Magnitude(){ return sqrt(x*x + y*y); }
	myVector Normalize(){ float length = Magnitude(); return myVector(x / length, y / length); }
	myVector operator + (myVector u){ return myVector(x + u.x, y + u.y); }
	myVector operator - (myVector u){ return myVector(u.x - x, u.y - y); }
	myVector operator += (myVector u){ return myVector(x + u.x, y + u.y); }
	myVector operator ~(){ return myVector(-x, -y); }
	myVector operator *(float scale){ return myVector(x*scale, y*scale); }
	float operator * (myVector  v){ return  x*v.x + y*v.y; }
};

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

myVector AiPos1, AiPos2, nextPoint;
vector <myVector> wayPoints, intrusionPoints, myStack;

void cAI::init()
{
	wayPoints.push_back(myVector(-offset, -offset));
	wayPoints.push_back(myVector(-offset, offset));
	wayPoints.push_back(myVector(offset, offset));
	wayPoints.push_back(myVector(offset, -offset));

	intrusionPoints.push_back(myVector(-1.2f*offset, -0.3f*offset));
	intrusionPoints.push_back(myVector(-1.2f*offset, 0.3f*offset));
	intrusionPoints.push_back(myVector(1.2f*offset, 0.3f*offset));
	intrusionPoints.push_back(myVector(1.2f*offset, -0.3f*offset));
	AiPos1.SetPosition(wayPoints[0].GetX(), wayPoints[0].GetY());

	int randomIndex = RandomInteger(1, 3);
	AiPos2.SetPosition(intrusionPoints[randomIndex].GetX(), intrusionPoints[randomIndex].GetY());
	state = PATROL;
	wayPointIndex = 1;
	arrived = false;
	probability = 30.0f;
}

void cAI::FSM()
{
	switch (state)
	{
		case PATROL:
			// if probability == true, idle
			randNum = RandomInteger(1, 100);
			if (randNum <= probability)
			{
				state = IDLE;
			}
			break;

		case IDLE:
			for (int timer = 0; timer < 100; timer++)
			{
				if (timer == 100)
				{
					state = PATROL;
				}
			}
			break;

		case SCAN:
			// if detect, go to fsm2
			switch (state)
			{
				case ATTACK:
					break;
			}
			break;
	}
}

void cAI::update(double dt)
{
	if (state == PATROL)
	{
		if (myStack.size() == 0)
			nextPoint = wayPoints[wayPointIndex];
		else
			nextPoint = myStack[myStack.size() - 1];

		myVector direction = (AiPos1 - nextPoint).Normalize();
		float distance = GetDistance(AiPos1.GetX(), AiPos1.GetY(), nextPoint.GetX(), nextPoint.GetY());

		if (distance < AiSpeed)
		{
			AiPos1 = nextPoint;
			arrived = true;
		}
		else
			AiPos1 = AiPos1 + direction*AiSpeed;

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
				myStack.clear();
			arrived = false;
		}
	}
}