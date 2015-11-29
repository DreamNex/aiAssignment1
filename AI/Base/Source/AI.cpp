#include "AI.h"

cAI::cAI()
	: offset(2.f)
{

}

cAI::~cAI()
{

}

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
	AiPos1.Set(wayPoints[0].GetX(), wayPoints[0].GetY());

	int randomIndex = RandomInteger(1, 3);
	AiPos2.Set(intrusionPoints[randomIndex].GetX(), intrusionPoints[randomIndex].GetY());
	state = PATROL;
	wayPointIndex = 1;
	arrived = false;
}

int RandomInteger(int lowerLimit, int upperLimit)
{
	return rand() % (upperLimit - lowerLimit + 1) + lowerLimit;
}

void cAI::update(double dt)
{
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