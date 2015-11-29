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

cAI::cAI()
	: offset(2.f)
{

}

cAI::~cAI()
{

}

std::vector <myVector> wayPoints, intrusionPoints, myStack;

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