#ifndef SCENE_COLLISION_H
#define SCENE_COLLISION_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include <irrKlang.h>
using namespace irrklang;

class SceneCollision : public SceneBase
{
	static const int MAX_SPEED = 10;
	static const int BULLET_SPEED = 50;
	static const int MISSILE_SPEED = 20;
	static const int MISSILE_POWER = 1;

public:
	SceneCollision();
	~SceneCollision();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();

	//void AddScore(GameObject* go);
	void CollisionResponse(GameObject *go1, GameObject *go2);
	float CheckCollsion2(GameObject *go1, GameObject *go2);
	bool CheckCollision(GameObject *go1, GameObject *go2, float dt);

protected:

	enum STATE_GAME
	{
		MENU,
		INSTRUCTION,
		GAME,
		DRAW,
		WINMENU1,
		WINMENU2,
	};

	STATE_GAME GAMESTATE;

	bool Perspec;

	//Physics
	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	GameObject *m_ghost;
	GameObject *p1;
	GameObject *p2;
	GameObject *gp1;
	GameObject *gp2;
	
	//Keep these variable 
	int m_objectCount;
	int timer;
	int NumOfBalls;
	float Spawn;
	bool AllowSpawn;

	//keep the force vairables
	Vector3 m_force;
	Vector3 m_force2;
	
	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2;
	Vector3 initialMomentum, finalMomentum;
	float initialKE, finalKE;

	//FSM variables

	//Sound
	ISoundEngine* soundEngine;
	

};

#endif