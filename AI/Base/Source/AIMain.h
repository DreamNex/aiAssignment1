#ifndef AI_MAIN_H
#define AI_MAIN_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include <irrKlang.h>
using namespace irrklang;

class AIMain : public SceneBase
{
public:
	AIMain();
	~AIMain();


	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject* go);
	GameObject* FetchGO();

	//Simple Variables
	GameObject* AI;
	GameObject* AI2;
protected:

	//Might not use these Enums since we might use Integers to do 
	//the states
	enum FSM_ONE
	{
		ATTACK,
		DODGE,
		RESET,
	};

	FSM_ONE FSM1;

	enum FSM_TWO
	{
		PATROL,
		SCAN,
		IDLE,
	};

	FSM_TWO FSM2;
	//Protected Varian;es
	std::vector<GameObject*> m_goList;

private:
	
	//Variables here
	bool Perspec;
	float m_worldWidth;
	float m_worldHeight;

	//For Sound (do only if have time)
	ISoundEngine* soundEngine;
};

#endif