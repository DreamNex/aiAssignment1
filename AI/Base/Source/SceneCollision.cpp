#include "SceneCollision.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneCollision::SceneCollision()
	:soundEngine(NULL)
{

}

SceneCollision::~SceneCollision()
{
}

void SceneCollision::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	m_objectCount = 0;
	Spawn = 0.f;
	AllowSpawn = true;
	Perspec = false;
	NumOfBalls = 0;
	timer = 9000;

	m_ghost = new GameObject(GameObject::GO_BALL);

	Mtx44 rotation;
	Vector3 normal (1,0,0);
	const float size = 37.5;
	//Vector3 centre = (m_worldWidth *0.5f, m_worldHeight * 0.5);
	for(int i = 3; i != 257; i+= 127)
	{
		GameObject * go = FetchGO();
		go->pos = Vector3(i, m_worldHeight * 0.5f,0);
		go->type = GameObject::GO_WALL;
		//rotation.SetToRotation(i * 90,0,0,1);

		go->normal.Set(1,0,0);
		go->scale.Set(5, 80, 5);

	}

	soundEngine = createIrrKlangDevice();

	Vector3 pos;
	Vector3 tar;
	Vector3 view;
	Vector3 up;
	Vector3 right;
	pos = Vector3(m_worldWidth * 0.5f, 200, -66);
	tar = Vector3(m_worldWidth * 0.5f,0, 10);
	up = Vector3(0,1,0);

	view = (tar - pos).Normalized();
	right = view.Cross(up);
	up = view.Cross(right);
	camera.Init(pos, tar, up);
	GAMESTATE = MENU;
}

GameObject* SceneCollision::FetchGO()
{
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(!go->active)
		{
			go->active = true;
			++m_objectCount;
			return go;
		}
	}
	for(unsigned i = 0; i < 10; ++i)
	{
		GameObject *go = new GameObject(GameObject::GO_BALL);
		m_goList.push_back(go);
	}
	GameObject *go = m_goList.back();
	go->active = true;
	++m_objectCount;
	return go;
}

float SceneCollision::CheckCollsion2(GameObject *go1, GameObject *go2)
{
	switch(go2->type)
	{
	case GameObject::GO_BALL:
		Vector3 p1 = go1->pos;
		Vector3 p2 = go2->pos;
		Vector3 v1 = go1->vel;
		Vector3 v2 = go2->vel;
		Vector3 D = p1 - p2;
		Vector3 C = v1 - v2;

		if(C.Dot(-D) < 0)
			return -1;

		float r = go1->scale.x + go2->scale.x;
		float a = C.Dot(C);
		float b = 2 * C.Dot(D);
		float c = D.Dot(D) - r * r;
		float disc = b * b - 4 * a * c;
		if(disc < 0)
			return -1;

		float t = (-b - sqrt(disc)) / (2 * a);
		if(t < 0 )
			t = (-b + sqrt(disc)) / (2 * a);
		return t;
	}
}

void SceneCollision::CollisionResponse(GameObject *go1, GameObject *go2)
{
	switch(go2->type)
	{
	case GameObject::GO_BALL:
		{
			m1 = go1->mass;
			m2 = go2->mass;
			u1 = go1->vel;
			u2 = go2->vel;

			Vector3 N = (go2->pos - go1->pos).Normalized();
			Vector3 u1N = u1.Dot(N) * N;
			Vector3 u2N = u2.Dot(N) * N;

			go1->vel = u1 + ((2 * m2) / (m1 + m2)) * (u2N - u1N);
			go2->vel = u2 + ((2 * m1) / (m1 + m2)) * (u1N - u2N);
		}
		break;

	case GameObject::GO_PLAYER1:
	case GameObject::GO_PLAYER2:
	case GameObject::GO_WALL:
		{
			soundEngine->play2D("Sound//rebound.wav", false, false);
			Vector3 w0 = go2->pos;
			Vector3 b1 = go1->pos;
			Vector3 N = go2->normal;
			float r = go1->scale.x;
			float h = go2->scale.x;
			float l = go2->scale.y;
			Vector3 NP(-N.y, N.x, 0);

			if(abs((w0 - b1).Dot(N)) > r+ h* 0.5f)
			{
				Vector3 u = go1->vel;
				Vector3 N = go2->normal;
				Vector3 v = u - 2 * u.Dot(N) * N;
				go1->vel = v;
			}

			if(abs((w0 - b1).Dot(NP)) > r + l * 0.5f)
			{
				Vector3 u = go1->vel;
				Vector3 N = go2->normal;
				Vector3 v = u - 2 * u.Dot(NP) * NP;
				go1->vel = v;
			}
		}
		break;

	case GameObject::GO_PILLAR:
		{
			soundEngine->play2D("Sound//rebound.wav", false, false);
			u1 = go1->vel;

			Vector3 N = (go2->pos - go1->pos).Normalized();
			Vector3 u1N = u1.Dot(N) * N;
			Vector3 u2N = u2.Dot(N) * N;

			go1->vel = u1 - (2 * u1).Dot(N) * N;
		}
		break;

	case GameObject::GO_GOALPOST:
		{
			soundEngine->play2D("Sound//goalpost.wav",false, false);
			//AddScore(go2);
			go1->active = false;
			NumOfBalls--;
			AllowSpawn = true;
		}
	}

	/*v1 = go1->vel;
	v2 = go2->vel;*/
}

