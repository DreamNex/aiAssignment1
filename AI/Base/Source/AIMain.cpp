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




void AIMain::Init()
{
	SceneBase::Init();
	srand((unsigned)time(NULL));
	//float offset = 2.0;

	cAI *ai = new cAI();
	ai->active = true;
	ai->pos.Set(0, 0, 0);
	ai->scale.Set(1, 1, 1);
	ai->mesh = meshList[GEO_SHIP];
	m_goList.push_back(ai);
}

void AIMain::Update(double dt)
{
	

	for (unsigned i = 0; i < m_goList.size(); ++i)
	{
		cAI *ai = dynamic_cast<cAI*>(m_goList[i]);

		if (ai != NULL)
		{
			ai->update(dt);
		}
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

void AIMain::RenderGO()
{
	for (unsigned i = 0; i < m_goList.size(); ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(m_goList[i]->pos.x, m_goList[i]->pos.y, m_goList[i]->pos.z);
		modelStack.Scale(m_goList[i]->scale.x, m_goList[i]->scale.y, m_goList[i]->scale.z);
		RenderMesh(m_goList[i]->mesh, false);
		modelStack.PopMatrix();
	}
}

void AIMain::RenderObjects()
{
	RenderGO();

	modelStack.PushMatrix();
	// AI 1
	modelStack.Translate(AiPos1.x, AiPos1.y, 0);
	RenderMesh(meshList[GEO_BALL], false);
	modelStack.PopMatrix();
	
	// AI 2
	modelStack.PushMatrix();
	modelStack.Translate(AiPos2.x, AiPos2.y, 0);
	RenderMesh(meshList[GEO_BALL], false);
	modelStack.PopMatrix();

	//// Waypoints
	//modelStack.PushMatrix();
	//for (unsigned int i = 0; i < wayPoints.size(); i++)
	//	modelStack.Translate(wayPoints[i].GetX(), wayPoints[i].GetY(), 0);
	//RenderMesh(meshList[GEO_WAYPOINTS], false);
	//modelStack.PopMatrix();
}

void AIMain::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

//	RenderMesh(meshList[GEO_AXES], false);
	/*modelStack.PushMatrix();
	modelStack.Translate(100, 50, 0);
	RenderMesh(meshList[GEO_BALL], false);
	modelStack.PopMatrix();*/
	RenderObjects();
}

void AIMain::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
}