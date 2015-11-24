#include "AIMain.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include <vector>
using namespace std;

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

AIMain::AIMain()
{

}

AIMain::~AIMain()
{

}

int state;
int wayPointIndex;
bool arrived;
const float playerRadius = 0.25;
const float proximity = 0.4f;
vector <myVector> wayPoints, intrusionPoints, myStack;
myVector playerPos, enemyPos;

int RandomInteger(int lowerLimit, int upperLimit)
{
	return rand() % (upperLimit - lowerLimit + 1) + lowerLimit;
}

void AIMain::Init()
{
	SceneBase::Init();
	srand((unsigned)time(NULL));
	float offset = 2.0;
	wayPoints.push_back(myVector(-offset, -offset));
	wayPoints.push_back(myVector(-offset, offset));
	wayPoints.push_back(myVector(offset, offset));
	wayPoints.push_back(myVector(offset, -offset));
	intrusionPoints.push_back(myVector(-1.2f*offset, -0.3f*offset));
	intrusionPoints.push_back(myVector(-1.2f*offset, 0.3f*offset));
	intrusionPoints.push_back(myVector(1.2f*offset, 0.3f*offset));
	intrusionPoints.push_back(myVector(1.2f*offset, -0.3f*offset));
	playerPos.SetPosition(wayPoints[0].GetX(), wayPoints[0].GetY());
	int randomIndex = RandomInteger(1, 3);
	enemyPos.SetPosition(intrusionPoints[randomIndex].GetX(), intrusionPoints[randomIndex].GetY());
	state = PATROL;
	wayPointIndex = 1;
	arrived = false;
}

void AIMain::Update(double dt)
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

GameObject* AIMain::FetchGO()
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
		{
			go->active = true;
			return go;
		}
	}
	for (unsigned i = 0; i < 10; ++i)
	{
		GameObject *go = new GameObject(GameObject::GO_BALL);
		m_goList.push_back(go);
	}
	GameObject *go = m_goList.back();
	go->active = true;
	return go;
}

void RenderCircle(GLfloat x, GLfloat y, GLfloat radius, GLfloat r, GLfloat g, GLfloat b)
{
	int n = 360;
	glColor3f(r, g, b);
	glBegin(GL_POINTS);
	for (int i = 0; i <= n; i++)
	{
		float angle = (float)(i * (2.0 * 3.14159 / n));
		glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
	}
	glEnd();
}

void RenderFillCircle(GLfloat x, GLfloat y, GLfloat radius, GLfloat r, GLfloat g, GLfloat b)
{
	int n = 360;
	glColor3f(r, g, b);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);
	for (int i = 0; i <= n; i++)
	{
		float angle = (float)(i * (2.0 * 3.14159 / n));
		glVertex2f(x + radius * cos(angle), y + radius * sin(angle));
	}
	glEnd();
}

void AIMain::RenderGO(GameObject* go)
{
	//Render the Gameobjects

	switch (go->type)
	{
		//simple model stack push pop rendering
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, -10.0f);

		// Player
		RenderFillCircle(playerPos.GetX(), playerPos.GetY(), playerRadius, 0.0f, 0.0f, 1.0f);
		RenderCircle(playerPos.GetX(), playerPos.GetY(), playerRadius + proximity, 0.1f, 0.1f, 0.1f);
		// Waypoints
		for (unsigned int i = 0; i < wayPoints.size(); i++)
			RenderCircle(wayPoints[i].GetX(), wayPoints[i].GetY(), playerRadius + 0.1f, 1.0f, 0.0f, 0.0f);

		glPopMatrix();
	}
}

void AIMain::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	viewStack.LoadIdentity();
	if (Perspec)
	{
		projection.SetToPerspective(60, 4 / 3, 0.1, 5000);
		viewStack.LookAt(
			camera.position.x, camera.position.y, camera.position.z,
			camera.target.x, camera.target.y, camera.target.z,
			camera.up.x, camera.up.y, camera.up.z
			);
	}

	else
	{
		projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);

	}
	projectionStack.LoadMatrix(projection);
	// Camera matrix


	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	//RenderMesh(meshList[GEO_AXES], false);

}

void AIMain::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
}