bool SceneCollision::CheckCollision(GameObject *go, GameObject *go2, float dt)
{
	if(go->type != GameObject::GO_BALL)
	{
		if(go2->type != GameObject::GO_BALL)
		{
			return false;

		}

		else
		{
			GameObject *temp = go;
			go = go2;
			go2 = temp;
		}

	}

	switch(go2->type)
	{
	case GameObject::GO_BALL:
		{
			//Exercise 1: move collision code to CheckCollision()
			float distSquared = (go->pos - go2->pos).LengthSquared();
			float combinedRadius = go->scale.x + go2->scale.x;

			//Practical 4, Exercise 13: improve collision detection algorithm
			if(distSquared <= combinedRadius * combinedRadius)
			{
				return true;
			}

			return false;
		}
		break;
	case GameObject::GO_PLAYER1:
	case GameObject::GO_PLAYER2:
	case GameObject::GO_GOALPOST:
	case GameObject::GO_WALL:
		{
			Vector3 w0 = go2->pos;
			Vector3 b1 = go->pos + go->vel * dt;
			Vector3 N = go2->normal;
			float r = go->scale.x;
			float h = go2->scale.x;
			float l = go2->scale.y;
			Vector3 NP(-N.y, N.x, 0);

			if(abs((w0 - b1).Dot(N)) < r+ h* 0.5f && abs((w0 - b1).Dot(NP)) < r + l * 0.5f)
				return true;
			else
				return false;
		}
		break;

	case GameObject::GO_PILLAR:
		{
			//Exercise 1: move collision code to CheckCollision()
			float distSquared = (go->pos - go2->pos).LengthSquared();
			float combinedRadius = go->scale.x + go2->scale.x;

			//Practical 4, Exercise 13: improve collision detection algorithm
			if(distSquared <= combinedRadius * combinedRadius)
			{
				return true;
			}
			return false;
		}
		break;
	}
	return false;
}

void SceneCollision::Update(double dt)
{
	SceneBase::Update(dt);

	m_force.SetZero();
	m_force2.SetZero();

	timer -= dt;
	Spawn += dt;


	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics Simulation Section
	Vector3 accel = m_force * (1.f / p1->mass);
	p1->vel += accel * dt;

}


void SceneCollision::RenderGO(GameObject *go)
{

	//Render GameObjects

	switch(go->type)
	{
	
	}
}

void SceneCollision::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	viewStack.LoadIdentity();
	if(Perspec)
	{
		projection.SetToPerspective(60, 4/3, 0.1, 5000);
		viewStack.LookAt(
			camera.position.x, camera.position.y, camera.position.z,
			camera.target.x, camera.target.y, camera.target.z,
			camera.up.x, camera.up.y, camera.up.z
			);
	}

	else
	{
		projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10 , 10);

	}
	projectionStack.LoadMatrix(projection);
	// Camera matrix


	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	//RenderMesh(meshList[GEO_AXES], false);

}

void SceneCollision::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if(m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}

	if(soundEngine != NULL)
	{
		soundEngine->drop();
	}
}
