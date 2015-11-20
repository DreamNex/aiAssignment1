#include "AIMain.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

AIMain::AIMain()
{

}

AIMain::~AIMain()
{

}

void AIMain::Init()
{
	SceneBase::Init();
}

void AIMain::Update(double dt)
{

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

void AIMain::RenderGO(GameObject* go)
{
	//Render the Gameobjects

	switch (go->type)
	{
		//simple model stack push pop rendering
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