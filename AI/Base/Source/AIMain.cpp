#include "AIMain.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include <vector>
using namespace std;



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

	RenderObjects();
}

void AIMain::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
}