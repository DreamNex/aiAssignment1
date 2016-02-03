#include "AIMain.h"
#include "GL\glew.h"
#include "Application.h"
#include "LoadTGA.h"
#include "MeshBuilder.h"
#include <sstream>
#include <vector>

using namespace irrklang;
using namespace MyAI;


AIMain::AIMain()
{
	
}

AIMain::~AIMain()
{

}

void AIMain::Init()
{
	SceneBase::Init();

	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	meshList[GEO_BACKGROUND] = MeshBuilder::GenerateQuad("background", Color(1, 1, 1), 2.f);
	meshList[GEO_BACKGROUND]->textureID = LoadTGA("Image//AI_Background.tga");

	srand((unsigned)time(NULL));
	
	WayPoints[0].Set(25, 20, 0);	// soldier1 pos
	WayPoints[1].Set(25, 70, 0);	// medic1 pos
	WayPoints[2].Set(55, 45, 0);	// attack1 pos
	WayPoints[3].Set(110, 20, 0);	// soldier2 pos
	WayPoints[4].Set(110, 70, 0);	// medic2 pos
	WayPoints[5].Set(80, 45, 0);	// attack2 pos
	WayPoints[6].Set(25, 45, 0);	// leader1 pos
	WayPoints[7].Set(110, 45, 0);	// leader2 pos

	//Team1(right) - leader
	cAI* ai = new cAI();
	ai->active = true;
	ai->pos.Set(WayPoints[6].x, WayPoints[6].y, 1);
	ai->scale.Set(1, 1, 1);
	ai->mesh = meshList[GEO_BALL];
	ai->SetFightPt(WayPoints[2]);
	ai->startwPoint = WayPoints[6];
	ai->mbController.SetMessage(ai->mbController.GetCommand(3));
	ai->FSM1 = AGGRESIVE;
	ai->FSM2 = STOP2;
	ai->init();
	m_goList.push_back(ai);

	//Team1(right) - medic
	cAI* ai2 = new cAI();
	ai2->active = true;
	ai2->pos.Set(WayPoints[1].x, WayPoints[1].y, 1);
	ai2->startwPoint = (WayPoints[1]);
	ai2->SetFightPt(WayPoints[2]);
	ai2->scale.Set(1, 1, 1);
	ai2->mesh = meshList[GEO_BALL2];
	ai2->init();
	m_goList.push_back(ai2);

	//Team1(right) - soldier
	cAI* ai3 = new cAI();
	ai3->active = true;
	ai3->pos.Set(WayPoints[0].x, WayPoints[0].y, 1);
	ai3->scale.Set(1, 1, 1);
	ai3->startwPoint = WayPoints[0];
	ai3->SetFightPt(WayPoints[2]);
	ai3->mesh = meshList[GEO_BALL3];
	ai3->init();
	m_goList.push_back(ai3);

	//Team2(left) - leader
	cAI* ai4 = new cAI();
	ai4->active = true;
	ai4->pos.Set(WayPoints[7].x, WayPoints[7].y, 1);
	ai4->Volunteer = true;
	ai4->startwPoint = WayPoints[7];
	ai4->SetFightPt(WayPoints[5]);
	ai4->scale.Set(1, 1, 1);
	ai4->mesh = meshList[GEO_BALL];
	ai4->init();
	m_goList.push_back(ai4);

	//Team2(left) - medic
	cAI* ai5 = new cAI();
	ai5->active = true;
	ai5->pos.Set(WayPoints[4].x, WayPoints[4].y, 1);
	ai5->SetFightPt(WayPoints[5]);
	ai5->startwPoint = WayPoints[4];
	ai5->scale.Set(1, 1, 1);
	ai5->mesh = meshList[GEO_BALL2];
	ai5->init();
	m_goList.push_back(ai5);

	//Team2(left) - soldier
	cAI* ai6 = new cAI();
	ai6->active = true;
	ai6->pos.Set(WayPoints[3].x, WayPoints[3].y, 1);
	ai6->startwPoint = WayPoints[3];
	ai6->SetFightPt(WayPoints[6]);
	ai6->scale.Set(1, 1, 1);
	ai6->mesh = meshList[GEO_BALL3];
	ai6->init();
	m_goList.push_back(ai6);

	state = 1;
	state2 = 1;
	respawntime = 6;
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

	respawntime--;
}

void AIMain::Render2DMesh(Mesh *mesh, bool enableLight, int size, int x, int y, bool rotate, bool flip)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, 800, 0, 600, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate((float)x, (float)y, 0);
	modelStack.Scale((float)size, (float)size, (float)size);
	if (rotate)
		modelStack.Rotate(rotateAngle, 0, 0, 1);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
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
		if (m_goList[i]->active == true)
		{
			modelStack.PushMatrix();
			modelStack.Translate(m_goList[i]->pos.x, m_goList[i]->pos.y, m_goList[i]->pos.z);
			RenderMesh(m_goList[i]->mesh, false);
			modelStack.PopMatrix();
		}
	}
}

void AIMain::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	//ortho.SetToOrtho(0, m_window_width, 0, m_window_height, -10, 10);
	ortho.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation((i*0.5f) + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
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

	modelStack.PushMatrix();
	modelStack.Translate(67, 50, 0);
	modelStack.Scale(70, 70, 0);
	RenderMesh(meshList[GEO_BACKGROUND], false);
	modelStack.PopMatrix();

	RenderObjects();
	
	/*switch (state)
	{
	case 1:
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "AI: Patrol", Color(1, 0, 0), 5, 3, 90);
		modelStack.PopMatrix();
		break;

	case 2:
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "AI: Idle", Color(1, 0, 0), 5, 3, 90);
		modelStack.PopMatrix();
		break;

	case 3:
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "AI: Attack", Color(1, 0, 0), 5, 3, 90);
		modelStack.PopMatrix();
		break;

	case 4:
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "AI: Dodge", Color(1, 0, 0), 5, 3, 90);
		modelStack.PopMatrix();
		break;
	}

	switch (state2)
	{
	case 1:
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "AI2: Patrol", Color(1, 0, 0), 5, 100, 90);
		modelStack.PopMatrix();
		break;

	case 2:
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "AI2: Idle", Color(1, 0, 0), 5, 100, 90);
		modelStack.PopMatrix();
		break;

	case 3:
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "AI2: Attack", Color(1, 0, 0), 5, 100, 90);
		modelStack.PopMatrix();
		break;

	case 4:
		modelStack.PushMatrix();
		RenderTextOnScreen(meshList[GEO_TEXT], "AI2: Dodge", Color(1, 0, 0), 5, 100, 90);
		modelStack.PopMatrix();
		break;
	}*/

	modelStack.PushMatrix();
	RenderTextOnScreen(meshList[GEO_TEXT], "Blue:Leader| Purple:Soldier | Green:Medic", Color(1, 0, 0), 5, 25, 0);
	modelStack.PopMatrix();

	for (int i = 0; i < 8; i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(WayPoints[i].x, WayPoints[i].y, WayPoints[i].z);
		modelStack.Scale(3, 3, 1);
		RenderMesh(meshList[GEO_WAYPOINTS], false);
		modelStack.PopMatrix();
	}
}

void AIMain::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
}