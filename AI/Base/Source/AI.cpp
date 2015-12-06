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
	,wayPointIndex(0)
	,arrived(false)
	,probability(30.f)
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
	wayPoints.push_back(Vector3(-offset + 10, -offset + 10));
	wayPoints.push_back(Vector3(-offset + 10, offset + 10));
	wayPoints.push_back(Vector3(offset + 10, offset + 10));
	wayPoints.push_back(Vector3(offset + 10, -offset + 10));
	//pos.Set(wayPoints[0].x, wayPoints[0].y);
	int randomIndex = RandomInteger(1, 3);
	FSM2 = PATROL;
	FSM1 = STOP1;
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
			if (randNum <= probability)
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
					 if (timer == 100)
					 {
						 FSM2 = PATROL;
					 }
				 }
				 break;
		}

		case 3:
		{
				// FSM1 = ATTACK;
				// FSM2 = STOP2;
				 break;
		}
	
	}

}

void cAI::RenderState(Mesh* mesh, std::string text, Color color)
{
	//if (!mesh || mesh->textureID <= 0)
	//	return;

	//glDisable(GL_DEPTH_TEST);
	//glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	//glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	//glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	//glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	//glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	//for (unsigned i = 0; i < text.length(); ++i)
	//{
	//	Mtx44 characterSpacing;
	//	characterSpacing.SetToTranslation(i * 0.8f, 0, 0); //1.0f is the spacing of each character, you may change this value
	//	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
	//	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	//	mesh->Render((unsigned)text[i] * 6, 6);
	//}
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	//glEnable(GL_DEPTH_TEST);

	//std::string stateString = "";
	//switch (FSM2)
	//{
	//case PATROL:
	//	stateString = "PATROL";
	//	break;
	//}
}